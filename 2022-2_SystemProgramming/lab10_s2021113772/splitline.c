#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smsh.h"

#define is_delim(x) ((x)==' '||(x)=='\t')

char *next_cmd (char *prompt, FILE *fp)
{
    char *buf; // the buffer
    int bufspace = 0; // total size
    int pos = 0; // current position
    int c; // input char

    printf ("%s", prompt); // prompt user
    while ((c = getc (fp)) != EOF)
    {
        // need space?
        if (pos + 1 >= bufspace) // 1 for \0
        {
            if (bufspace == 0) // y: 1st time
                buf = emalloc (BUFSIZ);
            else // or expand
                buf = erealloc (buf, bufspace + BUFSIZ);
            bufspace += BUFSIZ; // update size
        }

        // end of command? break
        if (c == '\n')
            break;

        // no, add to buffer
        buf[pos++] = c;
    }

    if (c == EOF && pos == 0) // EOF and no input
        return NULL;

    buf[pos] = '\0';
    return buf;
}

char **splitline (char *str)
{
    char *newstr ();
    char *cp = str; // pos in string
    char *start;
    int len;
    char **args;
    int spots = 0; // spots in table

    if (str == NULL) // handle special case
        return NULL;

    args = emalloc (sizeof (char *) * (spots + 1)); // initialize array
    while (*cp != '\0')
    {
        while (*cp == ' ' || *cp == '\t') // skip leading spaces
            cp++;
        start = cp; // start of word
        len = 1;
        while (*++cp != '\0' && !(*cp == ' ' || *cp == '\t')) // find end
            len++;
        args[spots++] = newstr (start, len);
        args = erealloc (args, sizeof (char *) * (spots + 1));
    }
    args[spots] = NULL;
    return args;
}

char *newstr (char *s, int l)
{
    char *rv = emalloc (l + 1);

    rv[l] = '\0';
    strncpy (rv, s, l);
    return rv;
}

void freelist (char **list)
{
    char **cp = list;
    while (*cp)
        free (*cp++);
    free (list);
}

void *emalloc (size_t n)
{
    void *rv;
    if ((rv = malloc (n)) == NULL)
        fatal ("out of memory", "", 1);
    return rv;
}

void *erealloc (void *p, size_t n)
{
    void *rv;
    if ((rv = realloc (p, n)) == NULL)
        fatal ("realloc() failed", "", 1);
    return rv;
}