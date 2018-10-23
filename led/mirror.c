#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "gpio.h"
#include "shm.h"

static void blink(int fd, int count)
{
	struct gpiohandle_data data;
	struct share_data *sd;
	int err;

	sd = share_get();
	assert(sd);

	memset(&data, 0, sizeof(data));

	pthread_mutex_lock(&sd->m);

	while (1) {
		err = pthread_cond_wait(&sd->c, &sd->m);
		if (err) {
			pthread_mutex_unlock(&sd->m);
			break;
		}

		data.values[sd->index % count] = sd->val;
		err = gpio_set_pins(fd, &data);
		assert(!err);
	}
}

int main(int argc, char *argv[])
{
	int fd;

	fd = gpio_get_output_pins("blink", "/dev/gpiochip4", 8, 6);
	assert(fd != -1);

	blink(fd, 6);

	close(fd);

	return 0;
}
