CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall
LDFLAGS = -lpthread -lrt

all: ledmaster ledmirror

ledmaster: master.c gpio.c shm.c
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o$@

ledmirror: mirror.c gpio.c shm.c
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o$@

clean:
	rm -f ledmaster ledmirror

.PHONY: clean
