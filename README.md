# G5Fly

<img src="Resources/Logo_G5Fly_line.png" alt="G5Fly logo" width="60%">

Open-source GNSS navigation board for drones and robotics.

G5Fly is a compact board built around the Septentrio Mosaic-G5 module for lightweight UAV, robotics, and navigation projects. The repository contains the hardware design files, firmware sources, and supporting documentation needed to use the board, manufacture it, or create a custom derivative.

Author: <a href="https://github.com/TheophileRaimbault">Théophile Raimbault</a><br>
Maintainer: Septentrio GNSS GitHub user <githubuser@septentrio.com><br>
External website: https://github.com/septentrio-gnss/G5Fly<br>
License: <a href="https://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0</a> and <a href="https://www.oshwa.org/definition/">Open Source Hardware</a><br>

## Who is this repository for?

This project serves two audiences:

- Users who want to buy, assemble, or use the board as-is.
- Designers who want to understand the hardware in detail and create a custom version.

If you are a user, start with the sections below. If you want to modify the design, go to [DESIGN.md](DESIGN.md).

## For users

### What is G5Fly?
G5Fly is an open-source GNSS board intended for small drones, mobile robots, and embedded navigation systems. It combines a GNSS receiver, a compact FPV-friendly form factor, and expansion interfaces for telemetry and sensing.

### Key features
- Compact 30.5 x 30.5 mm FPV-friendly form factor
- Septentrio Mosaic-G5 GNSS receiver support
- USB and external 5 V power options
- TTL serial, I2C, and DroneCAN interfaces
- Reset input and status LEDs
- Expansion support for future peripherals and hub integration

### Quick start
1. Use the current production design files in [hardware/versions/Kicad G5Fly REV-2/kicad](hardware/versions/Kicad%20G5Fly%20REV-2/kicad) or [hardware/versions/v1/kicad](hardware/versions/v1/kicad) if you want to build the board without changing the design.
2. Connect a suitable GNSS antenna and power the board through USB or an external 5 V source.
3. Use the available UART, I2C, or DroneCAN interfaces depending on your integration target.
4. Validate the board with the status LEDs and the expected GNSS fix behavior before full integration.

### Production and manufacturing
For a production build without hardware changes:

- Use the current KiCad project and generated manufacturing files from the hardware folder.
- Provide the manufacturer with the schematic, PCB files, BOM, and CPL.
- Verify antenna connector type, jumper configuration, and power options before assembly.
- Reference schematic: [Resources/G5Fly-schematics.pdf](Resources/G5Fly-schematics.pdf)

### Power and antenna
- Power can be supplied by USB or from an external 5 V rail.
- Active antennas must be powered at the correct voltage specified by the antenna.
- Keep GNSS antenna cables away from noisy power traces where possible.

### Recommended antenna options
The following antenna families are commonly considered for evaluation:

- D-Helix antenna HX-CHX600A
- D-Helix antenna HX-CH7609A
- HC990EXF Calian antenna

A SMA-to-MMCX adapter is often useful for compatibility with the board connector.

## For designers and contributors

The detailed hardware design guide is available in [DESIGN.md](DESIGN.md).

That document covers:
- Overall architecture and major blocks
- Component selection and board organization
- How to create a custom derivative board
- Manufacturing and validation recommendations
- Suggested workflows for changing the hardware safely

## Repository structure
- [hardware](hardware): PCB sources, KiCad projects, and mechanical assets
- [firmware](firmware): firmware and configuration files
- [Resources](Resources): images, schematics, and support documents
- [docs](docs): additional notes and guidance

## Licensing
This project is released under the Creative Commons Attribution-ShareAlike 4.0 license and follows the Open Source Hardware definition.
