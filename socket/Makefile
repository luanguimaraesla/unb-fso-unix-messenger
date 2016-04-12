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
	rm -rf $(OBJ_COMMON_FOLDER)*.o
	rm -rf $(OBJ_CLIENT_FOLDER)*.o
	rm -rf $(OBJ_SERVER_FOLDER)*.o

run_server:
	$(BINFOLDER)messenger_server 2> $(LOGFOLDER)messenger_server.log

run_client:
	$(BINFOLDER)messenger_client 2> $(LOGFOLDER)messenger_client.log

run:
	$(shell tmux new-session -d && tmux split-window -d -t 0 -h)
	$(shell tmux send-keys -t 0 '$(BINFOLDER)messenger_server 2> $(LOGFOLDER)messenger_server.log' enter)
	$(shell tmux send-keys -t 1 'sleep 1' enter)
	$(shell tmux send-keys -t 1 '$(BINFOLDER)messenger_client 2> $(LOGFOLDER)messenger_client.log' enter)
	$(shell tmux attach)

kill: PROCESS = $(shell ps aux | grep messenger | head -n 1 | grep -o -P "\d*" | head -n 1)
kill:
	@kill -12 ${PROCESS}
