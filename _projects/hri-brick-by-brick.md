---
title: "Brick by Brick: A Social Robot"
subtitle: Course Project, Cornell University · HRI '26 Student Design Challenge
summary: A mobile social robot that invites strangers in public spaces to build a shared LEGO structure together, using person tracking, an LQR heading controller, and an actuated drawer.
thumbnail: /assets/projects/hri-brick-by-brick/thumb.jpg
order: 5
tags: [ROS 2, Raspberry Pi 5, Intel RealSense D455, ODrive, LQR Control, TensorFlow Lite, Arduino, Human-Robot Interaction]
---

<dl class="glance">
  <dt>Role</dt><dd>Drivetrain and drawer hardware: integrated the hoverboard wheels with the ODrive motor controller, set up and tuned Pi-to-ODrive communication, built the servo-driven rack-and-pinion drawer, and handled overall hardware integration</dd>
  <dt>Team</dt><dd>Joyce Yang, Phillip Johnson, Nyra Graham, Karen Shamir</dd>
  <dt>Context</dt><dd>Cornell course project, published in the Companion Proceedings of the 21st ACM/IEEE International Conference on Human-Robot Interaction (HRI Companion '26) as part of the Student Design Challenge</dd>
</dl>

<figure>
  <img src="{{ '/assets/projects/hri-brick-by-brick/interaction-patterns.jpg' | relative_url }}" alt="Bricky the social robot and interaction patterns from field testing">
  <figcaption>Interaction patterns discovered during field testing</figcaption>
</figure>

## Overview

Social isolation in shared spaces threatens community cohesion and well-being. Inspired by public art projects, our robot invites individuals to collaborate on a shared LEGO structure by using expressive eye tracking, autonomous turning, and servo-actuated drawer movement. Field deployments in Cornell University spaces showed the robot effectively acted as a social catalyst: diverse participants contributed to a shared structure, and strangers initiated conversations about the robot.

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
- Drawer driven by a servo through a rack and pinion, controlled by an Arduino Uno over serial.
- 7" display showing animated eyes that follow detected people.

<figure class="portrait">
  <img src="{{ '/assets/projects/hri-brick-by-brick/robot.jpg' | relative_url }}" alt="The robot: a black column on two hoverboard wheels with a LEGO baseplate top, a screen showing cartoon eyes, a depth camera, and an open drawer of LEGO bricks">
  <figcaption>The finished robot: hoverboard drivetrain, RealSense camera, eye display, and the drawer open to offer bricks</figcaption>
</figure>

<figure>
  <video autoplay muted loop playsinline preload="metadata" poster="{{ '/assets/projects/hri-brick-by-brick/rack-and-pinion-poster.jpg' | relative_url }}" src="{{ '/assets/projects/hri-brick-by-brick/rack-and-pinion.mp4' | relative_url }}"></video>
  <figcaption>Bench test of the drawer's servo-driven rack and pinion</figcaption>
</figure>

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

The robot was deployed for 15–30 minute sessions in Duffield Atrium, Statler Lobby, and Gates Hall. Observed interactions fell into four categories: passive engagement, active human-robot interaction, human-to-human interaction, and unintended interactions (like being used as a coffee stand).

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

**What we learned:**

- Familiar features invite use. People treated the robot like everyday furniture (one participant set a coffee cup on it), and LEGO bricks felt approachable because there's no wrong way to place one.
- Mobility helped but wasn't critical. Even stationary, the drawer and eye tracking attracted participants.
- Turning to face a person before approaching is not the lowest-cost control strategy, but it read as more natural and made the robot more approachable.

## Limitations

These were short, single observational sessions on one campus, with approximate counts, so the results are exploratory rather than a controlled study.

## Links

- [Paper: Brick by Brick (ACM Digital Library)](https://doi.org/10.1145/3776734.3794595)
