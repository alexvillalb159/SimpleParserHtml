#include <stddef.h>

int getLengthArray(void *array[]) {
	int i;
	for(i=0; array[i] != NULL; i++) ;
	return i;
}

// Pasa una letra caracter a mayúscula
unsigned char caseU(char caracter, int ucase) {

	if(ucase) {
		if(caracter >= 'a' && caracter <= 'z') {
			caracter= 'A' + caracter - 'a';
		}
	}
	return caracter; 
}

// Verifica si caracter esta en la lista *delimiters
int isCharacter(int  caracter, const char *delimiters ) {
	int i;
	for(i=0; delimiters[i] != 0 && delimiters[i] != caracter; i++) ;
	return delimiters[i] == caracter;
}

