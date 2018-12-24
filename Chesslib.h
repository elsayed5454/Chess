#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MOVES 256

char board[8][8];
char p1_dead[16], p2_dead[16];
int p1_dead_c, p2_dead_c, found_dead, promotion_found;
char save[MAX_MOVES][8][8];
char save_dead1[MAX_MOVES][16], save_dead2[MAX_MOVES][16];
int i, j, k, cout;
int end_game, valid_move, p_turn, king_in_check, i_king_p1, j_king_p1 , i_king_p2, j_king_p2, checkmate, stalemate;

void initialize_board();
void print(char arr[8][8]);
int is_move_valid(char p[]);
int is_char_valid(char p[]);
int the_move(int i1, int j1, int i2, int j2);
int is_road_empty(int i1, int j1, int i2, int j2);
int is_king_in_check(int p, int i_check, int j_check);
void swap(int i1, int j1, int i2, int j2);
int is_checkmate();
int king_can_run();
int piece_save_king();
void promotion(int i1, int j1, int i2, int j2, char to[]);
void save_places();
void undo();
void redo();
void save_game();
int load_game();
