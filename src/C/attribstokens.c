#include <string.h>
#include <stdlib.h>

/**
 * Indica el número de tokens en una cadena
 * Retorna el número de tokens encontrados (al menos 1)
 */

int getNumtokens(char *buffer, const char *delim )  {
	char *cad = strdup(buffer);
	int getNumtokens_r = 0;
	char *token;
	token = strtok(cad, delim); 

	while(token != NULL) {
		getNumtokens_r++;
		token = strtok(NULL, delim); 
	}
	free(cad);
	return getNumtokens_r;
}
/**
 * Devuelve todos los tokens dados por los de limitadores de delim en un array de strings
 */
char ** getTokens (char *buffer, const char *delim)  {
	int tokensnum;
	char **getTokens_r;
	char *cad;
	tokensnum = getNumtokens(buffer, delim );


	getTokens_r = malloc(sizeof(char *) * (tokensnum + 1));
	cad = strdup(buffer);


	getTokens_r[0] = strtok(cad, delim);

	for(int i=0; getTokens_r[i] != NULL; )  {
		i++;
		getTokens_r[i] = strtok(NULL, delim) ;
		
	}
	



	return getTokens_r;
}

