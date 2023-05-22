.SUFFIXES: .c .cpp .o .x .h
DIR = $(notdir $(CURDIR))

NAME1 = a

EXEC1  = $(NAME1).x  
OBJS1  = $(NAME1).o

CFLAGS = -Wall -std=c99 -pedantic -O 
LFLAGS = -Wall -std=c99 -pedantic -O

CO = gcc
LD = $(CO)

%.o: %.c
	$(CO) $(CFLAGS) -c $<

.PHONY: all
all: $(EXEC1) 

$(EXEC1): $(OBJS1) 
	$(LD) -o $@ $(LFLAGS) $^


.PHONY: run1 

run1: $(EXEC1) 
	./$(EXEC1) $(DANE) $(WYNIK) ; diff -s $(DANE) $(WYNIK)

.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )