---
title: Cornell Mars Rover
subtitle: Team Lead & Systems Engineer · 2021 – 2025
summary: Led Cornell's 65-person Mars Rover team through design, build, and competition at the University Rover Challenge, with hands-on work in rugged mechanical design, actuators, harnessing, and rapid prototyping.
thumbnail: /assets/projects/cmr/thumb.jpg
order: 1
tags: [CAN Bus, Wiring Harnesses, Arduino, Actuators, Additive Manufacturing, Sheet Metal, Machining, Systems Engineering]
---

<dl class="glance">
  <dt>Role</dt><dd>AstroTech subteam member (2021–23) → AstroTech mechanical lead (2023–24) → Team lead (2024–25)</dd>
  <dt>Team</dt><dd>About 65 students across arm, drives, electrical, software, science, and business subteams</dd>
  <dt>Competition</dt><dd>University Rover Challenge (Mars Society), Hanksville, Utah. Competed in May 2023, 2024, and 2025.</dd>
  <dt>Rovers</dt><dd>Atlas (2023), Hyperion (2024), Cayuga (2025)</dd>
</dl>

<figure>
  <img src="{{ '/assets/projects/cmr/rover-closeup.jpg' | relative_url }}" alt="Hyperion, a four-wheeled rover with a robotic arm and radio mast, in the Utah desert">
  <figcaption>Hyperion at the University Rover Challenge in Hanksville, Utah (2024)</figcaption>
</figure>

## Overview

Led Cornell's Mars Rover Project Team through design, build, and competition at the University Rover Challenge, an international robotics competition in Utah's Mars-analog desert. Managed a 65-person interdisciplinary team and contributed directly to rugged mechanical design, actuator integration, electrical harnessing, and rapid prototyping for extreme environments.

## AstroTech: Onboard Science Hardware

AstroTech builds the rover's onboard science lab: the mechanisms that collect soil, split it into samples, and run those samples through chemical and spectroscopic tests for signs of life. The system is a set of smaller mechanisms that have to work together, and it is redesigned every year along with the rest of the rover.

- **2022–23:** I independently designed and integrated the [analysis chamber]({{ '/projects/analysis-chamber/' | relative_url }}), a single-motor carousel that fills and positions cuvettes for the spectrometer.
- **2023–24:** As AstroTech mechanical lead, I designed much of Hyperion's soil collection system. A four-bar linkage lowers a scoop to the ground, and each sample is split into chambers: one sealed to deliver to the judges, one mixed with dilute acid to check for carbonates, and the rest mixed with solvent, filtered, and pumped into a cuvette for the onboard spectrometer. The scoop was first driven by a DC motor and later by a servo for positional control.
- Wrote Arduino firmware to integrate servos and sensors via I²C with the central comms board.
- Reinforced linkage designs (3D printed + sheet metal) to reduce backlash and maintain motion reliability.

<figure>
  <video autoplay muted loop playsinline preload="metadata" poster="{{ '/assets/projects/cmr/soil-collection-poster.jpg' | relative_url }}" src="{{ '/assets/projects/cmr/soil-collection.mp4' | relative_url }}"></video>
  <figcaption>Hyperion's soil collection system: scoop, sample chambers, and pumps to the spectrometer (from our 2024 SAR video)</figcaption>
</figure>

<div class="gallery cols-2" markdown="0">
  <figure>
    <video autoplay muted loop playsinline preload="metadata" poster="{{ '/assets/projects/cmr/scoop-prototype-poster.jpg' | relative_url }}" src="{{ '/assets/projects/cmr/scoop-prototype.mp4' | relative_url }}"></video>
    <figcaption>Early printed scoop prototype, turned by hand to check how it picks up and holds soil</figcaption>
  </figure>
  <figure>
    <video autoplay muted loop playsinline preload="metadata" poster="{{ '/assets/projects/cmr/torque-test-poster.jpg' | relative_url }}" src="{{ '/assets/projects/cmr/torque-test.mp4' | relative_url }}"></video>
    <figcaption>Bench torque test: resisting the scoop by hand while the motor drives it</figcaption>
  </figure>
</div>

## Rugged Mechanical Systems

- Designed and field-tested rover systems against sand, dust storms, heat, and vibration.
- Improved reliability of electronic enclosures via dust sealing, thermal coatings, and insulation.
- Adapted to motor failures and overheating during competition, applying lessons in ruggedization and cooling.

<figure>
  <video autoplay muted loop playsinline preload="metadata" poster="{{ '/assets/projects/cmr/hyperion-terrain-poster.jpg' | relative_url }}" src="{{ '/assets/projects/cmr/hyperion-terrain.mp4' | relative_url }}"></video>
  <figcaption>Hyperion's suspension on steps and curbs, at real-time speed (from our 2024 SAR video)</figcaption>
</figure>

## Wiring Harnesses & Electrical Integration

- Hand-built CAN and power harnesses routed through rotating subsystems (6-DOF arm, wheels, swerve drive).
- Debugged CAN bus communication with analyzers and established continuity/termination standards.
- Structured power distribution with modular PDBs for isolated testing and subsystem debugging.

<div class="gallery cols-2" markdown="0">
  <figure>
    <img src="{{ '/assets/projects/cmr/ecore.jpg' | relative_url }}" alt="Looking through the open electronics core of Cayuga, full of wiring and connectors">
    <figcaption>Me inside Cayuga's electronics core during wiring (2025)</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/cmr/servo-wiring.jpg' | relative_url }}" alt="Microcontroller board, breadboard, and servo leads on a bench">
    <figcaption>Arduino servo control for the soil scoop and the arm's end effector, wired up on the bench (2024)</figcaption>
  </figure>
</div>

## Rapid Prototyping & Fabrication

- Led team's additive manufacturing: introduced PETG for chemical resistance, TPU for rover wheels.
- Maintained and upgraded printers (Ender → Prusa → Bambu) to support nonstop iteration.
- Designed and printed mounts, enclosures, and housings; deployed 3D printer on-site in Utah for emergency repairs.
- Combined in-house machining and vendor sheet-metal fabrication for fast, lightweight parts.

<figure>
  <video autoplay muted loop playsinline preload="metadata" poster="{{ '/assets/projects/cmr/machining-poster.jpg' | relative_url }}" src="{{ '/assets/projects/cmr/machining.mp4' | relative_url }}"></video>
  <figcaption>Machining brackets for the drive system on a manual mill (2025)</figcaption>
</figure>

<div class="gallery cols-2" markdown="0">
  <figure>
    <img src="{{ '/assets/projects/cmr/hotel-printing.jpg' | relative_url }}" alt="3D printer running on a hotel counter next to a laptop">
    <figcaption>Printing replacement parts in our hotel at competition (2023)</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/cmr/gearbox-assembly.jpg' | relative_url }}" alt="Machined motor housings, wheel hubs, and bearings laid out on a workbench">
    <figcaption>Motor and gearbox parts before I assembled them (2025)</figcaption>
  </figure>
</div>

## Testing

Each year the team splits into four task teams, one for each URC mission. Each task team brings the rover up to full functionality step by step, and the whole competition team runs two full mock competitions before Utah.

<figure>
  <video autoplay muted loop playsinline preload="metadata" poster="{{ '/assets/projects/cmr/testing-2025-poster.jpg' | relative_url }}" src="{{ '/assets/projects/cmr/testing-2025.mp4' | relative_url }}"></video>
  <figcaption>Winter testing of Cayuga on stairs, snow, and pavement, sped up (from our 2025 SAR video)</figcaption>
</figure>

<figure class="portrait">
  <img src="{{ '/assets/projects/cmr/leg-joint-failure.jpg' | relative_url }}" alt="Rover on the lab floor with one leg separated at its epoxy joint">
  <figcaption>Testing failure (2024): a leg's epoxy joint between the carbon fiber tube and aluminum fitting let go. The cause was a weak first epoxy job plus a sideways load on the leg, because one moteus motor controller was configured differently from the others.</figcaption>
</figure>

## Field Deployment Highlights

- Managed competition logistics and led debugging under pressure in Utah.
- Recovered from a failed drive motor and overheating Jetson computer on-site.
- Developed contingency workflows to keep the rover operational despite harsh terrain and environmental stressors.

<figure>
  <img src="{{ '/assets/projects/cmr/rover-landscape.jpg' | relative_url }}" alt="Cayuga driving across a gravel plain below banded desert hills">
  <figcaption>Cayuga in Utah's Mars-analog desert (2025)</figcaption>
</figure>

<div class="gallery cols-2" markdown="0">
  <figure>
    <img src="{{ '/assets/projects/cmr/atlas-comp.jpg' | relative_url }}" alt="Atlas, a six-wheeled rover with an arm and antenna mast, on rocky desert ground">
    <figcaption>Atlas at competition (2023)</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/cmr/carrying-arm.jpg' | relative_url }}" alt="Me in a sun hat carrying Hyperion's robotic arm over my shoulder">
    <figcaption>Carrying Hyperion's arm at competition (2024)</figcaption>
  </figure>
</div>

## Cross-Disciplinary Collaboration & Leadership

- Coordinated mechanical, electrical, software, and science subteams for integration milestones.
- Mediated conflicts and made system-level tradeoffs (e.g., reverting to a proven arm for testing while developing a new one in parallel).
- Mentored teammates individually, tailoring guidance to learning styles and balancing autonomy with support.
- Directed reviews (PDR, SAR, design reviews) and documented integration in Confluence.

## System Acceptance Review Videos

Each spring the team submits a System Acceptance Review (SAR) video to qualify for URC. The 2025 video was my year as team lead.

<iframe src="https://www.youtube-nocookie.com/embed/HoFIuPtQGmM" title="Cornell Mars Rover URC SAR 2025" allow="accelerometer; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

- [SAR 2025: Cayuga](https://www.youtube.com/watch?v=HoFIuPtQGmM) (Team Lead)
- [SAR 2024: Hyperion](https://www.youtube.com/watch?v=2INSaNfNNKU&t=242s) (AstroTech Mechanical Lead; my section starts at 4:02)
- [SAR 2023: Atlas](https://www.youtube.com/watch?v=O57xvhn_Gn4) (AstroTech Subteam Member)
- [SAR 2022: Orion](https://www.youtube.com/watch?v=K9QMXXtJkQI) (AstroTech Subteam Member)

## Links

- [Cornell Mars Rover team website](https://marsrover.engineering.cornell.edu/)
