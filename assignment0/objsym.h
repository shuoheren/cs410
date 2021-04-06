typedef struct node {
  const char* name;
  unsigned int vma;
  struct node * next;
} sym;

sym * getsymtable(char *file);