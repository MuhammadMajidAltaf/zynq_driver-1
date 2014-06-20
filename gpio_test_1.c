#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "include/ZYNQ_driver.h"

#define DEFAULT_PL (const char *) ("/store/mep/zynq_fpga_bin_files/ucm1_0.bin")

int main()
{

	int rv = 0;
	
	int debug = 7;

	char filename[255];

	uint32_t opmode = OP_TEST_MODE;

	uint32_t initmode = INIT_OPEN_MODE; 

	uint32_t direction[MAX_CHANS];

	uint32_t data[MAX_CHANS];

	uint32_t channel_mask;

	direction[0] = 0x00000000;
	direction[1] = 0x00000000;

	data[0] = 0x00010001;
	data[1] = 0x00000001;

	channel_mask = 3;

	sprintf(filename, DEFAULT_PL);

	if ( (rv = zynq_set_debug_level(debug) ) != 0 )
	{
		printf("ERROR setting debug level...\n");
	}

	if ( (rv = zynq_init(opmode, initmode) ) != 0 )
	{
		printf("ERROR calling zynq_init()...\n");
	}

	direction[0] = 0xffffffff;
	direction[1] = 0xffffffff;

	/* Setup Control Registers to be outputs */
	if ( (rv = zynq_set_gpio_direction(ID_REV, direction, channel_mask) ) != 0 )
	{
		printf("ERROR calling zynq_set_gpio_direction(0x%8.8x)...\n", direction[0]);
	}

	if ( (rv = zynq_get_gpio_direction(ID_REV, direction, channel_mask) ) != 0 )
	{
		printf("ERROR calling zynq_get_gpio_direction...\n");
	}

	printf("REG=%d: direction[0]=0x%8.8x, direction[1]=0x%8.8x...\n", ID_REV, direction[0], direction[1]);

	direction[0] = 0x00000000;
	direction[1] = 0x00000000;

	/* Setup Control Registers to be outputs */
/*
	if ( (rv = zynq_set_gpio_direction(CR, direction, channel_mask) ) != 0 )
	{
		printf("ERROR calling zynq_set_gpio_direction(0x%8.8x)...\n", direction[0]);
	}
*/

	if ( (rv = zynq_get_gpio_direction(CR, direction, channel_mask) ) != 0 )
	{
		printf("ERROR calling zynq_get_gpio_direction...\n");
	}

	printf("REG=%d: direction[0]=0x%8.8x, direction[1]=0x%8.8x...\n", CR, direction[0], direction[1]);

	direction[1] = 0xffffffff;

	/* Setup Data Registers to be outputs */
	if ( (rv = zynq_set_gpio_direction(DR, direction, channel_mask) ) != 0 )
	{
		printf("ERROR calling zynq_set_gpio_direction(0x%8.8x)...\n", direction[0]);
	}

	if ( (rv = zynq_get_gpio_direction(DR, direction, channel_mask) ) != 0 )
	{
		printf("ERROR calling zynq_get_gpio_direction...\n");
	}

	printf("REG=%d: direction[0]=0x%8.8x, direction[1]=0x%8.8x...\n", DR, direction[0], direction[1]);

/*
	if ( (rv = zynq_write(CR, data, CH2_MASK) ) != 0 )
	{
		printf("ERROR calling zynq_write(0x%8.8x,0x%8.8x)...\n", data[0],data[1]);
	}
*/

	if ( (rv = zynq_read(CR, data, channel_mask) ) != 0 )
	{
		printf("ERROR calling zynq_read()...\n");
	}

	data[0] = 0x00030003;
	data[1] = 0x00000000;

	if ( (rv = zynq_write(DR, data, CH1_MASK) ) != 0 )
	{
		printf("ERROR calling zynq_write(0x%8.8x,0x%8.8x)...\n", data[0],data[1]);
	}




	if ( (rv = zynq_read(DR, data, channel_mask) ) != 0 )
	{
		printf("ERROR calling zynq_read()...\n");
	}

/*
	if ( (rv = zynq_read(DR, data, channel_mask) ) != 0 )
	{
		printf("ERROR calling zynq_read()...\n");
	}

*/

/*
	data[0] = 0x00000001;
	

	if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
	{
		printf("ERROR calling zynq_write(0x%8.8x,0x%8.8x)...\n", data[0],data[1]);
	}

	if ( (rv = zynq_read(DR, data, channel_mask) ) != 0 )
	{
		printf("ERROR calling zynq_read()...\n");
	}

*/


	if ( (rv = zynq_close() ) != 0 )
	{
		printf("ERROR calling zynq_close()...\n");
	}

	return 0;

}
