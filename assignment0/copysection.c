#include "objcopy.h"

int main(int argc, char *argv[]) {
	// Filename cmd line arg
	char *filename = argv[1];
	// Call library function, which is void and does all of the work itself
	copysec(filename);
	// So we're done !
	return 0;
}