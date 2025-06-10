#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <limits.h>

#include "./board.h"
#include "./heurestics_utils.h"
#include "./openings.h"

#define SIZE 5
#define ALPHA -100000
#define BETA 100000

static const int directions[4][2] = {{0,1},{1,0},{1,1},{1,-1}};

int evaluate(int player) {
    int opponent = 3 - player;
    int score = 0;
    
    if(winCheck(opponent) == 1) return -1000;
    if(loseCheck(player) == 1) return -1000;
    if(winCheck(player) == 1) return 1000;
    if(loseCheck(opponent) == 1) return 1000;

    score += check_triangles(player);

    score += one_empty_in_beetween_patern(player);
    
    return score;
}


// Check if any moves left
int isMovesLeft() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (board[i][j] == 0)
                return 1;
    return 0;
}

int minimax(int depth, int alpha, int beta, int player, int isMax) {
    int score = evaluate(player);
    if (depth == 0 || !isMovesLeft() || score == 1000 || score == -1000)
        return score;

    if (isMax) {
        int best = INT_MIN;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = player;
                    int val = minimax(depth - 1, alpha, beta, player, 0);
                    board[i][j] = 0;
                    
                    best = (val > best) ? val : best;
                    alpha = (alpha > best) ? alpha : best;
                    if (beta <= alpha) return best;
                }
            }
        }
        return best;
    } else {
        int best = INT_MAX;
        int opponent = 3 - player;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == 0) {
                    board[i][j] = opponent;
                    int val = minimax(depth - 1, alpha, beta, player, 1);
                    board[i][j] = 0;

                    best = (val < best) ? val : best;
                    beta = (beta < best) ? beta : best;
                    if (beta <= alpha) return best;
                }
            }
        }
        return best;
    }
}

int findBestMove(int player, int depth) {
    int opponent = 3 - player;
    if((getOpeningMove(player, depth) != -1)) {
        printf("Opening move found\n");
        return getOpeningMove(player, depth);
    }
    int bestVal = INT_MIN;
    int bestMove = -1;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                board[i][j] = player;
                int moveVal = minimax(depth, ALPHA, BETA, player, 0);
                printf("Move: %d, Value: %d\n", (i+1) * 10 + j + 1, moveVal);
                board[i][j] = 0;
                if (moveVal > bestVal) {
                    bestVal = moveVal;
                    bestMove = (i+1) * 10 + j +1;
                }
            }
        }
    }
    return bestMove;
}

int main(int argc, char *argv[]) {
    int server_socket;
    struct sockaddr_in server_addr;
    char server_message[16], player_message[16];

    bool end_game;
    int player, msg, move;

    if (argc != 6) {
        printf("Wrong number of arguments\n");
        return -1;
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("Unable to create socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Unable to connect\n");
        return -1;
    }
    printf("Connected with server successfully\n");

    memset(server_message, '\0', sizeof(server_message));
    if (recv(server_socket, server_message, sizeof(server_message), 0) < 0) {
        printf("Error while receiving server's message\n");
        return -1;
    }

    memset(player_message, '\0', sizeof(player_message));
    snprintf(player_message, sizeof(player_message), "%s %s", argv[3], argv[4]);
    if (send(server_socket, player_message, strlen(player_message), 0) < 0) {
        printf("Unable to send message\n");
        return -1;
    }

    setBoard();
    end_game = false;
    sscanf(argv[3], "%d", &player);

    int depth;
    sscanf(argv[5], "%d", &depth);

    while (!end_game) {
        memset(server_message, '\0', sizeof(server_message));
        if (recv(server_socket, server_message, sizeof(server_message), 0) < 0) {
            printf("Error while receiving server's message\n");
            return -1;
        }
        sscanf(server_message, "%d", &msg);
        move = msg % 100;
        msg = msg / 100;
        if (move != 0) {
            setMove(move, 3 - player);
        }
        if (msg == 0 || msg == 6) {
            move = findBestMove(player, depth);
            printf("Best move: %d\n", move);
            setMove(move, player);
            memset(player_message, '\0', sizeof(player_message));
            snprintf(player_message, sizeof(player_message), "%d", move);
            if (send(server_socket, player_message, strlen(player_message), 0) < 0) {
                printf("Unable to send message\n");
                return -1;
            }
        } else {
            end_game = true;
            switch (msg) {
                case 1: printf("You won.\n"); break;
                case 2: printf("You lost.\n"); break;
                case 3: printf("Draw.\n"); break;
                case 4: printf("You won. player%2 + 1 error.\n"); break;
                case 5: printf("You lost. Your error.\n"); break;
            }
        }
    }

    close(server_socket);
    return 0;
}