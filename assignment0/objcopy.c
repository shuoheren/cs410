#include <bfd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

void copysec(char *file) {
	// Initialize the BFD
	bfd_init();
	// Architecture
	const char* target = "elf64-x86-64";
	// Open specified BFD
	bfd *abfd = bfd_openr(file, target);
	// Necessary check format function
	bfd_boolean checkformat = bfd_check_format(abfd, bfd_object);
	// Get .text section
	asection *section = bfd_get_section_by_name(abfd, ".text");
	// Get the size of the section
	bfd_size_type sec_size = bfd_section_size(abfd, section);
	// Get a buffer for the size of the section
	char sec_contents[sec_size];
	// Write contents of section to the buffer we just made
	bfd_get_section_contents(abfd, section, sec_contents, (file_ptr)0, (bfd_size_type)sec_size);
	// Write buffer w contents of section to fd we get by calling creat() and then close the fd
	int fd = creat("text-output", 0644);
	write(fd, sec_contents, sizeof(sec_contents));
	close(fd);
}