#include <stdlib.h>
#include <pthread.h>

struct Point {
	int x;
	int y;
};

int calculate_sum(int a, int b) {
	struct Point* p = malloc(sizeof(struct Point)); // Heap allocation
	p->x = a;
	p->y = b;
	int sum = p->x + p->y;
	free(p);
	return sum;
}

struct Point {
	int x;
	int y;
};

int calculate_sum(int a, int b) {
	struct Point p;  // Stack allocation (potential optimization)
	p.x = a;
	p.y = b;
	int sum = p.x + p.y;
	return sum;
}

struct Data { int value, flag; };

void scalar_replacement_before(struct Data d) {
	if (d.flag)
		return d.value;
}

// Object fields as parameters
int scalar_replacement_after(int d_value, int d_flag) {
	if (d_flag)
		return d_value;
}

pthread_mutex_t Lock;

void* thread_func(void* arg) {
	int x;
	pthread_mutex_lock(&Lock);
	x++;
	pthread_mutex_unlock(&Lock);
	return NULL;
}

int GV = 0;

void func(int flag) {
	int a = 0, b = 0, c = 0;
	if (flag) {
		GV = a;
	} else {
		GV = b;
	}
	c = b;
}