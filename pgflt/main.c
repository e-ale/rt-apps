#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern void timestamp(const char *msg, int val);
extern void testfunc_malloc(void);
extern void testfunc_deepstack(void);
extern void setup_rt(unsigned int opts);

static void usage(const char *cmd)
{
	printf("usage: %s [opts-bitmask]\n", cmd);
	printf("  opts-bits:\n");
	printf("  0x01 = mallopt\n");
	printf("  0x02 = mlockall\n");
	printf("  0x04 = prefault-stack\n");
	printf("  0x08 = prefault-heap\n");
	printf("  0x10 = run tests\n");
	printf("\n");
	printf("  0x10 = no rt tweaks + tests\n");
	printf("  0x1f = full rt tweaks + tests\n");
	printf("\n");
}

int main(int argc, char *argv[])
{
	unsigned int i;

	if (argc != 2) {
		usage(argv[0]);
		return 1;
	}

	if (sscanf(argv[1], "%x", &i) != 1) {
		usage(argv[0]);
		return 1;
	}

	printf("options: 0x%x\n", i);

	timestamp("init", -1);

	setup_rt(i);

	timestamp("main setup", -1);

	if (i & 0x10) {
		for (i = 0; i < 4; i++) {
			testfunc_malloc();
			timestamp("testfunc_malloc", i);
		}

		for (i = 0; i < 4; i++) {
			testfunc_deepstack();
			timestamp("testfunc_deepstack", i);
		}
	}

	return 0;
}
