
#ifndef ALX_PARSER_H
#define ALX_PARSER_H

#include <stdio.h>
#include "genericlist.h"



struct tag  {
	char *name;
	long offset_b; 	// Inicio de la definición de la etiqueta 
	long offset_e;  // fin de la definición de la etiqueta 
	char **keys;
	char **values; 
};

/* Funciones internas del modulo */

static int findParentsTag2(FILE *in , char *content, struct listgeneric  *listOfList , struct listgeneric *listOfTags,
		struct listgeneric *offsets, long offsetlimit, int deepth);
static struct listgeneric  * findParentsTag3 (FILE *in, long offset, struct listgeneric *listOfTags, unsigned maxelemens );



/* ------------------------------------------------------------------------------- */


static char **getHtmltags();
int isValidtaghtml(FILE *in, unsigned offset);
static int isValidtaghtml2(char *tagname) ;

int 	findanytag (FILE *);

int 	findTag(FILE *in, struct tag *tag, char *tagCtx);
int 	findTag_n (FILE *, const char *tag_d, const char *tag_s );
int 	findTag_n0 (FILE *, const char *tag );
int 	findTag_n1(FILE *in, const char *tagname, const char *tag_context);

int 	findendtag (FILE *, long offset);

long 	findCommentTag(FILE * ); 
int  	findendComment(FILE *);
int 	findTagpairEnd (FILE *, const char *) ;
int 	findParentsTag(FILE *in, char *content, struct listgeneric  **listOflist, struct listgeneric  **offsets, unsigned deepth);

int 	popTagOfList(struct listgeneric *list, char *tagname) ;

int iscommentTag(FILE *, const long ) ;
int isTag(FILE *in, const char * , const long  ) ;
int isEndtag (FILE *, const char *tag_s, long offset);


struct listgeneric * 	getcontent	(FILE *in, char **tags,  char ***attribs);
int 			getcontent_n1	(FILE *in, char **tags,  char ***attribs,  char *tagscontexto, int nivel, 
	struct listgeneric *offsets, struct listgeneric *offsets_c  );


//struct tag *getTag(FILE *in, char *tagname);
struct tag *getTag_2(FILE *in);

int getAttribs(FILE *in, char ***keys, char ***values);

int cmpAttribs(struct tag *tag1, struct tag *tag2 );
int hasKey (struct tag *tag, char *key );

void getKeysandValues (char **, char ***, char ***) ;

int readcontent(FILE *, char **buffer, const char *tag);

/**
 * Retorna una copia de la lista desde los numNodes ultimos puestos de listsource.
 */
static struct listgeneric * getCopyList(struct listgeneric *,  int ) ;

void printTags (struct listgeneric * );

#endif
