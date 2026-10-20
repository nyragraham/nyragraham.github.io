---
title: Automated 3D Plant Phenotyping
subtitle: M.Eng. Project, Mechanical Engineering (Robotics) · Fall 2025
summary: An end-to-end pipeline that turns Intel RealSense RGB-D captures of hydroponic plants into 3D leaf tip and base trajectories, using Mask R-CNN leaf segmentation and PCA-based geometry.
thumbnail: /assets/projects/meng-plant-phenotyping/thumb.jpg
order: 4
tags: [Python, Intel RealSense D435i, Open3D, PyTorch, Mask R-CNN, LabelMe, PCA, Point Clouds]
---

<dl class="glance">
  <dt>Role</dt><dd>Camera capture and data analysis: capture scripts, leaf segmentation, 3D base/tip extraction, and trajectory plots</dd>
  <dt>Context</dt><dd>M.Eng. project at Cornell, advised by Prof. Sunghwan Jung. I worked with a PhD student on the lab's plant circumnutation study; the hydroponic growth chamber was existing lab equipment.</dd>
  <dt>Tools</dt><dd>Intel RealSense D435i, Python, Open3D, PyTorch/torchvision, LabelMe, NumPy, Matplotlib</dd>
</dl>

## Overview

Plant circumnutation and leaf motion contain information about growth dynamics and environmental response, but are difficult to measure quantitatively at scale. This project develops a complete engineering pipeline that converts raw data from an Intel RealSense D435i depth camera into structured trajectories of leaf tip and base positions over time.

The system takes as input a time series of synchronized RGB images and 3D point clouds captured in a hydroponic growth chamber. It performs minimal geometric preprocessing to preserve delicate leaf structure, uses a machine-learning model trained on LabelMe polygon annotations to segment individual leaves in 2D, and then back-projects these masks into 3D to isolate per-leaf point clouds. For each leaf, principal component analysis (PCA) is used to estimate a main axis, and geometric rules locate the leaf base and tip without any additional machine learning.

<figure>
  <img src="{{ '/assets/projects/meng-plant-phenotyping/pipeline.jpg' | relative_url }}" alt="Six-stage pipeline diagram">
  <figcaption>End-to-end pipeline from raw capture to leaf tip/base trajectories</figcaption>
</figure>

## Hardware Setup

All trials were run in the lab's existing hydroponic growth chamber. A RealSense D435i is mounted above a PVC hydroponic rail holding bean plants, inside a chamber lined with reflective foil under artificial lighting.

<figure class="portrait">
  <img src="{{ '/assets/projects/meng-plant-phenotyping/growth-chamber.jpg' | relative_url }}" alt="Hydroponic growth chamber with RealSense camera">
  <figcaption>Hydroponic chamber and RealSense D435i mounting configuration</figcaption>
</figure>

## Capture Pipeline

I heavily adapted the lab's existing snapshot script into an automated capture system:

- Synchronized PNG + PLY captures with ISO 8601 timestamps, organized into per-trial folders (PNG, Point Cloud, NPZ, Logs).
- Axis-aligned cropping controlled by a `capture_bounds.json` file, applied at capture time.
- Automated captures every five minutes using Windows Task Scheduler, about 9 hours a day while the chamber lights were on. Each trial totaled 24–36 hours of capture (roughly 250–450 frames).
- Logging and repeated test captures to track down depth-scaling errors, USB bandwidth drops, and empty PLY files. Empty frames are flagged and skipped.

Preprocessing was kept intentionally minimal. Statistical outlier removal and RANSAC plane fitting were tested but abandoned because they removed real leaf points or truncated leaf bases.

<div class="gallery" markdown="0">
  <figure>
    <img src="{{ '/assets/projects/meng-plant-phenotyping/crop-before.jpg' | relative_url }}" alt="Raw point cloud showing the full chamber, rail, and background clutter">
    <figcaption>Raw point cloud: full chamber, rail, and background clutter</figcaption>
  </figure>
  <figure>
    <img src="{{ '/assets/projects/meng-plant-phenotyping/crop-after.jpg' | relative_url }}" alt="Same frame after axis-aligned cropping around the plant">
    <figcaption>Same frame after the axis-aligned crop around the plant</figcaption>
  </figure>
</div>

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
  <img src="{{ '/assets/projects/meng-plant-phenotyping/mask-to-3d.jpg' | relative_url }}" alt="2D leaf masks on an RGB frame next to the matching colored 3D point cloud">
  <figcaption>2D leaf masks (left) mapped onto the 3D point cloud (right)</figcaption>
</figure>

<figure class="portrait">
  <img src="{{ '/assets/projects/meng-plant-phenotyping/3d-leaf-segmentation.jpg' | relative_url }}" alt="3D point cloud with each leaf in a different color">
  <figcaption>3D point cloud with distinct colors for each leaf</figcaption>
</figure>

## Base and Tip Detection

PCA on each leaf cluster gives its main axis. The tip is the point with the maximum projection along that axis and the base is the minimum. An earlier plan to train a model to predict bases and tips directly in 3D was replaced by this geometric method, because the small labeled dataset and complex leaf geometry couldn't support it.

<figure>
  <img src="{{ '/assets/projects/meng-plant-phenotyping/tip-vs-ground-truth.jpg' | relative_url }}" alt="Predicted leaf tips compared to ground truth">
  <figcaption>Multi-leaf tip predictions (red) compared to manual ground truth (green)</figcaption>
</figure>

## Results

The pipeline produces a CSV for each trial with the 3D base and tip position of each main leaf at every time step. Evaluation was qualitative: on good-quality trials, predicted tips were visually very close to manually marked ground truth, and the trajectories show leaf motion that would be difficult to measure by hand.

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

## Limitations

- Some abrupt jumps in the trajectories lined up with the chamber doors opening or the camera support being bumped. Because the motion is millimeter-scale, even small camera shifts show up. A rigid mount fixed to the chamber frame would fix this.
- PCA assumes a roughly straight leaf, so strongly curved or twisted leaves can give inaccurate base and tip estimates.
- The segmentation model was trained on a small dataset, so large changes in lighting, background, or species would need retraining. Retraining with a few images from a problem trial noticeably improved results.
- Each frame is processed independently, with no smoothing or tracking between frames.

## Future Work

- Curved-axis (spline) fitting for strongly curved leaves.
- Temporal smoothing and tracking across frames.
- A larger labeled dataset across plant stages and lighting conditions.
- Real-time integration with ROS 2.
- Higher-level metrics such as circumnutation amplitude, frequency, and direction.

## Links

- [M.Eng. Final Report (PDF)]({{ '/assets/projects/meng-plant-phenotyping/meng-final-report.pdf' | relative_url }})
