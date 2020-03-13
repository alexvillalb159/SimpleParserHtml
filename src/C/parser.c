#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "attribstokens.h"
#include "genericlist.h"
#include "utils.h"
#include "file.h"
#include "array.h"

/**
 * Rutina que devuelve todo la lista de tags Html del standar official de w3c, leyendolas en el
 * archivo src/htmltags.txt
 */
const char *filehtmltags = "src/htmltags.txt";

/**
 * Esta rutina devuelve un arrays de todos los nombres de tags validos en Html .
 * El Primer miembro de este array, es decir el correspondiente al indice 0, apunta a 
 * la estructura defArray que indica la longitud del array
 */
static char **getHtmltags()  {
	static char **arraytags = NULL;
	char *string;
	char tag[256];

	if(arraytags == NULL) {
		int tag_length;

		FILE *fp = fopen(filehtmltags, "rt");

		readline(fp, &string); free(string); readline(fp, &string); free(string); 
		readline(fp, &string); free(string); readline(fp, &string); free(string); // Ignora 4 lineas
		
		initArray((void ***) &arraytags);
		while(readline(fp, &string) != EOF) {

			if(sscanf(string, "<%s", tag) == 1) {
				char *tag_mem;
				tag_length = strlen(tag);
				tag[tag_length - 1] = 0;
				tag_mem = malloc(tag_length + 1);
				memcpy(tag_mem, tag, tag_length + 1);
				addArray((void ***) &arraytags, tag_mem );
				//printf("%s\n", tag);				
			}
			free(string);
		}
	}
	return arraytags;

}

	
/**
 * Encuentra cualquier tag (excepto los tag de comentarios) utilizando menor esfuerzo (puede dar errores)
 * El cursor in se posición exactamente al principio de la etiqueta. Después del caracter '<'
 */	 
int findanytag (FILE *in) {
	long offset;
	int caracter;
	while (1) { 
		do {
			caracter = fgetc (in);



		} while (caracter != '<' && NOTEOF(caracter));
		if(NOTEOF(caracter)) { 

			offset = ftell(in);

			if (iscommentTag(in, offset )) {	
				findendComment(in );
				continue;

			} else if(isValidtaghtml(in, offset) ) {
				// Bien			
			
			} else {
				continue;
			}
		
			fseek(in, offset, SEEK_SET);
		}
		break;

	}
	return caracter; 
}

/**
 * Encuentra tag en el apuntador de archivo 
 */
int findTag (FILE *in, struct tag *tag, char *tagctx ) {

	struct tag *tag_r  ;

	// Banderas de busqueda
	int existTag = 0;
	int contbusq = 1;
	do {


		if(findTag_n (in, tag->name, tagctx )) {
			//tag_r = getTag(in,tag->name );
			tag_r = getTag_2(in);
			if(cmpAttribs(tag, tag_r )) {
				existTag = 1;

			}
 
		}  else  {
			contbusq = 0;
		}



	} while ( !existTag && contbusq) ;

}


/**
 * Se posiciona el stream in en la etiqueta definida por *tag_d pero sin salirse del ambito de la etiqueta *tag_context.
 * Si no puede conseguir la etiqueta *tag_d dentro del ambito de tag_context retorna 0, en caso de que se consiga
 * retorna 1	
 */
int findTag_n (FILE *in, const char *tagname, const char *tag_context ) {
	int rest;
	if(tag_context == NULL) {
		return findTag_n0 (in, tagname );			
	}


	return findTag_n1(in, tagname, tag_context );


}

/**
 * Se posiciona el stream  en la etiqueta definida por *tag_d pero 
 * Si no puede conseguir la etiqueta *tag_d retorna 0, en caso de contrario
 * retorna 1
 * Encuentra una tag casi independientemente del contexto
 */
int findTag_n0 (FILE *in, const char  *tagname) {


	int result;
	long offset_i;
	// Posiciona el marcador de principio de tag al comienzo de cualquier etiqueta
	result = findanytag (in);

	while (NOTEOF(result) ) {		

		// toma el offset del posicionador de archivo 
		offset_i = ftell (in );

		if(isTag (in, tagname, offset_i)) {
			// Encontro la etiqueta
			// Posicina el stream al principio del nombre de la etiqueta
			fseek(in, offset_i, SEEK_SET);			
			return 1;
			 
		} else  {
			
			// Pasa la etiqueta
			findendtag (in, offset_i);		
		}
		
		result = findanytag (in);
	
	}
	return 0;
}

/**
 * Busca la etiqueta que haga match con definición de tag (en caso de que los hubiese) sin salirse del ambito de tag_context 
 * (la busqueda depende totalmente del contexto)
 * Si tags es una etiqueta final debe pasarse con  el caracter '/' al principio dentro del campo name de tag, 
 * caso contrario solo el nombre
 * 
 */	
int findTag_n1(FILE *in, const char *tagname, const char *tag_context) {
	int nivel = 0;
	int result;
	long offset_i;

	
	 	

	// Posiciona el marcador de principio de tag al comienzo de cualquier etiqueta
	result = findanytag (in);

	while (NOTEOF(result) ) {		

		// toma el offset del posicionador de archivo 
		offset_i = ftell (in );



		if(isEndtag (in, tag_context, offset_i) )  {
			if(nivel <= 0 ) {
				// Falta si tiene atributos
				//cmpattrib(in, tag_context );
				return 0;
			} 
				
			nivel--;
			
	
		} else if( isTag (in, tag_context, offset_i ) ) {
			nivel++;

		} else if(isTag (in, tagname, offset_i)) {
			// Encontro la etiqueta

			

			// Restaura el cursor
			fseek(in, offset_i, SEEK_SET);			
			return 1;
			 
		} else  {
			
			// Pasa la etiqueta
			findendtag (in, offset_i);		
		}
		
		result = findanytag (in);
	
	}
	return 0;
 
}

/**
 * Busca la etiqueta par de tag_d que representa el fin de bloque, a partir de la posición del flujo in.
 * Devuelve 0 si no consigue la etiqueta y 1 si la consigue. El stream in termina posicionandose en el caracter '/'
 * que representa el inicio del nombre de la etiqueta par final
 */

int findTagpairEnd(FILE *in, const char *tag_d ) {
	int nivel = 0;
	int result;
	long offset_i;
	unsigned char tagend = 0;


	 	

	// Posiciona el marcador de principio de tag al comienzo de cualquier etiqueta
	result = findanytag (in);

	while (NOTEOF(result) ) {		

		// toma el offset del posicionador de archivo 
		offset_i = ftell (in );


		if(isEndtag (in, tag_d, offset_i) )  {
			if(nivel == 0 ) {
				fseek(in, offset_i, SEEK_SET);
				return 1;
			} 
				
			nivel--;
			
	
		} else if( isTag (in, tag_d, offset_i ) ) {
			nivel++;

		}  else  {
			
			// Pasa la etiqueta
			findendtag (in, offset_i);		
		}
		
		result = findanytag (in);
	
	}
	return 0;
 
}

/**
 * Encuentra el final de un bloque de comentarios
 */

int findendComment(FILE *in ) {
	return findString(in, "->") ;
}

/**
 * Posiciona el stream in al final de una etiqueta comenzando por el offset (puede dar errores)
 */
int findendtag (FILE *in, long offset) {
	int caracter;
	fseek(in, offset, SEEK_SET);
	do {
		caracter = fgetc (in);
	} while (caracter != '>' && NOTEOF(caracter));
	return caracter; 
}





/**
 * Devuelve todos los artributos de una etiqueta y coloca las nombres en keys y los valores en values. 
 * Presuponiendo que el stream in esta ubicado al principio del nombre de la etiqueta, después del caracter '<'.
 * Al terminar el stream se posiciona al final de los atributos de la etiqueta.
 * Devuelve 1 en caso de éxito y 0 si la etiqueta no tiene atribuos y -1 si ocurre algún error de sintáxis
 */ 

int getAttribs(FILE *in, char ***keys, char ***values) {
	int caract;
	unsigned long offset_b, offset_e;
	unsigned length; 
	char *buffer;
	char **tokens;
	unsigned tokens_n = 0;
	int getAttribs_r = 0;
	char tagname[512];

	// Dirección nula por default
	*keys = NULL;
	*values = NULL;

	

	offset_b = ftell(in); 


	//if(fgetc(in) != '>') { 
	
	

		findendtag (in, offset_b); 
		offset_e = ftell(in); 

		length = offset_e - offset_b;
		buffer = malloc(length + 1); 
	
		fseek(in, offset_b, SEEK_SET); 
		fread(buffer, length , 1, in);

		buffer[length-1] = '\0';

		tokens_n = getNumtokens(buffer, "\t\r\n ");	
		if(tokens_n > 0) {

			tokens	= getTokens (buffer, "\t\r\n ");


			*keys   = malloc (sizeof(void *) * (tokens_n + 1));
			*values = malloc (sizeof(void *) * (tokens_n + 1));
 
		
			int i=0; // Excluye el nombre de la etiqueta de los tokens a considerar 
			for(; tokens[i] != NULL; i++  )  {
 				(*keys)[i] = strtok(tokens[i], "=");
				(*values)[i] = strtok(NULL, "=");
				if(strtok(NULL, "=") != NULL) {
					// Error de sintáxis
					getAttribs_r = -1;
					
				}
			
			}

			(*keys)		[i] = NULL;
			(*values)	[i] = NULL;
		
		}
		if(!getAttribs_r) getAttribs_r = 1;

	//}
	return  getAttribs_r;

}

/**
 * Indica si la etiqueta en la que esta posicionado el stream en offset es de tipo comentario
 */

int iscommentTag(FILE *in, const long offset ) {
	fseek(in, offset, SEEK_SET);
	if((char) getc(in) == '-'  && (char) getc(in) == '-') {
		return 1;
	}
	return 0;
}


/**
 * Indica si la etiqueta que comienza a leer en [offset - 1] tiene el nombre de tag_s
 * Retorna
 */
int isTag(FILE *in, const char * tag_s, const long offset ) {
	int caracter ;
	fseek(in, offset, SEEK_SET);

	caracter = strcmp_m (in, tag_s );
	if(!NOTEOF(caracter) ) return 0;
	// Consiguio el nombre de la etiqueta
	// Lee el próximo carácter	
	caracter = fgetc (in);
	// si la etiqueta termina allí perfecto
	if(caracter == '>') return fgetc(in);
	// Si el nombre es solo un sufijo lo descarta
	else if(caracter != ' ') {
		return 0;
	} 
	// termina de leer el fin de la etiqueta hasta posicionar el stream al final de la misma en el comienzo del contenido	
	return 1;

}
/**
 * Indica si es un nombre de etiqueta válido
 */
int isValidtaghtml(FILE *in, unsigned offset) {
	fseek(in, offset, SEEK_SET);
	char *name = getField(in, " >");
	if(name[0] == '/') {
		// Es una etiqueta final
		name = name + 1;
	}
	return isValidtaghtml2(name) != -1 ;

}
/**
 * Busca la etiqueta tagname en el array devuelto por getHtmltags utilizando un algoritmo de 
 * busqueda binaria
 */
static int isValidtaghtml2(char *tagname) {
	char **tags = getHtmltags();
	int length = getLengthArray2((void **) tags);
	int ind_a = 1,	 ind_z = length, ind_m =	(ind_z + ind_a) / 2;
	int str_r;

	while((str_r = strcmp (tags[ind_m], tagname)) != 0) {
		// tags[ind_m] > tagname
		int ind_m1 = ind_m;
		if(str_r > 0) {
			// La cadena esta entre ind_a y ind_m
			ind_z = ind_m ; ind_m =	(ind_z + ind_a) / 2;


			
		}
		else if(str_r < 0) {
			// La cadena esta entre ind_m y ind_z
			ind_a = ind_m; 	 ind_m = (ind_z + ind_a) / 2;
		} 

		if(ind_m1 == ind_m) {
			ind_m = -1;
			break;
		} 
		

	}
	return ind_m;

}




/**
 * Indica si tagpair es el bloque final de tag_s
 * el nombre o el marcador de tag de fin de bloque.
 * Si la tag es final entonces:	
 * [offset - 1] = '<', [offset] = '/' 	
 */

int isEndtag (FILE *in, const char *tag_s, long offset) {
	int caracter ;
	fseek(in, offset, SEEK_SET);
	caracter = getc(in);
	if(caracter == '/')  {	
		if(isTag (in, tag_s, offset + 1 ) ) {
			return 1;
		}
		fseek(in, offset, SEEK_SET);
	}
	return 0;
}

/**
 *	Asumiendo que el cursor in este posicionado al principio de la etiqueta tagname, llena
 *	la estructura tag con todos los atributos que esta contenga y estén en el stream in. 
 */
/*
struct tag *getTag(FILE *in, char *tagname) {
	struct tag *getTag_r = malloc (sizeof (struct tag ) ); 
	char **keys,  **values;
	getAttribs(in, &keys, &values);

	memcpy ( getTag_r, &((struct tag ) {tagname, keys,values}), sizeof(struct tag));

	return getTag_r;
}
*/
/**
 *	Verifica si todas las keys de tag1 están incluidas dentro de tag2 y si los atributos de estas keys
 *      son iguales a los de las correspondientes keys de tag2. Si es así devuelve 1, caso contrario devuelve 0. 
 */
int cmpAttribs(struct tag *tag1, struct tag *tag2 ) {
	int cmpAttribs_r = 0;
	// banderas de busqueda
	//int haskey1 = 1;
	int haskey2 = 1;

	if(tag2->keys != NULL)	{
		if(tag1->keys != NULL) {

			for(int i=0; tag1->keys[i] != NULL && haskey2 ; i++) {
				int ind ;



				if((ind = hasKey(tag2, tag1->keys[i]) ) == -1 ) {
					haskey2 = 0;

				} else  {
					if(strcmp(tag1->values[i],  tag2->values[ind]  )  ) {
						haskey2 = 0;
					}

 				}
		
			}
		}

	} else {
		if(tag1->keys != NULL) {
			haskey2 = 0;
		}

	}


	return haskey2;
}

/**
 * Toma el contenido que esta entre los bloques de etiquetas definidos por tags 
 * Llama a getcontent_n1
 */


struct listgeneric * getcontent(FILE *in, char **tags,  char ***attribs) {
	struct listgeneric *offsets;
	struct listgeneric *offsets_c; 		

 

	offsets = createlistgeneric(sizeof(unsigned long)	, 0);
	offsets_c = createlistgeneric(sizeof(unsigned long) 	, 0);





	getcontent_n1(in, tags, attribs, NULL, 0, offsets,  offsets_c );


	return offsets_c;

}

/**
 * Toma el contenido que esta entre los bloques de etiquetas definidos por tags 
 * (función recursiva y final)
 */

int getcontent_n1(FILE *in, char **tags,  char ***attribs,  char *tagscontexto, int nivel, 
	struct listgeneric *offsets, struct listgeneric *offsets_c  ) {

	char *buffer = NULL;
	unsigned result=0;
	int indUltTag; // indice de la ultima tag de la lista
	unsigned long offset;
	char **keys, **values;	



	if(tags[nivel]== NULL) {
		// Chukity 
		// lo consiguio
		// Aquí imprime el exito de la busqueda		
		
		addlist(offsets_c, offsets->end->element );
		return 1;
	}


	getKeysandValues (attribs[nivel], &keys, &values) ;

	struct tag tag = { 
			tags[nivel],
			0,
			0,
			keys,
			values		
	
		};





	// Busca hasta agotar la existencia de la etiqueta tags[nivel] en el nivel actual
	while(findTag (in, &tag, tagscontexto ) ) {
		// si la consigue guarda el offset y busca la otra 
		offset = ftell(in);

		addlist(offsets, &offset);

		result = result + getcontent_n1(in, tags, attribs, tags[nivel], nivel + 1, offsets, offsets_c );

		// remueve el offset de la lista
		dellist(offsets, &offset);
		// restaura la posición del stream
		fseek(in, offset, SEEK_SET);

		// Se posiciona en el final de la etiqueta de fin de bloque
		findTagpairEnd (in, tags[nivel]);	
	}

	return 	 result ;

}

/**
 * Indica si en la estructura *tag de tipo tag esta presente la clave *key
 */
int hasKey (struct tag *tag, char *key ) {
	int hasKey_r = -1;

	for(int i=0; tag->keys[i] != NULL ; i++) {
		if(!strcmp(tag->keys[i], key) ) {
			hasKey_r = i;
			break;
		}
	}

	return hasKey_r;
}

/**
 * Descompone la lista de attributos en attribs en keys y values
 */

void getKeysandValues (char **attribs, char ***keys, char ***values) {
	
	
	if( attribs[0] != NULL) {
		int attribs_length = getLengthArray(attribs); 
		*keys	=	malloc((attribs_length + 1) * sizeof(void * )); 
		*values =	malloc((attribs_length + 1) * sizeof(void * )); 


		for(int i=0; attribs[i] != NULL; i++)  {
			(*keys)[i]   =	strtok(attribs[i], "=");
			(*values)[i] =	strtok(NULL, "=");
		}
	}
	else {
		*keys	= NULL; 
		*values = NULL;

	}
	
}

/**
 * Lee todo lo que hay entre entre el principio del stream in y una marca de etiqueta.
 * Retorna 0 en caso de fallo y 1 si tiene éxito
 */	

int readcontent(FILE *in, char **buffer, const char *tag) {

	long offset_i = ftell(in);

	// Se posiciona al comienzo de la etiqueta de fin de bloque del contenido
	if(!findTagpairEnd (in, tag)) {
		// Error: No se encontro el fin de bloque de la etiqueta
		return 0;

	}
	long offset_f = ftell(in);


	// Calcula el tamaño del contenido5
	int length = offset_f - offset_i  - 1; // normalmente seria: length = offset_f - offset_i + 1, pero debe 
					// resta 2 bytes dado que el apuntador de string esta despues de "</" 
					// con la resta queda exactamente al final del bloque de contenido

	*buffer = malloc(length + 1);	// reserva un byte para el byte 0 de fin se string
	fseek(in, offset_i, SEEK_SET);

	fread(*buffer, length , 1, in);

	fseek(in, offset_f, SEEK_SET);  // mueve la posición de stream al final de la etiqueta

	buffer[0][length] = '\0';	
	return 1;
}
/**
 * Busca el contenido *content en el stream y devuelve una lista de las listas de tags que hacen referencia a ese contenido.
 * En cada lista no puede haber más de deepth tags. 
 * Devuelve 1 si consigue el contenido y cero en caso contrario. 
 */

int findParentsTag(FILE *in, char *content, struct listgeneric  **listOflist, struct listgeneric  **offsets, 
	unsigned deepth) {

	unsigned long cursor ;
	long offsetcomment;	
	long offset_c; // Offset del contenido
	int findParentsTag2_r;
 	struct listgeneric *listOfTags = createlistgeneric(sizeof(struct tag),  0);

 	*offsets = createlistgeneric(sizeof(long),  0);
	*listOflist = createlistgeneric(sizeof(struct listgeneric *),  0);
	int findParentsTag_r = 0;


	// Bucle para buscar y ignorar los comentarios

	do {
		// salva el cursor
		cursor = ftell(in);

		offsetcomment = findCommentTag(in);


		// restuara el cursor
		fseek(in, cursor, SEEK_SET) ;

		findParentsTag2_r = findParentsTag2(in , content, *listOflist , listOfTags, *offsets, offsetcomment, deepth);	
		if( findParentsTag2_r >  0 && !findParentsTag_r)  {

			findParentsTag_r = 1;
		} else  {
			fseek(in , offsetcomment, SEEK_SET);
 			findendComment(in );
			
  		}

		//findContent(FILE *in , char *content, struct listgeneric  *listOfList, offsetcomment,  int deepth) {
		
		
	} while (offsetcomment != -1 );

	// Si no hay más comentarios pero quedan etiquetas por revisar ejecuta este bucle
	while (findParentsTag2_r !=  0)  {
		findParentsTag2_r = findParentsTag2(in , content, *listOflist , listOfTags, *offsets, offsetcomment, deepth);
		if( findParentsTag2_r >  0 && !findParentsTag_r) {
			findParentsTag_r = 1;
		}

	}

	return findParentsTag_r; 

}
/**
 * Subrutina de findParentsTag para buscar el contenido apuntado por content. 
 * offsetlimit: es el offset de un bloque de comentario. Si no existe su valor es -1. 
 * Devuelve 0 si no consigue el contenido, -1 si lo consigue dentro de un comentario 
 * o dentro de la definición de una etiqueta, y 1 si consigue el contenido, sin que se 
 * cumpla ningua de las 2 excepciones. 
 */
static int findParentsTag2(FILE *in , char *content, struct listgeneric  *listOfList , struct listgeneric *listOfTags, 
		struct listgeneric *offsets, long offsetlimit, int deepth) {


	long offset_c, cursor;
	int findContent_r = 0;
	int findString_r; 
	struct listgeneric  *listoftagsRelv; 
	int content_length = strlen(content);
	cursor = ftell(in);

	findString_r = findString (in, content);
	
	if(NOTEOF(findString_r) )  {
		// Encontro el contenido			 
		offset_c = ftell(in);
		// Si se paso de la linea de comentario no lo toma en cuenta
		if(offsetlimit > -1 && offset_c > offsetlimit)  {
			findContent_r = -1;			

		} 
		else {
			// restuara el cursor
			fseek(in, cursor, SEEK_SET) ;	
			listoftagsRelv = findParentsTag3 (in, offset_c, listOfTags, deepth );
			if(listoftagsRelv == NULL) {
				// El contenido se encontro dentro de la definición de la etiqueta por tanto se ignora
				//fseek(in, offset_c + content_length, SEEK_SET) ; // pasa el contenido encontrado
				findContent_r = -1;			
			} else {
			
				fseek(in, offset_c + content_length, SEEK_SET) ;
				addlist(listOfList , &listoftagsRelv);
				addlist(offsets, &offset_c);
				
				findContent_r = 1;
			}

		}
	} else  {
		findContent_r = 0;
	}

	return findContent_r;

}
/**
 * Hace las busquedas de las tags y las coloca en listOfTags y offsets. Devuelve las maxelemens ultimas tags de la lista
 */
static struct listgeneric  * findParentsTag3 (FILE *in, long offset, struct listgeneric *listOfTags, 
	 unsigned maxelemens ) {
	// El funcionamiento de esta rutina se basa en buscar las etiquetas y e irlas idexando en la lista listOfTags
	// hasta pasar la posición de offset. Cuando se pasa esa posición se devuelve las últimas maxelemens etiquetas
	// indexadas, etiquetas en las que estaría el contenido apuntado por offset. 

	int result;
	long offset_i, offset_f; // Los dos offsets entre los cuales esta la definición de una etiqueta. 
	struct tag *tag;
	struct listgeneric  * findParentsTag3  ;
	


	// Posiciona el marcador de principio de tag al comienzo de cualquier etiqueta
	result = findanytag (in);
	offset_i = ftell(in) - 1;
	findendtag(in, offset_i);
	offset_f = ftell(in) - 1;
	// Restaura la posición de la etiqueta
	fseek(in, offset_i + 1, SEEK_SET);
	
	/*
	if(offset >= offset_i && offset < offset_f) {
		// Si el offset esta dentro de la definición de una etiqueta (no confundir con el bloque de etiqueta) se ignora
	
		findParentsTag3 = NULL;

	}
	else  { */
		while (NOTEOF(result) && ftell(in) < offset) {	
			tag = getTag_2(in);


			if(tag->name[0] == '/')  {
				// Es una tag final
				//("Quitando etiqueta: %s\n",  tag->name);
				popTagOfList (listOfTags, &tag->name[1] );
				//("Lista luego de quitar:\n");		
				//printTags (listOfTags );		

			}
			else { 
				//("Incluyendo tag: %s\n", tag->name);
				tag->offset_b = offset_i ;
				tag->offset_e = offset_f ;	
				addlist(listOfTags, tag);

				//("Lista luego de incluir:\n");		
				//printTags (listOfTags );

			}
			result = findanytag (in);
			offset_i = ftell(in) - 1;
			findendtag(in, offset_i);
			offset_f = ftell(in) - 1;


			// Restaura la posición de la etiqueta
			fseek(in, offset_i + 1, SEEK_SET);
		

/*
			if(offset >= offset_i && offset < offset_f) {
				// Si el offset esta dentro de la definición de una etiqueta (no confundir 
				// con el bloque de etiqueta) se ignora
				return	NULL;			

			}
*/
	
		}
		findParentsTag3 = getCopyList(listOfTags, maxelemens ); 
	//}

	return findParentsTag3;


}





/**
 * Imprime todas en stdout todas las etiquetas de listOfTags
 */
void printTags (struct listgeneric *listOfTags ) {
	struct list *piv = listOfTags->head;
	while(piv != NULL) {
		struct tag *tag = piv->element;
		printf("%s\n", tag->name);
		piv=piv->sig;

	}
	

}


/**
 * Retorna una copia de la lista desde los numNodes ultimos puestos de listsource.
 */
static struct listgeneric * getCopyList(struct listgeneric *listsource,  int numNodes)  {

	struct listgeneric  *listdest =  createlistgeneric(sizeof (struct tag ), numNodes );
	struct list  *pivsrc  = listsource->end;
	//struct list  *pivdest; 


	for(int i=1; i < numNodes; i++)  {
		pivsrc = pivsrc->ant;

	} 
	//listdest->length = numNodes;
	//listdest->head = malloc (sizeof( struct tag));
	//listdest->head->ant = NULL;

	//pivdest = listdest->head;
	addlist(listdest, pivsrc->element);

	pivsrc  = pivsrc->sig;

	while(pivsrc != NULL)  {
                /*pivdest->sig = malloc (sizeof( struct tag));
		memcpy(pivdest->sig, pivsrc, sizeof( struct tag));
		pivdest->sig->ant = pivdest;

		pivdest = pivdest->sig; */
		addlist(listdest, pivsrc->element);
		pivsrc  = pivsrc->sig; 
		
	}
	//pivdest->sig = NULL;	
	//listdest->end =  pivdest;
	return listdest;

}



/**
 * Quita la tag con el nombre tagname y todos las tag que vienen después de este de la lista 
 */
int popTagOfList(struct listgeneric *list, char *tagname)  {
	struct list *piv_list = list->end;
	struct tag *tag = piv_list->element;
	int popTagOfList_r = 0;		
	do { 
		tag = piv_list->element;
		if(!strcmp(tag->name, tagname) )  {
			popTagOfList_r = deleteOfNodeToEndNode(list, piv_list);
			break;
		} 
		piv_list = piv_list->ant;
		
	} while(strcmp(tag->name, tagname) &&  piv_list != NULL);
	return popTagOfList_r;

}

/**
 * Toma la tag que se asume que esta en el stream in. 	
 */
struct tag * getTag_2 (FILE *in )  {


	char *name;
	char **keys, **values;
	int name_length; 
	long offset = ftell(in);
	struct tag  *getTag_r = malloc(sizeof( struct tag ));


	// Toma el nombre del tag
	name = getField(in, " >");
	/*
	name_length = strlen(name);
	if(name[name_length - 1] == '>')  {
		name[name_length - 1] = 0;
		//fseek(in, -1, SEEK_CUR);
	}
	*/

	getAttribs(in, &keys, &values);
	memcpy( getTag_r, &((struct tag)   { name, 0, 0, keys, values}), sizeof(struct tag) );
	return getTag_r;

}



/**
 * Encuentra un tag de comentario. Busca desde la posción en la que ya esta el stream in. 
 * Devuelve -1 si no encuentra ningún comentario en caso contrario devuelve el offset del stream in en el cual se 
 * encuentra el comentario. 
 */
long findCommentTag(FILE *in ) {
	long offset;
	int caracter;
	do { 
		do {
			caracter = fgetc (in);

		} while (caracter != '<' && NOTEOF(caracter));

 		offset = ftell(in);



	} while (!iscommentTag(in, offset ) && NOTEOF(caracter));
	if(!NOTEOF(caracter) || !iscommentTag(in, offset ) )  offset = -1; 

	return offset; 


}
