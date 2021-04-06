#include <bfd.h>
#include <stdlib.h>

// Defining symbol node struct with only relevant info
typedef struct node{
	const char* name;
	unsigned int vma;
	struct node * next;
} sym;

// Main library function for accessing symbol table
sym * getsymtable(char *file) {
	// Initialize the BFD
	bfd_init();
	// Architecture
	const char* target = "elf64-x86-64";
	// Open specified BFD
	bfd *abfd = bfd_openr(file, target);
	// Necessary check format function
	bfd_boolean checkformat = bfd_check_format(abfd, bfd_object);
	// Reading symbol table from a BFD
	long bytes_needed = bfd_get_symtab_upper_bound(abfd);
	asymbol **symtable = malloc(bytes_needed);
	unsigned int symcount = bfd_canonicalize_symtab(abfd, symtable);
	// Initialize head of symbol ll
	sym * head = NULL;
	head = malloc(sizeof(sym));
	head->next = NULL;
	sym * current = head;
	// Iteratively build up linked list of symbols
	int i;
	for (i = 0; i < symcount; i++) {
		asymbol *sym = symtable[i];
		current->next = malloc(sizeof(sym));
		current->next->name = (*sym).name;
		struct bfd_section *sec = (*sym).section;
		unsigned int addr = (*sec).vma;
		current->next->vma = addr + (*sym).value;
		current->next->next = NULL;
		current = current->next;
	}
	// Remove empty null element at beginning of LL bc once again I don't know how to use a computer
	head = head->next;
	return head;
}