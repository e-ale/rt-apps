#include <unistd.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "gpio.h"
#include "shm.h"

#define NSEC_PER_SEC 1000000000

static void blink(int fd, int count)
{
	struct gpiohandle_data data;
	struct share_data *sd;
	struct timespec ts;
	int i = 0;
	int err;

	err = share_create();
	assert(!err);

	sd = share_get();
	assert(sd);

	share_init(sd);

	memset(&data, 0, sizeof(data));
	clock_gettime(CLOCK_MONOTONIC, &ts);

	while (1) {
		pthread_mutex_lock(&sd->m);

		data.values[i % count] = !data.values[i % count];
		err = gpio_set_pins(fd, &data);
		assert(!err);

		sd->index = i;
		sd->val = data.values[i % count];

		pthread_cond_signal(&sd->c);
		pthread_mutex_unlock(&sd->m);

		i++;

		ts.tv_nsec += 50000000;
		while (ts.tv_nsec >= NSEC_PER_SEC) {
			ts.tv_sec++;
			ts.tv_nsec -= NSEC_PER_SEC;
		}

		if (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL) != 0)
			break;
	}
}

int main(int argc, char *argv[])
{
	int fd;

	fd = gpio_get_output_pins("blink", "/dev/gpiochip4", 0, 6);
	assert(fd != -1);

	blink(fd, 6);

	close(fd);

	return 0;
}
