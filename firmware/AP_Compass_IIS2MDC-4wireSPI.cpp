/*
 *This file is part of the ArduPilot project but modified for match the IIS2MDC compass sensor in 4 wire SPI mode.
 *
 */
#include "AP_Compass_config.h"

#if AP_COMPASS_IIS2MDC_ENABLED

#include "AP_Compass_IIS2MDC.h"

// IIS2MDC Registers
#define IIS2MDC_ADDR_CFG_REG_A  0x60
#define IIS2MDC_ADDR_CFG_REG_B  0x61
#define IIS2MDC_ADDR_CFG_REG_C  0x62
#define IIS2MDC_ADDR_STATUS_REG 0x67
#define IIS2MDC_ADDR_OUTX_L_REG 0x68
#define IIS2MDC_ADDR_WHO_AM_I   0x4F

// IIS2MDC Definitions
#define IIS2MDC_WHO_AM_I         0b01000000
#define IIS2MDC_STATUS_REG_READY 0b00001111
// CFG_REG_A
#define COMP_TEMP_EN    (1 << 7)
#define MD_CONTINUOUS   (0 << 0)
#define ODR_100         ((1 << 3) | (1 << 2))
// CFG_REG_B
#define OFF_CANC        (1 << 1)
// CFG_REG_C
// CFG_REG_C
#define BDU             (1 << 4)
#define SPI_4WIRE       (1 << 2)

extern const AP_HAL::HAL &hal;

AP_Compass_Backend *AP_Compass_IIS2MDC::probe(AP_HAL::OwnPtr<AP_HAL::Device> dev,
        bool force_external,
        enum Rotation rotation)
{
    if (!dev) {
        return nullptr;
    }

    AP_Compass_IIS2MDC *sensor = NEW_NOTHROW AP_Compass_IIS2MDC(std::move(dev),force_external,rotation);

    if (!sensor || !sensor->init()) {
        delete sensor;
        return nullptr;
    }

    return sensor;
}

AP_Compass_IIS2MDC::AP_Compass_IIS2MDC(AP_HAL::OwnPtr<AP_HAL::Device> dev,
        bool force_external,
        enum Rotation rotation)
    : _dev(std::move(dev))
    , _rotation(rotation)
    , _force_external(force_external)
{
}

bool AP_Compass_IIS2MDC::init()
{
    WITH_SEMAPHORE(_dev->get_semaphore());

    _dev->set_retries(10);

    const bool is_spi = (_dev->bus_type() == AP_HAL::Device::BUS_TYPE_SPI);

    if (is_spi) {
        // LIS2MDL/IIS2MDC SPI reads use MSB=1 and auto-increment.
        // The LIS2MDL powers up in 3-wire SPI mode, so force 4-wire mode
        // before the first WHO_AM_I read.
        _dev->set_read_flag(0xC0);

        // Enable 4-wire SPI and block data update.
        // This write uses MOSI only, so it can work before MISO is active.
        _dev->write_register(IIS2MDC_ADDR_CFG_REG_C, BDU | SPI_4WIRE);

        hal.scheduler->delay(10);
    }

    if (!check_whoami()) {
        return false;
    }

    if (!_dev->write_register(IIS2MDC_ADDR_CFG_REG_A, MD_CONTINUOUS | ODR_100 | COMP_TEMP_EN)) {
        return false;
    }

    if (!_dev->write_register(IIS2MDC_ADDR_CFG_REG_B, OFF_CANC)) {
        return false;
    }

    // IMPORTANT:
    // Keep SPI_4WIRE set, otherwise this write would return the LIS2MDL
    // back to 3-wire mode and MISO would stop working.
    const uint8_t cfg_reg_c = is_spi ? (BDU | SPI_4WIRE) : BDU;

    if (!_dev->write_register(IIS2MDC_ADDR_CFG_REG_C, cfg_reg_c)) {
        return false;
    }

    // lower retries for run
    _dev->set_retries(3);

    // register compass instance
    _dev->set_device_type(DEVTYPE_IIS2MDC);

    if (!register_compass(_dev->get_bus_id())) {
        return false;
    }

    set_rotation(_rotation);

    if (_force_external) {
        set_external(true);
    }

    // Enable 100Hz
    _dev->register_periodic_callback(10000, FUNCTOR_BIND_MEMBER(&AP_Compass_IIS2MDC::timer, void));

    return true;
}

bool AP_Compass_IIS2MDC::check_whoami()
{
    uint8_t whoami = 0;
    if (!_dev->read_registers(IIS2MDC_ADDR_WHO_AM_I, &whoami, 1)){
        return false;
    }

    return whoami == IIS2MDC_WHO_AM_I;
}

void AP_Compass_IIS2MDC::timer()
{
    struct PACKED {
        uint8_t xout0;
        uint8_t xout1;
        uint8_t yout0;
        uint8_t yout1;
        uint8_t zout0;
        uint8_t zout1;
        uint8_t tout0;
        uint8_t tout1;
    } buffer;

    const float range_scale = 100.f / 65.535f; // +/- 50,000 milligauss, 16bit

    uint8_t status = 0;
    if (!_dev->read_registers(IIS2MDC_ADDR_STATUS_REG, &status, 1)) {
        return;
    }

    if (!(status & IIS2MDC_STATUS_REG_READY)) {
        return;
    }

    if (!_dev->read_registers(IIS2MDC_ADDR_OUTX_L_REG, (uint8_t *) &buffer, sizeof(buffer))) {
        return;
    }

    const int16_t x = ((buffer.xout1 << 8) | buffer.xout0);
    const int16_t y = ((buffer.yout1 << 8) | buffer.yout0);
    const int16_t z = -1 * ((buffer.zout1 << 8) | buffer.zout0);

    Vector3f field{ x * range_scale, y * range_scale, z * range_scale };

    accumulate_sample(field);
}

#endif //AP_COMPASS_IIS2MDC_ENABLED