---
title: Automated 3D Plant Phenotyping
subtitle: M.Eng. Project, Mechanical Engineering (Robotics) · Fall 2025
summary: An end-to-end pipeline that turns Intel RealSense RGB-D captures of hydroponic plants into 3D leaf tip and base trajectories, using Mask R-CNN leaf segmentation and PCA-based geometry.
thumbnail: /assets/projects/meng-plant-phenotyping/thumb.jpg
order: 5
tags: [Python, Intel RealSense D435i, Open3D, PyTorch, Mask R-CNN, LabelMe, PCA, Point Clouds]
---

**Advisor:** Sunghwan Jung, Cornell University

## Overview

Plant circumnutation and leaf motion contain information about growth dynamics and environmental response, but are difficult to measure quantitatively at scale. This project develops a complete engineering pipeline that converts raw data from an Intel RealSense D435i depth camera into structured trajectories of leaf tip and base positions over time.

The system takes as input a time series of synchronized RGB images and 3D point clouds captured in a hydroponic growth chamber. It performs minimal geometric preprocessing to preserve delicate leaf structure, uses a machine-learning model trained on LabelMe polygon annotations to segment individual leaves in 2D, and then back-projects these masks into 3D to isolate per-leaf point clouds. For each leaf, principal component analysis (PCA) is used to estimate a main axis, and geometric rules locate the leaf base and tip without any additional machine learning.

<figure>
  <img src="{{ '/assets/projects/meng-plant-phenotyping/pipeline.jpg' | relative_url }}" alt="Six-stage pipeline diagram">
  <figcaption>End-to-end pipeline from raw capture to leaf tip/base trajectories</figcaption>
</figure>

## Hardware Setup

All trials were conducted in a small hydroponic growth chamber built for the plant circumnutation study. A RealSense D435i is mounted above a PVC hydroponic rail holding bean plants, inside a chamber lined with reflective foil under artificial lighting.

<figure>
  <img src="{{ '/assets/projects/meng-plant-phenotyping/growth-chamber.jpg' | relative_url }}" alt="Hydroponic growth chamber with RealSense camera">
  <figcaption>Hydroponic chamber and RealSense D435i mounting configuration</figcaption>
</figure>

## Capture Pipeline

- Synchronized PNG + PLY captures with ISO 8601 timestamps, organized into per-trial folders (PNG, Point Cloud, NPZ, Logs).
- Axis-aligned cropping controlled by a `capture_bounds.json` file, applied at capture time.
- Automated multi-hour trials using Windows Task Scheduler, capturing every five minutes.
- Fixed early reliability issues: depth scaling, USB bandwidth drops, and empty PLY files.

Preprocessing was kept intentionally minimal. Statistical outlier removal and RANSAC plane fitting were tested but abandoned because they removed real leaf points or truncated leaf bases.

## Machine-Learning Leaf Segmentation

Leaves were annotated with polygons in LabelMe, converted to COCO format, and used to fine-tune a torchvision Mask R-CNN (ResNet backbone) on fewer than 20 labeled images, with flips, crops, and color jitter for augmentation.

<div class="gallery" markdown="0">
  <figure>
    <img src="{{ '/assets/projects/meng-plant-phenotyping/labelme-annotations.jpg' | relative_url }}" alt="LabelMe polygon annotations">
    <figcaption>LabelMe polygon annotations</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/meng-plant-phenotyping/maskrcnn-overlay.jpg' | relative_url }}" alt="Mask R-CNN segmentation overlay">
    <figcaption>Mask R-CNN segmentation on an RGB frame</figcaption>
  </figure>
</div>

## 2D Masks to 3D Leaves

Using camera intrinsics and extrinsics cached per frame, each 3D point is projected into the image and assigned to the leaf mask it falls inside. Within each leaf, a lightweight z-band clustering step keeps the largest contiguous band along the camera axis to prune floating fragments.

<figure>
  <img src="{{ '/assets/projects/meng-plant-phenotyping/3d-leaf-segmentation.jpg' | relative_url }}" alt="3D point cloud with each leaf in a different color">
  <figcaption>3D point cloud with distinct colors for each leaf</figcaption>
</figure>

## Base and Tip Detection

PCA on each leaf cluster gives its main axis. The tip is the point with the maximum projection along that axis and the base is the minimum. An earlier plan to train a model to predict bases and tips directly in 3D was replaced by this geometric method, which transfers much more robustly across trials.

<figure>
  <img src="{{ '/assets/projects/meng-plant-phenotyping/tip-vs-ground-truth.jpg' | relative_url }}" alt="Predicted leaf tips compared to ground truth">
  <figcaption>Multi-leaf tip predictions (red) compared to manual ground truth (green)</figcaption>
</figure>

## Results

Trials ran 24–36 hours with captures every five minutes (roughly 250–450 frames each). The resulting trajectories capture millimeter-scale leaf motion that would be difficult to quantify manually.

<div class="gallery" markdown="0">
  <figure>
    <img src="{{ '/assets/projects/meng-plant-phenotyping/birds-eye-trajectories.jpg' | relative_url }}" alt="Bird's-eye leaf tip trajectories">
    <figcaption>Bird's-eye (x–y) trajectories of three leaf tips</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/meng-plant-phenotyping/oscillations-vs-time.jpg' | relative_url }}" alt="Leaf tip oscillations over time">
    <figcaption>Δx, Δy, and Δz vs. time for the same three leaves</figcaption>
  </figure>
</div>

## Future Work

- Curved-axis (spline) fitting for strongly curved leaves.
- Temporal smoothing and tracking across frames.
- A larger labeled dataset across plant stages and lighting conditions.
- Real-time integration with ROS 2.
- Higher-level metrics such as circumnutation amplitude, frequency, and direction.

## Links

- [M.Eng. Final Report (PDF)]({{ '/assets/projects/meng-plant-phenotyping/meng-final-report.pdf' | relative_url }})
