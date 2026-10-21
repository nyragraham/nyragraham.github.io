---
title: Regional Farm Systems
subtitle: Co-founder · 2026
summary: An AI agent for small-farm operations and an agent-based simulation of a regional farm-to-buyer marketplace, built as a co-founder of an early-stage agriculture startup.
thumbnail: /assets/projects/rfs/thumb.jpg
order: 3
tags: [AI Agents, Agent-Based Simulation, TypeScript, Next.js, Supabase, SwiftUI, Claude Code, Drone Hardware]
---

<dl class="glance">
  <dt>Role</dt><dd>Co-founder. Product and technical architecture, the marketplace simulation, testing of the agent workflows, and drone hardware assembly and flight testing</dd>
  <dt>Team</dt><dd>Two co-founders, plus a few contributors. My co-founder built much of the web dashboards, the agent-testing framework, and the drones' flight software.</dd>
  <dt>Status</dt><dd>Prototype. The company started as FarmLocal, became Regional Farm Systems, and stopped later in 2026. No farm used it in production.</dd>
  <dt>Grant</dt><dd>Awarded a 2026 PA Agricultural Innovation Grant, which we declined when we stopped the company</dd>
</dl>

## Overview

Small farms keep their records in notebooks, binders, and hand-drawn field maps. Regional Farm Systems set out to turn those records into structured data, then use that data to help farms sell to local buyers and complete paperwork like organic certification. The product was Demeter, an AI agent a farmer could talk to instead of filling in forms.

<div class="gallery cols-2" markdown="0">
  <figure>
    <img src="{{ '/assets/projects/rfs/records-notebook.jpg' | relative_url }}" alt="Handwritten notebook page listing seedling varieties, tray sizes, and counts">
    <figcaption>Seeding notes in a notebook</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/rfs/records-map.jpg' | relative_url }}" alt="Hand-drawn field map with crops labeled along a row scale">
    <figcaption>A hand-drawn map of one season's field layout</figcaption>
  </figure>
</div>

<figure>
  <img src="{{ '/assets/projects/rfs/records-spreadsheet.jpg' | relative_url }}" alt="Printed seeding spreadsheet in a binder, covered in handwritten counts and notes">
  <figcaption>A printed seeding schedule, updated by hand through the season (farm name blurred)</figcaption>
</figure>

## Farmer Dashboard → Demeter

Our first prototype was a web dashboard for farmers: an advisor chat, planting calendar, field and bed map, inventory, buyer inbox with approve/counter/reject, and an organic-certification checker that tracked how complete each form was. We then moved to Demeter, an agent that could take all of those dashboard actions from a conversation, so a farmer would only need to talk to it.

I set the architecture: farm records as the source of truth, deterministic backend tools that do the actual reads and writes, and an agent layer that decides which tool to call. Claude Code wrote much of the implementation under my direction. My job was to break the work into specs, review what came back, and test the workflows end to end.

<figure>
  <img src="{{ '/assets/projects/rfs/architecture-whiteboard.jpg' | relative_url }}" alt="Whiteboard sketch of the Demeter stack: data sources feeding Supabase and a knowledge graph, CLI tools, and the Hermes agent connected to chat interfaces">
  <figcaption>Whiteboard plan for the Demeter stack and who owned each part (teammate's name blurred)</figcaption>
</figure>

<figure class="portrait phone">
  <video autoplay muted loop playsinline preload="metadata" poster="{{ '/assets/projects/rfs/demeter-app-poster.jpg' | relative_url }}" src="{{ '/assets/projects/rfs/demeter-app.mp4' | relative_url }}"></video>
  <figcaption>The Demeter iOS app with mock farm data: asking what's growing, sorting it by field, and checking organic-certification progress, then the farm log, crop tracker, and certification forms (2.5× speed, loading pauses cut)</figcaption>
</figure>

<figure>
  <video autoplay muted loop playsinline preload="metadata" poster="{{ '/assets/projects/rfs/farmer-dashboard-poster.jpg' | relative_url }}" src="{{ '/assets/projects/rfs/farmer-dashboard.mp4' | relative_url }}"></video>
  <figcaption>The farmer dashboard prototype with mock farm data: advisor, calendar, fields, certification, and marketplace inbox (3× speed)</figcaption>
</figure>

## Marketplace Simulation

Before any real farm or buyer was on the platform, I wanted to see how a regional marketplace would behave over a whole season. I specified and built an agent-based simulation where farmers and buyers trade on a daily tick: buyers send inquiries, farmers make offers, and each side approves, counters, or rejects reservations until orders are fulfilled.

- **Personalities.** Each agent has a profile (cautious, eager, or selective farmer; chef, bulk, or community buyer) with settings for risk, trust, and negotiation style.
- **Two decision modes.** A fast rules mode, and an LLM mode where Claude makes each agent's decisions.
- **Adaptive mode.** Personalities shift with experience. A buyer who keeps getting filled builds trust, and a farmer who keeps getting rejected changes how they negotiate.
- **Dashboard.** A live activity feed, a graph explorer showing who trades with whom, and a season summary with supply-and-demand gaps.

<figure>
  <video autoplay muted loop playsinline preload="metadata" poster="{{ '/assets/projects/rfs/sim-activity-poster.jpg' | relative_url }}" src="{{ '/assets/projects/rfs/sim-activity.mp4' | relative_url }}"></video>
  <figcaption>Activity feed: farmer and buyer agents making offers and approving reservations day by day</figcaption>
</figure>

<div class="gallery cols-2" markdown="0">
  <figure>
    <video autoplay muted loop playsinline preload="metadata" poster="{{ '/assets/projects/rfs/sim-graph-poster.jpg' | relative_url }}" src="{{ '/assets/projects/rfs/sim-graph.mp4' | relative_url }}"></video>
    <figcaption>Graph explorer: who trades with whom, and each agent's history (1.5× speed)</figcaption>
  </figure>
  <figure>
    <video autoplay muted loop playsinline preload="metadata" poster="{{ '/assets/projects/rfs/sim-summary-poster.jpg' | relative_url }}" src="{{ '/assets/projects/rfs/sim-summary.mp4' | relative_url }}"></video>
    <figcaption>Season summary: market health, supply-and-demand gaps, and top relationships (1.5× speed)</figcaption>
  </figure>
</div>

The farm names are real Centre County, PA farms, used for realism. Their products, prices, and behavior are all invented.

Running full seasons turned up problems that unit tests didn't catch: farmers promising more than they had in stock, buyers flooding farmers' inboxes, duplicate buyers, and farmers in LLM mode not responding to the marketplace at all. I logged each one, turned it into a fix task, and re-ran the season.

## Testing the Agent

An LLM agent can fail in ways you can't list ahead of time, so I designed a testing arena for Demeter. A simulated user with a private goal (the "truth" only the test knows) talks to the real agent. Afterward, the arena checks two things: that the goal was met, and that nothing forbidden happened, like writing to another farm's records or creating duplicate entries. It also generates adversarial users that try to trip the agent up. My co-founder implemented most of the framework from that design, and it caught a real bug in the live Demeter integration.

## Drone Hardware

Alongside the software, we built three prototype drones: a large agricultural spray quadcopter, a hexacopter, and a small quadcopter for testing. I assembled, wired, and soldered the power, flight controller (Pixhawk 6C), camera, and communications hardware, and took part in propeller-free bench tests and live flights. When the drone first wouldn't power up, I traced the problem to a mismatch between the battery cable and the power distribution board. The autonomous and natural-language flight software was mainly my co-founder's work.

<div class="gallery cols-2" markdown="0">
  <figure>
    <img src="{{ '/assets/projects/rfs/drones.jpg' | relative_url }}" alt="A large quadcopter with an orange spray tank next to a small quadcopter on grass">
    <figcaption>The spray quadcopter and the small test quadcopter</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/rfs/hexacopter.jpg' | relative_url }}" alt="Black carbon-fiber hexacopter on grass beside a building, with a dog sniffing underneath">
    <figcaption>The hexacopter, with a curious inspector</figcaption>
  </figure>
</div>

<div class="gallery cols-2" markdown="0">
  <figure>
    <img src="{{ '/assets/projects/rfs/pixhawk-pdb.jpg' | relative_url }}" alt="Pixhawk 6C flight controller beside a power distribution board with soldered battery leads">
    <figcaption>Pixhawk 6C flight controller and power distribution board, with the battery leads soldered on</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/rfs/sprayer-build.jpg' | relative_url }}" alt="Spray quadcopter frame mid-assembly with motors mounted and wiring exposed, small quadcopter on the floor in front">
    <figcaption>The spray quadcopter mid-assembly, before propellers, with the small quadcopter in front</figcaption>
  </figure>
</div>

<figure class="portrait">
  <video controls playsinline preload="metadata" poster="{{ '/assets/projects/rfs/drone-voice-poster.jpg' | relative_url }}" src="{{ '/assets/projects/rfs/drone-voice.mp4' | relative_url }}"></video>
  <figcaption>Me giving the hexacopter spoken commands in the field (with sound)</figcaption>
</figure>

## Limitations

- Everything here is a prototype. No farm or buyer used the platform in production, and we have no adoption or field results.
- The simulation's farms and buyers act on invented personalities and data, so it shows how the system behaves, not how a real market would.
- LLM mode in the simulation was too slow to run full seasons quickly.
- The drones flew prototype demonstrations only. They were never used for real farm work.
