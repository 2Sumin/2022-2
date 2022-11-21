// smsh.h -- declares function profiles for external references

#define YES 1
#define NO 0

char *next_cmd(char *, FILE *);

char **splitline(char *);

// free the list returned by splitline
void freelist(char **);

void *emalloc(size_t);

void *erealloc(void *, size_t);

// run a program
int execute(char **);

// report an error
void fatal(char *, char *, int);
int process(char **);