#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/mman.h>

extern void timestamp(const char *msg, int val);
extern void testfunc_malloc(void);
extern void testfunc_deepstack(void);

void prefault_stack(void)
{
#define STACK_SIZE (7680 * 1024) /* 7.5 MiB */
	char buf[STACK_SIZE];
	long pagesize;
	int i;

	pagesize = sysconf(_SC_PAGESIZE);

	for (i = 0; i < STACK_SIZE; i += pagesize)
		buf[i] = 0;
}

void prefault_heap(void)
{
#define HEAP_SIZE (20 * 1024 * 1024) /* 20 MiB */
	long pagesize;
	char *buf;
	int i;

	pagesize = sysconf(_SC_PAGESIZE);

	buf = malloc(HEAP_SIZE);
	if (!buf) {
		fprintf(stderr, "MALLOC FAILED: %s:%d\n", __FILE__, __LINE__);
		return;
	}

	for (i = 0; i < HEAP_SIZE; i += pagesize)
		buf[i] = 0;

	free(buf);
}

void setup_rt(unsigned int opts)
{
	if (opts & 0x1) {
		if (mallopt(M_TRIM_THRESHOLD, -1) == 0) {
			fprintf(stderr, "MALLOPT FAILED: %s:%d\n",
				__FILE__, __LINE__);
		}

		if (mallopt(M_MMAP_MAX, 0) == 0) {
			fprintf(stderr, "MALLOPT FAILED: %s:%d\n",
				__FILE__, __LINE__);
		}
	}

	if (opts & 0x2) {
		if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1) {
			fprintf(stderr, "MLOCKALL FAILED: %s:%d\n",
				__FILE__, __LINE__);
		}
	}

	if (opts & 0x4)
		prefault_stack();

	if (opts & 0x8)
		prefault_heap();
}

