# G5Fly STM32G473

<img src="Resources/Logo_G5Fly_line.png" alt="G5Fly logo" width="60%">

Open-source GNSS navigation board for drones and robotics.

G5Fly is a compact GNSS board built around the Septentrio Mosaic-G5 module and an STM32G473 microcontroller. This branch contains the STM32 hardware design and AP_Periph firmware configuration for DroneCAN integration.

Author: <a href="https://github.com/TheophileRaimbault">Théophile Raimbault</a><br>
Maintainer: Septentrio GNSS GitHub user <githubuser@septentrio.com><br>
External website: https://github.com/septentrio-gnss/G5Fly<br>
License: <a href="https://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0</a> and <a href="https://www.oshwa.org/definition/">Open Source Hardware</a><br>

<div style="background-color:#e3f2fd; border-left:5px solid #1976D2; padding:10px; border-radius:5px; margin-top:10px;">
  ℹ️ <strong>Note:</strong> G5Fly is optimized for a 30.5 x 30.5 mm FPV drone form factor and is intended for open source drone or robot projects.
</div><br>


STM32 schematic: [Resources/G5Fly-schematics-STM32.pdf](Resources/G5Fly-schematics-STM32.pdf)

<p align="center">
  <img src="Resources/G5Fly_Flyer 1.3_page-0001.jpg" alt="3D view of the G5Fly STM32 board" width="100%">
</p>

## Who is this repository for?

This project serves two audiences:

- Users who want to buy, assemble, or use the board as-is.
- Designers who want to understand the hardware in detail and create a custom version.

If you are a user, start with the sections below. If you want to modify the design, go to [DESIGN.md](DESIGN.md).

## For users

### Key features
- Compact 30.5 x 30.5 mm FPV-friendly form factor
- Septentrio Mosaic-G5 GNSS receiver support
- USB and external 5 V power options
- DroneCAN, TTL, serial and I2C interfaces
- Reset input and status LEDs
- STM32G473-based AP_Periph node with DroneCAN support

### Quick start
1. Use the current production design files in [hardware/versions/v1/kicad/production](hardware/versions/v1/kicad/production/) if you want to build the board without changing the design.
2. Connect a suitable GNSS antenna and power the board through USB or an external 5 V source.
3. Build and flash the AP_Periph firmware using the configuration in [firmware](firmware/).
4. Use the available UART, I2C, or DroneCAN interfaces depending on your integration target.
5. Validate the board with the status LEDs and the expected GNSS fix behavior before full integration.

### Antenna and power
- Power the board according to the STM32 schematic and the regulator design.
- Use an active antenna only with the voltage specified by the antenna manufacturer.
- Verify antenna power selection and connector wiring before powering the board.

<div style="background-color:#ffebee; border-left:5px solid #c62828; padding:10px; border-radius:5px; margin:10px 0;">
  <strong>Warning:</strong> Solder <code>JP1</code> before using an active antenna. If the jumper is left open, the <code>VANT</code> net is not powered and the antenna may not work, causing a significant loss of GNSS performance. Select only one voltage according to the antenna datasheet.
</div>

<table>
  <tr>
    <td align="center"><img src="Resources/Vant-3V3.png" alt="JP1 configured for 3.3 V antenna power" width="300"><br><strong>3.3 V antenna</strong><br>Connect <code>VANT</code> to <code>+3.3V_RF</code>.</td>
    <td align="center"><img src="Resources/Vant-5V.png" alt="JP1 configured for 5 V antenna power" width="300"><br><strong>5 V antenna</strong><br>Connect <code>VANT</code> to <code>+5V</code>.</td>
  </tr>
</table>

### Production and manufacturing
For a production build without hardware changes:

- Use the current KiCad project and generated manufacturing files from the hardware folder: [hardware/versions/v1/kicad](hardware/versions/v1/kicad/)
- Provide the manufacturer with the schematic, PCB files, BOM, and CPL.
- Verify antenna connector type, jumper configuration, and power options before assembly.
- Reference schematic: [Resources/G5Fly-schematics-STM32.pdf](Resources/G5Fly-schematics-STM32.pdf)

#### JLCPCB manufacturing requirements

- Keep the default KiCad PCB setup and its calculated impedance settings.
- Manufacture the board as a 6-layer PCB.
- `PCB Requirement: User.1 layer should be used as the V-cut guide layer.`
- Refer to the PCB design rules in [hardware/common/design_rules/design-rules.md](hardware/common/design_rules/design-rules.md) when preparing or reviewing the PCB for production.
- Use the [G5Fly_STM32_Rev2_JLCPCB production archive](hardware/versions/v1/kicad/production/G5Fly_STM32_Rev2_JLCPCB.zip) for the files prepared specifically for manufacturing at JLCPCB.

### Power and antenna
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
- AP_Periph firmware configuration and DroneCAN bring-up

## Repository structure
- [hardware](hardware): PCB sources, KiCad projects, and mechanical assets
- [firmware](firmware): firmware and configuration files
- [Resources](Resources): images, schematics, and support documents
- [firmware/DroneCAN-Setup.md](firmware/DroneCAN-Setup.md): AP_Periph, DroneCAN, Mission Planner, and debugging procedure

## Licensing
This project is released under the Creative Commons Attribution-ShareAlike 4.0 license and follows the Open Source Hardware definition.
