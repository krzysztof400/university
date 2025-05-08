#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <limits.h>

#include "./board.h"

#define SIZE 5
#define ALPHA -100000
#define BETA 100000

static const int directions[4][2] = {{0,1},{1,0},{1,1},{1,-1}};

int evaluate(int player) {
    // Heuristic weights
    const int winScore = 100000;
    const int weight4[5] = {0, 1, 10, 100, winScore};    // for len=4 windows
    const int open3Score = 500;
    const int open2Score = 20;

    // Positional heatmap (favor center)
    const int posW[5][5] = {
        {3, 4, 5, 4, 3},
        {4, 6, 8, 6, 4},
        {5, 8,10, 8, 5},
        {4, 6, 8, 6, 4},
        {3, 4, 5, 4, 3}
    };

    int opponent = 3 - player;
    int score = 0;
    
    // Check for winning patterns (four in a row)
    for (int i = 0; i < 28; i++) {
        int playerCount = 0;
        int opponentCount = 0;
        int emptyCount = 0;
        
        for (int j = 0; j < 4; j++) {
            int row = win[i][j][0];
            int col = win[i][j][1];
            
            if (board[row][col] == player) playerCount++;
            else if (board[row][col] == opponent) opponentCount++;
            else emptyCount++;
        }
        
        // If player has four in a row, it's a win
        if (playerCount == 4) return 1000;
        
        // If opponent has four in a row, it's a loss
        if (opponentCount == 4) return -1000;
        
        // Evaluate potential winning positions
        if (opponentCount == 0) {
            // Player has potential to win in this line
            if (playerCount == 3 && emptyCount == 1) score += 100;
            else if (playerCount == 2 && emptyCount == 2) score += 10;
            else if (playerCount == 1 && emptyCount == 3) score += 1;
        }
        
        if (playerCount == 0) {
            // Opponent has potential to win in this line
            if (opponentCount == 3 && emptyCount == 1) score -= 100;
            else if (opponentCount == 2 && emptyCount == 2) score -= 10;
            else if (opponentCount == 1 && emptyCount == 3) score -= 1;
        }
    }
    
    // Check for losing patterns (three in a row)
    for (int i = 0; i < 48; i++) {
        int playerCount = 0;
        int opponentCount = 0;
        int emptyCount = 0;
        bool isPartOfFour = false;
        
        for (int j = 0; j < 3; j++) {
            int row = lose[i][j][0];
            int col = lose[i][j][1];
            
            if (board[row][col] == player) playerCount++;
            else if (board[row][col] == opponent) opponentCount++;
            else emptyCount++;
        }
        
        // Check if this three-in-a-row is part of a potential four-in-a-row
        // This requires checking if adding one more piece could make a four
        // For simplicity, we'll just check if all three are the same player
        
        // If player has three in a row that isn't part of a four-in-a-row strategy,
        // it's a losing position (unless it's already part of a four)
        if (playerCount == 3 && !isPartOfFour) return -1000;
        
        // If opponent has three in a row, it's a win for player
        if (opponentCount == 3 && !isPartOfFour) return 1000;
    }
    
    // Center control bonus
    if (board[2][2] == player) score += 5;
    else if (board[2][2] == opponent) score -= 5;
    
    // Control of corners and adjacent squares
    int corners[4][2] = {{0,0}, {0,4}, {4,0}, {4,4}};
    for (int i = 0; i < 4; i++) {
        int row = corners[i][0];
        int col = corners[i][1];
        if (board[row][col] == player) score += 3;
        else if (board[row][col] == opponent) score -= 3;
    }
    
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