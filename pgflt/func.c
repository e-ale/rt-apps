#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

void timestamp(const char *msg, int val)
{
	static struct timespec tsl;
	static long minfltl;

	struct timespec ts;
	unsigned long diff;
	struct rusage r;
	long rdiff;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	getrusage(RUSAGE_SELF, &r);

	if (tsl.tv_sec != 0) {
		diff = (ts.tv_sec - tsl.tv_sec) * 1000000000;
		diff += ts.tv_nsec - tsl.tv_nsec;

		rdiff = r.ru_minflt - minfltl;

		printf("% 10d ns % 5ld faults : %s", diff, rdiff, msg);
		if (val >= 0)
			printf(" (%d)", val);
		printf("\n");
	}

	tsl = ts;
	minfltl = r.ru_minflt;
}

/* 10 MiB */
#define SIZE (10 * 1024 * 1024)

void testfunc_malloc(void)
{
	char *p;

	p = malloc(SIZE);
	if (!p) {
		fprintf(stderr, "MALLOC FAILED: %s:%d\n", __FILE__, __LINE__);
		return;
	}

	memset(p, 42, SIZE);

	if (p[SIZE - 1] != 42)
		fprintf(stderr, "MEMSET FAILED: %s:%d\n", __FILE__, __LINE__);

	free(p);
}

void recursive_stack(int i)
{
	char buf[1024];
	if (i > 0)
		recursive_stack(i - 1);
}

void testfunc_deepstack(void)
{
	char buf[1024];
	recursive_stack(7 * 1024);
}
