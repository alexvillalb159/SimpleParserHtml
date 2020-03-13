CC = gcc
ELF = simpleparserhtml
OBJ = simpleparserhtml.o
CSRCDIR = $(wildcard src/C)
HSRCDIR = $(wildcard src/include)
CSRC =  $(wildcard ${CSRCDIR}/*.c)
HSRC =  $(wildcard ${HSRCDIR}/*.h)
SOURCES = $(wildcard ${CSRC} ${HSRC} Makefile)
OBJS = $(CSRC:.c=.o)
#CFLAGS = -std=c99 -Wall -Wextra -pedantic

 

all: $(ELF)
	@echo "Build complete"

$(ELF): $(SOURCES)
	$(CC) $(CFLAGS) -ggdb  -c  -std=c11  -I $(HSRCDIR)   $(CSRC);  mv *.o  $(CSRCDIR)
	$(CC)  -o $(ELF) $(OBJS)  && rm $(OBJS)





#clean:
#	rm  $(OBJS)
# Resume: gcc -I src/include/ -ggdb -o simpleparserhtml src/C/*.c

