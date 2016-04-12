LOGFOLDER := ./log/
BINFOLDER := ./bin/
INCFOLDER := ./include/
SRCFOLDER := ./source/
OBJFOLDER := ./obj/
COMPILER := gcc
FLAGS := -I$(INCFOLDER)

SRCFILES := $(wildcard source/*.c)

all:	 $(SRCFILES:source/%.c=obj/%.o)
		$(COMPILER) $(FLAGS) $(OBJFOLDER)*.o -o $(BINFOLDER)messenger

obj/%.o:	source/%.c
				$(COMPILER) $(FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJFOLDER)*.o
	rm -rf $(LOGFOLDER)*.log
	ipcrm --all=msg

# Each screen show
SCREEN_A := $(shell echo -e '$(BINFOLDER)messenger 2> $(LOGFOLDER)messengerA.log')
SCREEN_B := $(shell echo -e '$(BINFOLDER)messenger 2> $(LOGFOLDER)messengerB.log')
SCREEN_C := $(shell echo -e 'tail -f $(LOGFOLDER)messengerA.log')
SCREEN_D := $(shell echo -e 'tail -f $(LOGFOLDER)messengerB.log')

run:
	$(shell tmux new-session -d && tmux split-window -d -t 0 -h)
	$(shell tmux send-keys -t 0 '${SCREEN_A}' enter)
	$(shell tmux send-keys -t 1 'sleep 1; ${SCREEN_B}' enter)
	$(shell tmux attach)

run_and_show_logs:
	$(shell tmux new-session -d && tmux split-window -d -t 0 -h)
	$(shell tmux split-window -d -t 0 -v)
	$(shell tmux split-window -d -t 2 -v)
	$(shell tmux send-keys -t 0 '${SCREEN_A}' enter)
	$(shell tmux send-keys -t 3 'sleep 1; ${SCREEN_B}' enter)
	$(shell tmux send-keys -t 2 'sleep 1; ${SCREEN_C}' enter)
	$(shell tmux send-keys -t 1 'sleep 2; ${SCREEN_D}' enter)
	$(shell tmux attach)

kill: PROCESS = $(shell ps aux | grep messenger | head -n 1 | grep -o -P "\d*" | head -n 1)
kill:
	@kill -12 ${PROCESS}
