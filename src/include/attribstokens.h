#ifndef ALX_ATTRIBSTOKENS_H
#define ALX_ATTRIBSTOKENS_H

/**
 * Indica el número de tokens en una cadena
 * Retorna el número de tokens encontrados (al menos 1)
 */

int getNumtokens(char *, const char * );  

/**
 * Devuelve todos los tokens dados por los de limitadores de delim en un array de strings
 */
char ** getTokens (char *, const char *); 

#endif
