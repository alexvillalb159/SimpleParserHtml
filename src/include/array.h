#ifndef ALX_ARRAY_H
#define ALX_ARRAY_H

struct defArray {
	int length;	
	//int elementsize;
};

void initArray(void ***array) ;

void addArray(void ***array, void *element );
void movArray(void **array_src, void **array_dst);
void delArray (void ***array);
int getLengthArray2(void **array);

#endif
