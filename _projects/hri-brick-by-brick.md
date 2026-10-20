---
title: "Brick by Brick: A Social Robot"
subtitle: HRI Companion '26 · Edinburgh, Scotland
summary: A mobile social robot that invites strangers in public spaces to build a shared LEGO structure together, using person tracking, an LQR heading controller, and an actuated drawer.
thumbnail: /assets/projects/hri-brick-by-brick/thumb.jpg
order: 5
tags: [ROS 2, Raspberry Pi 5, Intel RealSense D455, ODrive, LQR Control, TensorFlow Lite, Arduino, Human-Robot Interaction]
---

**Team:** Joyce Yang, Phillip Johnson, Nyra Graham, Karen Shamir<br>
**Published in:** Companion Proceedings of the 21st ACM/IEEE International Conference on Human-Robot Interaction (HRI '26)

<figure>
  <img src="{{ '/assets/projects/hri-brick-by-brick/interaction-patterns.jpg' | relative_url }}" alt="Bricky the social robot and interaction patterns from field testing">
  <figcaption>Interaction patterns discovered during field testing</figcaption>
</figure>

## Overview

Social isolation in shared spaces threatens community cohesion and well-being. Inspired by public art projects, our robot invites individuals to collaborate on a shared LEGO structure by using expressive eye tracking, autonomous turning, and servo-actuated drawer movement. Field deployments in Cornell University spaces showed the robot effectively acted as a social catalyst: diverse participants contributed to a shared structure, and strangers initiated conversations about the robot.

<figure>
  <video controls playsinline preload="metadata" poster="{{ '/assets/projects/hri-brick-by-brick/video-poster.jpg' | relative_url }}" src="{{ '/assets/projects/hri-brick-by-brick/brick-by-brick-video.mp4' | relative_url }}"></video>
  <figcaption>Project video: Bricky the social robot in the field</figcaption>
</figure>

## Design Process

Low-fidelity prototypes tested whether a static, table-like robot with a LEGO base plate and a hidden operator presenting bricks could engage passersby. They showed that a front-facing drawer and a face were essential, and that mobility would help turn curiosity into full interactions.

<figure>
  <img src="{{ '/assets/projects/hri-brick-by-brick/low-fi-prototypes.jpg' | relative_url }}" alt="Low-fidelity prototype sketch and field testing">
  <figcaption>Low-fidelity prototype sketch and field testing</figcaption>
</figure>

<figure>
  <img src="{{ '/assets/projects/hri-brick-by-brick/storyboard.jpg' | relative_url }}" alt="Interaction flow storyboard">
  <figcaption>Interaction flow: approach, attract, participate, attract, connect</figcaption>
</figure>

## Hardware

- Hoverboard wheels driven by an ODrive motor controller, commanded from a Raspberry Pi 5.
- Intel RealSense D455 stereo/depth camera for person detection and distance.
- Drawer actuated by a servo and linear actuator through an Arduino Uno over serial.
- 7" display showing animated eyes that follow detected people.

## Software & Control

- **ROS 2** middleware connecting camera, screen, controller, drawer, and teleoperation nodes.
- **Person detection** with TensorFlow Lite MobileNet bounding boxes, using depth to find the closest person.
- **LQR heading controller** to turn toward the person, followed by a low-speed proportional controller to approach and stop at a safety distance.
- **Teleoperation** via an Xbox controller for deadlocks and stop commands.
- **Tiered safety:** the robot only moves when a person is detected, always stops within stopping distance, and can be stopped by a teleoperator at any time. There were zero collisions during field testing.

<div class="gallery" markdown="0">
  <figure>
    <img src="{{ '/assets/projects/hri-brick-by-brick/ros2-network.jpg' | relative_url }}" alt="ROS 2 network overview">
    <figcaption>ROS 2 network overview</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/hri-brick-by-brick/person-detection.jpg' | relative_url }}" alt="Person detection bounding box and midpoint">
    <figcaption>Bounding box and midpoint output</figcaption>
  </figure>
</div>

## Field Deployments

The robot was deployed for 15-minute sessions in Duffield Atrium, Statler Lobby, and Gates Hall. Observed interactions fell into four categories: passive engagement, active human-robot interaction, human-to-human interaction, and unintended interactions (like being used as a coffee stand).

<figure>
  <img src="{{ '/assets/projects/hri-brick-by-brick/deployments.jpg' | relative_url }}" alt="LEGO structure before and after deployments">
  <figcaption>The robot pre-deployment at Gates Hall, after a 15-minute session at Gates Hall, and after a 15-minute session at Statler Lobby</figcaption>
</figure>

| Metric | Duffield Atrium | Statler Lobby | Gates Hall |
|---|---|---|---|
| People passed by | ~50 | ~93 | ~23 |
| Passive | ~44 | ~23 | ~15 |
| Active | 5 | 8 | 5 |
| Human-to-human | 2 | 4 | 0 |

## Links

- [Paper: Brick by Brick (ACM Digital Library)](https://doi.org/10.1145/3776734.3794595)
