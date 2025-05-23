#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "./board.h"

static const int heat_map[5][5] = {
    { 1, 3, 1, 3, 1 },
    { 3, 2, 0, 2, 3 },
    { 1, 0, -1, 0, 1 },
    { 3, 2, 0, 2, 3 },
    { 1, 3, 1, 3, 1 }
};

static const int triangle_patterns[8][3][2] = {
    {{0,0}, {0,3}, {3,0}},
    {{0,1}, {0,4}, {3,1}},
    {{1,0}, {1,3}, {4,0}},
    {{1,1}, {1,4}, {4,1}},
    {{4,4}, {4,1}, {1,4}},
    {{4,3}, {4,0}, {1,3}},
    {{3,4}, {3,1}, {0,4}},
    {{3,3}, {3,0}, {0,3}}
};

int one_empty_in_beetween_patern(int player, int opponent) {
    int add_to_score = 0;
    for(int i = 0; i < 4; i++) {
        for(int j =0; j<4; j++){
            if(board[i][j] == player && board[i+1][j] == 0 && board[i+2][j] == player) {
                add_to_score -= 10;
            }
            if(board[i][j] == player && board[i][j+1] == 0 && board[i][j+2] == player) {
                add_to_score -= 10;
            }
            if(board[i][j] == player && board[i+1][j+1] == 0 && board[i+2][j+2] == player) {
                add_to_score -= 10;
            }
            if(board[i][j] == opponent && board[i+1][j] == 0 && board[i+2][j] == opponent) {
                add_to_score += 10;
            }
            if(board[i][j] == opponent && board[i][j+1] == 0 && board[i][j+2] == opponent) {
                add_to_score += 10;
            }
            if(board[i][j] == opponent && board[i+1][j+1] == 0 && board[i+2][j+2] == opponent) {
                add_to_score += 10;
            }
        }
    }
    return add_to_score;
}

