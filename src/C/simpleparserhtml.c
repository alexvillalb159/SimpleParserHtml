#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "genericlist.h"
#include "arguments.h"
#include "utils.h"
#include "parser.h"

#define GETCONTENT 	1
#define GETPARENTSTAG	2


void showhelp ();
void getKeysandValues (char **attribs, char ***keys, char ***values);
unsigned char caseU(char caracter);
char * printbuffer(char *nueva);
void showcontent(FILE *in, const char *, struct listgeneric *offsets);
void showcontent_n1(FILE *in, const char *lasttag, long offset);

void showparentstags(FILE *in, struct listgeneric *listOflists, struct listgeneric *offsets);


int main (int argc, char *argv[]) {
	FILE *in;
	char namefile[2048];	

	int opc;
	char **tags, ***attribs;
	int length;
	int deepth;

	if(argc < 2) {
		showhelp (argv[0]);
		return -1;
	} else if(!strcmp(argv[1],"-d") ) {
		if(argc < 5) {
			fprintf(stderr, "Faltan parametros\n");
			showhelp(argv[0]);
			return -1;
		}

		strcpy (namefile, argv[3] );
		deepth = atoi(argv[2]); ; 
		opc = GETPARENTSTAG;

	} else if(argc < 3) {
		fprintf(stderr, "Faltan parametros\n");
		showhelp(argv[0]);
		return -1;
	} else {
		// Get content
		opc = GETCONTENT;
		strcpy (namefile, argv[1] );		

	}
	

	if((in=fopen(namefile  , "r")) == NULL) {
		printf("No se pudo abrir el archivo %s\n", namefile);
		return -1;
	}


	if(getTagsAndAttribs (&argv[2], &tags, &attribs, &length)) {
		if(opc == GETCONTENT) {
		



			struct listgeneric *offsets = getcontent(in, tags, attribs);

			if( offsets->length > 0) {
				//printf("Encontro %u contenidos entre la etiquetas pasadas en el archivo: %s\n", 
				//	offsets->length, tags[0] );
				showcontent(in, tags[length - 1], offsets);
			
			}




		}
		else if(opc == GETPARENTSTAG) {
			struct listgeneric *listOfList; 
			struct listgeneric *offsets;
			if(findParentsTag(in, argv[4], &listOfList, &offsets, deepth))  {
				showparentstags(in, listOfList, offsets);				

			}

		}

	} else {
		// Error de sintaxis
	}


	return 0;
}






void showhelp (char *selfname) {
	printf("Usage:\n");
	printf("\t\x1b[1;32mDisplay the content of one o more parents tags with attribs or no:\x1b[0m\n");
	printf("\t%s [FILE] [TAG1] [ATRIB1.1]... [TAB2] [ATTRIB2.1]...\n", 		selfname);
	printf("\t\x1b[1;32mDisplay the parents tags that includes the content:\x1b[0m\n");
	printf("\t%s -d number [FILE] [CONTENT]\n", selfname);
	printf("\tOptions: -d n: indicate the number of parents tag to display\n");	
	printf("\tRemember: for catch the attrib class='nada' you must writing in the arguments class=\'nada\'");		

	


}


/**
 * Guarda todas las cadenas pasadas en "nueva" a un buffer de salidad que puede ser utilizado para imprimir en la
 * salidad estandar con printf u otra función. 	
 * Devuelve el buffer con todas las cadenas *nueva copiadas en cada una de las llamadas
 */
char * printbuffer(char *nueva) {
	static char *output = "\0";
	static int elements = 1;

	if(nueva[0] != '\0') 
		asprintf(&output, "%s\n%d: %s", output, elements, nueva );
	elements++;
	return output;

}

/**
 * Muestra el contenido leyendo del stream in, en función de la lista de offsets. 
 * Cada elemento de la lista de offset se presupone que apunta al comienzo de una etiqueta con un mismo nombre cuyo contenido 
 * (lo que estre la e tiqueta de inicio y final del bloque etiquetado) se va a mostrar.  
 * No se incluye ni las etiquetas ni sus atributos en el contenido.  
 */

void showcontent(FILE *in, const char *lasttag, struct listgeneric *offsets) {

	int tag_length;
	char *buffer;
	unsigned long offset;
	struct list *piv;


	piv = offsets->head;
	fseek(in, CASTULONG(piv->element), SEEK_SET);

	do {
		offset = CASTULONG(piv->element);
		fseek(in, offset, SEEK_SET);
		if(readcontent(in, &buffer, lasttag) ) {
			printbuffer(buffer);


		}
		piv = piv->sig;
	} while ( piv != NULL );

	buffer = printbuffer("\0");
	printf("%s\n", buffer);
	free(buffer);

}

/**
 *
 */
void showcontent_n1(FILE *in, const char *lasttag, long offset) {
	char *buffer;
	fseek(in, offset, SEEK_SET);
	if(readcontent(in, &buffer, lasttag) ) {
		printf("%s", buffer);
	}
}

/**
 *
 */
void showparentstags(FILE *in, struct listgeneric *listOfLists, struct listgeneric *offsets) {
	struct list *listoflist_piv 	= listOfLists->head;
	struct list *offsets_piv 	= offsets->head;
	// offset final de la lista de offsets


	do {
		struct listgeneric *listOftags = *((struct listgeneric **) listoflist_piv->element);
		struct list *listOftags_piv = listOftags->head;  
		long offset_e =  *((long *) offsets_piv->element);
		printf("%Ld: ", offset_e);
		do {
			struct tag *tag = (struct tag *) listOftags_piv->element;
			printf("<");
			printf("%s", tag->name);
			if(tag->keys != NULL) 

				for(int i=0; tag->keys[i] != NULL; i++) {
					if(tag->values[i] != NULL)
						printf(" %s=%s", tag->keys[i], tag->values[i]);
					else printf(" %s", tag->keys[i]);
				}
			
			printf(">");
			listOftags_piv = listOftags_piv->sig;  
		} while (listOftags_piv != NULL );




		// Verifica si es un contenido dentro de la definición de una etiqueta (<..contenido..>) o si eta dentro de
		// un bloque de etiqueta <tag ... >contenido</tag> 
 
		if(   !(((struct tag *) listOftags->end->element)->offset_b <=  offset_e && 
			((struct tag *) listOftags->end->element)->offset_e >=  offset_e))  {
			showcontent_n1(in, ((struct tag *) listOftags->end->element)->name,  offset_e);
		}

		printf("\n");

		listoflist_piv 	= listoflist_piv->sig;
		offsets_piv	= offsets_piv->sig;
	} while(listoflist_piv != NULL); 
	
	


}
