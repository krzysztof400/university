#ifndef OPENINGS_H
#define OPENINGS_H

#include "./board.h"  // or adjust relative path

typedef struct {
    int board[5][5];
    int next_move;
    int length;
} Opening;

static inline int boardMatches(const int a[5][5], const int b[5][5]) {
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            if (b[i][j] != 0 && a[i][j] != b[i][j])
                return 0;
    return 1;
}

static inline int getOpeningMove(int player, int depth) {
    extern int board[5][5];

    static const Opening opening_book[] = {
        {
            .board = {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            },
            .next_move = 12,
            .length = 3
        },
        {
            .board = {
                {1, 0, 0, 0, 0},
                {2, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            },
            .next_move = 13,
            .length = 2
        }
    };

    static const int opening_count = sizeof(opening_book) / sizeof(Opening);

    for (int i = 0; i < opening_count; ++i) {
        const Opening *o = &opening_book[i];
        if (boardMatches(board, o->board)) {
            int row = (o->next_move / 10) - 1;
            int col = (o->next_move % 10) - 1;
            if (board[row][col] == 0) {
                return o->next_move;
            }
        }
    }

    return -1;
}

#endif
