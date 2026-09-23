# hw definition file for processing by chibios_pins.py
# Target MCU: STM32G473RCTx / built as STM32G474xx family

MCU STM32G4xx STM32G474xx

FLASH_RESERVE_START_KB 0

# Parameter storage near end of first 128 kB flash bank
STORAGE_FLASH_PAGE 80
define HAL_STORAGE_SIZE 2000

define AP_BOOTLOADER_FLASHING_ENABLED 0

# Unique Board ID for your custom hardware
APJ_BOARD_ID 9999

# Setup build for a peripheral firmware
env AP_PERIPH 1

# Internal oscillator
OSCILLATOR_HZ 0

# Hardware Flash Allocation
FLASH_SIZE_KB 256

# Keep NRST as reset input only
define HAL_FLASH_SET_NRST_MODE 0x01

# UART order
SERIAL_ORDER USART1 USART2

# Minimal HAL configuration
define HAL_USE_SERIAL TRUE
define HAL_USE_EMPTY_IO TRUE

define STM32_SERIAL_USE_USART1 TRUE
define STM32_SERIAL_USE_USART2 TRUE
define STM32_SERIAL_USE_USART3 FALSE

# No ADC used
define HAL_USE_ADC FALSE

# DMA not needed/reserved for this small AP_Periph target
define DMA_RESERVE_SIZE 0

# ----------------------------------------------------------------------
# USART1: Mosaic-G5 GPS connection
# ----------------------------------------------------------------------
PA10 USART1_RX USART1
PA9  USART1_TX USART1

# ----------------------------------------------------------------------
# USART2: temporary debug/test port
# ----------------------------------------------------------------------
PA3 USART2_RX USART2
PA2 USART2_TX USART2

# ----------------------------------------------------------------------
# SWD debug
# ----------------------------------------------------------------------
PA13 JTMS-SWDIO SWD
PA14 JTCK-SWCLK SWD

# ----------------------------------------------------------------------
# I2C3 external port
# ----------------------------------------------------------------------
PA8 I2C3_SCL I2C3
PC9 I2C3_SDA I2C3

I2C_ORDER I2C3

# ----------------------------------------------------------------------
# SPI bus for LIS2MDLTR / IIS2MDC-compatible magnetometer
# Replace these pins with your real SPI pins if different.
# ----------------------------------------------------------------------
PA5 SPI1_SCK  SPI1
PA6 SPI1_MISO SPI1
PA7 SPI1_MOSI SPI1

# Chip select for magnetometer
# Replace PB0 with your real CS pin.
PA4 LIS2MDL_CS CS

SPI_ORDER SPI1

# SPI device.
# LIS2MDL uses the IIS2MDC-compatible driver path in recent ArduPilot.
# Try MODE3 first. If detection fails, test MODE0.
SPIDEV lis2mdl SPI1 DEVID1 LIS2MDL_CS MODE3 1*MHZ 5*MHZ

# Compass declaration.
# Use IIS2MDC driver for LIS2MDLTR-compatible sensor.
COMPASS IIS2MDC SPI:lis2mdl true ROTATION_NONE

# ----------------------------------------------------------------------
# FDCAN1 setup
# ----------------------------------------------------------------------
PA11 CAN1_RX CAN1
PA12 CAN1_TX CAN1

# ----------------------------------------------------------------------
# Stack / parameter limits
# ----------------------------------------------------------------------
define HAL_DEVICE_THREAD_STACK 768
define AP_PARAM_MAX_EMBEDDED_PARAM 256

# ----------------------------------------------------------------------
# Strict instance limits
# ----------------------------------------------------------------------
define GPS_MAX_RECEIVERS 1
define GPS_MAX_INSTANCES 1

# Enable one compass
define HAL_COMPASS_MAX_SENSORS 1

DMA_NOSHARE USART1*

# ----------------------------------------------------------------------
# Strip unneeded drivers to preserve flash
# ----------------------------------------------------------------------
define AP_PERIPH_IMU_ENABLED 0
define AP_PERIPH_BARO_ENABLED 0
define AP_PERIPH_NOTIFY_ENABLED 0

# Magnetometer enabled
define AP_PERIPH_MAG_ENABLED 1

# GPS enabled
define AP_PERIPH_GPS_ENABLED 1

# Point GPS default port to first serial array entry: USART1
define HAL_PERIPH_GPS_PORT_DEFAULT 0

# Optional: force IIS2MDC compass backend enabled if your branch supports it
define AP_COMPASS_IIS2MDC_ENABLED 1

# Keep compression disabled/commented as before
# env ROMFS_UNCOMPRESSED True