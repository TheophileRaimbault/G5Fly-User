# G5Fly hardware design guide

This document is intended for designers, maintainers, and advanced contributors who want to understand the hardware in detail or create a custom version of the board.

## 1. Purpose of this document

Use this guide when you want to:

- understand the overall architecture of the board,
- modify the schematic or PCB with confidence,
- create a derivative version for a different form factor or feature set,
- prepare the design for manufacturing and validation.

## 2. Project overview

G5Fly is a compact GNSS navigation board built around the Septentrio Mosaic-G5 module and an STM32G473-based AP_Periph node. The board is intended for small drones, robots, and embedded navigation systems where space, integration flexibility, and open hardware are important.

The main design themes are:

- compact FPV-friendly mechanical envelope,
- GNSS positioning with optional heading support,
- serial and I2C expansion for system integration,
- DroneCAN connectivity for flight-controller ecosystems,
- a clear separation between the user-facing product and the underlying hardware design.



## 3. Repository structure

The main areas of interest are:

- [hardware](hardware): KiCad projects, PCB layout, symbols, footprints, and mechanical assets
- [firmware](firmware): embedded firmware, configuration files, and board-specific setup notes
- [Resources](Resources): schematics, images, and supporting documentation
- [docs](docs): notes, bring-up guides, and troubleshooting information

## 4. Hardware architecture

### 4.1 Main functional blocks

The board can be understood as a combination of the following functional blocks:

- GNSS receiver section: Septentrio Mosaic-G5 module, antenna path, and RF support
- Microcontroller section: STM32G473 running the AP_Periph firmware
- Sensor section: magnetometer and related support circuitry
- Connectivity section: USB, UART, I2C, and DroneCAN interfaces
- Power section: 5 V input, local regulation, and jumper-based configuration

### 4.2 Interfaces and expansion

The design exposes several interfaces that are useful for integration and extension:

- UART for GNSS configuration and data streaming
- I2C for external sensors or peripherals
- DroneCAN for flight-controller communication
- optional expansion support for future hub or sensor integration

These interfaces should be treated as part of the product roadmap and should be documented clearly if they are changed.

## 5. Recommended workflow for a custom derivative

### Step 1: Start from a stable revision

Use one of the existing hardware revisions as the baseline:

- [hardware/versions/Kicad G5Fly REV-2/kicad](hardware/versions/Kicad%20G5Fly%20REV-2/kicad)
- [hardware/versions/v1/kicad](hardware/versions/v1/kicad)

Do not start from scratch unless you need a completely new architecture.

### Step 2: Clone and rename the project

Create a new project directory with a distinct name, for example:

- G5Fly-MyVariant
- G5Fly-Compact-Rev2
- G5Fly-Industrial

This makes it easier to keep track of your changes and avoid confusion with the original design.

### Step 3: Review the schematic first

Before changing the PCB, review the following elements carefully:

- power supply topology,
- RF and antenna routing,
- connector pin assignments,
- MCU peripheral assignment,
- jumper and reset circuitry.

Changing one area often affects another, especially around the GNSS front end and the MCU interfaces.

### Step 4: Make a small and testable change

Prefer incremental changes such as:

- changing a connector,
- adding an LED or jumper,
- adjusting footprint or component value,
- modifying the board outline for a new mechanical envelope.

Keep changes small and documented so you can debug them easily.

### Step 5: Update the PCB and run design checks

After editing the schematic:

1. update the PCB from the schematic,
2. run the electrical rule check,
3. review the design rules,
4. check footprint orientation and manufacturing clearances,
5. generate manufacturing outputs.

### Step 6: Prepare production files

Before sending the board for manufacturing, prepare:

- schematic export or PDF,
- KiCad project archive or source files,
- BOM,
- CPL/placement file,
- Gerbers and drill files,
- assembly notes for connectors, jumpers, and antennas.

![alt text](Resources/G5Fly_STM32_REV-2.drawio.png)

## 6. Design recommendations

### 6.1 RF and antenna layout

- keep the antenna path short and direct,
- avoid unnecessary stubs or sharp bends,
- keep noisy digital traces away from the GNSS front end,
- validate the antenna connector choice carefully.

### 6.2 Power integrity

- separate the analog and digital power domains where practical,
- verify that the selected regulator and ferrites meet the load requirements,
- confirm that the supply path is suitable for the chosen antenna and USB configuration.

### 6.3 Mechanical integration

- validate the board outline against the target drone or robot frame,
- confirm connector access and clearance with the enclosure or mounting system,
- check the antenna placement relative to the structure and nearby metal parts.

## 7. Validation checklist

Before considering the design ready:

- confirm the board powers up correctly,
- verify UART and I2C behavior,
- test the GNSS receiver with a known-good antenna,
- confirm the reset and LED functions,
- validate the board under realistic mechanical and thermal conditions.

## 8. Suggested improvements

The following areas are good candidates for future work:

- add clearer mechanical drawings and connector maps,
- provide a dedicated production checklist,
- add example integration diagrams for drones and robots,
- document firmware and configuration steps in a dedicated guide,
- add a stronger versioning and release strategy for hardware revisions.

## 9. Final recommendation

Keep the repository split between two user journeys:

- a short, product-oriented README for users and integrators,
- a detailed design guide for hardware modification and derivative development.

That split makes the project easier to understand for both buyers and hardware designers.
