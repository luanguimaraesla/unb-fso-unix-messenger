LOGFOLDER := ./log/
BINFOLDER := ./bin/
INCFOLDER := ./include/
SRC_COMMON_FOLDER := ./source/common/
SRC_SERVER_FOLDER := ./source/server/
SRC_CLIENT_FOLDER := ./source/client/
OBJ_COMMON_FOLDER := ./obj/common/
OBJ_SERVER_FOLDER := ./obj/server/
OBJ_CLIENT_FOLDER := ./obj/client/
COMPILER := gcc
FLAGS := -w -I$(INCFOLDER)
CFLAGS := -pthread

SRC_COMMON_FILES := $(wildcard source/common/*.c)
SRC_SERVER_FILES := $(wildcard source/server/*.c)
SRC_CLIENT_FILES := $(wildcard source/client/*.c)

all: client server

common: $(SRC_COMMON_FILES:source/common/%.c=obj/common/%.o)

client: common $(SRC_CLIENT_FILES:source/client/%.c=obj/client/%.o)
		$(COMPILER) $(FLAGS) $(OBJ_COMMON_FOLDER)*.o $(OBJ_CLIENT_FOLDER)*.o -o $(BINFOLDER)messenger_client $(CFLAGS)
	
server: common $(SRC_SERVER_FILES:source/server/%.c=obj/server/%.o)
		$(COMPILER) $(FLAGS) $(OBJ_COMMON_FOLDER)*.o $(OBJ_SERVER_FOLDER)*.o -o $(BINFOLDER)messenger_server $(CFLAGS)

obj/common/%.o: source/common/%.c
				$(COMPILER) $(FLAGS) -c $< -o $@

obj/server/%.o:	source/server/%.c
				$(COMPILER) $(FLAGS) -c $< -o $@

obj/client/%.o:	source/client/%.c
				$(COMPILER) $(FLAGS) -c $< -o $@

.PHONY: clean
clean:
	ipcrm --all=msg
	rm -rf $(OBJ_COMMON_FOLDER)*.o
	rm -rf $(OBJ_CLIENT_FOLDER)*.o
	rm -rf $(OBJ_SERVER_FOLDER)*.o
	rm -rf $(LOGFOLDER)*

run_server:
	$(BINFOLDER)messenger_server 2> $(LOGFOLDER)messenger_server.log

run_client:
	$(BINFOLDER)messenger_client 2> $(LOGFOLDER)messenger_client.log

# Each screen show
SCREEN_A := $(shell echo -e '$(BINFOLDER)messenger_server 2> $(LOGFOLDER)messenger_server.log')
SCREEN_B := $(shell echo -e '$(BINFOLDER)messenger_client 2> $(LOGFOLDER)messenger_client.log')
SCREEN_C := $(shell echo -e 'tail -f $(LOGFOLDER)messenger_server.log')
SCREEN_D := $(shell echo -e 'tail -f $(LOGFOLDER)messenger_client.log')

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
	$(shell tmux send-keys -t 2 'sleep 1;${SCREEN_C}' enter)
	$(shell tmux send-keys -t 1 'sleep 2;${SCREEN_D}' enter)
	$(shell tmux attach)

kill: PROCESS = $(shell ps aux | grep messenger | head -n 1 | grep -o -P "\d*" | head -n 1)
kill:
	@kill -12 ${PROCESS}
