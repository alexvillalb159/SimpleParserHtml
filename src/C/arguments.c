#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "genericlist.h"
#include "arguments.h"

/**
 * 	Colocar las etiquetas en tags y los atributos en attribs.
 *	Devuelve 1 si hizo la asignación
 *	0 en caso de no poder hacer la asignación a causa de un error de sintaxis.  
 *	
 */
int getTagsAndAttribs (char *argvs[], char ***tags, char ****attribs, int *length) {
	//	strcasestr( );

	// Crea una lista de punteros
	struct listgeneric *tags_l	= createlistgeneric(sizeof(char * ),  0);  	// lista de tags
	struct listgeneric *tagsAndAttributos  = createlistgeneric(sizeof(struct listgeneric *), 0 ); // lista de lista 
									// que todos los tags con sus atributos 

	struct listgeneric *attribs_l  	= createlistgeneric(sizeof(char *), 0 ); // lista de atributos asociada a un tag

	int getTagsAndAttribs_r = 1;
	int isAttrib_r = isAttrib (argvs[0] );	
	if(isAttrib_r ||  isAttrib_r == -1 ) {
		// Error de sintaxis el primer parámetro debe ser un tags
		getTagsAndAttribs_r = 0;
		
	}
	else {
		addlist(tags_l,		&argvs[0]  	);
		addlist(tagsAndAttributos, 	&attribs_l	);



		for(int i=1; argvs[i] != NULL; i++) {

			isAttrib_r = isAttrib(argvs[i]);
			if( isAttrib_r == 1 ) {
				// Es un attrib
				addlist(attribs_l, &argvs[i]);
			} else if (!isAttrib_r ){
				// Es un tag
				addlist(tags_l, &argvs[i]);
				attribs_l 	= createlistgeneric(sizeof(struct listgeneric *), 0 );
				addlist(tagsAndAttributos, 	&attribs_l	);
			} else {
				getTagsAndAttribs_r = 0; break;
   			}

		}

	}
	if(getTagsAndAttribs_r) {
		*length = tags_l->length;
		*tags = passlisttoArray(tags_l);
		dellAllList(tags_l);

		*attribs = passlisttolisttoArraytoArray( tagsAndAttributos);
	}
	return getTagsAndAttribs_r ;
}
/**
 * Indica si la cadena es un atributo verificando si corresponde al patron attributo=valor, no es un verficación completa
 * sino mínima e incompleta de la sintaxis.
 * Devuelve: 0: si no es un atributo, 1: si es un atributo, -1 en el caso sintaxis incorrecta	
 */
int isAttrib (char *cadena ) {
	int isAttrib_r = 1;
	char *ptr; 
	if((ptr =index(cadena, '=')) != NULL) {

		// hace una verificación mínima de la sintaxis
		if(ptr == cadena) {
			// Sintaxis incorrecta
			isAttrib_r = 	-1;
		} 
		else if(ptr == cadena + strlen(cadena) - 1) {
			// Sintaxis incorrect
			isAttrib_r = 	-1;
		}
		else if(isspace(*(ptr + 1) ) || isspace(*(ptr - 1)) ) {
			// Sintaxis incorrecta
			isAttrib_r = 	-1;
			
		} 			

	} else 	isAttrib_r = 	0;
	return isAttrib_r ;

}

/**
 * Pasa la lista a un array.
 */
char ** passlisttoArray(struct listgeneric *list) {
	char **passlisttoArray_r  = malloc((list->length + 1) * sizeof (void *) );

	struct list *piv = list->head;

	for(int i=0; i < list->length; i++) { 
		passlisttoArray_r[i] =  *((char **) piv->element) ;
		piv = piv->sig;
	}
	passlisttoArray_r[list->length] = NULL;

	return passlisttoArray_r;
}

/**
 * Pasa la lista de listas a un array de array de array, es decir un array de 3 dimensiones
 */
char *** passlisttolisttoArraytoArray(struct listgeneric *list_ll) {
        
	char ***passlisttolisttoArraytoArray_r  = malloc((list_ll->length + 1) * sizeof (void *) );

	struct list *piv = list_ll->head;

	for(int i=0; i < list_ll->length; i++) { 
		struct listgeneric *listp = *((void **) piv->element); //  piv->element;
 
		passlisttolisttoArraytoArray_r[i] =  malloc((listp->length + 1) * sizeof (void *) );
		int j=0;
		struct list *piv2 = listp->head;

		for(; j < listp->length; j++)  {
			passlisttolisttoArraytoArray_r[i][j] = *((char **) piv2->element);
			//(char *[]) **((void ***) piv2->element)

			piv2 = piv2->sig;

		}
		passlisttolisttoArraytoArray_r[i][listp->length] = NULL;
		piv = piv->sig;
	}
	passlisttolisttoArraytoArray_r [list_ll->length] = NULL;

	return passlisttolisttoArraytoArray_r;

}


