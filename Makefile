.SUFFIXES: .c .cpp .o .x .h
DIR = $(notdir $(CURDIR))

NAME1 = a
NAME2 = b
NAME3 = c
NAME4 = dodatkowy

EXEC1  = $(NAME1).x  
OBJS1  = $(NAME1).o
EXEC2  = $(NAME2).x  
OBJS2  = $(NAME2).o
EXEC3  = $(NAME3).x  
OBJS3  = $(NAME3).o
EXEC4  = $(NAME4).x  
OBJS4  = $(NAME4).o

CFLAGS = -Wall -std=c99 -pedantic -O 
LFLAGS = -Wall -std=c99 -pedantic -O

CO = gcc
LD = $(CO)

%.o: %.c
	$(CO) $(CFLAGS) -c $<

.PHONY: all
all: $(EXEC1) $(EXEC2) $(EXEC3) $(EXEC4)
# Zmienna wewnetrzna $@ oznacza cel, tzn. obiekt, ktory jest po lewej stronie 
# dwukropka, tu: $(EXEC1);  a zmienna $^ oznacza wszystkie prerekwizyty, tzn.
# obiekty stojace po prawej stronie dwukropka (tutaj wszystkie pliki .o, ktore
# powinny byc zlinkowane).
$(EXEC1): $(OBJS1) 
	$(LD) -o $@ $(LFLAGS) $^
$(EXEC2): $(OBJS2) 
	$(LD) -o $@ $(LFLAGS) $^
$(EXEC3): $(OBJS3) 
	$(LD) -o $@ $(LFLAGS) $^
$(EXEC4): $(OBJS4)  
	$(LD) -o $@ $(LFLAGS) $^


.PHONY: run1 run2 run3 run4

run1: $(EXEC1) 
	./$(EXEC1) $(A) $(B)
run2: $(EXEC2) 
	./$(EXEC2) $(A) $(B)
run3: $(EXEC3) $(EXEC1) $(EXEC4)
	./$(EXEC3) $(A) $(B)
run4: $(EXEC4) 
	./$(EXEC4) $(A) $(B)

.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )