<!-- 
  _____  _                   _                           _
 |  __ \| |                 | |                         (_)
 | |__) | |__   __ _ _ __ | |_ ___  _ __ ___ ___  ___ _ 
 |  ___/| '_ \ / _` | '_ \| __/ _ \| '_ ` _ \ / _ \/ __| |
 | |    | | | | (_| | | | | || (_) | | | | | |  __/ (__| |
 |_|    |_| |_|\__,_|_| |_|\__\___/|_| |_| |_|\___|\___|_|
                                                          
-->

<div align="center">

# Phantom

### A Custom openpilot Fork

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Fork Status](https://img.shields.io/badge/Fork%20Status-Active-brightgreen)](https://github.com/JackHars/Phantom)
[![openpilot Base](https://img.shields.io/badge/openpilot-0.8.9-blue)](https://github.com/commaai/openpilot/tree/v0.8.9)
[![Comma 2](https://img.shields.io/badge/Hardware-Comma%202-orange)](https://comma.ai/shop/products/two)

*Continued development for Comma 2 hardware with unique, community-driven driving enhancements.*

<p align="center">
  <img src="@maindemo.png" alt="Phantom Main Interface" width="85%">
</p>

</div>

> **Note:** Phantom is an independent, hard-forked project based on `openpilot` version 0.8.9. It is not affiliated with or endorsed by comma.ai.

---

## Table of Contents

- [Overview](#overview)
- [Target Audience](#target-audience)
- [Why Phantom?](#why-phantom)
- [Key Features](#key-features)
  - [ARNE (Always Ready Neural Engagement)](#arne-always-ready-neural-engagement)
  - [Vision-Based Stop Detection](#vision-based-red-light-and-stop-sign-detection)
  - [Mapbox Navigation](#integrated-mapbox-navigation)
  - [Additional Enhancements](#additional-enhancements)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage-highlights)
- [Safety Notice](#safety-notice-read-carefully-)
- [Contributing](#contributing)
- [Community & Support](#community--support)
- [License](#license)

---

## Overview

Phantom is an open-source driver assistance system derived from the `openpilot` codebase, specifically forked from version 0.8.9. It focuses on maintaining robust support for the Comma 2 hardware while integrating custom driving features developed independently from the main `openpilot` project. This fork aims to provide a tailored and enhanced experience for users seeking specific functionalities or continued use of legacy hardware.

---

## Target Audience

This fork is primarily intended for:

*   **Comma 2 Owners:** Users dedicated to maximizing the utility of their Comma 2 hardware with ongoing development.
*   **Driving Enthusiasts:** Individuals interested in exploring and experimenting with alternative driver assistance features and tuning.
*   **Users Seeking Specific Features:** Drivers who desire the unique functionalities offered by Phantom, such as `ARNE` or fully integrated on-device navigation.

---

## Why Phantom?

As the official `openpilot` project evolves, hardware support and feature priorities naturally change. Phantom provides a dedicated platform offering:

*   **Comma 2 Longevity:** Actively ensuring continued compatibility and performance optimization specifically for the Comma 2.
*   **Bespoke Features:** Implementing unique, community-driven enhancements like `ARNE` and vision-based stops, designed according to user feedback and specific driving preferences.
*   **Alternative Development Path:** Exploring and refining different approaches to driver assistance functionalities, potentially offering capabilities not found in official releases.

---

## Key Features

Phantom integrates the core `openpilot` functionality (based on v0.8.9) with several distinctive modifications:

### `ARNE` (Always Ready Neural Engagement)

Inspired by early community contributions from Arne Schwarek, `ARNE` enhances usability in stop-and-go conditions by decoupling lateral (steering) and longitudinal (gas/brake) control engagement:

- **Persistent Lateral Control:** Steering assistance remains active and ready even when longitudinal control is manually overridden by braking to a stop. The UI clearly indicates this persistent "ready" state.
- **Smoother Transitions:** Minimizes the need for frequent system re-engagement after stops, leading to a less interactive and potentially smoother driving experience in city traffic.
- **Standard Pedal Behavior:** Normal accelerator and brake pedal functionality remains unaffected when the main cruise control system is disengaged.

### Vision-Based Red Light and Stop Sign Detection

<p align="center">
  <img src="@stop.png" alt="Stop Sign Detection - 82% Probability" width="80%">
  <br>
  <em>Vision system detecting a stop sign with 82% confidence</em>
</p>

- **Automatic Stopping:** Employs a custom-trained vision model to identify and react to red lights and stop signs directly ahead.
- **Confidence Threshold:** Primarily initiates automatic braking when the model detects a relevant traffic control with high confidence (tunable, often default > 0.9 probability).
- **Manual Resumption:** The driver always retains control and resumes driving by pressing the accelerator pedal after the vehicle has stopped.
- **Limitations:** Performance is inherently dependent on environmental conditions (e.g., weather, lighting, visibility, sun glare) and the limitations of the vision model. **This is an experimental feature requiring constant driver vigilance and readiness to intervene.**

### Integrated Mapbox Navigation

<div align="center">
  <div style="display: flex; justify-content: space-between; flex-wrap: wrap;">
    <img src="@navfullscreen.png" alt="Navigation Full Screen View" width="49%" style="margin-bottom: 5px;">
    <img src="@navsearch.png" alt="Navigation Search Interface" width="49%" style="margin-bottom: 5px;">
  </div>
  <em>Left: Turn-by-turn directions with lane guidance | Right: Destination search interface</em>
</div>

- **On-Device Navigation:** Features a full navigation system powered by Mapbox APIs, displayed seamlessly on the Comma device.
- **Turn-by-Turn Guidance:** Provides clear visual and audible turn-by-turn directions integrated within the main driving interface.
- **Simple Setup:** Requires users to obtain a free or paid personal Mapbox API key from the Mapbox website and enter it once in the Phantom settings menu.
- **Cost-Effective Solution:** Eliminates the need for separate, often costly, dedicated navigation hardware or phone mounts.

### Additional Enhancements
- **Custom UI:** Distinctive "Phantom" branding and refined interface elements for a unique look and feel.
- **Modified Driving Models:** Includes custom tuning adjustments for lateral and longitudinal control behavior, potentially offering a different, preferred driving dynamic compared to stock `openpilot`.

---

## Requirements

Ensure your setup meets the following criteria before proceeding with installation:

| Component | Specification | Notes |
|-----------|---------------|-------|
| **Vehicle** | Compatible Vehicle | Refer to `openpilot` 0.8.9 compatibility lists for guidance. |
| **Hardware** | Comma Two | **Recommended.** |
|           | Comma Three / EON | Compatibility may vary significantly; support not guaranteed. |
| **Internet** | Stable Connection | Required for initial setup, navigation map data, traffic info, and potential updates. |
| **API Key** | Mapbox API Key | Obtain from [mapbox.com](https://www.mapbox.com/) (free tier available). Required for navigation. |

---

## Installation

Follow these steps meticulously to install Phantom on your Comma device:

1.  **Prepare Hardware:** Ensure your Comma device is properly set up, connected to the internet, and accessible (e.g., via SSH) according to standard comma.ai instructions. Verify the underlying operating system (e.g., NEOS) is compatible with `openpilot` 0.8.9 dependencies.

2.  **Clone Repository:** Access your device's terminal (typically via SSH) and execute the following commands to clone the Phantom repository into the `/data` directory. This will replace any existing `openpilot` installation:
    ```bash
    cd /data && rm -rf openpilot && git clone https://github.com/JackHars/Phantom.git openpilot && cd openpilot
    ```

3.  **Run Installer:** Execute the standard installer script located within the `scripts` directory. This crucial step handles dependency management and compiles the necessary software components:
    ```bash
    cd scripts && ./installer.sh
    # Follow any on-screen prompts carefully. The device may reboot.
    ```

4.  **Configure Navigation:** After installation completes and the device reboots into Phantom, navigate to the device's settings menu. Locate the Mapbox or Navigation configuration section and enter your personal Mapbox API key obtained in the Requirements step.

---

## Usage Highlights

Familiarize yourself with the operation of Phantom's key custom features:

| Feature | Usage Notes |
|---------|-------------|
| **`ARNE`** | Engage the system as usual. When you manually brake to a stop (disengaging longitudinal control), observe that the lateral (steering) control UI indicator remains active/ready. Pedal inputs function normally when cruise control is fully disengaged. |
| **Vision Stops** | **Ensure the feature is explicitly enabled in the Phantom settings menu.** Under favorable conditions (good visibility, clear signage/lights), the vehicle *should* initiate braking for detected red lights/stop signs. **Crucially, always remain vigilant and prepared to brake manually.** Resume driving post-stop by pressing the accelerator. |
| **Navigation** | Access the map/navigation interface via the device's touchscreen. Use the search function to find destinations and select a route to receive integrated turn-by-turn guidance during drives. |

---

## 🚨 Safety Notice: READ CAREFULLY 🚨

<div align="center" style="margin-top: 15px; margin-bottom: 15px;">
<table>
<tr>
<td align="center" style="background-color: #fff0f0; padding: 20px; border: 3px solid #d9534f; border-radius: 8px;">

<h2 style="color: #d9534f;">⚠️ CRITICAL WARNING: EXPERIMENTAL RESEARCH SOFTWARE ⚠️</h2>

<p style="font-size: 1.1em; margin-top: 10px; margin-bottom: 15px;"><strong>Phantom is highly experimental software intended ONLY for research, development, and enthusiast use. It is NOT a certified safety product, NOT autonomous driving, and should NEVER be treated as such.</strong></p>

<ul align="left" style="margin-left: 25px; margin-top: 10px; margin-bottom: 15px; line-height: 1.6;">
<li><strong>Independent & Unofficial:</strong> This is an independent fork, NOT tested, validated, or endorsed by comma.ai. Features may be unstable, unpredictable, or contain unknown bugs.</li>
<li><strong>Use Entirely At Your Own Risk:</strong> Phantom is provided "as is" without any warranty, express or implied. **You** are solely and entirely responsible for the safe operation of your vehicle at all times.</li>
<li>**Constant Driver Vigilance is MANDATORY:** This system provides Level 2 driver *assistance*. You MUST keep your hands on the steering wheel, eyes actively monitoring the road and surroundings, and be prepared to take immediate, full control at ANY moment without warning. Never become complacent or overly reliant on this system.</li>
<li>**Experimental Feature Limitations:** Features like vision-based stopping (`Vision Stops`) and decoupled engagement (`ARNE`) are highly experimental. Their performance is subject to significant limitations and they may fail unexpectedly due to various factors including (but not limited to) weather, lighting, sensor obstructions, complex road geometries, or software errors. **DO NOT TRUST THESE FEATURES IMPLICITLY.** Always anticipate potential failures.</li>
</ul>

<p style="font-weight: bold; font-size: 1.15em; color: #d9534f; margin-top: 15px;">Failure to exercise extreme caution, maintain constant vigilance, and prioritize safety can result in serious property damage, severe injury, or death.</p>

</td>
</tr>
</table>
</div>

---

## Contributing

We enthusiastically welcome contributions to improve Phantom! Whether it's reporting bugs with detailed information, suggesting well-considered new features, enhancing documentation, or submitting code improvements via Pull Requests, your input is valuable to the project's growth.

_Please review our `CONTRIBUTING.md` file (if available) or open an Issue on GitHub to discuss significant changes before starting work._

---

## Community & Support

Connect with other users and the developers:

- **[GitHub Issues](https://github.com/JackHars/Phantom/issues)**: The primary channel for reporting bugs, tracking issues, and requesting specific features.
- **[GitHub Discussions](https://github.com/JackHars/Phantom/discussions)**: For general questions, sharing experiences, and broader community interaction.
- **Discord**: Join our community chat for real-time discussions and informal support _(Invite link: TBD - Check Discussions/Issues/Wiki)_
- **[Wiki](https://github.com/JackHars/Phantom/wiki)**: Find additional documentation, user guides, FAQs, and troubleshooting tips (content evolves over time).

---

## License

This project operates under the terms of the [MIT License](https://github.com/JackHars/Phantom/blob/master/LICENSE), inherited from the original `openpilot` software (© comma.ai). All custom code introduced within the Phantom fork is also released under the MIT License unless explicitly stated otherwise.

<div align="center">
<sub>Phantom © 2023 - Developed by enthusiasts, for enthusiasts. **Drive Safely!**</sub>
</div>
