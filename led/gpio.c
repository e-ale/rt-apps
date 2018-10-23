#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include "gpio.h"

int gpio_get_output_pins(const char *label, const char *devname,
			 int offset, int count)
{
	struct gpiohandle_request req;
	int ret;
	int fd;
	int i;

	memset(&req, 0, sizeof(req));
	strcpy(req.consumer_label, label);
	req.flags = GPIOHANDLE_REQUEST_OUTPUT;
	req.lines = count;
	for (i = 0; i < count; i++)
		req.lineoffsets[i] = i + offset;

	fd = open(devname, 0);
	assert(fd != -1);

	ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
	assert(ret != -1 && req.fd > 0);

	close(fd);

	return req.fd;
}

int gpio_set_pins(int fd, struct gpiohandle_data *data)
{
	int ret;

	ret = ioctl(fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, data);
	assert(ret != -1);

	return ret;
}
