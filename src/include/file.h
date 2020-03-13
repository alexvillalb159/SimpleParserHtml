#ifndef ALX_FILE_H
#define ALX_FILE_H

#include <stdio.h>

int getLengthline(FILE *fp, int *caracter);
int readline(FILE *, char ** );
int findString	(	FILE *, const char *string) ;
int  strcmp_m 	(	FILE *, const char * );
char * getField	(	FILE *, const char *);

#endif
