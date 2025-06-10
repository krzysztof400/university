#ifndef OPENINGS_H
#define OPENINGS_H

#include "./board.h"  // or adjust relative path

typedef struct {
    int board[5][5];
    int next_move;
    int length;
} Opening;

// a: current game board, b: opening book pattern board
static inline int boardMatches(const int a[5][5], const int b[5][5]) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            // If the opening book pattern cell is 0, it's a "don't care", matches anything.
            // If the opening book pattern cell is 3, it's a wildcard, matches anything (0, 1, or 2).
            // Otherwise (pattern cell is 1 or 2), it must match the current game board cell.
            if (b[i][j] != 0 && b[i][j] != 3 && a[i][j] != b[i][j]) {
                return 0; // Mismatch
            }
        }
    }
    return 1; // All relevant cells matched
}

static inline int getOpeningMove(int player, int depth) {
    extern int board[5][5];

    static const Opening opening_book[] = {
        {
            .board = {
                {3, 0, 3, 3, 3},
                {3, 3, 3, 3, 3},
                {3, 3, 3, 3, 3}, 
                {3, 3, 3, 3, 3},
                {3, 3, 3, 3, 3}
            },
            .next_move = 12, 
            .length = 1      
        },
        {
            .board = {
                {0, 1, 0, 0, 0},
                {0, 0, 3, 0, 0}, 
                {0, 0, 2, 3, 0}, 
                {0, 0, 3, 0, 0}, 
                {0, 0, 0, 0, 0}
            },
            .next_move = 42, 
            .length = 3
        }
    };

    static const int opening_count = sizeof(opening_book) / sizeof(Opening);

    for (int i = 0; i < opening_count; ++i) {
        const Opening *o = &opening_book[i];
        if (boardMatches(board, o->board)) {
            int row = (o->next_move / 10) - 1; 
            int col = (o->next_move % 10) - 1;
            if (row >= 0 && row < 5 && col >= 0 && col < 5) {
                 if (board[row][col] == 0) { 
                    return o->next_move;
                }
            }
        }
    }

    return -1; 
}

#endif
