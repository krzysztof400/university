#!/bin/bash

IP="127.0.0.1"
SOCKET=$((RANDOM % 101 + 8080))
DEPTH=3
SESSION="game_session"

echo "Starting server on $IP:$SOCKET"
echo "Client 1 will connect as player"
echo "Client 2 will connect as bot with depth $DEPTH"

# Create tmux session and panes
tmux new-session -d -s $SESSION -n main

tmux send-keys -t $SESSION:0.0 "./game_server $IP $SOCKET" C-m
tmux split-window -h -t $SESSION
tmux send-keys -t $SESSION:0.1 "./experimental $IP $SOCKET 1 experimental $DEPTH" C-m
tmux split-window -v -t $SESSION:0.1
tmux send-keys -t $SESSION:0.2 "./game_min_max_bot $IP $SOCKET 2 bot $DEPTH" C-m

# Arrange layout
tmux select-layout -t $SESSION tiled

# Select the client pane (pane 1) so we land there after attach
tmux select-pane -t $SESSION:0.1

# Attach to the session
tmux attach-session -t $SESSION
