
.PHONY: all clean depend

OBJS = main.o tap_util.o rs232.o

CFLAGS += -Wall

INCLUDES = -I./include 


all: tapsend

tapsend: $(OBJS)
	@echo 'CC $@'
	@$(CC)  $(CFLAGS) $(INCLUDES) -o $@ $? $(LIBS) -lpthread

%.o: %.c
	@echo 'CC $@'
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f *.elf *.gdb *.o *~ *.a .depend cscope* tapsend include/*~

depend: 

install: 

