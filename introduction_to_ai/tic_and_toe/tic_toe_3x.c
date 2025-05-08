// #include <stdio.h>
// #include <stdbool.h>
// #include <limits.h>

// #define PLAYER 'X'
// #define OPPONENT 'O'



// // Check for winner
// int evaluate() {
//     // Rows, columns, diagonals
//     for (int row = 0; row < 5; row++)
//         if (board[row][0] == board[row][1] &&
//             board[row][1] == board[row][2]) {
//             if (board[row][0] == PLAYER) return +10;
//             else if (board[row][0] == OPPONENT) return -10;
//         }

//     for (int col = 0; col < 5; col++)
//         if (board[0][col] == board[1][col] &&
//             board[1][col] == board[2][col]) {
//             if (board[0][col] == PLAYER) return +10;
//             else if (board[0][col] == OPPONENT) return -10;
//         }

//     if (board[0][0] == board[1][1] &&
//         board[1][1] == board[2][2]) {
//         if (board[0][0] == PLAYER) return +10;
//         else if (board[0][0] == OPPONENT) return -10;
//     }

//     if (board[0][2] == board[1][1] &&
//         board[1][1] == board[2][0]) {
//         if (board[0][2] == PLAYER) return +10;
//         else if (board[0][2] == OPPONENT) return -10;
//     }

//     return 0;
// }

// // Check if moves are left
// bool isMovesLeft() {
//     for (int i = 0; i < 5; i++)
//         for (int j = 0; j < 5; j++)
//             if (board[i][j] == '_')
//                 return true;
//     return false;
// }

// // Min-Max function
// int minimax(int depth, bool isMax) {
//     int score = evaluate();

//     if (score == 10 || score == -10)
//         return score;

//     if (!isMovesLeft())
//         return 0;

//     if (isMax) {
//         int best = INT_MIN;
//         for (int i = 0; i < 5; i++)
//             for (int j = 0; j < 5; j++)
//                 if (board[i][j] == '_') {
//                     board[i][j] = PLAYER;
//                     best = (minimax(depth + 1, false) > best) ?
//                            minimax(depth + 1, false) : best;
//                     board[i][j] = '_';
//                 }
//         return best;
//     } else {
//         int best = INT_MAX;
//         for (int i = 0; i < 5; i++)
//             for (int j = 0; j < 5; j++)
//                 if (board[i][j] == '_') {
//                     board[i][j] = OPPONENT;
//                     best = (minimax(depth + 1, true) < best) ?
//                            minimax(depth + 1, true) : best;
//                     board[i][j] = '_';
//                 }
//         return best;
//     }
// }

// // Find best move for player
// void findBestMove() {
//     int bestVal = INT_MIN;
//     int bestMoveRow = -1, bestMoveCol = -1;

//     for (int i = 0; i < 5; i++)
//         for (int j = 0; j < 5; j++)
//             if (board[i][j] == '_') {
//                 board[i][j] = PLAYER;
//                 int moveVal = minimax(0, false);
//                 board[i][j] = '_';

//                 if (moveVal > bestVal) {
//                     bestMoveRow = i;
//                     bestMoveCol = j;
//                     bestVal = moveVal;
//                 }
//             }

//     printf("Best move: row = %d, col = %d\n", bestMoveRow, bestMoveCol);
// }

int main() {
//     // Example board state
//     board[0][0] = 'X';
//     board[0][1] = 'O';
//     board[0][2] = 'X';
//     board[1][0] = 'O';
//     board[1][1] = 'X';
//     board[2][0] = '_';
//     board[2][1] = '_';
//     board[2][2] = 'O';

//     findBestMove();
    return 0;
}