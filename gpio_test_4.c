#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "include/ZYNQ_driver.h"

#define DEFAULT_PL (const char *) ("/store/mep/zynq_fpga_bin_files/Z_wrapper_atten3.bin")

int main()
{

	int rv = 0;
	
	int debug = 1;

	int err = 0;

	char filename[255];

	uint32_t direction[MAX_CHANS];

	uint32_t data[MAX_CHANS];

	uint32_t channel_mask;

	direction[0] = 0;
	direction[1] = 0;

	data[0] = 0x00010001;
	data[1] = 0x00000000;

	channel_mask = 0x1;

	sprintf(filename, DEFAULT_PL);

	if ( (rv = zynq_set_debug_level(debug) ) != 0 )
	{
		printf("ERROR setting debug level...\n");
	}

	if ( (rv = zynq_init(OP_TEST_MODE,INIT_OPEN_MODE) ) != 0 )
	{
		printf("ERROR calling zynq_init()...\n");
	}

	if ( (rv = zynq_set_gpio_direction(DR, direction, channel_mask) ) != 0 )
	{
		printf("ERROR calling zynq_set_gpio_direction(0x%8.8x)...\n", direction[0]);
	}

	if ( (rv = zynq_get_gpio_direction(DR, direction, channel_mask) ) != 0 )
	{
		printf("ERROR calling zynq_get_gpio_direction...\n");
	}

	printf("direction[0]=0x%8.8x, direction[1]=0x%8.8x...\n", direction[0], direction[1]);

	while (!err) 
	{

		printf("No attenuation\n");
		data[0] = 0x00000000;
		data[1] = 0x00000000;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("0.06 dB attenuation\n");
		data[0] = 0x00010001;
		data[1] = 0x00010001;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("0.13 dB attenuation\n");
		data[0] = 0x00020002;
		data[1] = 0x00020002;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("0.25 dB attenuation\n");
		data[0] = 0x00040004;
		data[1] = 0x00040004;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("0.5 dB attenuation\n");
		data[0] = 0x00080008;
		data[1] = 0x00080008;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("1.0 dB attenuation\n");
		data[0] = 0x00100010;
		data[1] = 0x00100010;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("2.0 dB attenuation\n");
		data[0] = 0x00200020;
		data[1] = 0x00200020;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("4.0 dB attenuation\n");
		data[0] = 0x00400040;
		data[1] = 0x00400040;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("8.0 dB attenuation\n");
		data[0] = 0x00800080;
		data[1] = 0x00800080;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("16.0 dB attenuation\n");
		data[0] = 0x01000100;
		data[1] = 0x01000100;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("32.0 dB attenuation\n");
		data[0] = 0x02000200;
		data[1] = 0x02000200;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("33.0 dB attenuation\n");
		data[0] = 0x02100210;
		data[1] = 0x02100210;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("34.0 dB attenuation\n");
		data[0] = 0x02200220;
		data[1] = 0x02200220;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("36.0 dB attenuation\n");
		data[0] = 0x02400240;
		data[1] = 0x02400240;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("40.0 dB attenuation\n");
		data[0] = 0x02800280;
		data[1] = 0x02800280;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("48.0 dB attenuation\n");
		data[0] = 0x03000300;
		data[1] = 0x03000300;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("50.0 dB attenuation\n");
		data[0] = 0x03200320;
		data[1] = 0x03200320;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("52.0 dB attenuation\n");
		data[0] = 0x03400340;
		data[1] = 0x03400340;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("56.0 dB attenuation\n");
		data[0] = 0x03800380;
		data[1] = 0x03800380;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("58.0 dB attenuation\n");
		data[0] = 0x03a003a0;
		data[1] = 0x03a003a0;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("60.0 dB attenuation\n");
		data[0] = 0x03c003c0;
		data[1] = 0x03c003c0;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

		printf("62.0 dB attenuation\n");
		data[0] = 0x03e003e0;
		data[1] = 0x03e003e0;
		if ( (rv = zynq_write(DR, data, channel_mask) ) != 0 )
		{
			printf("ERROR calling zynq_write(0x%8.8x)...\n", data[0]);
			err = 1;
		}
		sleep(10);

	}

	if ( (rv = zynq_close() ) != 0 )
	{
		printf("ERROR calling zynq_close()...\n");
	}

	return 0;

}
