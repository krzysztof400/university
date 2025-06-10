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

static const int triangle_patterns[16][3][2] = {
    {{0,0}, {0,3}, {3,0}},
    {{0,0}, {0,3}, {3,3}},
    {{0,0}, {3,0}, {3,3}},
    {{3,3}, {3,0}, {0,3}},

    {{0,1}, {0,4}, {3,1}},
    {{0,1}, {0,4}, {3,4}},
    {{0,1}, {3,1}, {3,4}},
    {{3,4}, {3,1}, {0,4}},

    {{1,0}, {1,3}, {4,0}},
    {{1,0}, {1,3}, {4,3}},
    {{1,0}, {4,0}, {4,3}},
    {{4,3}, {4,0}, {1,3}},

    {{1,1}, {1,4}, {4,1}},
    {{1,1}, {1,4}, {4,4}},
    {{1,1}, {4,1}, {4,4}},
    {{4,4}, {4,1}, {1,4}},
};

int one_empty_in_beetween_patern(int player) {
    int opponent = 3 - player;
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

int heat_map_score(int player) {
    for(int i=0; i<=4; i++){
        for(int j=0; j<=4; j++){
            
        }
    }
}

int check_triangles(int player) {
    int score = 0;
    for (int i = 0; i < 8; i++) {
        int p1 = board[triangle_patterns[i][0][0]][triangle_patterns[i][0][1]];
        int p2 = board[triangle_patterns[i][1][0]][triangle_patterns[i][1][1]];
        int p3 = board[triangle_patterns[i][2][0]][triangle_patterns[i][2][1]];

        if (p1 == player && p2 == player && p3 == player) {
            score += 100;
            // printf("Triangle found for player %d at pattern %d\n", player, i);
        } else if (p1 == 3 - player && p2 == 3 - player && p3 == 3 - player) {
            // printf("Triangle found for opponent %d at pattern %d\n", 3 - player, i);
            score -= 100;
        }
    }
    return score;
}