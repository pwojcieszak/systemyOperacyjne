.SUFFIXES: .c .cpp .o .x .h
DIR = $(notdir $(CURDIR))
SYSNAME = $(shell uname -s)

NAME1 = sem_fun
NAME2 = powielacz
NAME3 = implement

EXEC1  = $(NAME1).x  
OBJS1  = $(NAME1).o
EXEC2  = $(NAME2).x  
OBJS2  = $(NAME2).o
EXEC3  = $(NAME3).x  
OBJS3  = $(NAME3).o

HEAD1  = $(NAME1).h


LIB_NAME = MojeFunkcje
# Katalog dla bibliotek
LIB_DIR = ./lib
# Biblioteka statyczna (static library)
LIB_STAT = lib$(LIB_NAME).a
# Biblioteka dzielona linkowana dynamicznie - nazwa zalezy od systemu:
ifeq ($(SYSNAME), Linux)
  LIB_SHAR = lib$(LIB_NAME).so
endif
# Pliki skompilowane wchodzace do biblioteki
LIB_OBJS = $(OBJS1)

$(LIB_STAT): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $?
	mkdir -p $(LIB_DIR)
	mv $(LIB_STAT) $(LIB_DIR)
# Tworzenie biblioteki dzielonej (shared library)
$(LIB_SHAR): $(LIB_OBJS)
# Linux - opcja linkera: -shared
ifeq ($(SYSNAME), Linux)
	$(LD) -shared -o $@ $(LFLAGS) $(LIB_OBJS)
endif
	mkdir -p $(LIB_DIR)
	mv $(LIB_SHAR) $(LIB_DIR)

CFLAGS = -Wall -std=c99 -pedantic -O 
LFLAGS = -Wall -std=c99 -pedantic -O -pthread

LDLIBS = -lm
CO = gcc
LD = $(CO)

%.o: %.c
	$(CO) $(CFLAGS) -c $<

$(EXEC1): $(OBJS1) 
	$(LD) -o $@ $(LFLAGS) $^
$(EXEC2): $(OBJS2) $(LIB_STAT)
	rm -f $(LIB_DIR)/$(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS)
$(EXEC3): $(OBJS3) 
	rm -f $(LIB_DIR)/$(LIB_SHAR)
	$(LD) -o $@ $(LFLAGS) $(OBJS3) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS)	

$(OBJS2): $(HEADS3)
.PHONY: run1 run2 run3

run1: $(EXEC1) 
	./$(EXEC1) 
run2: $(EXEC2) $(EXEC3)
	./$(EXEC2) $(NAZWA) $(PROGRAM) $(L_PROC) $(L_SEK)
#make run2 NAZWA="SEM" PROGRAM="./implement.c" L_PROC=1 L_SEK=1

run3: $(EXEC3) 
	./$(EXEC3) 
	
.PHONY: clean tar
clean:                                                     
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )