#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "Adafruit_DRV2605.h"
#include <OneWire.h>
#include <DallasTemperature.h>


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40); //PCA9685

// -------------------- ERMs + LRAs --------------------
// ERMs on PCA9685 channels
const uint8_t ERM1_CH = 4;
const uint8_t ERM2_CH = 5;

const uint16_t PWM_MAX = 4095;
const uint16_t ERM_MAX_DUTY = 2500;

// DRV2605L LRAs via PCA9548A mux
Adafruit_DRV2605 drv;

const uint8_t MUX_ADDR = 0x70;
const uint8_t LRA1_MUX_CH = 0; //DRV2605L #1
const uint8_t LRA2_MUX_CH = 1; //DRV2605L #2

// good LRA effects from library
const uint8_t LRA_SOFT_CLICK = 5; // light/medium rain
const uint8_t LRA_STRONG_CLICK = 34; // heavier / sharper
const uint8_t LRA_BIG = 24; // bigger impact?

// -------------------- FANS (all wired to PCA9685) --------------------
const uint8_t FAN_SMALL_LEFT_CH = 8; // small, side 1
const uint8_t FAN_BIG_LEFT_CH = 11; // big left (ONLY WORKS AT FULL SPEED)
const uint8_t FAN_SMALL_RIGHT_CH = 9; // small, side 2
const uint8_t FAN_BIG_RIGHT_CH = 10; // big, side 2

const uint16_t FAN_MAX_DUTY = 4095;

// duty fractions different modes (except FAN_BIG_LEFT_CH is always on high)
const float FAN_OFF_LEVEL = 0.0f;
const float FAN_LOW_LEVEL = 0.3f;
const float FAN_MED_LEVEL = 0.6f;
const float FAN_HIGH_LEVEL = 1.0f;

// -------------------- TECs + Temperature --------------------
const uint8_t TEC_HEAT_CH = 3;   // Peltier oriented so this feels warm
const uint8_t TEC_COOL_CH = 2;   // Peltier oriented so this feels cool

// ------------ Temperature Sensors ------------
// heater sensor on digital pin 9
const uint8_t HOT_SENSOR_PIN = 10;
// cooler sensor on digital pin 10
const uint8_t COLD_SENSOR_PIN = 9;

OneWire oneWireHot(HOT_SENSOR_PIN);
OneWire oneWireCold(COLD_SENSOR_PIN);

DallasTemperature hotSensor(&oneWireHot);
DallasTemperature coldSensor(&oneWireCold);

// ------------ Temp control parameters ------------
float TARGET_TEMP_C = 28.0;
float HYSTERESIS_C = 0.5; // bound to be considered at target
bool  HEATING_MODE = true; // true = use heater, false = use cooler
float TEC_POWER = 1.0f;
bool  tempControlEnabled = false; // use so that heaters dont run all the time

unsigned long lastTempMillis = 0;
const unsigned long TEMP_PERIOD_MS = 1000; // how often to update temp control (our sensors don't even update this fast)
const unsigned long IDLE_MONITOR_PERIOD_MS = 5000; // so we can have constant temp updates even if a "day" isn't running
const unsigned long DAY_DURATION_MS = 15000; // run each day for 15 seconds

// ------------ SAFETY LIMITS ------------
// basically implemented this part because our temp sensors did not ever work consistently so we needed some safety features
const unsigned long TEC_MAX_CONTINUOUS_MS = 3000; // how long the heater/cooler can be on before safety activated

// safety cycle:
const unsigned long HEATER_ON_MS = 0; //started as 1 on, 2 off but changed based on experimenting to just fully off bc day is only 15 seconds long anyway
const unsigned long HEATER_OFF_MS = 4000; 
const unsigned long COOLER_ON_MS = 1000; //cooler on for 1 sec
const unsigned long COOLER_OFF_MS = 2000; //cooler off for 2 sec

// state for heater safety
bool heaterCurrentlyOn = false;
bool heaterSafetyActive = false;
unsigned long heaterOnStartMs = 0; // when it last transitioned from OFF→ON
unsigned long heaterSafetyStartMs = 0; // start of safety timing

// state for cooler safety
bool coolerCurrentlyOn = false;
bool coolerSafetyActive = false;
unsigned long coolerOnStartMs = 0;
unsigned long coolerSafetyStartMs = 0;

// used to make percipitation patterns later instead of putting in day code
typedef void (*PrecipPatternFn)(int, float);

// ------------ PWM helper ------------
void setChannelPWM(uint8_t ch, float level, uint16_t maxDuty = PWM_MAX) {
  if (level < 0.0f) level = 0.0f;
  if (level > 1.0f) level = 1.0f;

  uint16_t duty = (uint16_t)(level * maxDuty);

  if (duty == 0) {
    pwm.setPWM(ch, 0, 0);
  } else {
    pwm.setPWM(ch, 0, duty);
  }
}

// ------------ ERM helpers ------------
void setERM(uint8_t channel, float intensity) {
  setChannelPWM(channel, intensity, ERM_MAX_DUTY);
}

void ermOff(uint8_t ch) {
  setERM(ch, 0.0f);
}

// short gentle tap (maybe good for rain? tbd)
void ermSoftTap(uint8_t ch, float level, uint16_t durationMs) {
  setERM(ch, level);
  delay(durationMs);
  ermOff(ch);
}

// ------------ MUX and LRA helpers ------------
void selectMuxChannel(uint8_t ch) {
  if (ch > 7) return;
  Wire.beginTransmission(MUX_ADDR);
  Wire.write(1 << ch);
  Wire.endTransmission();
}

bool initLRAonChannel(uint8_t muxChannel) {
  selectMuxChannel(muxChannel);

  if (!drv.begin()) {
    Serial.print(F("DRV2605 not found on mux channel "));
    Serial.println(muxChannel);
    return false;
  }

  drv.useLRA();
  drv.selectLibrary(6);
  drv.setMode(DRV2605_MODE_INTTRIG);
  return true;
}

void lraPlayEffect(uint8_t muxChannel, uint8_t effect) {
  selectMuxChannel(muxChannel);
  drv.setWaveform(0, effect);
  drv.setWaveform(1, 0);
  drv.go();
}

void lraSoftClickEffect(uint8_t muxChannel) {
  lraPlayEffect(muxChannel, LRA_SOFT_CLICK);
}

void lraStrongClickEffect(uint8_t muxChannel) {
  lraPlayEffect(muxChannel, LRA_STRONG_CLICK);
}

// ------------ FAN helpers ------------
void setFan(uint8_t channel, float level) {
  setChannelPWM(channel, level, FAN_MAX_DUTY);
}

// used for easier per day config setup
void configureDayFans(float smallLeft, float bigLeft, float smallRight, float bigRight) {
  // start all with kick bc sometimes they don't start if you don't give them a push?
  auto setWithKick = [&](uint8_t ch, float level) {
    if (level <= 0.0f) {
      setFan(ch, 0.0f);
      return;
    }
    // kick
    pwm.setPWM(ch, 0, PWM_MAX);
    delay(300);
    // settle
    setFan(ch, level);
  };

  setWithKick(FAN_SMALL_LEFT_CH,  smallLeft);
  setWithKick(FAN_BIG_LEFT_CH,    bigLeft);
  setWithKick(FAN_SMALL_RIGHT_CH, smallRight);
  setWithKick(FAN_BIG_RIGHT_CH,   bigRight);
}

void allFansOff() {
  setFan(FAN_SMALL_LEFT_CH,  0.0f);
  setFan(FAN_BIG_LEFT_CH,    0.0f);
  setFan(FAN_SMALL_RIGHT_CH, 0.0f);
  setFan(FAN_BIG_RIGHT_CH,   0.0f);
}

// ------------ TEC and temp helpers ------------
void setTECChannelPower(uint8_t tecChannel, float level) {
  setChannelPWM(tecChannel, level, PWM_MAX);
}

// ------------ SAFETY helpers ------------
void applyHeaterCommand(bool wantOn) {
  unsigned long now = millis();

  if (!wantOn) {
    // turn off and reset safety
    heaterCurrentlyOn    = false;
    heaterSafetyActive   = false;
    setTECChannelPower(TEC_HEAT_CH, 0.0f);
    return;
  }

  // if we want heat:
  if (!heaterCurrentlyOn) {
    // just transitioned OFF -> ON
    heaterCurrentlyOn   = true;
    heaterOnStartMs     = now;
    heaterSafetyActive  = false;
    heaterSafetyStartMs = now;
    setTECChannelPower(TEC_HEAT_CH, TEC_POWER);
    return;
  }

  // already ON
  if (!heaterSafetyActive) {
    // check if we've exceeded continuous limit
    if (now - heaterOnStartMs >= TEC_MAX_CONTINUOUS_MS) {
      heaterSafetyActive  = true;
      heaterSafetyStartMs = now;
      Serial.println(F("[SAFETY] Heater entering duty-cycle mode"));
    }
    // while below limit, just keep full power
    setTECChannelPower(TEC_HEAT_CH, TEC_POWER);
  } else {
    // safety mode:
    unsigned long elapsed = now - heaterSafetyStartMs;
    unsigned long cycle   = HEATER_ON_MS + HEATER_OFF_MS;
    unsigned long pos     = elapsed % cycle;

    bool shouldBeOn = (pos < HEATER_ON_MS);
    heaterCurrentlyOn = shouldBeOn;
    setTECChannelPower(TEC_HEAT_CH, shouldBeOn ? TEC_POWER : 0.0f);
  }
}

void applyCoolerCommand(bool wantOn) {
  unsigned long now = millis();

  if (!wantOn) {
    // turn off and reset safety
    coolerCurrentlyOn    = false;
    coolerSafetyActive   = false;
    setTECChannelPower(TEC_COOL_CH, 0.0f);
    return;
  }

  if (!coolerCurrentlyOn) {
    // just transitioned OFF -> ON
    coolerCurrentlyOn   = true;
    coolerOnStartMs     = now;
    coolerSafetyActive  = false;
    coolerSafetyStartMs = now;
    setTECChannelPower(TEC_COOL_CH, TEC_POWER);
    return;
  }

  if (!coolerSafetyActive) {
    if (now - coolerOnStartMs >= TEC_MAX_CONTINUOUS_MS) {
      coolerSafetyActive  = true;
      coolerSafetyStartMs = now;
      Serial.println(F("[SAFETY] Cooler entering duty-cycle mode"));
    }
    // while below limit, keep full power
    setTECChannelPower(TEC_COOL_CH, TEC_POWER);
  } else {
    // safety mode:
    unsigned long elapsed = now - coolerSafetyStartMs;
    unsigned long cycle   = COOLER_ON_MS + COOLER_OFF_MS;
    unsigned long pos     = elapsed % cycle;

    bool shouldBeOn = (pos < COOLER_ON_MS);
    coolerCurrentlyOn = shouldBeOn;
    setTECChannelPower(TEC_COOL_CH, shouldBeOn ? TEC_POWER : 0.0f);
  }
}

// configure globals for a given "day"
void configureDayTemp(float targetC, float hystC, bool heatingMode, float tecPower) {
  TARGET_TEMP_C = targetC;
  HYSTERESIS_C  = hystC;
  HEATING_MODE  = heatingMode;
  TEC_POWER     = tecPower;
  enableTempControl();

  Serial.print(F("Configured temp: target="));
  Serial.print(TARGET_TEMP_C);
  Serial.print(F(" °C, hyst="));
  Serial.print(HYSTERESIS_C);
  Serial.print(F(", mode="));
  Serial.print(HEATING_MODE ? F("HEAT") : F("COOL"));
  Serial.print(F(", power="));
  Serial.println(TEC_POWER);
}

void tempControlStep() {
  float tempHotC, tempColdC;
  readHeaterCoolerTemps(tempHotC, tempColdC);

  Serial.print(F("Hot sensor: "));
  Serial.print(tempHotC);
  Serial.print(F(" °C | Cold sensor: "));
  Serial.print(tempColdC);
  Serial.print(F(" °C | Target: "));
  Serial.print(TARGET_TEMP_C);
  Serial.print(F(" °C | Mode: "));
  Serial.print(HEATING_MODE ? F("HEAT") : F("COOL"));
  Serial.print(F(" | TEC: "));

  if (HEATING_MODE) {
    bool wantHeat;
    // HEATING: use HOT sensor (pin 9)
    if (tempHotC < TARGET_TEMP_C - HYSTERESIS_C) {
      // too cold near heater pad → heater requested ON
      wantHeat = true;
      Serial.println(F("HEATER REQUEST"));
    } else if (tempHotC > TARGET_TEMP_C + HYSTERESIS_C) {
      // warm enough → heater requested OFF
      wantHeat = false;
      Serial.println(F("HEATER OFF (above band)"));
    } else {
      // inside deadband -> hold last state
      wantHeat = heaterCurrentlyOn;
      Serial.println(F("HEATER HOLD"));
    }

    // apply heater with safety; make sure cooler is off
    applyHeaterCommand(wantHeat);
    applyCoolerCommand(false);

  } else {
    bool wantCool;
    // COOLING: use COLD sensor (pin 10)
    if (tempColdC > TARGET_TEMP_C + HYSTERESIS_C) {
      // too hot near cooler pad → cooler requested ON
      wantCool = true;
      Serial.println(F("COOLER REQUEST"));
    } else if (tempColdC < TARGET_TEMP_C - HYSTERESIS_C) {
      // cool enough → cooler requested OFF
      wantCool = false;
      Serial.println(F("COOLER OFF (below band)"));
    } else {
      // inside deadband -> hold last state
      wantCool = coolerCurrentlyOn;
      Serial.println(F("COOLER HOLD"));
    }

    // apply cooler with safety; make sure heater is off
    applyCoolerCommand(wantCool);
    applyHeaterCommand(false);
  }
}

void enableTempControl() {
  tempControlEnabled = true;
}

void disableTempControl() {
  tempControlEnabled = false;
  // reset TECs and safety state
  heaterCurrentlyOn   = false;
  heaterSafetyActive  = false;
  coolerCurrentlyOn   = false;
  coolerSafetyActive  = false;

  setTECChannelPower(TEC_HEAT_CH, 0.0f);
  setTECChannelPower(TEC_COOL_CH, 0.0f);
}

// read both temp sensors:
// hotTempC -> sensor near HEATER
// coldTempC -> sensor near COOLER
void readHeaterCoolerTemps(float &hotTempC, float &coldTempC) {
  hotSensor.requestTemperatures();
  coldSensor.requestTemperatures();

  hotTempC  = hotSensor.getTempCByIndex(0);
  coldTempC = coldSensor.getTempCByIndex(0);
}

// ------------ Idle temperature monitor ------------
void logTempsOnly() {
  float hotC, coldC;
  readHeaterCoolerTemps(hotC, coldC);

  Serial.print(F("[MONITOR] Hot sensor: "));
  Serial.print(hotC);
  Serial.print(F(" °C | Cold sensor: "));
  Serial.print(coldC);
  Serial.println(F(" °C"));
}

// ------------ GLOBAL OFF ------------
void allOffEverything() {
  // ERMs
  ermOff(ERM1_CH);
  ermOff(ERM2_CH);
  // TECS
  disableTempControl(); // also turns TECs off and resets safety
  // fans
  allFansOff();
}

// ------------ percipitation patterns ------------
// light rain: background ERM and LRA pattern
void weatherDelay(uint32_t ms); //forward declare

void patternLightRain(int loops, float ermLevel) {
  Serial.println(F("Pattern: LIGHT RAIN"));

  setERM(ERM1_CH, ermLevel);
  setERM(ERM2_CH, ermLevel);

  for (int i = 0; i < loops; i++) {
    lraSoftClickEffect(LRA1_MUX_CH);
    weatherDelay(1200);

    lraSoftClickEffect(LRA2_MUX_CH);
    weatherDelay(1200);

    lraSoftClickEffect(LRA1_MUX_CH);
    weatherDelay(600);

    lraSoftClickEffect(LRA2_MUX_CH);
    weatherDelay(600);

    lraSoftClickEffect(LRA2_MUX_CH);
    weatherDelay(300);

    lraSoftClickEffect(LRA2_MUX_CH);
    weatherDelay(1200);

    lraSoftClickEffect(LRA1_MUX_CH);
    weatherDelay(300);

    lraSoftClickEffect(LRA2_MUX_CH);
    weatherDelay(600);

    lraSoftClickEffect(LRA1_MUX_CH);
    weatherDelay(600);
  }

  ermOff(ERM1_CH);
  ermOff(ERM2_CH);
}

// heavy rain: denser, stronger hits
void patternHeavyRain(int bursts, float ermLevel) {
  Serial.println(F("Pattern: HEAVY RAIN"));

  setERM(ERM1_CH, ermLevel);
  setERM(ERM2_CH, ermLevel);

  for (int burst = 0; burst < bursts; burst++) {
    lraStrongClickEffect(LRA1_MUX_CH);
    weatherDelay(300);

    lraSoftClickEffect(LRA2_MUX_CH);
    weatherDelay(300);

    lraStrongClickEffect(LRA1_MUX_CH);
    weatherDelay(150);

    lraStrongClickEffect(LRA2_MUX_CH);
    weatherDelay(150);

    lraSoftClickEffect(LRA2_MUX_CH);
    weatherDelay(50);

    lraStrongClickEffect(LRA2_MUX_CH);
    weatherDelay(300);

    lraSoftClickEffect(LRA1_MUX_CH);
    weatherDelay(100);

    lraStrongClickEffect(LRA2_MUX_CH);
    weatherDelay(150);

    lraStrongClickEffect(LRA1_MUX_CH);
    weatherDelay(150);
  }

  ermOff(ERM1_CH);
  ermOff(ERM2_CH);
}

void weatherDelay(uint32_t ms) { // needed for vibration and temp to run at same time bc in loop
  uint32_t start = millis();
  while (millis() - start < ms) {
    // keep temp controller alive while we wait
    if (tempControlEnabled && (millis() - lastTempMillis >= TEMP_PERIOD_MS)) {
      lastTempMillis = millis();
      tempControlStep();
    }
    delay(10); //small sleep to avoid a tight spin
  }
}

// run one weather day for a fixed duration.
// - patternFn = nullptr -> no precipitation, just fans and temp
// - loopsPerCycle: how many loops/bursts inside each pattern call
// - ermLevel: ERM background intensity for that pattern
void runDayForDuration(PrecipPatternFn patternFn,
                       int loopsPerCycle,
                       float ermLevel) {
  unsigned long start = millis();

  while (millis() - start < DAY_DURATION_MS) {
    if (patternFn) {
      // eun one pattern cycle (blocking, but temp is updated via weatherDelay)
      patternFn(loopsPerCycle, ermLevel);
    } else {
      //no rain: just keep temp controller alive during the 15 s
      weatherDelay(500); // short wait with tempControlStep inside
    }
  }

  Serial.println(F("Day complete: turning everything OFF"));
  allOffEverything();
}

// ------------ weather day wrappers ------------
// Day 1: sunny, breezy, no rain
void runDay1() {
  Serial.println(F("=== DAY 1: sunny, light breeze ==="));

  // temperature: mild warm day (heater)
  configureDayTemp(
    28.0f, //target C
    0.5f, //hysteresis
    true, //HEATING_MODE = HEAT
    0.1f //TEC power
  );

  // fans: light breeze
  configureDayFans(
    FAN_OFF_LEVEL, // small left
    0.0f, // big left
    FAN_HIGH_LEVEL, // small right
    FAN_MED_LEVEL // big right
  );

  // no precipitation pattern
  runDayForDuration(nullptr, 0, 0.0f);
}


// Day 2: light cold rainstorm
void runDay2() {
  Serial.println(F("=== DAY 2: Light Rainstorm ==="));

  //temperature: cooler
  configureDayTemp(
    15.0f,
    0.5f,
    false, // COOL
    0.1f
  );

  //fans: one side windier
  configureDayFans(
    FAN_OFF_LEVEL, // small left
    0.0f, // big left off
    FAN_OFF_LEVEL, // small right off
    FAN_HIGH_LEVEL // big right strong
  );

  // light rain looped over 15s
  runDayForDuration(
    patternLightRain,
    1, // loops per call
    0.20f // ERM background level
  );
}


// Day 3: heavy cold rainstorm
void runDay3() {
  Serial.println(F("=== DAY 3: Heavy Rain, Windy, Cold ==="));

  // temperature: cold!!!, use cooler
  configureDayTemp(
    15.0f,
    0.5f,
    false, // HEATING_MODE = COOL
    0.1f
  );

  // fans: strong wind both sides for stormy feel
  configureDayFans(
    FAN_HIGH_LEVEL, // small left
    1.0f, // big left full
    FAN_HIGH_LEVEL, // small right
    FAN_HIGH_LEVEL // big right
  );

  // heavy rain pattern repeats until 15s
  runDayForDuration(
    patternHeavyRain,
    1, //bursts per call
    0.225f // ERM background level
  );
}


// Day 4: windy fall, no rain
void runDay4() {
  Serial.println(F("=== DAY 4: Windy Fall Day ==="));

  // temperature: cool
  configureDayTemp(
    15.0f,
    0.50f,
    false, // COOL
    0.1f
  );

  // fans: strong wind, no rain
  configureDayFans(
    FAN_HIGH_LEVEL, // small left
    1.0f, // big left full
    FAN_HIGH_LEVEL, // small right
    FAN_HIGH_LEVEL // big right
  );

  // no precipitation
  runDayForDuration(nullptr, 0, 0.0f);
}

// Day 5: warm summer light rain
void runDay5() {
  Serial.println(F("=== DAY 5: Light Summer Rainstorm ==="));

  // temperature: warmer, heater
  configureDayTemp(
    28.0f,
    0.25f,
    true, // HEAT
    0.1f
  );

  // fans: light breeze
  configureDayFans(
    FAN_OFF_LEVEL, // small left
    0.0f, // big left off
    FAN_HIGH_LEVEL, // small right
    FAN_MED_LEVEL // big right
  );

  runDayForDuration(
    patternLightRain,
    1,
    0.20f
  );
}


// ------------ serial instructions ------------
void printHelp() {
  Serial.println(F("=== Weather Day Selector ==="));
  Serial.println(F("  1 = Day 1 (sunny, breezy)"));
  Serial.println(F("  2 = Day 2 (light COLD rainstorm)"));
  Serial.println(F("  3 = Day 3 (heavy COLD rainstorm)"));
  Serial.println(F("  4 = Day 4 (windy fall day)"));
  Serial.println(F("  5 = Day 5 (light SUMMER rainstorm)"));
  Serial.println(F("  0 = ALL OFF (fans + haptics + TECs)"));
  Serial.println(F("  h = help"));
}


// ------------ setup and loop ------------
void setup() {
  Serial.begin(115200);
  while (!Serial) {;}

  Serial.println(F("=== Weather Machine: Day-based Control ==="));

  Wire.begin();
  pwm.begin();
  pwm.setPWMFreq(250);
  delay(10);

  allOffEverything();

  //Temp sensors
  hotSensor.begin();
  coldSensor.begin();

  //LRAs on DRV2605L via mux
  if (initLRAonChannel(LRA1_MUX_CH)) {
    Serial.println(F("LRA1 init OK (mux ch 0)"));
  } else {
    Serial.println(F("LRA1 init FAILED"));
  }

  if (initLRAonChannel(LRA2_MUX_CH)) {
    Serial.println(F("LRA2 init OK (mux ch 1)"));
  } else {
    Serial.println(F("LRA2 init FAILED"));
  }

  printHelp();
}

void loop() {
  // --- serial commands: choose day ---
  if (Serial.available()) {
    char c = Serial.read();

    switch (c) {
      case '1':
        runDay1();
        break;

      case '2':
        runDay2();
        break;
      
      case '3':
        runDay3();
        break;

      case '4':
        runDay4();
        break;
      
      case '5':
        runDay5();
        break;

      case '0':
        Serial.println(F("ALL OFF (fans + haptics + TECs)"));
        allOffEverything();
        break;

      case 'h':
      case 'H':
        printHelp();
        break;

      default:
        if (c != '\n' && c != '\r') {
          Serial.print(F("Unknown command: "));
          Serial.println(c);
          Serial.println(F("Press h for help."));
        }
        break;
    }
  }

  // --- periodic temperature handling ---
  unsigned long now = millis();

  if (tempControlEnabled) {
    // normal control mode: 1 Hz temp controller
    if (now - lastTempMillis >= TEMP_PERIOD_MS) {
      lastTempMillis = now;
      tempControlStep();
    }
  } else {
    // udle monitor mode: just log temps for reference
    if (now - lastTempMillis >= IDLE_MONITOR_PERIOD_MS) {
      lastTempMillis = now;
      logTempsOnly();
    }
  }
}

