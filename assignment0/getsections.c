#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "objsect.h"

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

// Main function to call library function and print output
int main(int argc, char *argv[]) {
	// Filename cmd line arg
	char *filename = argv[1];
	// Call library function to get a pointer to a ll
	sec * head = getsections(filename);
	// Recursively iterate through ll and print to std out
	sec * current = head;
	// Print header
	const char* header = "Name                VMA       cSize     Position\n";
	write(1, header, strlen(header));
	while (current != NULL) {
		char* buffer = malloc(32 * sizeof(char));
		char newline[1] = {"\n"};
		char space[1] = {" "};
		
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

	return 0;
}