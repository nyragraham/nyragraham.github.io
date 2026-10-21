---
title: "The Weather Machine: Multi-modal Haptics"
subtitle: MAE 6800, Design and Control of Haptic Systems · Fall 2025
summary: A palm-sized haptic interface that lets users feel simplified weather through Peltier heating and cooling, airflow from four fans, and seven vibration motors, evaluated in a user study.
thumbnail: /assets/projects/haptic-weather-machine/thumb.jpg
order: 6
tags: [Arduino Mega, Peltier Modules, ERM & LRA Motors, DRV2605L, PWM, Laser Cutting, CAD, User Study]
---

<dl class="glance">
  <dt>Role</dt><dd>All mechanical CAD, most of the physical wiring, and all Arduino code</dd>
  <dt>Team</dt><dd>Hailey Wadell, Nyra Graham</dd>
  <dt>Context</dt><dd>Final project for MAE 6800 (Design and Control of Haptic Systems) at Cornell, taught by Prof. Cara Nunez</dd>
</dl>

## Overview

The Weather Machine is a compact haptic device designed to let users feel simplified weather conditions through temperature changes, airflow, and vibration. Inspired by the idea of reaching a hand out an open window, the system uses two Peltier modules, four fans, and a small array of vibration motors embedded in a palm-sized interface. During a user study, participants experienced several unlabeled stimuli and rated perceived temperature, wind, and vibration strength, as well as how closely each sensation matched a real weather event.

<div class="gallery" markdown="0">
  <figure>
    <img src="{{ '/assets/projects/haptic-weather-machine/window-frame.jpg' | relative_url }}" alt="Weather Machine enclosure with window frame door">
    <figcaption>Open the window to experience the weather</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/haptic-weather-machine/internal-components.jpg' | relative_url }}" alt="Labeled internal components">
    <figcaption>Internal components and electronics housing</figcaption>
  </figure>
</div>

## System Design

- **Enclosure:** 11" × 11" box of laser-cut ¼" acrylic with a wooden "window frame" door.
- **Palm station:** components mount flush with the top plate so the user's palm directly contacts the Peltier modules and vibration motors. It also houses the electronics.
- **Thermal:** two 12 V Peltier modules in opposite orientations, PWM-controlled through IRLZ44N MOSFETs, with two DS18B20 sensors and a safety threshold (about 19–28 °C).
- **Airflow:** one 80 mm and one 40 mm fan on each side wall for cross-flow.
- **Vibration:** two ERMs near the wrist for broad, low-frequency sensations and five LRAs in an inverted T toward the fingers, driven by two DRV2605L drivers through an I²C multiplexer.
- **Control:** an Arduino Mega runs each weather type through predefined functions from a single 12 V supply.

<figure>
  <img src="{{ '/assets/projects/haptic-weather-machine/wiring-diagram.jpg' | relative_url }}" alt="Wiring diagram">
  <figcaption>Wiring diagram of the palm station and how all components are powered and controlled (diagram by my teammate)</figcaption>
</figure>

## Development

We bench-tested each subsystem on its own first (fans, Peltiers, and the ERM and LRA drivers). Those tests showed that both the large fans and the Peltiers needed MOSFETs for speed and temperature control. Before committing to CAD, a cardboard mockup set the placement of the fans, palm station, and electronics. The final enclosure was then modeled and laser-cut from that layout.

<div class="gallery" markdown="0">
  <figure>
    <img src="{{ '/assets/projects/haptic-weather-machine/cardboard-mockup.jpg' | relative_url }}" alt="Cardboard mockup labeled with fan, heater/cooler, LRA/ERM, palm, and electronics positions">
    <figcaption>Cardboard mockup used to plan component placement</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/haptic-weather-machine/cad-perspective.jpg' | relative_url }}" alt="CAD model of the enclosure with fan mounts and the palm station">
    <figcaption>CAD model of the enclosure and palm station</figcaption>
  </figure>
</div>

<div class="gallery" markdown="0">
  <figure>
    <img src="{{ '/assets/projects/haptic-weather-machine/fan-bench-test.jpg' | relative_url }}" alt="Fan wired to an Arduino on a breadboard for bench testing">
    <figcaption>Fan bench test</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/haptic-weather-machine/erm-bench-test.jpg' | relative_url }}" alt="ERM vibration motor wired to a DRV2605L driver and an Arduino">
    <figcaption>ERM vibration motor on a DRV2605L driver</figcaption>
  </figure>
</div>

## Stimuli

| Weather | Thermal | Airflow | Vibration |
|---|---|---|---|
| Sunny Day | Mild heating | Light | None |
| Heavy Rainstorm | Cooling | High-speed | Dense bursts |
| Light Rainstorm | Low cooling | Light | Smaller, dispersed bursts |
| Windy Fall Day | Cooling | High-speed | None |
| Summer Rain | Mild heating | Light | Smaller, dispersed bursts |

## User Study & Results

Ten participants tried the device at an open-house demo. Each stimulus played for 15 seconds in a fixed order, and participants weren't told the options beforehand. After each one they rated temperature, airflow, and vibration and described the weather they felt. Not every participant tried every stimulus, so each condition has only a few ratings. The analysis and charts below are my teammate's.

- Sunny Day was rated warmest (M = 4.50) and Windy Fall Day coldest (M = 1.75).
- Heavy Rainstorm produced the strongest airflow (M = 3.86) and vibration (M = 3.14), and was most often labeled "realistic."
- Participants ranked stimuli correctly by relative strength, but the biggest gaps between intended and perceived intensity were at the high end, where increases became harder to notice.
- Participants who tend to feel warm rated mainly thermal stimuli warmer (Sunny Day, r = 0.78). With so few ratings per condition, these correlations are suggestive only.

<figure>
  <img src="{{ '/assets/projects/haptic-weather-machine/user-ratings.jpg' | relative_url }}" alt="Mean user ratings across weather stimuli">
  <figcaption>Mean user ratings for each perceptual dimension across the five weather stimuli</figcaption>
</figure>

<div class="gallery" markdown="0">
  <figure>
    <img src="{{ '/assets/projects/haptic-weather-machine/temp-actual-vs-perceived.jpg' | relative_url }}" alt="Actual vs perceived temperature">
    <figcaption>Temperature: actual vs. perceived</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/haptic-weather-machine/airflow-actual-vs-perceived.jpg' | relative_url }}" alt="Actual vs perceived airflow">
    <figcaption>Airflow: actual vs. perceived</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/haptic-weather-machine/vibration-actual-vs-perceived.jpg' | relative_url }}" alt="Actual vs perceived vibration">
    <figcaption>Vibration: actual vs. perceived</figcaption>
  </figure>
</div>

<figure>
  <img src="{{ '/assets/projects/haptic-weather-machine/thermal-bias.jpg' | relative_url }}" alt="Scatter plots of cold-warm bias vs perceived temperature for each stimulus">
  <figcaption>Each participant's cold–warm bias vs. perceived temperature, per stimulus</figcaption>
</figure>

## Limitations

- Few participants per stimulus, different participants on different days, and a fixed stimulus order.
- Without heat sinks, the Peltiers were slow to return to baseline between stimuli, and the temperature sensors drifted over the demo.

## Future Work

- Faster thermal sensing (thermistor or thermocouple) and heat sinks so the Peltiers return to baseline quickly.
- Periodic re-zeroing to handle temperature sensor drift.
- More participants and a more controlled study to measure detection thresholds and interactions between cues.

## Links

- [Arduino code (Mega_Weather_Script.ino)]({{ '/assets/projects/haptic-weather-machine/Mega_Weather_Script.ino' | relative_url }})
