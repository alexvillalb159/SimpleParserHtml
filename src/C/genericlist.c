
#include <stdlib.h>
#include <string.h>
#include "genericlist.h"


/**
 * Función que devuelve la lista de offsets si esta no existe la crea
 */
struct listgeneric  *createlistgeneric(const unsigned elementsize, unsigned maxelemens ) {
	struct listgeneric *self;

	//if(self == NULL)  {
	self = malloc (sizeof(struct listgeneric ));
	self->head = NULL;
	self->end = NULL;
	self->elementsize = elementsize;
	self->length = 0;
	self->maxelemens = maxelemens;

	//}
	return self;
}

/**
 *	Función que almacena los elementos de la lista
 */

static void* loadElement (void *element, unsigned length ) {
	void * newaddress = malloc(length);
	memcpy(newaddress, element, length);
	return newaddress;
}



/**
 * Función para agregar un elemento a la lista
 * Si se logra agregar el elemento devuelve 1, si list es NULL devuelve 0
 */

int addlist(struct listgeneric *list , void *element) {
	int addlist_r = 1;
	if(list == NULL) addlist_r = 0;
	else 
		if(list->head == NULL ) { 
			list->head = malloc (sizeof(struct list ));
			list->end = list->head;
			list->head->element = loadElement(element, list->elementsize );
			list->head->sig = NULL;
			list->head->ant = NULL;
			list->length = 1;
		} else { 	
			list->end->sig = malloc (sizeof(struct list));
			list->end->sig->element = loadElement(element, list->elementsize );
			list->end->sig->sig = NULL;
			list->end->sig->ant = list->end;
			list->end = list->end->sig; 
			list->length++;	

			if(list->maxelemens > 0 && list->length == list->maxelemens + 1) {
				// elimina la cabeza
				dellist(list, list->head->element );			
			}
			

		}
	return addlist_r;

}

/**
 *	Función por default para comparar los elementos de la lista
 */

int cmpbytes (const unsigned char  *element1, const unsigned char *element2, const unsigned length ) {
	int cmpbytes_r = 1;
	for(int i=0; i < length; i++ ) {
		if(element1[i]  != element2[i]) {
			cmpbytes_r = 0;
			break;
		}
	}
	return cmpbytes_r;
}

/**
 * Función que borra un elemento de la lista
 * Si se logra eliminar el elemto devuelve 1, si el elemento no se consigue devuelve 0
 */


int dellist(struct listgeneric *list, const void * element ) {
	struct list *piv = list->head;	
	int dellist_r = 0;

	/* Es el primero de la lista ? */
	if(cmpbytes (list->head->element, element, list->elementsize )) {

		// Es el único de la lista?
		if(list->length == 1) {
				free(list->head->element);
				free(list->head);
				list->head = NULL;
				list->end = NULL;
				list->length = 0;
		}
		else if(list->length > 1) {
			list->head = list->head->sig;
			list->head->ant = NULL;
			list->length--;
			free(piv->element);
			free(piv);
		}
		dellist_r = 1;
	}
	else {	
		/* Es el ultimo de la lista ? */
		if(cmpbytes (list->end->element, element, list->elementsize ) ) {
			piv = list->end;
			list->end->ant->sig = NULL;
			list->length--;
			list->end = piv->ant;
			free(piv->element);
			free(piv);

			dellist_r = 1;




		} else {

			do {
				piv = piv->sig;
				if(piv != NULL) {
					if(cmpbytes (piv->element, element, list->elementsize )  )  {
						piv->ant->sig = piv->sig;
						piv->sig->ant = piv->ant;
						list->length--;
						free(piv->element);
						free(piv);
						dellist_r = 1;
						break;	
					} 

				} 
			} while (piv != NULL);
		}

	}

	return dellist_r;
}

/**
 * Borra todos los elementos de la lista
 */
void dellAllList(struct listgeneric *list ) {
	struct list *piv = list->end;
	while(piv!=NULL) {
		list->end = list->end->ant;
		free(piv->element);
		free(piv);
		piv = list->end;

	}
	free(list);	

}

/**
 *  Borra la lista desde el nodo node hasta el nodo final
 */
int deleteOfNodeToEndNode(struct listgeneric *list, struct list *node)  {
	struct list *piv = list->head;
	int deleteOfNodeToEndNode_r = 1;


	// busca node en la lista list

	while (piv != node ) piv = piv->sig ;

	if(piv == node)  {
		piv = list->end;
		while(piv != node)  {
			deleteNode (list, piv);
			piv = list->end;
		}
		deleteNode(list, node);
	} else deleteOfNodeToEndNode_r = 0;
	return deleteOfNodeToEndNode_r;
}	
/**
 * Borra un nodo específico de la lista
 * Devuelve 1 si se logra conseguir y borrar en Nodo exitosamente , devuelve 0 en caso contrario
 */ 
int deleteNode (struct listgeneric *list, struct list *node )  {
	int deleteNode_r = 0;
	struct list *piv = list->head;


	/* Es el primero de la lista ? */
	if(list->head == node) {

		// Es el único de la lista?
		if(list->length == 1) {
				free(list->head->element);
				free(list->head);
				list->head = NULL;
				list->end = NULL;
				list->length = 0;
		}
		else if(list->length > 1) {
			list->head = list->head->sig;
			list->head->ant = NULL;
			list->length--;
			free(piv->element);
			free(piv);
		}
		deleteNode_r = 1;
	}
	else {	
		/* Es el ultimo de la lista ? */
		if(list->end == node)  {
			piv = list->end;
			list->end->ant->sig = NULL;
			list->length--;
			list->end = piv->ant;
			free(piv->element);
			free(piv);

			deleteNode_r = 1;




		} else {

			do {
				piv = piv->sig;
				if(piv != NULL) {
					if(piv == node)   {
						piv->ant->sig = piv->sig;
						piv->sig->ant = piv->ant;
						list->length--;
						free(piv->element);
						free(piv);
						deleteNode_r = 1;
						break;	
					} 

				} 
			} while (piv != NULL);
		}

	}

	return deleteNode_r;
}





/**
 * Se borra el nodo sin hacer ninguna busqueda para ver si existe o no en la lista, se asume que existe. 
 */ 
/*
int deleteNode_o ( struct listgeneric *list, struct list *node)  {

	return 0; 
} 
*/



