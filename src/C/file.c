#include <string.h>
#include <stdlib.h>
#include "file.h"
#include "utils.h"
/**
 * Encuentra una string independientemente del contexto de busqueda en el stream in. 
 * Si no consigue el string devuelve un valor > 255 ó < 0 caso contrario devuelve
 * un valor correspondiente al ultimo byte leído del string buscado y coloca el cursor del Stream
 * al principio de la cadena encontrada. Este valor es menor o igual a 255
 */

int findString (FILE *in, const char *string) {
	int f_igualesext_u = 1; // esta bandera indica si todos los caracteres son iguales excepto el ultimo
	char initcaract = string[0]; 
	int caracter, i=0; 
	int string_length = strlen(string);

	// Verifica si todos los caracteres son iguales excepto el ultimo			
	if(string[1] == '\0') { 		// Caso de un carácter
		f_igualesext_u = 0;
	} 		
	else if(string[2] == '\0')  { 	// Caso de 2 caracteres
			
		if(string[0] == string[1]) f_igualesext_u = 0;
	} 
	else { 			// Caso más de 2 caracteres
		for(i = 1; string[i + 1] != '\0'; i++) {
			if(string[0] != string[i] ) {
				f_igualesext_u = 0;	
			}
		}	
	}


	if(f_igualesext_u ) {
		do {	 
			caracter = fgetc(in);
			if(caracter == string[i]) {
				i++;
			} else if(caracter != initcaract) {
				i=0;
			} 
		} while	( NOTEOF(caracter) && i != string_length);	
	} else {
		do {
			caracter = fgetc(in);
			if(caseU(caracter) == caseU(string[i]) ) {
				i++;
			} else if(caseU(caracter) != caseU(initcaract)) {
				i=0;
			} else {
				i=1;
			}
		} while	(NOTEOF(caracter) && i != string_length);
	}
	if(i == string_length ) {
		fseek(in,(-1)*string_length, SEEK_CUR);
	}

	return caracter;
}

/**
 * Verifica si la string que esta al comiento del stream tiene el valor de tag.
 * Devuelve el ultimo caracter que leyo si es asi en caso de o EOF o -1 en caso de fallo. 
 */ 

int  strcmp_m (FILE *in, const char *tag ) {
	int i=0;
	int caracter;
	do {
		caracter = fgetc (in);
		if(caseU(tag[i]) != caseU(caracter)) {
			return -1; 
			break;
		}
		i++;

		if(tag[i]== '\0') { 
			return caracter;
		}
		
	} while (NOTEOF(caracter) ) ;

	return caracter;
}

/**
 * Toma el campo del stream *in deliminado al final por cualquiera de los caracteres que esten en la cadena delimiters. 
 */
char * getField(FILE *in, const char *delimiters) {
	long offset;
	char *field;
	int caracter; 
	int i, name_length; 

	offset = ftell(in);

	caracter = fgetc(in);
	for(i=0; NOTEOF(caracter) && !isCharacter( caracter, delimiters ); ) {
		caracter = fgetc(in);
		i++;
	} 
	name_length = i;
	fseek(in, offset, SEEK_SET);	

	field = malloc(name_length + 1);

	for(i=0; i < name_length; i++  ) {
		caracter = fgetc(in);
		field[i] = caracter;
	} 
	field[name_length]= 0;
	//fseek(in, -1, SEEK_CUR);

	return field;

}
