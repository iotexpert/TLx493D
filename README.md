# XENSIV TLx493D 3D magentic sensor library for use with PSoC® 6

This is a driver library to interface with an Infineon XENSIV TLx493D 3D magentic sensor using a PSOC 6. It is based on the [XENSIV TLx493D Library](https://www.infineon.com/cms/en/product/sensor/magnetic-sensors/magnetic-position-sensors/3d-magnetics/#!tools).

The library is compatible with the following sensors:
- TLV493D-A1B6
- TLE493D-A2B6
- TLE493D-W2B6
- TLI493D-W2BW

## Library Organization

The src directory contains all source code for this library. The PSoC sub-directory contains the PSoC specific implementations which the user will call.

## Quick Start

To use the library:

1. Include the header file PSoC\_TLx\_interface.h.

2. Call the function TLxI2CInit to initialize the I2C interface. The user specifies the SDA pin, SCL pin, sensor I2C address (7-bit), clock speed, and PSoC clock to use.

    - Setting the I2C address to 0 will use a default address of 0x35 (which is 0x6A shifted right by one to turn the 8-bit address into a 7-bit address).
    - Setting the Speed to 0 will use a default speed of 115200.
    - Setting the Clock to NULL will auto-assign a clock with the appropriate speed.

3. Call the function TLx493D\_init to initialize the sensor.

4. Call TLx403D\_set\_operation\_mode to setup the sensor's mode. For simple operation, use TLx493D\_OP\_MODE\_MCM** for the argument.

5. Call TLx403D\_read\_frame with a pointer to a structur of type TLx493D\_data\_frame\_t to read sensor data. The structure declaration is:

        typedef struct {
            int16_t x;      // Magnetic field intensity raw value on the X axis
            int16_t y;      // Magnetic field intensity raw value on the Y axis
            int16_t z;      // Magnetic field intensity raw value on the Z axis
            int16_t temp;   // Raw Temperature value
        } TLx493D_data_frame_t;

**Example Code**

The folowing code will initialize the sensor and will then read values and print results every 200ms.

    #include "PSoC_TLx_interface.h"

    TLxI2CInit( CYBSP_I2C_SDA, CYBSP_I2C_SCL, 0, 0 ,NULL );

    TLx493D_init();

    TLx493D_set_operation_mode( TLx493D_OP_MODE_MCM );

    for (;;)
    {
        TLx493D_read_frame(&frame);
        printf("X:%d, Y:%d, Z:%d, Temp:%d\n", frame.x, frame.y, frame.z, frame.temp);
        Cy_SysLib_Delay(200);
    }

## Advanced Use

Lower-level interaction and control of the sensors is supported in the libray. To find out more about the lower-level details of the libary and all supported modes of operation, see the files in doc/html/index.html and doc/latex/refman.pdf.
