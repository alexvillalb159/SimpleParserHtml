#ifndef ALX_GENERICLIST_H
#define ALX_GENERICLIST_H


/**
 * Lista genérica
 */

struct listgeneric {
	struct list *head;
	struct list *end;
	unsigned elementsize;
	unsigned length;
	unsigned maxelemens ;
	void * funcoverride; 	// Una función de comparación alternativa para el caso en la cual no sirva la default

};
struct list {
	void *element;
	struct list *sig;
	struct list *ant;
};


/**
 * Función que devuelve la lista de offsets si esta no existe la crea
 */
struct listgeneric  *createlistgeneric(const unsigned, unsigned );

/**
 *	Función que almacena los elementos de la lista
 */
static void* loadElement (void *, unsigned );

/**
 * Función para agregar un elemento a la lista
 * Si se logra agregar el elemento devuelve 1, si list es NULL devuelve 0
 */

int addlist(struct listgeneric *, void *);
/**
 *	Función por default para comparar los elementos de la lista
 */
int cmpbytes (const unsigned char  *, const unsigned char *, const unsigned );

/**
 * Función que borra un elemento de la lista
 * Si se logra eliminar el elemto devuelve 1, si el elemento no se consigue devuelve 0
 */


int dellist(struct listgeneric *, const void * );
/**
 * Borra todos los elementos de la lista
 */
void dellAllList(struct listgeneric * );


/**
 * Borra un nodo específico de la lista
 * Devuelve 1 si se logra conseguir y borrar en Nodo exitosamente , devuelve 0 en caso contrario
 */ 
int deleteNode (struct listgeneric *list, struct list *node );

/**
 *  Borra la lista desde el nodo node hasta el nodo final
 */
int deleteOfNodeToEndNode(struct listgeneric *list, struct list *node);

#endif
