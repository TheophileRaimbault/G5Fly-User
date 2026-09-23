### How to flash G5Fly

Build with AP_Periph.

The build and flashing process can be performed with a Raspberry Pi and OpenOCD.

The following document records the validated setup and provides a practical debugging procedure.

***

````markdown
# Bring-up AP_Periph SeptentrioG5fly — Mosaic-G5 → STM32G473 → DroneCAN

## 1. Objective

The objective is to turn a Septentrio Mosaic-G5 receiver connected over UART to an STM32G4 into an ArduPilot-compatible DroneCAN peripheral.

Final architecture:

```text
Septentrio Mosaic-G5
    ↓ SBF / UART
STM32G473 / AP_Periph
    ↓ DroneCAN
Transceiver CAN
    ↓ CANH / CANL
Cube Orange / ArduPilot
````

Validated results:

* AP\_Periph starts correctly on the STM32.
* The DroneCAN node appears in Mission Planner.
* DroneCAN GNSS messages are published.
* The Cube Orange can use the receiver as a DroneCAN GPS.
* The most reliable Mosaic-G5 data stream is **SBF**, not NMEA.

AP\_Periph uses the ChibiOS/ArduPilot `hwdef.dat` system to define the MCU, pins, interfaces, and embedded drivers. STM32G4 devices are supported by AP\_Periph.

***

# 2. Configuration files used

The configuration files are placed in:

```bash
~/ardupilot/libraries/AP_HAL_ChibiOS/hwdef/SeptentrioG5fly/
```

Main files:

```text
hwdef.dat
defaults.parm
```

A `hwdef-bl.dat` was created to generate a bootloader, but the currently working version is flashed directly without an ArduPilot bootloader.

***

# 3. STM32 / AP\_Periph hardware configuration

## 3.1 MCU

The hardware uses an STM32G473RCT6, but the ArduPilot build was made with the STM32G474xx target because ArduPilot/ChibiOS supports this family for this bring-up.

```text
MCU STM32G4xx STM32G474xx
FLASH_SIZE_KB 256
```

***

## 3.2 Flash and parameter storage

The final working version does not use a bootloader:

```text
FLASH_RESERVE_START_KB 0
define AP_BOOTLOADER_FLASHING_ENABLED 0
```

Parameter storage is placed at the end of the first flash bank:

```text
STORAGE_FLASH_PAGE 63
define HAL_STORAGE_SIZE 2048
```

This configuration prevents a conflict between the AP\_Periph application and the parameter storage area.

***

## 3.3 UARTs

### USART1 - GPS link to the Mosaic-G5

```text
PA10 USART1_RX USART1
PA9  USART1_TX USART1
```

Connection:

```text
Mosaic TX → PA10 / USART1_RX
Mosaic RX ← PA9  / USART1_TX
GND       ↔ GND
```

### USART2 - temporary debug port

```text
PA3 USART2_RX USART2
PA2 USART2_TX USART2
```

Possible connection for PC/Raspberry Pi debugging:

```text
USB-TTL TX → PA3
USB-TTL RX ← PA2
GND        ↔ GND
```

***

## 3.4 Serial port order

```text
SERIAL_ORDER USART1 USART2
```

The AP\_Periph GPS is assigned to logical port 0:

```text
define HAL_PERIPH_GPS_PORT_DEFAULT 0
```

Therefore:

```text
GPS_PORT = 0 → USART1 → Mosaic-G5
```

***

## 3.5 CAN

```text
PA11 CAN1_RX CAN1
PA12 CAN1_TX CAN1
```

The STM32 must not be connected directly to the CAN bus. A CAN transceiver is required between PA11/PA12 and CANH/CANL.

CAN bus:

```text
STM32 PA11/PA12
    ↓
  CAN transceiver
    ↓
CANH / CANL
    ↓
Cube Orange
```

The standard CAN termination is a 120-ohm resistor between CANH and CANL at each end of the bus. With two terminations, the resistance measured between CANH and CANL with the bus powered off should be close to 60 ohms. [\[docs.ncnynl.com\]](https://docs.ncnynl.com/en/px4/en/can/)

***

# 4. Important AP\_Periph configuration

Key excerpts from `hwdef.dat`:

```text
env AP_PERIPH 1

define HAL_USE_SERIAL TRUE
define HAL_USE_EMPTY_IO TRUE

define STM32_SERIAL_USE_USART1 TRUE
define STM32_SERIAL_USE_USART2 TRUE
define STM32_SERIAL_USE_USART3 FALSE

define HAL_USE_ADC FALSE
define DMA_RESERVE_SIZE 0

define AP_PERIPH_GPS_ENABLED 1

define GPS_MAX_RECEIVERS 1
define GPS_MAX_INSTANCES 1
define HAL_COMPASS_MAX_SENSORS 0

define AP_PERIPH_IMU_ENABLED 0
define AP_PERIPH_BARO_ENABLED 0
define AP_PERIPH_NOTIFY_ENABLED 0
define AP_PERIPH_MAG_ENABLED 0
```

***

# 5. AP\_Periph default parameters

The following file is used:

```bash
libraries/AP_HAL_ChibiOS/hwdef/SeptentrioG5fly/defaults.parm
```

Recommended contents:

```text
# Default parameters for SeptentrioG5fly AP_Periph

CAN_BAUDRATE 1000000
CAN_NODE 0

GPS_PORT 0
GPS1_TYPE 10
GPS_AUTO_CONFIG 0
GPS1_RATE_MS 100
GPS_SAVE_CFG 0

DEBUG 0
```

Notes:

* `GPS1_TYPE = 10` selects the Septentrio SBF driver in AP\_Periph. ArduPilot documents `GPS1_TYPE = 10` for Septentrio/SBF GPS receivers. [\[github.com\]](https://github.com/ArduPilot/ardupilot/blob/master/libraries/AP_GPS/AP_GPS_SBF.h)
* `GPS_PORT = 0` maps to USART1 in our `SERIAL_ORDER`.
* `GPS_AUTO_CONFIG = 0` is used because the Mosaic-G5 configuration is performed manually.
* `CAN_NODE = 0` enables dynamic DroneCAN node ID allocation.

During the build, verify that the file is embedded:

```text
Embedding file defaults.parm:.../processed_defaults.parm
```

***

# 6. Build AP\_Periph

From the ArduPilot directory:

```bash
cd ~/ardupilot
rm -rf build/SeptentrioG5fly
./waf configure --board SeptentrioG5fly
./waf AP_Periph
```

Verify that the binary exists:

```bash
ls -lh build/SeptentrioG5fly/bin/
```

The main file is:

```text
build/SeptentrioG5fly/bin/AP_Periph
```

Verify that the firmware does not overlap the storage area:

```bash
arm-none-eabi-objdump -h build/SeptentrioG5fly/bin/AP_Periph | grep -E "text|data"
```

The end of the firmware must remain before the storage area, here close to:

```text
0x0801F800
```

***

# 7. Flash via Raspberry Pi over SWD

The Raspberry Pi was used as an SWD programmer through OpenOCD.

OpenOCD correctly detected the STM32:

```text
SWD DPIDR 0x2ba01477
Cortex-M4 detected
Examination succeed
```

Flash command:

```bash
sudo openocd -f ~/rpi_stm32g473_flash.cfg \
  -c "init" \
  -c "reset halt" \
  -c "program /home/septentriorpi/ardupilot/build/SeptentrioG5fly/bin/AP_Periph verify" \
  -c "reset run" \
  -c "shutdown"
```

Expected result:

```text
** Programming Finished **
** Verify Started **
** Verified OK **
```

***

# 8. Mosaic-G5 configuration

The working solution uses **SBF**, not NMEA.

## 8.1 Mosaic-G5 UART configuration

On the Mosaic port connected to the STM32:

```text
Baud rate : 115200
Protocol  : SBF
NMEA      : disabled on this port
```

## 8.2 Required SBF blocks

Configure the Mosaic to output at least:

```text
PVTGeodetic
DOP
ReceiverStatus
VelCovGeodetic
```

Recommended initial rate:

```text
1 Hz or 5 Hz
```

Once validated:

```text
10 Hz is possible if the port is not saturated
```

The ArduPilot SBF driver uses, among others, the `PVTGeodetic`, `DOP`, `ReceiverStatus`, and `VelCovGeodetic` blocks. [\[firmware.a...upilot.org\]](https://firmware.ardupilot.org/coverage/AP_GPS/AP_GPS_SBF.h.gcov.html), [\[deepwiki.com\]](https://deepwiki.com/ArduPilot/ardupilot/2.5-gps-and-sensor-integration)

***

# 9. Cube Orange / ArduPilot configuration

## 9.1 CAN configuration

If the peripheral is connected to CAN2 on the Cube:

```text
CAN_P2_DRIVER   = 1
CAN_D2_PROTOCOL = 1
CAN_P2_BITRATE  = 1000000
```

If the peripheral is connected to CAN1:

```text
CAN_P1_DRIVER   = 1
CAN_D1_PROTOCOL = 1
CAN_P1_BITRATE  = 1000000
```

In ArduPilot, `CAN_Dx_PROTOCOL = 1` selects DroneCAN. [\[github.com\]](https://github.com/ArduPilot/ardupilot/issues/30315)

After changing the parameters:

```text
Write Params
Reboot Autopilot
```

***

## 9.2 GPS configuration on the Cube

Important: on the Cube, the GPS is received through DroneCAN.

Therefore:

```text
GPS1_TYPE = 9
```

Do not set `GPS1_TYPE = 10` on the Cube.  
`10` is the SBF type for a GPS connected directly over serial, whereas the Cube receives a DroneCAN peripheral here. For a DroneCAN GPS, ArduPilot requires `GPSx_TYPE = 9`. [\[github.com\]](https://github.com/ArduPilot/ardupilot/issues/30315)

Optional:

```text
GPS1_CAN_NODEID = <AP_Periph node ID>
GPS1_CAN_OVRIDE = 1
```

For automatic selection:

```text
GPS1_CAN_OVRIDE = 0
```

***

# 10. Verification in Mission Planner

Open:

```text
CTRL-F → DroneCAN
```

Select:

```text
MAVLinkCAN2
```

if the peripheral is connected to CAN2.

Or:

```text
MAVLinkCAN1
```

if the peripheral is connected to CAN1.

Do not use `SLCAN` unless a USB-CAN adapter is connected directly to the PC.

***

## 10.1 Expected messages in the DroneCAN inspector

When everything is working, the AP\_Periph node should publish:

```text
uavcan_equipment_gnss_Fix2
uavcan_equipment_gnss_Auxiliary
ardupilot_gnss_Status
uavcan_protocol_NodeStatus
```

Observed example:

```text
ID 124 - org.ardupilot.SeptentrioG5fly
  ardupilot_gnss_Status
  uavcan_equipment_gnss_Auxiliary
  uavcan_equipment_gnss_Fix2
  uavcan_protocol_NodeStatus
```

If only `NodeStatus` appears, the node is running but the GPS data is not being decoded.

***

# 11. Debug procedure

## 11.1 The DroneCAN node does not appear

Check:

```text
CANH ↔ CANH
CANL ↔ CANL
GND  ↔ GND
```

With the bus powered off, check:

```text
≈60 ohms between CANH and CANL
```

In Mission Planner, check:

```text
CAN_Px_DRIVER = 1
CAN_Dx_PROTOCOL = 1
CAN_Px_BITRATE = 1000000
```

Verify that the correct channel is selected in the DroneCAN Inspector:

```text
MAVLinkCAN1 or MAVLinkCAN2
```

***

## 11.2 The node appears but no GNSS messages are received

Symptom:

```text
uavcan_protocol_NodeStatus only
```

Likely causes:

```text
Incorrect GPS1_TYPE in AP_Periph
Incorrect GPS_PORT
Mosaic still configured for NMEA
SBF is not being sent on the correct port
Incomplete SBF stream
Incorrect baud rate
```

Check the AP\_Periph node parameters:

```text
GPS_PORT = 0
GPS1_TYPE = 10
GPS_AUTO_CONFIG = 0
GPS1_RATE_MS = 100
```

Check the Mosaic:

```text
SBF enabled
NMEA disabled on this port
PVTGeodetic + DOP + ReceiverStatus + VelCovGeodetic enabled
Baud rate 115200
```

***

## 11.3 GNSS messages are present but values are zero

Symptom:

```text
Fix2 present
latitude = 0
longitude = 0
sats_used = 0
status = 0
```

Possible causes:

```text
GPS is indoors and has no fix
Incomplete SBF PVTGeodetic block
Not enough SBF blocks
Mosaic not initialized yet
```

Test outdoors or near a window.

Even without a fix, the presence of the `Fix2`, `Auxiliary`, and `ardupilot_gnss_Status` messages proves that the AP\_Periph → DroneCAN path is working.

***

## 11.4 SBF error 0x40

Observed message:

```text
GPS 1: SBF error changed (0x00000000/0x00000040)
GPS 1: probing for SBF at 115200 baud
```

`0x40` is associated with port congestion in some Septentrio/ArduPilot contexts. [\[ardupilot.org\]](https://ardupilot.org/copter/docs/common-gps-septentrio.html)

Actions:

```text
Reduce the SBF rate to 1 Hz or 5 Hz
Disable NMEA on the same port
Limit the output to the required blocks
Verify that the baud rate is 115200
```

***

## 11.5 Node parameters do not persist

Verify that the build embeds `defaults.parm`:

```text
Embedding file defaults.parm:.../processed_defaults.parm
```

Check the flash storage configuration:

```text
STORAGE_FLASH_PAGE 63
define HAL_STORAGE_SIZE 2048
```

Tester :

```text
Set DEBUG = 1
Commit parameters
Power cycle
Verify that DEBUG remains set to 1
```

If the parameters still do not persist, perform a mass erase and flash the firmware again:

```bash
sudo openocd -f ~/rpi_stm32g473_flash.cfg \
  -c "init" \
  -c "reset halt" \
  -c "stm32l4x mass_erase 0" \
  -c "shutdown"
```

Then flash AP\_Periph again.

***

# 12. Validated final state

The complete validated chain is:

```text
Mosaic-G5
  ↓ SBF 115200
STM32G473 AP_Periph
  ↓ DroneCAN
CAN Transceiver
  ↓ CANH/CANL
Cube Orange
  ↓ MAVLink
Mission Planner
```

Expected CAN messages:

```text
uavcan_equipment_gnss_Fix2
uavcan_equipment_gnss_Auxiliary
ardupilot_gnss_Status
```

Essential AP\_Periph parameters:

```text
GPS_PORT = 0
GPS1_TYPE = 10
GPS_AUTO_CONFIG = 0
CAN_BAUDRATE = 1000000
```

Essential Cube parameters:

```text
CAN_Px_DRIVER = 1
CAN_Dx_PROTOCOL = 1
GPS1_TYPE = 9
```

***

# 13. Short PX4 note

For PX4, the hardware principle is the same: the peripheral must publish valid DroneCAN GNSS messages on the CAN bus.

However, PX4 does not enable DroneCAN by default. PX4 documentation states that DroneCAN must be explicitly enabled, that PX4 still uses UAVCAN parameters for this function, and that an SD card is required for dynamic node ID allocation and firmware updates. [\[docs.px4.io\]](https://docs.px4.io/v1.14/en/dronecan/), [\[docs.px4.io\]](https://docs.px4.io/main/en/dronecan/)

Check the following on PX4/QGroundControl:

```text
UAVCAN_ENABLE = 2
```

to enable DroneCAN sensors with dynamic node ID allocation. A PX4 example documents that `UAVCAN_ENABLE = 2` enables UAVCAN/DroneCAN sensors with dynamic allocation and firmware updates. [\[mathworks.com\]](https://www.mathworks.com/help/uav/px4/ref/read-gps-uavcan-example.html)

If the AP\_Periph node already publishes:

```text
uavcan_equipment_gnss_Fix2
uavcan_equipment_gnss_Auxiliary
```

it should be protocol-compatible with a PX4 system configured to receive DroneCAN/UAVCAN v0 GNSS, subject to the PX4 version and the exact GNSS message support in use. PX4 documents support for DroneCAN/UAVCAN v0 GNSS devices. [\[docs.px4.io\]](https://docs.px4.io/v1.14/en/dronecan/), [\[docs.px4.io\]](https://docs.px4.io/main/en/dronecan/)

```

---

This completes the bring-up: the key result is that **SBF → AP_Periph → DroneCAN → ArduPilot** works. For PX4, the next step would mainly be to test node detection with `UAVCAN_ENABLE = 2` in QGroundControl.
```
