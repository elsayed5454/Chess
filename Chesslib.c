#include "Chesslib.h"

void initialize_board()
{
    board[7][0] = 'r', board[7][1] = 'n', board[7][2] = 'b', board[7][3] = 'q', board[7][4] = 'k';
    board[7][5] = 'b', board[7][6] = 'n', board[7][7] = 'r';
    // pawns places
    for (i = 1; i < 8; i += 5)
        for (j = 0; j < 8; j++)
            if (i == 6)
                board[i][j] = 'p';
            else
                board[i][j] = 'P';
    // empty places
    for (i = 2; i < 6; i++)
        for (j = 0; j < 8; j++)
            if((i + j) % 2 != 0)
                board[i][j]=' ';
            else
                board[i][j]='\xdb';
}

int is_move_valid(char p[])
{
    // check the characters
    if (!is_char_valid(p))
    {
        valid_move = 0;
        return 0;
    }
    // convert move
    int i1 = -(p[1] - '1') + 7, i2 = -(p[3] - '1') + 7;
    int j1 = p[0] - 'A', j2 = p[2] - 'A';
    // is it your pieces ?
    if ((p_turn % 2 == 0 && (!islower(board[i1][j1]) || islower(board[i2][j2]))) || (p_turn % 2 == 1 && (!isupper(board[i1][j1]) || isupper(board[i2][j2]))))
    {
        valid_move = 0;
        return 0;
    }
    // check the move for each piece
    if (!the_move(i1, j1, i2, j2))
    {
        valid_move = 0;
        return 0;
    }
    // follow the king
    if (board[i1][j1] == 'k' || board[i1][j1] == 'K')
    {
       if (p_turn % 2 == 0)
        {
            i_king_p1 = i2;
            j_king_p1 = j2;
        }
        else
        {
            i_king_p2 = i2;
            j_king_p2 = j2;
        }
    }
    // is the road empty ?
    if (!is_road_empty(i1, j1, i2, j2))
    {
        valid_move = 0;
        return 0;
    }
    // make the move
    // in case of capturing
    if (isalpha(board[i2][j2]))
    {
        if (p_turn % 2 == 0)
            p2_dead[p2_dead_c++] = board[i2][j2];
        else
            p1_dead[p1_dead_c++] = board[i2][j2];
        found_dead = 1;
    }
    board[i2][j2] = board[i1][j1];
    if ((i1 + j1) % 2 != 0)
        board[i1][j1] = ' ';
    else
        board[i1][j1] = '\xdb';
    // in case of promotion
    if (promotion_found)
    {
        promotion(i1, j1, i2, j2, p);
        promotion_found = 0;
    }
    // does the move removed the check ?
    if (king_in_check)
    {
        if (p_turn % 2 == 0)
        {
            if(!is_king_in_check(p_turn, i_king_p1, j_king_p1))
                king_in_check = 0;
            else
            {
                valid_move = 0;
                if (found_dead)
                {
                    p2_dead_c--;
                    board[i1][j1] = p2_dead[p2_dead_c++];
                    swap(i1, j1, i2, j2);
                    found_dead = 0;
                }
                else
                    swap(i1, j1, i2, j2);
                return 0;
            }
        }
        else
        {
            if(!is_king_in_check(p_turn, i_king_p2, j_king_p2))
                king_in_check = 0;
            else
            {
                valid_move = 0;
                if (found_dead)
                {
                    p1_dead_c--;
                    board[i1][j1] = p1_dead[p1_dead_c++];
                    swap(i1, j1, i2, j2);
                    found_dead = 0;
                }
                else
                    swap(i1, j1, i2, j2);
                return 0;
            }
        }
    }
    // check if the king is in check
    if (p_turn % 2 == 0)
    {
        if(is_king_in_check(p_turn + 1, i_king_p2, j_king_p2))
            king_in_check = 1;
        else
            king_in_check = 0;
    }
    else
    {
        if(is_king_in_check(p_turn + 1, i_king_p1, j_king_p1))
            king_in_check = 1;
        else
            king_in_check = 0;
    }
    // does the game ended as checkmate ?
    if (king_in_check)
        if (is_checkmate(i2, j2))
        {
            end_game = 1;
            checkmate = 1;
            return 0;
        }
    // does the game ended as stalemate ?
    if (!king_can_run() && p_turn > 8)
        if (!piece_save_king())
        {
            end_game = 1;
            stalemate = 1;
            return 0;
        }
    // no capture happened, the move is valid and save board
    found_dead = 0;
    valid_move = 1;
    save_places();
    return 0;
}

void print(char arr[8][8])
{
    // print the new board
    system("cls");
    printf("Welcome to EA chess game!\nThe rules are simple: type the column letter followed by the row for the current piece place"
           " then type the same for the place you want the piece to go to.\n");
    printf("ex: A2A4\nIn case of promotion type the same command followed by the first letter of the wanted piece."
           "\nex: A7A8Q\nALL COMMAND LETTERS ARE IN CAPITAL.\nHave fun!\n\n");
    printf("         A      B      C      D      E      F      G      H                No of moves = %i\n\n", p_turn);
    for (i = 0; i < 8; i++)
        {
            if(i % 2 != 0)
                printf("             \xdb\xdb\xdb\xdb\xdb\xdb\xdb       \xdb\xdb\xdb\xdb\xdb\xdb\xdb       \xdb\xdb\xdb\xdb\xdb\xdb\xdb       \xdb\xdb\xdb\xdb\xdb\xdb\xdb\n");
            else
                printf("      \xdb\xdb\xdb\xdb\xdb\xdb\xdb       \xdb\xdb\xdb\xdb\xdb\xdb\xdb       \xdb\xdb\xdb\xdb\xdb\xdb\xdb       \xdb\xdb\xdb\xdb\xdb\xdb\xdb       \n");
            printf("   %d  ",8 - i);
            for(j = 0; j < 8; j++)
                if((i + j) % 2 != 0)
                    printf("   %c   ",arr[i][j]);
                else
                    printf("\xdb\xdb\xdb%c\xdb\xdb\xdb",arr[i][j]);
            if (i == 0)
            {
                printf("  %d          Player 2 Dead Pieces: ", 8 - i);
                for (k = 0;k < 8; k++)
                    printf("%c ",p2_dead[k]);
            }
            else if (i == 1)
                printf("  %d", 8 - i);
            else if (i == 2)
                printf("  %d          Type 'U' to undo the move", 8 - i);
            else if (i == 3)
                printf("  %d          Type 'R' to redo the move", 8 - i);
            else if (i == 4)
                printf("  %d          Type 'S' to save game", 8 - i);
            else if (i == 5)
                printf("  %d          Type 'L' to load previous game", 8 - i);
            else if (i == 6)
                printf("  %d", 8 - i);
            else if (i == 7)
            {
                printf("  %d          Player 1 Dead Pieces: ", 8 - i);
                for (k = 0; k < 8; k++)
                    printf("%c ",p1_dead[k]);
            }
            printf("\n");
            if(i % 2 != 0)
                printf("             \xdb\xdb\xdb\xdb\xdb\xdb\xdb       \xdb\xdb\xdb\xdb\xdb\xdb\xdb       \xdb\xdb\xdb\xdb\xdb\xdb\xdb       \xdb\xdb\xdb\xdb\xdb\xdb\xdb\n");
            else
                printf("      \xdb\xdb\xdb\xdb\xdb\xdb\xdb       \xdb\xdb\xdb\xdb\xdb\xdb\xdb       \xdb\xdb\xdb\xdb\xdb\xdb\xdb       \xdb\xdb\xdb\xdb\xdb\xdb\xdb       \n");
        }
        printf("\n         A      B      C      D      E      F      G      H             \n\n");
}

int is_char_valid(char p[])
{
    if (p[0] < 'A' || p[0] > 'H' || p[2] < 'A' || p[2] > 'H')
        return 0;
    else if (p[1] < '1' || p[1] > '8' || p[3] < '1' || p[3] > '8')
        return 0;
    else if (strlen(p) == 5 && (p[4] == 'Q' || p[4] == 'N' || p[4] == 'R' || p[4] == 'B'))
    {
        promotion_found = 1;
        return 1;
    }
    else if (strlen(p) != 4)
        return 0;
    else
        return 1;
}

int the_move(int i1, int j1, int i2, int j2)
{
    if (board[i1][j1] == 'p' || board[i1][j1] == 'P')
    {
        // player 1 pawn
        if (board[i1][j1] == 'p')
        {
            // pawn is allowed to move two squares in the first move
            if (i1 == 6 && i1 - i2 == 2 && j1 == j2 && !isalpha(board[i2][j2]) && !isalpha(board[i2 + 1][j2]))
                return 1;
            else if (i1 == 6 && i1 - i2 == 1 && j1 == j2 && !isalpha(board[i2][j2]))
                return 1;
            // normal pawn move
            else if (i1 - i2 == 1 && j1 == j2 && !isalpha(board[i2][j2]))
                return 1;
            // in case of capturing
            else if (isupper(board[i2][j2]) && i1 - i2 == 1 && abs(j1 - j2) == 1)
                return 1;
            else
                return 0;
        }
        // repeat for player 2
        else if (board[i1][j1] == 'P')
        {
            if (i1 == 1 && i2 - i1 == 2 && j1 == j2 && !isalpha(board[i2][j2]) && !isalpha(board[i2 - 1][j2]))
                return 1;
            else if (i1 == 1 && i2 - i1 == 1 && j1 == j2 && !isalpha(board[i2][j2]))
                return 1;
            else if (i2 - i1 == 1 && j1 == j2 && !isalpha(board[i2][j2]))
                return 1;
            else if (islower(board[i2][j2]) && i2 - i1 == 1 && abs(j1 - j2) == 1)
                return 1;
            else
                return 0;
        }
    }
    // rooks move
    else if (board[i1][j1] == 'r' || board[i1][j1] == 'R')
    {
        if ((i1 == i2 || j1 == j2))
            return 1;
        else
            return 0;
    }
    // knights move
    else if (board[i1][j1] == 'n' || board[i1][j1] == 'N')
    {
        if (((abs(i1 - i2) == 1 && abs(j1 - j2) == 2) || (abs(i1 - i2) == 2 && abs(j1 - j2) == 1)))
            return 1;
        else
            return 0;
    }
    // bishops move
    else if (board[i1][j1] == 'b' || board[i1][j1] == 'B')
    {
        if (abs(i1 - i2) == abs(j1 - j2))
            return 1;
        else
            return 0;
    }
    // queens move
    else if (board[i1][j1] == 'q' || board[i1][j1] == 'Q')
    {
        if ((i1 == i2 || j1 == j2) || abs(i1 - i2) == abs(j1 - j2))
            return 1;
        else
            return 0;
    }
    // kings move
    else if ((board[i1][j1] == 'k' || board[i1][j1] == 'K') && !is_king_in_check(p_turn, i2, j2))
    {
        if ((abs(i1 - i2) == 1 && abs(j1 - j2) == 1) || (i1 == i2 && abs(j1 - j2) == 1) || (abs(i1 - i2) == 1 && j1 == j2))
            return 1;
        else
            return 0;
    }
    return 0;
}

int is_road_empty(int i1, int j1, int i2, int j2)
{
    // n is the no of squares to check, i is the initial row to check, j is the initial column to check
    int n;
    // check the road for rooks
    if (board[i1][j1] == 'r' || board[i1][j1] == 'R')
    {
        // moving vertically
        if (j1 == j2)
        {
            j = j1;
            if (i1 > i2)
                i = i2 + 1;
            else
                i = i1 + 1;
            n = abs(i1 - i2) + i - 1;
            for (; i < n; i++)
                if (isalpha(board[i][j]))
                    return 0;
            return 1;
        }
        // moving horizontally
        else if (i1 == i2)
        {
            i = i1;
            if (j1 > j2)
                j = j2 + 1;
            else
                j = j1 + 1;
            n = abs(j1 - j2) + j - 1;
            for (; j < n; j++)
                if (isalpha(board[i][j]))
                    return 0;
            return 1;
        }
    }
    // check the road for bishops
    else if (board[i1][j1] == 'b' || board[i1][j1] == 'B')
    {
        // from upper left to lower right
        if((i2 > i1 && j2 > j1) || (i2 < i1 && j2 < j1))
        {
            if (i1 > i2)
            {
                i = i2 + 1;
                j = j2 + 1;
            }
            else
            {
                i = i1 + 1;
                j = j1 + 1;
            }
            n = abs(i2 - i1) + i - 1;
            for(; i < n; i++, j++)
                if(isalpha(board[i][j]))
                    return 0;
            return 1;
        }
        // from upper right to lower left
        else if ((i1 > i2 && j1 < j2) || (i1 < i2 && j1 > j2))
            {
                if (i1 > i2)
                {
                    i = i2 + 1;
                    j = j2 - 1;
                }
                else
                {
                    i = i1 + 1;
                    j = j1 - 1;
                }
                n = abs(i2 - i1) + i - 1;
                for(; i < n; i++, j--)
                    if(isalpha(board[i][j]))
                        return 0;
                return 1;
            }
    }
    // check the road for queens
    else if (board[i1][j1] == 'q' || board[i1][j1] == 'Q')
    {
        // moving vertically
        if (j1 == j2)
        {
            j = j1;
            if (i1 > i2)
                i = i2 + 1;
            else
                i = i1 + 1;
            n = abs(i1 - i2) + i - 1;
            for (; i < n; i++)
                if (isalpha(board[i][j]))
                    return 0;
            return 1;
        }
        // moving horizontally
        else if (i1 == i2)
        {
            i = i1;
            if (j1 > j2)
                j = j2 + 1;
            else
                j = j1 + 1;
            n = abs(j1 - j2) + j - 1;
            for (; j < n; j++)
                if (isalpha(board[i][j]))
                    return 0;
            return 1;
        }
        // from upper left to lower right
        else if((i2 > i1 && j2 > j1) || (i2 < i1 && j2 < j1))
        {
            if (i1 > i2)
            {
                i = i2 + 1;
                j = j2 + 1;
            }
            else
            {
                i = i1 + 1;
                j = j1 + 1;
            }
            n = abs(i2 - i1) + i - 1;
            for(; i < n; i++, j++)
                if(isalpha(board[i][j]))
                    return 0;
            return 1;
        }
        // from upper right to lower left
        else if((i1 > i2 && j1 < j2) || (i1 < i2 && j1 > j2))
            {
                if (i1 > i2)
                {
                    i = i2 + 1;
                    j = j2 - 1;
                }
                else
                {
                    i = i1 + 1;
                    j = j1 - 1;
                }
                n = abs(i2 - i1) + i - 1;
                for(; i < n; i++, j--)
                    if(isalpha(board[i][j]))
                        return 0;
                return 1;
            }
    }
    return 1;
}

int is_king_in_check(int p, int i_check, int j_check)
{
    i = i_check, j = j_check;
    // check player 1 king
    if (p % 2 == 0)
    {
        // check for pawns in front of the king
        if (board[i - 1][j + 1] == 'P' || board[i - 1][j - 1] == 'P')
            return 1;
        // check for knights around the king
        if (board[i - 2][j + 1] == 'N' || board[i - 2][j - 1] == 'N' || board[i + 2][j + 1] == 'N' || board[i + 2][j - 1] == 'N' ||
            board[i + 1][j + 2] == 'N' || board[i - 1][j + 2] == 'N' || board[i + 1][j - 2] == 'N' || board[i - 1][j - 2] == 'N')
            return 1;
        // check down the king
        for (i = i_check + 1, j = j_check; i < 8; i++)
            if (board[i][j] == 'R' || board[i][j] == 'Q')
                return 1;
            else if (isalpha(board[i][j]))
                break;
        // check up the king
        for (i = i_check - 1, j = j_check; i >= 0; i--)
            if (board[i][j] == 'R' || board[i][j] == 'Q')
                return 1;
            else if (isalpha(board[i][j]))
                break;
        // check right the king
        for (i = i_check, j = j_check + 1; j < 8; j++)
            if (board[i][j] == 'R' || board[i][j] == 'Q')
                return 1;
            else if (isalpha(board[i][j]))
                break;
        // check left the king
        for (i = i_check, j = j_check - 1; j >= 0; j--)
            if (board[i][j] == 'R' || board[i][j] == 'Q')
                return 1;
            else if (isalpha(board[i][j]))
                break;
        // check upper right the king
        for (i = i_check - 1, j = j_check + 1; i >= 0 && j < 8; i--, j++)
            if (board[i][j] == 'B' || board[i][j] == 'Q')
                return 1;
            else if (isalpha(board[i][j]))
                break;
        // check lower left the king
        for (i = i_check + 1, j = j_check - 1; i < 8 && j >= 0; i++, j--)
            if (board[i][j] == 'B' || board[i][j] == 'Q')
                return 1;
            else if (isalpha(board[i][j]))
                break;
        // check upper left the king
        for (i = i_check - 1, j = j_check - 1; i >= 0 && j >= 0; i--, j--)
            if (board[i][j] == 'B' || board[i][j] == 'Q')
                return 1;
            else if (isalpha(board[i][j]))
                break;
        // check lower right the king
        for (i = i_check + 1, j = j_check + 1; i < 8 && j < 8; i++, j++)
            if (board[i][j] == 'B' || board[i][j] == 'Q')
                return 1;
            else if (isalpha(board[i][j]))
                break;
    }
    // repeat for player 2
    else
    {
        if (board[i + 1][j + 1] == 'p' || board[i + 1][j - 1] == 'p')
            return 1;
        if (board[i - 2][j + 1] == 'n' || board[i - 2][j - 1] == 'n' || board[i + 2][j + 1] == 'n' || board[i + 2][j - 1] == 'n' ||
            board[i + 1][j + 2] == 'n' || board[i - 1][j + 2] == 'n' || board[i + 1][j - 2] == 'n' || board[i - 1][j - 2] == 'n')
            return 1;
        for (i = i_check + 1, j = j_check; i < 8; i++)
            if (board[i][j] == 'r' || board[i][j] == 'q')
                return 1;
            else if (isalpha(board[i][j]))
                break;
        for (i = i_check - 1, j = j_check; i >= 0; i--)
            if (board[i][j] == 'r' || board[i][j] == 'q')
                return 1;
            else if (isalpha(board[i][j]))
                break;
        for (i = i_check, j = j_check + 1; j < 8; j++)
            if (board[i][j] == 'r' || board[i][j] == 'q')
                return 1;
            else if (isalpha(board[i][j]))
                break;
        for (i = i_check, j = j_check - 1; j >= 0; j--)
            if (board[i][j] == 'r' || board[i][j] == 'q')
                return 1;
            else if (isalpha(board[i][j]))
                break;
        for (i = i_check - 1, j = j_check + 1; i >= 0 && j < 8; i--, j++)
            if (board[i][j] == 'b' || board[i][j] == 'q')
                return 1;
            else if (isalpha(board[i][j]))
                break;
        for (i = i_check + 1, j = j_check - 1; i < 8 && j >= 0; i++, j--)
            if (board[i][j] == 'b' || board[i][j] == 'q')
                return 1;
            else if (isalpha(board[i][j]))
                break;
        for (i = i_check - 1, j = j_check - 1; i >= 0 && j >= 0; i--, j--)
            if (board[i][j] == 'b' || board[i][j] == 'q')
                return 1;
            else if (isalpha(board[i][j]))
                break;
        for (i = i_check + 1, j = j_check + 1; i < 8 && j < 8; i++, j++)
            if (board[i][j] == 'b' || board[i][j] == 'q')
                return 1;
            else if (isalpha(board[i][j]))
                break;
    }
    return 0;
}

void swap(int i1, int j1, int i2, int j2)
{
    // swap 2 pieces
    char tmp = board[i2][j2];
    board[i2][j2] = board[i1][j1];
    board[i1][j1] = tmp;
}

int is_checkmate()
{
    // check player 2 king after player 1 move
    if (p_turn % 2 == 0)
    {
        if(king_can_run() || piece_save_king())
            return 0;
    }
    // repeat for player 1 king
    else
    {
        if(king_can_run() || piece_save_king())
            return 0;
    }
    return 1;
}

int piece_save_king()
{
    // check all possible moves for all player 2 pieces which may save his king
    int m, n, p, l;
    char tmp;
    if (p_turn % 2 == 0)
    {
        for (m = 0; m < 8; m++)
            for (n = 0; n < 8; n++)
                if (isupper(board[m][n]) && board[m][n] != 'K')
                    for (p = 0; p < 8; p++)
                        for (l = 0; l < 8; l++)
                            if (!isupper(board[p][l]) && the_move(m, n, p, l) && is_road_empty(m, n, p, l))
                            {
                                tmp = board[p][l];
                                board[p][l] = '\0';
                                swap(m, n, p, l);
                                if (is_king_in_check(p_turn + 1, i_king_p2, j_king_p2))
                                {
                                    swap(m, n, p, l);
                                    board[p][l] = tmp;
                                }
                                else
                                {
                                    swap(m, n, p, l);
                                    board[p][l] = tmp;
                                    return 1;
                                }
                            }
    }
    // repeat for player 1 pieces
    else
    {
        for (m = 0; m < 8; m++)
            for (n = 0; n < 8; n++)
                if (islower(board[m][n]) && board[m][n] != 'k')
                    for (p = 0; p < 8; p++)
                        for (l = 0; l < 8; l++)
                            if (!islower(board[p][l]) && the_move(m, n, p, l) && is_road_empty(m, n, p, l))
                            {
                                tmp = board[p][l];
                                board[p][l] = '\0';
                                swap(m, n, p, l);
                                if (is_king_in_check(p_turn + 1, i_king_p1, j_king_p1))
                                {
                                    swap(m, n, p, l);
                                    board[p][l] = tmp;
                                }
                                else
                                {
                                    swap(m, n, p, l);
                                    board[p][l] = tmp;
                                    return 1;
                                }
                            }
    }
    return 0;
}

int king_can_run()
{
    int o, p;
    // is there safe and empty places around player 2 king ?
    if (p_turn % 2 == 0)
    {
        for (o = i_king_p2 - 1; o < i_king_p2 + 2; o++)
            for (p = j_king_p2 - 1; p < j_king_p2 + 2; p++)
            {
                if (isupper(board[o][p]))
                    continue;
                else if (!is_king_in_check(p_turn + 1, o, p) && o >= 0 && o < 8 && p >= 0 && p < 8)
                    return 1;
            }
    }
    // repeat for player 1 king
    else
    {
        for (o = i_king_p1 - 1; o < i_king_p1 + 2; o++)
            for (p = j_king_p1 - 1; p < j_king_p1 + 2; p++)
            {
                if (islower(board[o][p]))
                    continue;
                else if (!is_king_in_check(p_turn + 1, o, p) && o >= 0 && o < 8 && p >= 0 && p < 8)
                    return 1;
            }
    }
    return 0;
}

void promotion(int i1, int j1, int i2, int j2, char to[])
{
    if (p_turn % 2 == 0)
    {
        if (i1 == 1 && i2 == 0 && board[i2][j2] == 'p')
        {
            if (to[4] == 'Q')
                board[i2][j2] = 'q';
            else if (to[4] == 'N')
                board[i2][j2] = 'n';
            else if (to[4] == 'R')
                board[i2][j2] = 'r';
            else if (to[4] == 'B')
                board[i2][j2] = 'b';
        }
    }
    else
        if (i1 == 6 && i2 == 7 && board[i2][j2] == 'P')
        {
            if (to[4] == 'Q')
                board[i2][j2] = 'Q';
            else if (to[4] == 'N')
                board[i2][j2] = 'N';
            else if (to[4] == 'R')
                board[i2][j2] = 'R';
            else if (to[4] == 'B')
                board[i2][j2] = 'B';
        }
}

void save_places()
{
    cout++;
    // save all board places
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            save[cout][i][j] = board[i][j];
    // save dead pieces of player 1
    for (i = 0; i < 16; i++)
        save_dead1[cout][i] = p1_dead[i];
    // repeat for player 2
    for (i = 0; i < 16; i++)
        save_dead2[cout][i] = p2_dead[i];
}

void undo()
{
    // undo the last move
    cout--;
    p_turn -= 2;
    // put all places last turn places
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            board[i][j] = save[cout][i][j];
    // put dead pieces of player 1
    for(i = 0; i < 16; i++)
        p1_dead[i] = save_dead1[cout][i];
    // repeat for player 2
    for(i = 0; i < 16; i++)
        p2_dead[i] = save_dead2[cout][i];
    // check if the king is in check
    if (p_turn % 2 == 0)
    {
        if(is_king_in_check(p_turn + 1, i_king_p2, j_king_p2))
            king_in_check = 1;
        else
            king_in_check = 0;
    }
    else
    {
        if(is_king_in_check(p_turn + 1, i_king_p1, j_king_p1))
            king_in_check = 1;
        else
            king_in_check = 0;
    }
}

void redo()
{
    // redo the last move with the same algorithm
    cout++;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            board[i][j] = save[cout][i][j];
    for(i = 0; i < 16; i++)
        p1_dead[i] = save_dead1[cout][i];
    for(i = 0; i < 16; i++)
        p2_dead[i] = save_dead2[cout][i];
    // check if the king is in check
    if (p_turn % 2 == 0)
    {
        if(is_king_in_check(p_turn + 1, i_king_p2, j_king_p2))
            king_in_check = 1;
        else
            king_in_check = 0;
    }
    else
    {
        if(is_king_in_check(p_turn + 1, i_king_p1, j_king_p1))
            king_in_check = 1;
        else
            king_in_check = 0;
    }
}

void save_game()
{
    FILE *fptr = fopen("SAVEGAME.txt", "w");
    fputc(cout, fptr);
    fputc(p_turn, fptr);
    fputc(p1_dead_c, fptr);
    fputc(p2_dead_c, fptr);
    fputc(found_dead, fptr);
    fputc(king_in_check, fptr);
    for (k = 0; k < cout + 1; k++)
        for (i = 0; i < 8; i++)
            for (j = 0; j < 8; j++)
                fputc(save[k][i][j], fptr);
    for (k = 0; k < cout + 1; k++)
        for (i = 0; i < 16; i++)
            fputc(save_dead1[k][i], fptr);
    for (k = 0; k < cout + 1; k++)
        for (i = 0; i < 16; i++)
            fputc(save_dead2[k][i], fptr);
    fclose(fptr);
}

int load_game()
{
    FILE *fptr = fopen("SAVEGAME.txt", "r");
    // check the file
    if (fptr == NULL)
        return 0;
    // load variables
    cout = fgetc(fptr);
    p_turn = fgetc(fptr);
    p1_dead_c = fgetc(fptr);
    p2_dead_c = fgetc(fptr);
    found_dead = fgetc(fptr);
    king_in_check = fgetc(fptr);
    // load save
    k = 0;
    while (k != cout + 1)
    {
        for (i = 0; i < 8; i++)
            for (j = 0; j < 8; j++)
                save[k][i][j] = fgetc(fptr);
        k++;
    }
    // load save for player 1 dead pieces
    k = 0;
    while (k != cout + 1)
    {
        for (i = 0; i < 16; i++)
            save_dead1[k][i] = fgetc(fptr);
        k++;
    }
    // repeat for player 2
    k = 0;
    while (k != cout + 1)
    {
        for (i = 0; i < 16; i++)
            save_dead2[k][i] = fgetc(fptr);
        k++;
    }
    fclose(fptr);
    // put all places last turn places
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            board[i][j] = save[cout][i][j];
    // put dead pieces of player 1
    for(i = 0; i < 16; i++)
        p1_dead[i] = save_dead1[cout][i];
    // repeat for player 2
    for(i = 0; i < 16; i++)
        p2_dead[i] = save_dead2[cout][i];
    return 1;
}

