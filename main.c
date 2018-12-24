#include "Chesslib.h"

char board[8][8] = {{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}};
int p1_dead_c = 0, p2_dead_c = 0, found_dead = 0, promotion_found = 0, cout = -1;
int end_game = 0, p_turn = 0, king_in_check = 0, i_king_p1 = 7, j_king_p1 = 4, i_king_p2 = 0, j_king_p2 = 4, checkmate = 0, stalemate = 0;

int main()
{
    initialize_board();
    save_places();
    char p1_move[5], p2_move[5];
    while (!end_game)
    {
        printf("\n");
        print(board);
        // player 1 turn
        if (p_turn % 2 == 0)
        {
            if (king_in_check)
                printf("Your king is in check, choose your move wisely\n");
            do
            {
                printf("Player 1 move: ");
                scanf("%s", p1_move);
                is_move_valid(p1_move);
                if (end_game)
                    break;
                if (p1_move[0] == 'U' && strlen(p1_move) == 1 && cout > 0)
                {
                    undo();
                    break;
                }
                else if (p1_move[0] == 'R' && strlen(p1_move) == 1 && cout >= 0 && (save[cout + 1][0][0] == '\xdb' || isalpha(save[cout + 1][0][0])))
                {
                    redo();
                    break;
                }
                else if (p1_move[0] == 'S' && strlen(p1_move) == 1)
                {
                    save_game();
                    p_turn--;
                    break;
                }
                else if (p1_move[0] == 'L' && strlen(p1_move) == 1)
                {
                    if (!load_game())
                        printf("Error in SAVEGAME.txt file \n");
                    else
                        load_game();
                    p_turn--;
                    break;
                }
            }
            while (!valid_move);
        }
        // player 2 turn
        else
        {
            if (king_in_check)
                printf("Your king is in check, choose your move wisely\n");
            do
            {
                printf("Player 2 move: ");
                scanf("%s", p2_move);
                is_move_valid(p2_move);
                if (end_game)
                    break;
                if (p2_move[0] == 'U' && strlen(p2_move) == 1 && cout > 0)
                {
                    undo();
                    break;
                }
                else if (p2_move[0] == 'R' && strlen(p2_move) == 1 && cout >= 0 && (save[cout + 1][0][0] == '\xdb' || isalpha(save[cout + 1][0][0])))
                {
                    redo();
                    break;
                }
                else if (p2_move[0] == 'S' && strlen(p2_move) == 1)
                {
                    save_game();
                    p_turn--;
                    break;
                }
                else if (p2_move[0] == 'L' && strlen(p2_move) == 1)
                {
                    if (!load_game())
                        printf("Error in SAVEGAME.txt file \n");
                    else
                        load_game();
                    p_turn--;
                    break;
                }
            }
            while (!valid_move);
        }
        p_turn++;
        if (end_game)
            break;
    }
    p_turn--;
    print(board);
    if (checkmate)
        printf("Checkmate!!!\nPlayer %d wins.\nCongratulations!!\n", p_turn % 2 + 1);
    else if (stalemate)
        printf("Stalemate!!!\n");
    printf("To exit type 'E'\n");
    char byebye;
    do
        scanf("%c", &byebye);
    while (byebye != 'E');
    return 0;
}
