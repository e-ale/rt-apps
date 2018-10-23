#ifndef GPIO_H
#define GPIO_H

#include <linux/gpio.h>

extern int gpio_get_output_pins(const char *label, const char *devname,
				int offset, int count);
extern int gpio_set_pins(int fd, struct gpiohandle_data *data);

#endif /* GPIO_H */
