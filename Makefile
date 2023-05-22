.SUFFIXES: .c .cpp .o .x .h
DIR = $(notdir $(CURDIR))
SYSNAME = $(shell uname -s)

NAME1 = producent
NAME2 = konsument
NAME3 = a

EXEC1  = $(NAME1).x  
OBJS1  = $(NAME1).o
EXEC2  = $(NAME2).x  
OBJS2  = $(NAME2).o
EXEC3  = $(NAME3).x  
OBJS3  = $(NAME3).o

CFLAGS = -Wall -std=c99 -pedantic -O 
LFLAGS = -Wall -std=c99 -pedantic -O

LDLIBS = -lm
CO = gcc
LD = $(CO)

%.o: %.c
	$(CO) $(CFLAGS) -c $<

.PHONY: all
all: $(EXEC1) $(EXEC2) $(EXEC3) 

$(EXEC1): $(OBJS1) 
	$(LD) -o $@ $(LFLAGS) $^
$(EXEC2): $(OBJS2) 
	$(LD) -o $@ $(LFLAGS) $^
$(EXEC3): $(OBJS3) 
	$(LD) -o $@ $(LFLAGS) $^


.PHONY: run1 run2 run3 run4

run1: $(EXEC1) 
	./$(EXEC1) $(POTOK) $(DANE)
run2: $(EXEC2) 
	./$(EXEC2) $(POTOK) $(DANE)
run3: $(EXEC3) $(EXEC1) $(EXEC2) 
	./$(EXEC3) $(PLIK1) $(PLIK2) $(POTOK) $(DANE) $(WYNIK)
	diff -s $(DANE) $(WYNIK)
#make run3 PLIK1=./producent.x PLIK2=./konsument.x POTOK=./potokA DANE=./dane.txt WYNIK=./wynik.txt

run4: $(EXEC1) $(EXEC2) 
	mkfifo -m 777 $(POTOK) 
	xterm -title PRODUCENT -e ./producent.x $(POTOK) $(DANE) &
	xterm -title KONSUMENT -e ./konsument.x $(POTOK) $(WYNIK)
	rm $(POTOK) 
	diff -s $(DANE) $(WYNIK)
#make run4 POTOK=./potokB DANE=./dane.txt WYNIK=./wynik.txt

.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )