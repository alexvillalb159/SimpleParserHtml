#ifndef ALX_ARGUMENTS_H
#define ALX_ARGUMENTS_H

int getTagsAndAttribs (char *argvs[], char ***tags, char ****attribs, int *length) ;
int isAttrib (char *cadena ) ;
char ** passlisttoArray(struct listgeneric *list);
char *** passlisttolisttoArraytoArray(struct listgeneric *list_ll);

#endif
