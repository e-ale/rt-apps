#ifndef SHM_H
#define SHM_H

#include <pthread.h>

struct share_data {
	pthread_mutex_t m;
	pthread_cond_t c;
	int index;
	int val;
};

extern int share_create(void);
extern void share_init(struct share_data *data);
extern struct share_data *share_get(void);

#endif /* SHM_H */
