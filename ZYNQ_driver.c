/**********************************************************
 *
 *  Skeleton driver for writing and reading to the Zynq PL 
 *   from a Linux C program.
 * 
 **********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "include/ZYNQ_driver.h"

/* Output from Xilinx Vivado Memory Map Window, GPIO Memory Offset */
#define GPIO0_BASE_ADDRESS     0x41200000
#define GPIO1_BASE_ADDRESS     0x41201000
#define GPIO2_BASE_ADDRESS     0x41202000
 
/* Location in MicroZed Linux of xdevcfg char device, prog_done */
#define PL_PROG_DONE (const char *) ("/sys/dev/char/249:0/device/prog_done")
/* Hard code default file into driver */
#define DEFAULT_PL (const char *) ("/store/mep/zynq_fpga_bin_files/ucm1_0.bin")

#define MAP_SIZE (4096UL)
#define MAP_MASK (MAP_SIZE - 1)

/* Masks for testing log settings */
#define ERROR (0x01)
#define DEBUG (0x02)
#define DIAG  (0x04)

/* Macro to shorten log lines */
#define ERR  if( dbg_lvl & ERROR ) printf
#define DBG  if( dbg_lvl & DEBUG ) printf
#define DLOG if( dbg_lvl & DIAG ) printf


/* Debug level, initial setting set to lowest level */
static int dbg_lvl = 0;   

/* Memory file device is closed at initialization */
static int mem_fd = -1;

/* TO DO: Review if these are all necessary */
static void *_mapped_base0;
static void *_mapped_dev_base0;
static void *_mapped_base1;
static void *_mapped_dev_base1;
static void *_mapped_base2;
static void *_mapped_dev_base2;

static int _zynq_pl_prog = 0;
static int _zynq_pl_open = 0;
static int _zynq_pl_init = 0;
static int _opmode = 0;

/* Pointers to the starting address of each GPIO */
static volatile gpio_t *GPIO0 = NULL;
static volatile gpio_t *GPIO1 = NULL;
static volatile gpio_t *GPIO2 = NULL;

/* Low level functions */

int _check_offset(const char *fn, uint32_t offset)
{
	if (offset > (NUM_GPIO - 1))
	{
		ERR("%s: Error, offset=%d out of range...\n", fn, offset);
		return -1;
	}

	return 0;
}

volatile gpio_t * _get_gpio (const char *fn, uint32_t offset)
{
	int rv;

	/* Invalid offset, return NULL */
	if ( (rv = _check_offset(fn, offset)) != 0)
	{
		return NULL;
	}
	
	switch(offset) {
		case ID_REV:
			return GPIO0;
			break;
		case CR: 
			return GPIO1;	
			break;
		case DR: 
			return GPIO2;
			break;
		default:
			/* Invalid offset, return NULL */
			return NULL;
			break;
	}

	return NULL;

}


int _write(const  char *fn, uint32_t offset, uint32_t *data, uint32_t channel_mask)
{
	volatile gpio_t *gpio;
	
	gpio = _get_gpio(fn, offset);

	/* Check to verify gpio has been initialized */
	if (gpio == NULL)
	{
		return -1;
	}

	/* All error checking done in top level function call, just perform operations here */

	if (channel_mask & CH1_MASK) 
	{
		DBG("%s: Writing to Offset %d, Channel 1, data=0x%8.8x...\n", 
			fn, offset, data[CH1_INDEX]);
		/* Write data to Channel 1 */
		gpio->ch[CH1_INDEX].data = data[CH1_INDEX];
	}

	if (channel_mask & CH2_MASK)
	{
		DBG("%s: Writing to Offset %d, Channel 2, data=0x%8.8x...\n", 
			fn, offset, data[CH2_INDEX]);
		/* Write data to Channel 2 */
		gpio->ch[CH2_INDEX].data = data[CH2_INDEX];
	}

	return 0;
}


int _read(const char *fn, uint32_t offset, uint32_t *data, uint32_t channel_mask)
{
	volatile gpio_t *gpio;
	
	gpio = _get_gpio(fn, offset);

	/* Check to verify gpio has been initialized */
	if (gpio == NULL)
	{
		return -1;
	}

	/* All error checking done in top level function call, just perform operations here */

	/* Read Channel 1 to data[CH1_INDEX] */
	if (channel_mask & CH1_MASK)
	{
		data[CH1_INDEX] = gpio->ch[CH1_INDEX].data;

		DBG("%s: Reading from Offset %d, Channel 1, data=0x%8.8x...\n", 
			fn, offset, data[CH1_INDEX]);
	}

	/* Read Channel 2 to data[CH2_INDEX] */
	if (channel_mask & CH2_MASK)
	{
		data[CH2_INDEX] = gpio->ch[CH2_INDEX].data;

		DBG("%s: Reading from Offset %d, Channel 2, data=0x%8.8x...\n", 
			fn, offset, data[CH2_INDEX]);
	}

	return 0;

}



int _write_dir(const  char *fn, uint32_t offset, uint32_t *data, uint32_t channel_mask)
{
	volatile gpio_t *gpio;
	
	gpio = _get_gpio(fn, offset);

	/* Check to verify gpio has been initialized */
	if (gpio == NULL)
	{
		return -1;
	}

	/* All error checking done in top level function call, just perform operations here */

	if (channel_mask & CH1_MASK) 
	{
		DBG("%s: Writing to Offset %d, Channel 1, tri=0x%8.8x...\n", 
			fn, offset, data[CH1_INDEX]);
		/* Write data to Channel 1 */
		gpio->ch[CH1_INDEX].tri = data[CH1_INDEX];
	}

	if (channel_mask & CH2_MASK)
	{
		DBG("%s: Writing to Offset %d, Channel 2, tri=0x%8.8x...\n", 
			fn, offset, data[CH2_INDEX]);
		/* Write data to Channel 2 */
		gpio->ch[CH2_INDEX].tri = data[CH2_INDEX];
	}

	return 0;
}

int _read_dir(const char *fn, uint32_t offset, uint32_t *data, uint32_t channel_mask)
{
	volatile gpio_t *gpio;
	
	gpio = _get_gpio(fn, offset);

	/* Check to verify gpio has been initialized */
	if (gpio == NULL)
	{
		return -1;
	}

	/* All error checking done in top level function call, just perform operations here */

	/* Read Channel 1 to data[CH1_INDEX] */
	if (channel_mask & CH1_MASK)
	{
		data[CH1_INDEX] = gpio->ch[CH1_INDEX].tri;

		DBG("%s: Reading from Offset %d, Channel 1, tri=0x%8.8x...\n", 
			fn, offset, data[CH1_INDEX]);
	}

	/* Read Channel 2 to data[CH2_INDEX] */
	if (channel_mask & CH2_MASK)
	{
		data[CH2_INDEX] = gpio->ch[CH2_INDEX].tri;

		DBG("%s: Reading from Offset %d, Channel 2, tri=0x%8.8x...\n", 
			fn, offset, data[CH2_INDEX]);
	}

	return 0;

}

int _sw_clock(const char *fn)
{
	int rv = 0;

	uint32_t data[MAX_CHANS];

	DBG("_sw_clock\n");

	data[CH1_INDEX] = 0x00000001;	
	
	/* Write 1 followed by 0 to the CH1 of CR to generate clock edge */

	if ( (rv = _write(fn, CR, data, CH1_MASK)) != 0)
	{
		ERR("%s: Error in _write() call, rv=%d...\n", fn, rv);
		return rv;
	}


	if ( (rv = _read(fn, CR, data, CH1_MASK|CH2_MASK)) != 0)
	{
		ERR("%s: Error in _write() call, rv=%d...\n", fn, rv);
		return rv;
	}

	data[CH1_INDEX] = 0x00000000;

	if ( (rv = _write(fn, CR, data, CH1_MASK)) != 0)
	{
		ERR("%s: Error in _write() call, rv=%d...\n", fn, rv);
		return rv;
	}

	if ( (rv = _read(fn, CR, data, CH1_MASK|CH2_MASK)) != 0)
	{
		ERR("%s: Error in _write() call, rv=%d...\n", fn, rv);
		return rv;
	}

	return 0;

}

int _pl_program(const char *fn, char *filename)
{
	int rv = 0;

	char cmd[255] = "cat ";

	if (filename == NULL)
	{
		DBG("%s: filename==NULL, using default=%s\n", fn, DEFAULT_PL);
		sprintf(filename, DEFAULT_PL);
	}

	strcat(cmd, filename);
	strcat(cmd, " > /dev/xdevcfg");

	DBG("%s: Executing - %s...\n", fn, cmd);

	_zynq_pl_prog = 0;

	if ( (rv = system(cmd) != 0))
	{
		
		ERR("%s: ERROR programming the PL...\n", fn);	
		return rv;
	}

	_zynq_pl_prog = 1;

	return 0;
}

int _pl_check(const char *fn)
{

	char prog_done[80] = "";

	int ret = 0;

	int plprogdone_fd = -1;

	DBG("%s: Opening PL fd=%s...\n", fn, PL_PROG_DONE);

	plprogdone_fd = open(PL_PROG_DONE, O_RDONLY);
	if (plprogdone_fd == -1)
	{
       		ERR("%s: Can't open fd=%s...\n", fn, PL_PROG_DONE);
        	return -1;
	}

	ret = read(plprogdone_fd, prog_done, 1);
	prog_done[ret] = '\0';
	DBG("%s: Closing PL fd=%s...\n", fn, PL_PROG_DONE);
	close(plprogdone_fd);

	if (prog_done[0] != '1')
	{
		ERR("%s: PL not programmed...\n", fn);	
		_zynq_pl_prog = 0;
		return -1;	
	}
		
	else 
	{
		DBG("%s: PL programmed...\n", fn);
		_zynq_pl_prog = 1;
	}

	return 0;

}

int _pl_open(const char *fn)
{

	int rv = 0;

	off_t dev_base0 = GPIO0_BASE_ADDRESS;
	off_t dev_base1 = GPIO1_BASE_ADDRESS;
	off_t dev_base2 = GPIO2_BASE_ADDRESS;

	int gpio0_open = 0;
	int gpio1_open = 0;
	int gpio2_open = 0;

	/* Device already open */
	if (_zynq_pl_open)
	{
		ERR("%s: Device already opened...\n", fn);
		rv = -1;
	}
		
	else 
	{
		/* Attempt to open the memory device */		
		mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
        	if (mem_fd == -1) 
		{
        		ERR("%s: Can't open /dev/mem...\n", fn);
			rv = -1;
    		}

				
		/* Map GPIO pointers to memory file device */
		else 
		{
    	
			DBG("%s: /dev/mem opened...\n", fn);
			
			/*** Memory map GPIO0 ***/
			_mapped_base0 = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, dev_base0 & ~MAP_MASK);
        
			if (_mapped_base0 == (void *) -1) 
			{
       				ERR("%s: Can't map the memory to user space...\n", fn);
		        	gpio0_open = 0;
				GPIO0 = NULL;
				rv = -1;
    			
			}
				
			else
			{
	   			_mapped_dev_base0 = _mapped_base0 + (dev_base0 & MAP_MASK);				

				/*ZM = (zynq_mmap_t *) mapped_dev_base;*/
				GPIO0 = (gpio_t *) _mapped_dev_base0;
				gpio0_open = 1;
				
				DBG("%s: Memory mapped at address %p, %p, %p\n", fn, _mapped_base0, _mapped_dev_base0, GPIO0);
				DBG("%s: FPGAID=%x...\n", fn, GPIO0->ch[CH1_INDEX].data);				
				DBG("%s: REV=%x...\n", fn, GPIO0->ch[CH2_INDEX].data);

			}


			/*** Memory map GPIO1 ***/
			_mapped_base1 = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, dev_base1 & ~MAP_MASK);
        
			if (_mapped_base1 == (void *) -1) 
			{
       				ERR("%s: Can't map the memory to user space...\n", fn);
		        	gpio1_open = 0;
				GPIO1 = NULL;
				rv = -1;    			
			}
				
			else
			{
	   			_mapped_dev_base1 = _mapped_base1 + (dev_base1 & MAP_MASK);
				gpio1_open = 1;
				GPIO1 = (gpio_t *) _mapped_dev_base1;
				DBG("%s: Memory mapped at address %p, %p, %p\n", fn, _mapped_base1, _mapped_dev_base1, GPIO1);
			}

			/*** Memory map GPIO2 ***/
			_mapped_base2 = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, dev_base2 & ~MAP_MASK);
        
			if (_mapped_base2 == (void *) -1) 
			{
       				ERR("%s: Can't map the memory to user space...\n", fn);
		        	gpio2_open = 0;
				GPIO2 = NULL;
				rv = -1;    			
			}
				
			else
			{
	   			_mapped_dev_base2 = _mapped_base2 + (dev_base2 & MAP_MASK);			
				gpio2_open = 1;
				GPIO2 = (gpio_t *) _mapped_dev_base2;
				DBG("%s: Memory mapped at address %p, %p, %p\n", fn, _mapped_base2, _mapped_dev_base2, GPIO2);
			}

		}

	}

	/* Error occurred during memory mapping any one of the GPIOs */
	if (!gpio0_open || !gpio1_open || !gpio2_open)
	{
		_zynq_pl_open = 0;
	}

	/* No error, ZYNQ PL is now considered operational */
	else
	{
		_zynq_pl_open = 1;
	}

	return rv;
}

int _pl_close(const char *fn)
{

	if (munmap(_mapped_base0, MAP_SIZE) == -1) 
	{
        	ERR("%s: GPIO0 - Can't unmap memory from user space...\n", fn);
	        return -1;
    	}

	if (munmap(_mapped_base1, MAP_SIZE) == -1) 
	{
        	ERR("%s: GPIO1 - Can't unmap memory from user space...\n", fn);
	        return -1;
    	}

	if (munmap(_mapped_base2, MAP_SIZE) == -1) 
	{
        	ERR("%s: GPIO2 - Can't unmap memory from user space...\n", fn);
	        return -1;
    	}
 
    	close(mem_fd);
	
	GPIO0 = NULL;
	GPIO1 = NULL;
	GPIO2 = NULL;

	_zynq_pl_init = 0;
	_zynq_pl_open = 0;

	DBG("%s: Unmap memory successful...\n", fn);

	return 0;
}


int zynq_set_debug_level(int debug)
{
	dbg_lvl = debug;
	return 0;
}

int zynq_get_debug_level()
{
	return dbg_lvl;
}


int zynq_init(uint32_t opmode, uint32_t initmode)
{
	char *fn = "zynq_init";

	int rv = 0;

	uint32_t data[MAX_CHANS];

	DBG("%s: ", fn);

	if (initmode & INIT_PROG_MODE)
	{

		if ( (rv = _pl_program(fn, (char *)DEFAULT_PL)) != 0)
		{
			ERR("%s: Error in _pl_program(%s) call, rv=%d...\n", 
				fn, (char *)DEFAULT_PL, rv);
			return rv;
		}


	}

	/* Memory map Zynq PL */
	if (initmode & INIT_OPEN_MODE)
	{
		if ( (rv = _pl_check(fn)) != 0)
		{
			ERR("%s: Error in _pl_check() call rv=%d...\n", fn, rv);
			return rv;
		}

		if ( (rv = _pl_open(fn)) != 0)
		{
			ERR("%s: Error in _pl_open() call rv=%d...\n", fn, rv);
			return rv;
		}

	}

	if ((opmode == OP_TEST_MODE) && _zynq_pl_open)
	{

		data[CH1_INDEX] = 0x00000000;
		data[CH2_INDEX] = 0x00000000;
		
/*		if ( (rv = zynq_set_gpio_direction(CR, data, CH1_MASK|CH2_MASK)) != 0)*/

		if ( (rv = _write_dir(fn, CR, data, CH1_MASK|CH2_MASK)) != 0)
		{
			ERR("%s: Error in zynq_set_gpio_direction() call, rv=%d...\n", fn, rv);
			return rv;
		}
	
		data[CH1_INDEX] = 0x00000000;
		data[CH2_INDEX] = opmode;
/*	data[CH2_INDEX] = 0x00000000;*/

		_opmode = opmode;
		
		if ( (rv = _write(fn, CR, data, CH1_MASK|CH2_MASK)) != 0)
		{
			ERR("%s: Error in _write() call, rv=%d...\n", fn, rv);
			return rv;
		}

	}

	
	/* Only set _zynq_pl_init if no errors occurrd */
	if (rv != 0)
	{
		_zynq_pl_init = 0;
	}
	
	else
	{
		_zynq_pl_init = 1;

	}

	return rv;

}


int zynq_set_gpio_direction(uint32_t offset, uint32_t *direction, uint32_t channel_mask)
{
	char *fn = "zynq_set_gpio_direction";

	if (_zynq_pl_open != 1) 
	{
		ERR("%s: Device not open...\n", fn);
		return -1;
	}

	return _write_dir(fn, offset, direction, channel_mask);
}

int zynq_get_gpio_direction(uint32_t offset, uint32_t *direction, uint32_t channel_mask)
{
	char *fn = "zynq_get_gpio_direction";

	if (_zynq_pl_open != 1) 
	{
		ERR("%s: Device not open...\n", fn);
		return -1;
	}

	return _read_dir(fn, offset, direction, channel_mask);
	
}

int zynq_write(uint32_t offset, uint32_t *data, uint32_t channel_mask)
{
	char *fn = "zynq_write";

	int rv = -1;

	if (_zynq_pl_open != 1) 
	{
		ERR("%s: Device not open...\n", fn);
		return -1;
	}
	
	/* Verify data is not null */
	if (data == NULL)
	{
		ERR("%s: Data not available to write...\n", fn);
		return -1;
	}
	
	if ( (rv = _write(fn, offset, data, channel_mask)) != 0)
	{
		ERR("%s: Error in _write() call, rv=%d...\n", fn, rv);
		return rv;
	}

	/* If test mode, need to generate clock signal */
	if (_opmode)
	{

		if ( (rv = _sw_clock(fn)) != 0)
		{
			ERR("%s: Error in _sw_clock() call, rv=%d...\n", fn, rv);
			return rv;
		}

	}

	return 0;
}

int zynq_write_lw(uint32_t offset, uint32_t *data, uint32_t channel_mask)
{
	char *fn = "zynq_write_lw";

	int rv = -1;

	uint32_t tmp_data[MAX_CHANS];

	if (_zynq_pl_open != 1) 
	{
		ERR("%s: Device not open...\n", fn);
		return -1;
	}

	/* Verify data is not null */
	if (data == NULL)
	{
		ERR("%s: Data not available to write...\n", fn);
		return -1;
	}

	/* Read current in gpio registers to tmp_data */
	if ( (rv = _read(fn, offset, tmp_data, channel_mask)) != 0)
	{
		ERR("%s: Error in _read() call, rv=%d...\n", fn, rv);
		return rv;
	}

	/* Put current upper word data with lower word data into tmp_data */
	tmp_data[CH1_INDEX] = (tmp_data[CH1_INDEX] & UW_MASK) | (data[CH1_INDEX] & LW_MASK);
	tmp_data[CH2_INDEX] = (tmp_data[CH2_INDEX] & UW_MASK) | (data[CH2_INDEX] & LW_MASK);

	if ( (rv = _write(fn, offset, tmp_data, channel_mask)) != 0)
	{
		ERR("%s: Error in _write() call, rv=%d...\n", fn, rv);
		return rv;
	}

	/* If test mode, need to generate clock signal */
	if (_opmode)
	{

		DBG("%s: ", fn);
		if ( (rv = _sw_clock(fn)) != 0)
		{
			ERR("%s: Error in _sw_clock() call, rv=%d...\n", fn, rv);
			return rv;
		}

	}
	
	return 0;

}

int zynq_write_uw(uint32_t offset, uint32_t *data, uint32_t channel_mask)
{
	char *fn = "zynq_write_uw";

	int rv = -1;

	uint32_t tmp_data[MAX_CHANS];

	if (_zynq_pl_open != 1) 
	{
		ERR("%s: Device not open...\n", fn);
		return -1;
	}

	/* Verify data is not null */
	if (data == NULL)
	{
		ERR("%s: Data not available to write...\n", fn);
		return -1;
	}

	/* Read current in gpio registers to tmp_data */
	if ( (rv = _read(fn, offset, tmp_data, channel_mask)) != 0)
	{
		ERR("%s: Error in _read() call, rv=%d...\n", fn, rv);
		return rv;
	}

	/* Put current upper word data with lower word data into tmp_data */
	tmp_data[CH1_INDEX] = (data[CH1_INDEX] & UW_MASK) | (tmp_data[CH1_INDEX] & LW_MASK);
	tmp_data[CH2_INDEX] = (data[CH2_INDEX] & UW_MASK) | (tmp_data[CH2_INDEX] & LW_MASK);

	if ( (rv = _write(fn, offset, tmp_data, channel_mask)) != 0)
	{
		ERR("%s: Error in _write() call, rv=%d...\n", fn, rv);
		return rv;
	}

	/* If test mode, need to generate clock signal */
	if (_opmode)
	{

		if ( (rv = _sw_clock(fn)) != 0)
		{
			ERR("%s: Error in _sw_clock() call, rv=%d...\n", fn, rv);
			return rv;
		}

	}

	return 0;

}


int zynq_read(uint32_t offset, uint32_t *data, uint32_t channel_mask)
{
	char *fn = "zynq_read";

	int rv = -1;
	
	if (_zynq_pl_open != 1) 
	{
		ERR("%s: Device not open...\n", fn);
		return -1;
	}

	if ( (rv = _read(fn, offset, data, channel_mask)) != 0)
	{
		ERR("%s: Error in _read() call, rv=%d...\n", fn, rv);
		return rv;
	}

	data[CH1_INDEX] = data[CH1_INDEX] & (UW_MASK | LW_MASK);
	data[CH2_INDEX] = data[CH2_INDEX] & (UW_MASK | LW_MASK);

	return 0;
}


int zynq_read_lw(uint32_t offset, uint32_t *data, uint32_t channel_mask)
{
	char *fn = "zynq_read_mask";

	int rv = -1;


	if (_zynq_pl_open != 1) 
	{
		ERR("%s: Device not open...\n", fn);
		return -1;
	}

	if ( (rv = _read(fn, offset, data, channel_mask)) != 0)
	{
		ERR("%s: Error in _read() call, rv=%d...\n", fn, rv);
		return rv;
	}

	data[CH1_INDEX] = data[CH1_INDEX] & LW_MASK;
	data[CH2_INDEX] = data[CH2_INDEX] & LW_MASK;
	
	return 0;

}

int zynq_read_uw(uint32_t offset, uint32_t *data, uint32_t channel_mask)
{
	char *fn = "zynq_read_mask";

	int rv = -1;

	if (_zynq_pl_open != 1) 
	{
		ERR("%s: Device not open...\n", fn);
		return -1;
	}

	if ( (rv = _read(fn, offset, data, channel_mask)) != 0)
	{
		ERR("%s: Error in _read() call, rv=%d...\n", fn, rv);
		return -1;
	}

	data[CH1_INDEX] = data[CH1_INDEX] & UW_MASK;
	data[CH2_INDEX] = data[CH2_INDEX] & UW_MASK;

	return 0;

}

int zynq_close()
{
	char *fn = "zynq_close";

	int rv = 0;

	if ( (rv = _pl_close(fn)) != 0)
	{
		ERR("%s: Error in _pl_close() call, rv=%d...\n", fn, rv);
		return rv;
	}
	return 0;
}



