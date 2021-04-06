typedef struct node {
  const char* name;
  unsigned int i;
  unsigned int addr;
  unsigned int rsize;
  unsigned int csize;
  unsigned int pos;
  struct node * next;
} sec;

sec * getsections(char *file);