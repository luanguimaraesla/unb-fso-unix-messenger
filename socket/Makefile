LOGFOLDER := ./log/
BINFOLDER := ./bin/
INCFOLDER := ./include/
SRCFOLDER := ./source/
OBJFOLDER := ./obj/
COMPILER := gcc
FLAGS := -I$(INCFOLDER)
CFLAGS := -pthread

SRCFILES := $(wildcard source/*.c)

all:	 $(SRCFILES:source/%.c=obj/%.o)
		$(COMPILER) $(FLAGS) $(OBJFOLDER)*.o -o $(BINFOLDER)messenger $(CFLAGS)
obj/%.o:	source/%.c
				$(COMPILER) $(FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJFOLDER)*.o

run:
	$(BINFOLDER)messenger 2> $(LOGFOLDER)messenger.log

kill: PROCESS = $(shell ps aux | grep messenger | head -n 1 | grep -o -P "\d*" | head -n 1)
kill:
	@kill -12 ${PROCESS}
