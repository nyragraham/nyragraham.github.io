---
title: "GreensKeeper: Automated Plant Care"
subtitle: Senior Design Project · April – May 2025
summary: A smart plant shelf that automates watering, lighting, and monitoring for houseplants, controlled from a Raspberry Pi and a local Flask web dashboard.
thumbnail: /assets/projects/greenskeeper/thumb.jpg
order: 1
tags: [Raspberry Pi, Python, Flask, OpenCV, APScheduler, PCA9685, ADS1115, Peristaltic Pump, 3D Printing]
---

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

## Key Features

- Automated irrigation triggered by capacitive soil moisture sensors.
- Dimmable LED grow lighting controlled by PWM scheduling.
- Flask-based dashboard with live humidity data, pump/light status, and a camera feed.
- Modular hardware/software architecture supporting multi-plant scalability.

<figure>
  <video controls playsinline preload="metadata" poster="{{ '/assets/projects/greenskeeper/website-demo-poster.jpg' | relative_url }}" src="{{ '/assets/projects/greenskeeper/website-demo.mp4' | relative_url }}"></video>
  <figcaption>Website demonstration</figcaption>
</figure>

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
