/****************************
Maciej Gębala (CC BY-NC 4.0)
Random Boot ver. 0.2
2025-04-13
****************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <time.h>
#include <arpa/inet.h>

#include "./board.h"

int evaluate(int player) {
    // Rows, columns, diagonals
    for (int row = 0; row < 5; row++)
        if (board[row][0] == board[row][1] &&
            board[row][1] == board[row][2]) {
            if (board[row][0] == player) return +10;
            else if (board[row][0] == player%2 + 1) return -10;
        }

    for (int col = 0; col < 5; col++)
        if (board[0][col] == board[1][col] &&
            board[1][col] == board[2][col]) {
            if (board[0][col] == player) return +10;
            else if (board[0][col] == player%2 + 1) return -10;
        }

    if (board[0][0] == board[1][1] &&
        board[1][1] == board[2][2]) {
        if (board[0][0] == player) return +10;
        else if (board[0][0] == player%2 + 1) return -10;
    }

    if (board[0][2] == board[1][1] &&
        board[1][1] == board[2][0]) {
        if (board[0][2] == player) return +10;
        else if (board[0][2] == player%2 + 1) return -10;
    }

    return 0;
}

// Check if moves are left
bool isMovesLeft() {
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (board[i][j] == 0)
                return true;
    return false;
}

int findBestMove(int player) {
    int bestVal = INT_MIN;
    int bestMoveRow = -1, bestMoveCol = -1;

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (board[i][j] == 0) {
                board[i][j] = player;
                int moveVal = minimax(0, false);
                board[i][j] = 0;

                if (moveVal > bestVal) {
                    bestMoveRow = i;
                    bestMoveCol = j;
                    bestVal = moveVal;
                }
            }

    return i*10+j;
}

int minimax(int player, int depth, bool isMax) {
    int score = evaluate(player);

    if (score == 10 || score == -10)
        return score;

    if (!isMovesLeft())
        return 0;

    if (isMax) {
        int best = INT_MIN;
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (board[i][j] == 0) {
                    board[i][j] = player;
                    best = (minimax(player, depth + 1, false) > best) ?
                            minimax(player, depth + 1, false) : best;
                    board[i][j] = 0;
                }
        return best;
    } else {
        int best = INT_MAX;
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (board[i][j] == 0) {
                    board[i][j] = player%2 +1;
                    best = (minimax(player, depth + 1, true) < best) ?
                            minimax(player, depth + 1, true) : best;
                    board[i][j] = 0;
                }
        return best;
    }
}

int main(int argc, char *argv[]) {
  int server_socket;
  struct sockaddr_in server_addr;
  char server_message[16], player_message[16];

  bool end_game;
  int player, msg, move;

  gsl_rng *generator = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(generator, time(NULL));

  if ( argc != 5 ) {
    printf("Wrong number of arguments\n");
    return -1;
  }

  // Create socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if ( server_socket < 0 ) {
    printf("Unable to create socket\n");
    return -1;
  }
  printf("Socket created successfully\n");

  // Set port and IP the same as server-side
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  // Send connection request to server
  if ( connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0 ) {
    printf("Unable to connect\n");
    return -1;
  }
  printf("Connected with server successfully\n");

  // Receive the server message
  memset(server_message, '\0', sizeof(server_message));
  if ( recv(server_socket, server_message, sizeof(server_message), 0) < 0 ) {
    printf("Error while receiving server's message\n");
    return -1;
  }

  memset(player_message, '\0', sizeof(player_message));
  snprintf(player_message, sizeof(player_message), "%s %s", argv[3], argv[4]);
  // Send the message to server
  if ( send(server_socket, player_message, strlen(player_message), 0) < 0 ) {
    printf("Unable to send message\n");
    return -1;
  }

  setBoard();
  end_game = false;
  sscanf(argv[3], "%d", &player);

  while ( !end_game ) {
    memset(server_message, '\0', sizeof(server_message));
    if ( recv(server_socket, server_message, sizeof(server_message), 0) < 0 ) {
      printf("Error while receiving server's message\n");
      return -1;
    }
    sscanf(server_message, "%d", &msg);
    move = msg%100;
    msg = msg/100;
    if ( move != 0 ) {
      setMove(move, 3-player);
    }
    if ( (msg == 0) || (msg == 6) ) {
      move = findBestMove(player, player, generator);
      setMove(move, player);
      memset(player_message, '\0', sizeof(player_message));
      snprintf(player_message, sizeof(player_message), "%d", move);
      if ( send(server_socket, player_message, strlen(player_message), 0) < 0 ) {
        printf("Unable to send message\n");
        return -1;
      }
     } else {
       end_game = true;
       switch ( msg ) {
         case 1 : printf("You won.\n"); break;
         case 2 : printf("You lost.\n"); break;
         case 3 : printf("Draw.\n"); break;
         case 4 : printf("You won. player%2 + 1 error.\n"); break;
         case 5 : printf("You lost. Your error.\n"); break;
      }
    }
  }

  // Close socket
  close(server_socket);

  return 0;
}
