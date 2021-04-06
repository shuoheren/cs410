#include <bfd.h>
#include <stdlib.h>

// Defining section node struct with only relevant info
typedef struct node {
  const char* name;
  unsigned int i;
  unsigned int addr;
  unsigned int rsize;
  unsigned int csize;
  unsigned int pos;
  struct node * next;
} sec;

// Main library function for accessing sections of an executable or object file
sec * getsections(char* file) {
  // Initialize the BFD
  bfd_init();
  // Architecture
  const char* target = "elf64-x86-64";
  // Open specified BFD
  bfd *abfd = bfd_openr(file, target);
  // Necessary check format function
  bfd_boolean checkformat = bfd_check_format(abfd, bfd_object);
  // Pointer to LL of all sections - we want to make our own
  // That can be accessed without bfd library w only info we need
  struct bfd_section *secptr = (*abfd).sections;
  struct bfd_section *ptr = NULL;
  // Initialize head of our new LL
  sec * head = NULL;
  head = malloc(sizeof(sec));
  head->next = NULL;
  sec * current = head;
  // Iterate through LL of sections and build our own
  for (ptr = secptr; ptr != NULL; ptr = ptr->next) {
    current->next = malloc(sizeof(sec));
    current->next->name = (*ptr).name;
    current->next->i = (*ptr).index;
    current->next->addr = (*ptr).vma;
    current->next->rsize = (*ptr).size;
    current->next->csize = (*ptr).size;
    current->next->pos = (*ptr).filepos;
    current->next->next = NULL;
    current = current->next;
  }
  // Remove empty null element at beginning of LL bc I don't know how to program
  head = head->next;
  return head;
}