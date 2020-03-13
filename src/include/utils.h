#ifndef ALX_UTILS_H
#define ALX_UTILS_H

#define NOTEOF(caracter)	(caracter >= 0 && caracter <= 255)
#define CASTULONG(expr)  *((unsigned long *) expr)

int getLengthArray(void *);
unsigned char caseU(char, int);
int isCharacter(int  , const char * );

#endif
