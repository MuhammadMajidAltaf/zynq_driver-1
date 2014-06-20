#ifndef _ZYNQ_DRIVER_H_
#define _ZYNQ_DRIVER_H_

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>

/* Lower word mask */
#define LW_MASK (0x0000ffff)
/* Upper word mask */
#define UW_MASK (0xffff0000)

/* Masks for writing/reading from a GPIO */
#define CH1_MASK (0x1)
#define CH2_MASK (0x2)

/* Index values for Channel 1 and Channel 2 data and masks */
#define CH1_INDEX (0)
#define CH2_INDEX (1)

/* Number of GPIOs in design */
#define NUM_GPIO (3)

/* Xilinx GPIO core supports upto 2 Channels */
#define MAX_CHANS (2)

/* Define initialization mode masks */
#define INIT_PROG_MODE    (0x1)
#define INIT_OPEN_MODE    (0x2)

/* Define operating modes */
#define OP_NORMAL_MODE  (0)
#define OP_TEST_MODE    (1)

/* GPIO register offsets */
#define ID_REV       (0)
#define CR	     (1)
#define DR 	     (2)

typedef struct {
	uint32_t data;
	uint32_t tri;
} channel_t;

typedef struct {
	channel_t ch[MAX_CHANS];
	uint32_t _unused[1020];
} gpio_t;

/* Modify the Zynq MMAP data structure as needed */
typedef struct {
	gpio_t gpio[NUM_GPIO];
} zynq_mmap_t;

/* Add top level function prototypes here */
int zynq_set_debug_level(int debug);
int zynq_get_debug_level();
int zynq_init(uint32_t opmode, uint32_t initmode);
int zynq_set_gpio_direction(uint32_t channel_number, uint32_t *direction, uint32_t channel_mask);
int zynq_get_gpio_direction(uint32_t channel_number, uint32_t *direction, uint32_t channel_mask);
int zynq_write(uint32_t offset, uint32_t *data, uint32_t channel_mask);
int zynq_write_lw(uint32_t offset, uint32_t *data, uint32_t channel_mask);
int zynq_write_uw(uint32_t offset, uint32_t *data, uint32_t channel_mask);
int zynq_read(uint32_t offset, uint32_t *data, uint32_t channel_mask);
int zynq_read_lw(uint32_t offset, uint32_t *data, uint32_t channel_mask);
int zynq_read_uw(uint32_t offset, uint32_t *data, uint32_t channel_mask);
int zynq_close();

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
}
#endif

#endif  /* _ZYNQ_DRIVER_H_ */
