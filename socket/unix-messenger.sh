#! /bin/bash

LOGFOLDER="log"
BINFOLDER="bin"

SCREEN_A=$(echo -e "$BINFOLDER/messenger_server 2> $LOGFOLDER/messenger_server.log" )
SCREEN_B=$(echo -e "$BINFOLDER/messenger_client 2> $LOGFOLDER/messenger_client.log" )
SCREEN_C=$(echo -e "tail -f $LOGFOLDER/messenger_server.log" )
SCREEN_D=$(echo -e "tail -f $LOGFOLDER/messenger_client.log" )

tmux new-session -d && tmux split-window -d -t 0 -h
tmux split-window -d -t 0 -v
tmux split-window -d -t 2 -v
tmux send-keys -t 0 "$SCREEN_A" enter
tmux send-keys -t 3 "sleep 1; $SCREEN_B" enter
tmux send-keys -t 2 "sleep 1;$SCREEN_C" enter
tmux send-keys -t 1 "sleep 2;$SCREEN_D" enter
tmux attach
