.SUFFIXES: .c .cpp .o .x .h
DIR = $(notdir $(CURDIR))
SYSNAME = $(shell uname -s)
NAME1 = a
NAME2 = b
NAME3 = c
NAME4 = d
NAME5  = e
# Program wykonawczy: dodajemy rozszerzenie .x, ale moze byc dowolne.
EXEC1  = $(NAME1).x  
# Pliki powstale po kompilacji plikow zrodlowych, tzw. object files
OBJS1  = $(NAME1).o
# Analogicznie dla pozostalych programow
EXEC2  = $(NAME2).x
OBJS2  = $(NAME2).o 
EXEC3  = $(NAME3).x
OBJS3  = $(NAME3).o 
EXEC4  = $(NAME4).x
OBJS4  = $(NAME4).o 
EXEC5  = $(NAME5).x
OBJS5  = $(NAME5).o 
# Pliki naglowkowe
HEADS3 = funkcja.h
#------------------------------------------------------------------------------
# Opcje kompilatora i linkera; -fPIC potrzebna dla bibliotek dzielonych
CFLAGS = -Wall -std=c99 -pedantic -O -fPIC
LFLAGS = -Wall -std=c99 -pedantic -O
# Biblioteki linkera zainstalowane w systemie 
# (tu: biblioteka matematyczna libm)
LDLIBS = -lm
# Kompilator i linker (czesto ta sama nazwa)
CO = gcc
LD = $(CO)
###############################################################################
# Ponizej przyklady definiowania regul zaleznosci
###############################################################################
%.o: %.c %.h
	$(CO) $(CFLAGS) -c $<
# Jak wyzej, ale bez zaleznosci od plikow naglowkowych
%.o: %.c
	$(CO) $(CFLAGS) -c $<

.PHONY: all
all: $(EXEC1) $(EXEC2} $(EXEC3) $(EXEC4) $(EXEC5)

$(EXEC1): $(OBJS1) $(LIBS1) 
	$(LD) -o $@ $(LFLAGS) $^
$(EXEC2): $(OBJS2) $(LIBS1) 
	$(LD) -o $@ $(LFLAGS) $^
$(EXEC3): $(OBJS3) $(LIBS1) 
	$(LD) -o $@ $(LFLAGS) $^
$(EXEC4): $(OBJS4) $(LIBS1) 
	$(LD) -o $@ $(LFLAGS) $^
$(EXEC5): $(OBJS5) $(LIBS1) 
	$(LD) -o $@ $(LFLAGS) $^

###############################################################################
.PHONY: run1
.PHONY: run2
.PHONY: run3
.PHONY: run4
.PHONY: run5
# Uruchom program $(EXEC1); najpierw sprawdz, czy kod wykonawczy jest aktualny
run1: $(EXEC1)
	./$(EXEC1)
run2: $(EXEC2)
	./$(EXEC2)
run3: $(EXEC3)
	./$(EXEC3)
run4: $(EXEC4)
	./$(EXEC4)
run5: $(EXEC5)
	./$(EXEC5)
###############################################################################
# Sprzataczka
###############################################################################
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )