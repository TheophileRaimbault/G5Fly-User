# Versioning and change log

This document summarizes the main hardware changes between G5Fly revisions.

## REV1 -> REV2

The following changes were applied for the transition from REV1 to REV2:

### Mechanical and layout
- Increased the spacing between the connector and the screw hole.
- Reduced the board size slightly to better match the NXP board footprint.
- Added the REV-1 reference on the silkscreen.
- Adjusted the LED spacing.

### Electrical and component updates
- Changed the I2C pull-up resistors to a more suitable value range (from 10 kΩ to a more appropriate 2.2–4.7 kΩ range).
- Corrected the main antenna grounding connection.
- Replaced the wrong resistor value R16 from 24 kΩ to 13 kΩ for production.
- Switched the magnetometer interface from SPI to I2C.
- Changed the CAN termination approach to a shunt-based solution.
- Updated the magnetometer component used for the NXP board compatibility.

### Connectors and interfaces
- Changed to the correct Hirose connector part number: DF40TC(2.5)-50DS-0.4V(51).
- Removed unused STM32 CTS/RTS pins from the schematic.
- Added a PPS signal from the GNSS to the STM32 for time synchronization.
- Added a CAN status LED.

### Firmware and identification
- Added an internal crystal oscillator.
- Added hardware board ID information in the ArduPilot board definition file.
- Added silkscreen orientation guidance for the magnetometer.

### Not implemented in REV2
The following ideas were considered but were not implemented in REV2:
- reversed diode protection,
- adding a reset button or reset pin for the G5,
- adding additional flash memory.

## Future versioning guidance

For each new revision, it is recommended to keep this file updated with:

- the revision number,
- the date or milestone,
- a short summary of the main changes,
- the list of items intentionally not implemented,
- any important impact on firmware or integration.
