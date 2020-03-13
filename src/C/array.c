#include <stdlib.h>
#include <string.h>
#include "array.h"



void initArray(void ***array) {
	struct defArray *defarray =  malloc(sizeof(struct defArray));
	defarray->length=0;
	//defarray->elementsize = elementsize;

	// La primera vez el array solo tiene un elemento que es únicamente su definición
	*array = malloc(sizeof(void *));
	(*array)[0] = defarray;
	//printf ("*array = %x --- \n", *array);
	//printf ("defarray = %x --- \n", defarray); 
	//printf ("(*array)[0] = %x --- \n", (*array)[0]); 
}


void addArray(void ***array, void *element ) {
	void **oldarray = *array;
	struct defArray *defarray =  oldarray[0];
	//printf ("defarray2 = %x --- \n", defarray); 
	int newlength = defarray->length + 1;
	
	// Reserva espacio para los nuevos elementos del array y su definición. 
	*array =  malloc(sizeof(void *) * (newlength + 1));
	
	// mueve el array
	movArray(oldarray, *array);
	// copia el nuevo elemento
	(*array)[newlength] = element;
	defarray->length = newlength;
	// borra el viejo puntero del array
	free(oldarray) ;

}

void movArray(void **array_src, void **array_dst) {
	struct defArray *defarray =  *array_src;
	array_dst[0] =  array_src[0];
	for(int i=0; i <  defarray->length ; i++) array_dst[i+1] = array_src[i+1];

}

void delArray (void ***array) {
	struct defArray *defArray =  (*array)[0];	
	
	for(int i=0; i <= defArray->length; i++) free((*array)[i]);
}

int getLengthArray2(void **array) {
	struct defArray *defarray =  array[0];
	return defarray->length;
}


