#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NUM_THREADS 7

struct posize {
	unsigned int x;
	unsigned int y;
};

struct thread_data {
	unsigned int startx;
	unsigned int endx;
};

size_t max_size_x = 10000;
size_t max_size_y = 10000;

unsigned long _kolo = 0;

struct posize *points = NULL;

static pthread_mutex_t mutex;

void *check_points(void *_data)
{
	struct thread_data *data = _data;
	unsigned int startx = data->startx;
	unsigned int endx = data->endx;
	unsigned int kolo = 0;

	for(size_t x = startx; x < endx; x++)
	{
		for(size_t y = 0; y < max_size_y; y++)
		{
			if(sqrt(x * x + y * y) <= max_size_x)
				kolo++;
		}
	}

	pthread_mutex_lock(&mutex);
	_kolo += kolo;
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main()
{
	pthread_mutex_init(&mutex, NULL);
	pthread_t threads[NUM_THREADS];

	long double part_lenx = (long double)max_size_x / NUM_THREADS;

	struct thread_data data[NUM_THREADS];

	int i = 0;
	for(unsigned int x = 0; x < NUM_THREADS; x++)
	{
		data[i].startx = x * part_lenx;
		data[i].endx = (x+1) * part_lenx;

		int t = pthread_create(&threads[i], NULL, check_points, &data[i]);
		if(t)
		{
			printf("Failed to create %d thread (%d)\n", i, t);
			exit(-1);
		}

		i++;
	}

	for(unsigned int i = 0; i < NUM_THREADS; i++)
	{
		void *status;
		int t = pthread_join(threads[i], &status);
		if(t)
			printf("Error in thread %d (%d)", i, t);
	}

	printf("%lf\n", ((double)_kolo/(max_size_x * max_size_y)) * 4);

	return 0;
}
