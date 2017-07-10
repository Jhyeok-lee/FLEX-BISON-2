#include <stdio.h>
#include <time.h>

#define NSPS 1000000000.0

struct timespec ts;
static int first = 1;

void set_timer(){
	if (first) {
		clock_gettime(CLOCK_REALTIME, &ts);
		first = 0;
	}
}

void reset_timer(){
	clock_gettime(CLOCK_REALTIME, &ts);
}

double lap_timer(){
	struct timespec __ts;
	double lap_time;

	clock_gettime(CLOCK_REALTIME, &__ts);
	lap_time = (double) (__ts.tv_sec - ts.tv_sec) + (double) (__ts.tv_nsec - ts.tv_nsec) / NSPS;
	ts = __ts;
	return lap_time;
}

double get_timer(){
	struct timespec __ts;
	double lap_time;

	if (first){
		set_timer();
		return 0.0;
	}

	clock_gettime(CLOCK_REALTIME, &__ts);
	lap_time = (double) (__ts.tv_sec - ts.tv_sec) + (double) (__ts.tv_nsec - ts.tv_nsec) / NSPS;
	return lap_time;
}
