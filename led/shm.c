#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "shm.h"

#define SHARE_NAME "/blink.shm"

/* open and return a pointer to shared data */
struct share_data *share_get(void)
{
	struct share_data *data;
	int fd;

	fd = shm_open(SHARE_NAME, O_RDWR, 0);
	assert(fd != -1);

	data = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE,
		    MAP_SHARED, fd, 0);
	assert(data != MAP_FAILED);

	close(fd);

	return data;
}

/* initialize shared data */
void share_init(struct share_data *data)
{
	pthread_mutexattr_t mattr;
	pthread_condattr_t cattr;

	pthread_mutexattr_init(&mattr);
	pthread_mutexattr_setprotocol(&mattr, PTHREAD_PRIO_INHERIT);
	pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&data->m, &mattr);

	pthread_condattr_init(&cattr);
	pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
	pthread_cond_init(&data->c, &cattr);
}

/* create a zero'd out file of size 1 page */
int share_create(void)
{
	int ret;
	int fd;

	fd = shm_open(SHARE_NAME, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	assert(fd != -1);

	ret = ftruncate(fd, sysconf(_SC_PAGESIZE));

	close(fd);

	return ret;
}
