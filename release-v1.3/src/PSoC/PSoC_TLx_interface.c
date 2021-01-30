
/******************************************************************************
* File Name:  interface.c
*
* Description:  This defines the I2C interface required by the TLx493_Library.
*               The functions are mapped to the library in the file
*               TLx493_Library/TLx493D/interface.h.
*
*******************************************************************************/
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#define SLAVE_ADDRESS_RESET		0x00U
#define SLAVE_ADDRESS_RECOVER	0x7FU
#define SDA_DELAY 220
#define HIGH 1
#define TIMEOUT 100
#define DEFAULT_SLAVE_ADDRESS (0x6A>>1)
#define DEFAULT_I2C_SPEED     (115200)

/* I2C Object and slave address*/
static cyhal_i2c_t TLxI2CInstance;
static uint16_t slave_address;


/* Initialize the I2C interface */
/* Parameters:
 * sda - SDA pin
 * scl - SCL pin
 * address - I2C slave address (7 bit)(or use 0 for default of 0x06A>>1)
 * speed - I2C speed (or use 0 for default of 115200)
 * *clk - clock (or use NULL to auto-assign a clock)

 * speed - I2C speed (or use NULL to use 115200)
 */
cy_rslt_t TLxI2CInit(cyhal_gpio_t sda, cyhal_gpio_t scl, uint16_t sensor_address, uint32_t speed, const cyhal_clock_t *clk)
{
	cy_rslt_t result;

	/* Initialize I2C block with the specified pins and clock */
	result = cyhal_i2c_init(&TLxI2CInstance, sda, scl, clk);

	if (result == CY_RSLT_SUCCESS)
	{
		/* Configure the master interface */
		cyhal_i2c_cfg_t i2c_cfg; 	/* I2C configuration structure */
		i2c_cfg.is_slave = false; 	/* PSoC is I2C master */
		i2c_cfg.address = 0;		/* Master has no address */

		if(speed == 0)
		{
			i2c_cfg.frequencyhal_hz = DEFAULT_I2C_SPEED;
		}
		else
		{
			i2c_cfg.frequencyhal_hz = speed;
		}
		result = cyhal_i2c_configure(&TLxI2CInstance, &i2c_cfg);

		/* Save the specified slave address to a global so that the read/write functions can use it */
		if(sensor_address == 0)
		{
			slave_address = DEFAULT_SLAVE_ADDRESS;
		}
		else
		{
			slave_address = sensor_address;
		}
	}

	return result;
}

/* I2C Read */
cy_rslt_t TLxRead (uint8_t addr, uint8_t *data, uint8_t count)
{
	cy_rslt_t result;

	result = cyhal_i2c_master_read(&TLxI2CInstance, slave_address, data, count, TIMEOUT, true);

	return result;
}


/* I2C Write */
cy_rslt_t TLxWrite (uint8_t addr, const uint8_t* data, uint8_t count)
{
	cy_rslt_t result;

	result = cyhal_i2c_master_write(&TLxI2CInstance, slave_address, data, count, TIMEOUT, true);

	return result;
}


/* I2C Recover */
void TLxRecover(void)
{
	/* Send recover address with no data */
	/* The address is all 1's so we need to use I2C read which sets the LSB to 1 */
	cyhal_i2c_master_read(&TLxI2CInstance, SLAVE_ADDRESS_RECOVER, NULL, 0, TIMEOUT, true);

}


/* I2C Reset */
void TLxReset(void)
{
	/* Send reset address with no data */
	/* The address is all 0's so we need to use I2C write which sets the LSB to 0 */
	cyhal_i2c_master_write(&TLxI2CInstance, SLAVE_ADDRESS_RESET, NULL, 0, TIMEOUT, true);
}


/* Set SDA High and Wait */
void TLxSetSdaAndWait(bool value)
{
	/* Set SDA pin as requested */
	cyhal_gpio_write(CYBSP_I2C_SDA, value );

	/* Wait */
	Cy_SysLib_DelayUs(SDA_DELAY);

	/* Set SDA pin high */
	/* Note that the pin is open drain so this just releases it */
	cyhal_gpio_write(CYBSP_I2C_SDA, HIGH );
}


/* Power Control */
void TLxPowerControl (bool on)
{
	/* Not implemented */
}


/* Log Function */
void TLxLog(void *data, uint32_t count)
{
	/* Not implemented */
}
