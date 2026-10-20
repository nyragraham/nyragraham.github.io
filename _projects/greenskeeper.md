---
title: "GreensKeeper: Automated Plant Care"
subtitle: Senior Design Project · April – May 2025
summary: A smart plant shelf that automates watering, lighting, and monitoring for houseplants, controlled from a Raspberry Pi and a local Flask web dashboard.
thumbnail: /assets/projects/greenskeeper/thumb.jpg
order: 1
tags: [Raspberry Pi, Python, Flask, OpenCV, APScheduler, PCA9685, ADS1115, Peristaltic Pump, 3D Printing]
---

<figure>
  <video controls playsinline preload="metadata" poster="{{ '/assets/projects/greenskeeper/walkthrough-poster.jpg' | relative_url }}" src="{{ '/assets/projects/greenskeeper/walkthrough.mp4' | relative_url }}"></video>
  <figcaption>Narrated walkthrough of the finished prototype: wiring, web dashboard, and the pump and grow light responding to manual controls</figcaption>
</figure>

<dl class="glance">
  <dt>Role</dt><dd>Solo project: shelf design and CAD, 3D printing, wiring and soldering, Python/Flask software</dd>
  <dt>Context</dt><dd>Senior design project for my B.S. at Cornell. Advisor: Prof. Douglas MacMartin</dd>
  <dt>Hardware</dt><dd>Raspberry Pi 4, ADS1115 ADC, PCA9685 PWM driver, IRLZ44N MOSFET, TB6612FNG motor driver, 12 V peristaltic pump, capacitive soil moisture sensor, full-spectrum LED panel, USB camera</dd>
</dl>

## Overview

GreensKeeper is a smart plant shelf designed to automate watering, lighting, and monitoring for houseplants. Built on a Raspberry Pi with Python control scripts and a Flask web interface, it provides real-time soil moisture sensing, pump activation, and dimmable LED grow lights, all viewable and controllable from a local web dashboard.

<figure>
  <img src="{{ '/assets/projects/greenskeeper/system-diagram.png' | relative_url }}" alt="GreensKeeper system diagram">
  <figcaption>GreensKeeper system diagram</figcaption>
</figure>

<figure>
  <img src="{{ '/assets/projects/greenskeeper/wiring-diagram.png' | relative_url }}" alt="GreensKeeper wiring diagram">
  <figcaption>GreensKeeper wiring diagram</figcaption>
</figure>

<figure>
  <img src="{{ '/assets/projects/greenskeeper/electronics.jpg' | relative_url }}" alt="Self-soldered electronic unit">
  <figcaption>Self-soldered electronic unit</figcaption>
</figure>

## Shelf Design

The frame is two 6 × 1 × 24 in pine boards joined by 3D-printed hexagonal legs and end brackets, modeled in Fusion 360 and printed in matte black PLA. The top shelf carries the electronics and grow light, and the bottom shelf holds the pots.

<figure>
  <img src="{{ '/assets/projects/greenskeeper/shelf.jpg' | relative_url }}" alt="Assembled GreensKeeper shelf with two potted plants on the lower shelf and electronics on top">
  <figcaption>Assembled shelf with plants</figcaption>
</figure>

<div class="gallery" markdown="0">
  <figure>
    <img src="{{ '/assets/projects/greenskeeper/shelf-cad.jpg' | relative_url }}" alt="Fusion 360 model of the shelf with hexagonal legs">
    <figcaption>Fusion 360 model of the shelf</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/greenskeeper/print-plate.jpg' | relative_url }}" alt="Hexagonal leg segments and brackets arranged on a 3D printer build plate">
    <figcaption>Leg segments and brackets laid out for printing</figcaption>
  </figure>
</div>

## Build

I prototyped each subsystem on a breadboard before moving the electronics onto the top shelf. The moisture sensor (through the ADS1115) and the grow light (through the PCA9685) both sit on the Pi's I²C bus, which left GPIO pins free for the pump driver.

<div class="gallery cols-2" markdown="0">
  <figure>
    <img src="{{ '/assets/projects/greenskeeper/breadboard-prototype.jpg' | relative_url }}" alt="Breadboard prototype wired to the LED grow panel">
    <figcaption>Early breadboard prototype driving the LED panel</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/greenskeeper/electronics-top.jpg' | relative_url }}" alt="Top view of the shelf electronics: Raspberry Pi with PiTFT screen, breadboard, and protoboard">
    <figcaption>Electronics on the top shelf</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/greenskeeper/moisture-sensor.jpg' | relative_url }}" alt="Capacitive moisture sensor and watering tube in a terracotta pot">
    <figcaption>Moisture sensor and watering tube in the pot</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/greenskeeper/pump.jpg' | relative_url }}" alt="12 V peristaltic pump mounted on the shelf">
    <figcaption>12 V peristaltic pump</figcaption>
  </figure>
</div>

## Key Features

- Scheduled automated watering, with capacitive soil moisture readings shown live on the dashboard.
- Dimmable LED grow lighting controlled by PWM scheduling.
- Flask-based dashboard with live humidity data, pump/light status, and a camera feed.
- Modular hardware/software architecture supporting multi-plant scalability.

## Testing

I tested each component on its own before integrating:

- **Moisture sensor:** placed in dry and wet soil to check the voltage response and the linear mapping to percent.
- **Grow light:** stepped through each PWM level (off, low, medium, high) from a terminal script.
- **Pump:** triggered from GPIO and timed. A 2-second pulse gave consistent delivery, so I used that for each watering.

I then ran the full system with a live plant, checking scheduled light on/off, timed watering, and the dashboard's manual controls.

<figure>
  <video controls playsinline preload="metadata" poster="{{ '/assets/projects/greenskeeper/light-test-terminal-poster.jpg' | relative_url }}" src="{{ '/assets/projects/greenskeeper/light-test-terminal.mp4' | relative_url }}"></video>
  <figcaption>Bench test: setting grow-light brightness from a terminal script through the PCA9685</figcaption>
</figure>

The moisture readings were noisy and would need better calibration. Wiring got hard to manage as parts were added, so late in the project I reorganized the breadboard around labeled power rails. The live camera stream was the hardest piece to integrate without slowing down the rest of the system. All testing was indoors over a short period.

## Results

- Successfully built a functional single-slot system with reliable watering and scheduled lighting.
- Created a flexible JSON-based plant database for individualized care routines.
- Strengthened embedded systems and full-stack integration skills.

## Future Work

- Expand hardware to multi-slot support with independent schedules.
- Add event logging and sensor history visualization.
- Develop mobile/cloud connectivity for remote control.

## Links

- [GitHub Repository](https://github.com/nyragraham/GreensKeeper)
- [Technical Report (PDF)]({{ '/assets/projects/greenskeeper/greenskeeper-report.pdf' | relative_url }})
