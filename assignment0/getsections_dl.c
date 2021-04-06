#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "objsect.h"

#define rdtsc(var)												\
{																\
	int var##_lo, var##_hi;										\
	asm volatile("rdtsc" : "=a"(var##_lo), "=d"(var##_hi));		\
	var = var##_hi;												\
	var <<= 32;													\
	var |= var##_lo;											\
}																\

unsigned long long start, finish;

// Function to convert unsigned integer to char array w base
char* tostring(int val, char* buf, int base) {
	int i = 0;

	if (val == 0) {
		buf[i++] = '0';
		buf[i] = '\0';
		return buf;
	}
	while (val != 0) {
		int r = val % base;
		if (r > 9) {
			buf[i++] = (r - 10) + 'a';
		}
		else {
			buf[i++] = r + '0';
		}
		val = val / base;
	}
	buf[i] = '\0';

	char* str = malloc(sizeof(buf));
	str[i] = '\0';
	int x;
	for (x = 0; x < i; x++) {
		str[x] = buf[i - 1 - x];
	}
	return str;
}

// Function to space columns depending on width of specified column
void print_offset(const char* str, int offset) {
	char space[1] = {" "};
	int i;
	for (i = 0; i < (offset - strlen(str)); i++) {
		write(1, space, 1);
	}
}

// Function to run dlopen() 50 times with specified flag to micro-benchmark avg cost
void calc_stats(int flag) {
	void *libtest;
	float sum = 0;
	float timestat;
	char newline[1] = {"\n"};
	int i;
	for (i = 0; i < 50; i++) {
		if (flag == 0) {
			rdtsc(start);
			libtest = dlopen("libobjdata.so", RTLD_LAZY);
			rdtsc(finish);
			timestat = ((finish - start) / 2599.994);
			dlclose(libtest);
		}
		else {
			rdtsc(start);
			libtest = dlopen("libobjdata.so", RTLD_NOW);
			rdtsc(finish);
			timestat = ((finish - start) / 2599.994);
			dlclose(libtest);
		}
		sum = sum + timestat;
		char* timecalcbuf = malloc(32 * sizeof(char));
		gcvt(timestat, 7, timecalcbuf);
		write(1, timecalcbuf, strlen(timecalcbuf));
		free(timecalcbuf);
		write(1, newline, 1);
	}
	write(1, newline, 1);
	char *avgtime = "Average time: ";
	write(1, avgtime, strlen(avgtime));
	float avg = sum / 50;
	char* timecalcbuf = malloc(32 * sizeof(char));
	gcvt(avg, 7, timecalcbuf);
	write(1, timecalcbuf, strlen(timecalcbuf));
	write(1, newline, 1);
}

// Main function to dynamically load library and call library function
int main(int argc, char *argv[]) {
	// Filename cmd line arg
	char *filename = argv[1];
	// int flag arg
	const char *flag = argv[2];
	// RTLD_LAZY or RTLD_NOW
	void *lib;
	float time;
	int flagint;
	const char *lazy = "RTLD_LAZY";
	const char *now = "RTLD_NOW";
	if (strcmp(flag, lazy) == 0) {
		// CALL LAZY BINDING
		flagint = 0;
		rdtsc(start);
		lib = dlopen("libobjdata.so", RTLD_LAZY);
		rdtsc(finish);
		time = ((finish - start) / 2599.994);
	}
	else if (strcmp(flag, now) == 0) {
		// CALL IMMEDIATE BINDING
		flagint = 1;
		rdtsc(start);
		lib = dlopen("libobjdata.so", RTLD_NOW);
		rdtsc(finish);
		time = ((finish - start) / 2599.994);
	}
	else {
		exit(0);
	}

	// CALLING FUNCTION TO MICRO BENCHMARK AVERAGE COST OF dlopen()
	// COMMENT OUT IF NOT CALCULATING STATS
	// calc_stats(flagint);

	// Declaring space and newline character arrays to be used by write()
	char newline[1] = {"\n"};
	char space[1] = {" "};
	// Printing recorded time
	// Not even sure if time needs to be printed when we're not trying to benchmark
	// but I left it here anyways to be safe
	char* timebuf = malloc(32 * sizeof(char));
	gcvt(time, 7, timebuf);
	char *timeheader = "TIME: ";
	write(1, timeheader, strlen(timeheader));
	write(1, timebuf, strlen(timebuf));
	write(1, newline, 1);
	free(timebuf);

	// Call library function and write requested data
	sec * head = getsections(filename);
	// Recursively iterate through ll and print to std out
	sec * current = head;
	// Print header
	const char* header = "Name                VMA       cSize     Position\n";
	write(1, header, strlen(header));
	while (current != NULL) {
		char* buffer = malloc(32 * sizeof(char));
		
		write(1, current->name, strlen(current->name));
		print_offset(current->name, 20);

		char* vma = tostring(current->addr, buffer, 16);
		write(1, vma, strlen(vma));
		print_offset(vma, 10);

		char* cookedsize = tostring(current->csize, buffer, 16);
		write(1, cookedsize, strlen(cookedsize));
		print_offset(cookedsize, 10);

		char* position = tostring(current->pos, buffer, 16);
		write(1, position, strlen(position));
		write(1, newline, 1);

		free(buffer);
		current = current -> next;
	}

	dlclose(lib);
	return 0;
}