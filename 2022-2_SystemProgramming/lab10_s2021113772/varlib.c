#include	<stdio.h>
#include	<stdlib.h>
#include	"varlib.h"
#include	<string.h>

#define	MAXVARS	200		/* a linked list would be nicer */

struct var {
		char *str;		/* name=val string	*/
		int  global;		/* a boolean		*/
	};

static struct var tab[MAXVARS];			/* the table	*/

static char *new_string( char *, char *);	/* private methods	*/
static struct var *find_item(char *, int);

int VLstore( char *name, char *val )
{
	struct var *itemp;
	char	*s;
	int	rv = 1;

	/* find spot to put it              and make new string */
	if ((itemp=find_item(name,1))!=NULL && (s=new_string(name,val))!=NULL) 
	{
		if ( itemp->str )		/* has a val?	*/
			free(itemp->str);	/* y: remove it	*/
		itemp->str = s;
		rv = 0;				/* ok! */
	}
	return rv;
}

char * new_string( char *name, char *val )
{
	char	*retval;

	retval = malloc( strlen(name) + strlen(val) + 2 );
	if ( retval != NULL )
		sprintf(retval, "%s=%s", name, val );
	return retval;
}

char * VLlookup( char *name )
{
	struct var *itemp;

	if ( (itemp = find_item(name,0)) != NULL )
		return itemp->str + 1 + strlen(name);
	return "";

}

int VLexport( char *name )
{
	struct var *itemp;
	int	rv = 1;

	if ( (itemp = find_item(name,0)) != NULL ){
		itemp->global = 1;
		rv = 0;
	}
	else if ( VLstore(name, "") == 1 )
		rv = VLexport(name);
	return rv;
}

static struct var * find_item( char *name , int first_blank )
{
	int	i;
	int	len = strlen(name);
	char	*s;

	for( i = 0 ; i<MAXVARS && tab[i].str != NULL ; i++ )
	{
		s = tab[i].str;
		if ( strncmp(s,name,len) == 0 && s[len] == '=' ){
			return &tab[i];
		}
	}
	if ( i < MAXVARS && first_blank )
		return &tab[i];
	return NULL;
}


void VLlist()
{
	int	i;
	for(i = 0 ; i<MAXVARS && tab[i].str != NULL ; i++ )
	{
		if ( tab[i].global )
			printf("  * %s\n", tab[i].str);
		else
			printf("    %s\n", tab[i].str);
	}
}

int VLenviron2table(char *env[])
{
	int     i;
	char	*newstring;

	for(i = 0 ; env[i] != NULL ; i++ )
	{
		if ( i == MAXVARS )
			return 0;
		newstring = malloc(1+strlen(env[i]));
		if ( newstring == NULL )
			return 0;
		strcpy(newstring, env[i]);
		tab[i].str = newstring;
		tab[i].global = 1;
	}
	while( i < MAXVARS ){		/* I know we don't need this	*/
		tab[i].str = NULL ;	/* static globals are nulled	*/
		tab[i++].global = 0;	/* by default			*/
	}
	return 1;
}

char ** VLtable2environ()
{
	int	i,			/* index			*/
		j,			/* another index		*/
		n = 0;			/* counter			*/
	char	**envtab;		/* array of pointers		*/

	/*
	 * first, count the number of global variables
	 */

	for( i = 0 ; i<MAXVARS && tab[i].str != NULL ; i++ )
		if ( tab[i].global == 1 )
			n++;

	/* then, allocate space for that many variables	*/
	envtab = (char **) malloc( (n+1) * sizeof(char *) );
	if ( envtab == NULL )
		return NULL;

	/* then, load the array with pointers		*/
	for(i = 0, j = 0 ; i<MAXVARS && tab[i].str != NULL ; i++ )
		if ( tab[i].global == 1 )
			envtab[j++] = tab[i].str;
	envtab[j] = NULL;
	return envtab;
}