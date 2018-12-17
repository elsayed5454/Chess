#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

char board[8][8] = {{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}};
char p1_dead[16], p2_dead[16];
int p1_dead_c = 0, p2_dead_c = 0;
int i, j, k;
int end_game = 0, valid_move, p_turn = 0, king_in_check = 0, i_king_p1 = 7, j_king_p1 = 4, i_king_p2 = 0, j_king_p2 = 4, i_threat, j_threat;
COORD coord = {0, 0}; // sets coordinates to 0,0

void gotoxy (int x, int y);
void initialize_board();
void print(char arr[8][8]);
int is_move_valid(char p[]);
int check_char(char p[]);
int the_move(int i1, int j1, int i2, int j2);

int check_road(int i1, int j1, int i2, int j2);
int check_square(int i_check, int j_check);
int save_king(int king_in_safe);

void gotoxy (int x, int y)
{
    coord.X = x;
    coord.Y = y; // X and Y coordinates
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main()
{
    initialize_board();
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
            }
            while (!valid_move);
        }
        p_turn++;
    }
    return 0;
}

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
    if (!check_char(p))
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
    // make the move
    // in case of capturing
    if (isalpha(board[i2][j2]))
    {
        if (p_turn % 2 == 0)
            p2_dead[p2_dead_c++] = board[i2][j2];
        else
            p1_dead[p1_dead_c++] = board[i2][j2];
    }
    board[i2][j2] = board[i1][j1];
    if ((i1 + j1) % 2 != 0)
        board[i1][j1] = ' ';
    else
        board[i1][j1] = '\xdb';



    valid_move = 1;
    return 0;
}

void print(char arr[8][8])
{
    system("cls");
    printf("     A    B    C    D    E    F    G    H  \n");
    printf("   -----------------------------------------\n");
    for (i = 0; i < 8; i++)
        {
            printf(" %d  ",8 - i);
            for(j = 0; j < 8; j++)
                if((i + j) % 2 != 0)
                    printf("  %c  ",arr[i][j]);
                else
                    printf("\xdb\xdb%c\xdb\xdb",arr[i][j]);
            if (i == 0)
            {
                printf("          Player 2 Dead Pieces: ");
                for (k = 0;k < 8; k++)
                    printf("%c ",p2_dead[k]);
            }
            else if(i == 7)
            {
                printf("          Player 1 Dead Pieces: ");
                for (k = 0; k < 8; k++)
                    printf("%c ",p1_dead[k]);
         }
        printf("\n");
    }
}

int check_char(char p[])
{
    if (strlen(p) != 4)
        return 0;
    else if (p[0] < 'A' || p[0] > 'H' || p[2] < 'A' || p[2] > 'H')
        return 0;
    else if (p[1] < '1' || p[1] > '8' || p[3] < '1' || p[3] > '8')
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
            // pawn is allowed to move two squares in the first move
            if (i1 == 6 && (i1 - i2 == 2 || i1 - i2 == 1) && j1 == j2 && !isalpha(board[i2][j2]))
                return 1;
            // normal pawn move
            else if (i1 - i2 == 1 && j1 == j2 && !isalpha(board[i2][j2]))
                return 1;
            // in case of capturing
            else if (isupper(board[i2][j2]) && i1 - i2 == 1 && abs(j1 - j2) == 1)
                return 1;
            else
            {
                return 0;
            }
        // player 2 pawn
        else if (board[i1][j1] == 'P')
        {
            // pawn is allowed to move two squares in the first move
            if (i1 == 1 && (i2 - i1 == 2 || i2 - i1 == 1) && j1 == j2)
                return 1;
            // normal pawn move
            else if (i2 - i1 == 1 && j1 == j2)
                return 1;
            // in case of capturing
            else if (islower(board[i2][j2]) && i2 - i1 == 1 && abs(j1 - j2) == 1)
                return 1;
            else
                return 0;
        }
    }
    else if (board[i1][j1] == 'r' || board[i1][j1] == 'R')
    {
        if ((i1 == i2 || j1 == j2))
            return 1;
        else
            return 0;
    }
    else if (board[i1][j1] == 'n' || board[i1][j1] == 'N')
    {
        if (((abs(i1 - i2) == 1 && abs(j1 - j2) == 2) || (abs(i1 - i2) == 2 && abs(j1 - j2) == 1)))
            return 1;
        else
            return 0;
    }
    else if (board[i1][j1] == 'b' || board[i1][j1] == 'B')
    {
        if (abs(i1 - i2) == abs(j1 - j2))
            return 1;
        else
            return 0;
    }
    else if (board[i1][j1] == 'q' || board[i1][j1] == 'Q')
    {
        if (((i1 == i2 || j1 == j2) || abs(i1 - i2) == abs(j1 - j2)))
            return 1;
        else
            return 0;
    }
    else if (board[i1][j1] == 'k' || board[i1][j1] == 'K')
    {
        if ((abs(i1 - i2) == 1 && abs(j1 - j2) == 1) || (i1 == i2 && abs(j1 - j2) == 1) || (abs(i1 - i2) == 1 && j1 == j2))
            return 1;
        else
            return 0;
    }
    return 0;
}

/*int check_road(int i1, int j1, int i2, int j2)
{
    // n is the no of squares to check, i is the initial row to check, j is the initial column to check
    int n;
    // check the road for rooks
    if (board[i1][j1] == 'r' || board[i1][j1] == 'R')
    {
        // moving horizontally
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
        // moving vertically
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
        // moving horizontally
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
        // moving vertically
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

int check_square(int i_check, int j_check)
{
    // check player 1 square
    if (p_turn % 2 == 0)
    {
        i = i_check, j = j_check;
        // check for pawns in front of the king
        if (board[i - 1][j + 1] == 'P')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        else if (board[i - 1][j - 1] == 'P')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        // check for knights around the king
        if (board[i - 2][j + 1] == 'N')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        else if (board[i - 2][j - 1] == 'N')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        else if (board[i + 2][j + 1] == 'N')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        else if (board[i + 2][j - 1] == 'N')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        else if (board[i + 1][j + 2] == 'N')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        else if (board[i - 1][j + 2] == 'N')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        else if (board[i + 1][j - 2] == 'N')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        else if (board[i - 1][j - 2] == 'N')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        // check down the king
        for (i = i_check + 1, j = j_check; i < 8; i++)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'R' || board[i][j] == 'Q')
            {
                i_threat = i, j_threat = j;
                return 0;
            }
        // check up the king
        for (i = i_check - 1, j = j_check; i >= 0; i--)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'R' || board[i][j] == 'Q')
            {
                i_threat = i, j_threat = j;
                return 0;
            }
        // check right the king
        for (i = i_check, j = j_check + 1; j < 8; j++)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'R' || board[i][j] == 'Q')
            {
                i_threat = i, j_threat = j;
                return 0;
            }
        // check left the king
        for (i = i_check, j = j_check - 1; j >= 0; j--)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'R' || board[i][j] == 'Q')
            {
                i_threat = i, j_threat = j;
                return 0;
            }
        // check upper right the king
        for (i = i_check - 1, j = j_check + 1; i >= 0 && j < 8; i--, j++)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'B' || board[i][j] == 'Q')
            {
                i_threat = i, j_threat = j;
                return 0;
            }
        // check lower left the king
        for (i = i_check + 1, j = j_check - 1; i < 8 && j >= 0; i++, j--)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'B' || board[i][j] == 'Q')
            {
                i_threat = i, j_threat = j;
                return 0;
            }
        // check upper left the king
        for (i = i_check - 1, j = j_check - 1; i >= 0 && j >= 0; i--, j--)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'B' || board[i][j] == 'Q')
            {
                i_threat = i, j_threat = j;
                return 0;
            }
        // check lower right the king
        for (i = i_check + 1, j = j_check + 1; i < 8 && j < 8; i++, j++)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'B' || board[i][j] == 'Q')
            {
                i_threat = i, j_threat = j;
                return 0;
            }
        return 1;
    }
    // check player 2 king in check
    else if (p_turn % 2 == 1)
    {
        i = i_check, j = j_check;
        // check for pawns in front of the king
        if (board[i - 1][j + 1] == 'p')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        else if (board[i - 1][j - 1] == 'p')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        // check for knights around the king
        if (board[i - 2][j + 1] == 'n')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        else if (board[i - 2][j - 1] == 'n')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        else if (board[i + 2][j + 1] == 'n')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        else if (board[i + 2][j - 1] == 'n')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        else if (board[i + 1][j + 2] == 'n')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        else if (board[i - 1][j + 2] == 'n')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        else if (board[i + 1][j - 2] == 'n')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        else if (board[i - 1][j - 2] == 'n')
        {
            i_threat = i, j_threat = j;
            return 0;
        }
        // check down the king
        for (i = i_check + 1, j = j_check; i < 8; i++)
            if (isupper(board[i][j]))
                break;
            else if (board[i][j] == 'r' || board[i][j] == 'q')
            {
                i_threat = i, j_threat = j;
                return 0;
            }
        // check up the king
        for (i = i_check - 1, j = j_check; i >= 0; i--)
            if (isupper(board[i][j]))
                break;
            else if (board[i][j] == 'r' || board[i][j] == 'q')
            {
                i_threat = i, j_threat = j;
                return 0;
            }
        // check right the king
        for (i = i_check, j = j_check + 1; j < 8; j++)
            if (isupper(board[i][j]))
                break;
            else if (board[i][j] == 'r' || board[i][j] == 'q')
            {
                i_threat = i, j_threat = j;
                return 0;
            }
        // check left the king
        for (i = i_check, j = j_check - 1; j >= 0; j--)
            if (isupper(board[i][j]))
                break;
            else if (board[i][j] == 'r' || board[i][j] == 'q')
            {
                i_threat = i, j_threat = j;
                return 0;
            }
        // check upper right the king
        for (i = i_check - 1, j = j_check + 1; i >= 0 && j < 8; i--, j++)
            if (isupper(board[i][j]))
                break;
            else if (board[i][j] == 'b' || board[i][j] == 'q')
            {
                i_threat = i, j_threat = j;
                return 0;
            }
        // check lower left the king
        for (i = i_check + 1, j = j_check - 1; i < 8 && j >= 0; i++, j--)
            if (isupper(board[i][j]))
                break;
            else if (board[i][j] == 'b' || board[i][j] == 'q')
            {
                i_threat = i, j_threat = j;
                return 0;
            }
        // check upper left the king
        for (i = i_check - 1, j = j_check - 1; i >= 0 && j >= 0; i--, j--)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'b' || board[i][j] == 'q')
            {
                i_threat = i, j_threat = j;
                return 0;
            }
        // check lower right the king
        for (i = i_check + 1, j = j_check + 1; i < 8 && j < 8; i++, j++)
            if (isupper(board[i][j]))
                break;
            else if (board[i][j] == 'b' || board[i][j] == 'q')
            {
                i_threat = i, j_threat = j;
                return 0;
            }
        return 1;
    }
    return 1;
}

int save_king(int king_in_safe, int i1, int j1)
{
    if (king_in_safe)
        return 1;
    // n is the no of squares to check, i is the initial row to check, j is the initial column to check
    int n;
    if (p_turn % 2 == 0)
        i2 = i_king_p1, j2 = j_king_p1;
    else
        i2 = i_king_p2, j2 = j_king_p2;
    // check the road for rooks
    if (board[i_threat][j_threat] == 'r' || board[i_threat][j_threat] == 'R')
    {
        // moving horizontally
        if (j_threat == j2)
        {
            j = j_threat;
            if (i_threat > i2)
                i = i2 + 1;
            else
                i = i_threat + 1;
            n = abs(i_threat - i2) + i - 1;
            for (; i < n; i++)
                if (isalpha(board[i][j]))
                    return 0;
                return 1;
        }
        // moving vertically
        else if (i_threat == i2)
        {
            i = i_threat;
            if (j_threat > j2)
            {
                j = j2 + 1;
                n = abs(j_threat - j2) + j;
            }
            else
            {
                j = j_threat;
                n = abs(j_threat - j2) + j - 1;
            }
            for (; j < n; j++)
                if (!which_piece(i1, i2, i, j))
                    continue;
                else
                    return which_piece(i1, i2, i, j);
        }
    }
    // check the road for bishops
    else if (board[i_threat][j_threat] == 'b' || board[i_threat][j_threat] == 'B')
    {
        // from upper left to lower right
        if((i2 > i_threat && j2 > j_threat) || (i2 < i_threat && j2 < j_threat))
        {
            if (i_threat > i2)
            {
                i = i2 + 1;
                j = j2 + 1;
            }
            else
            {
                i = i_threat + 1;
                j = j_threat + 1;
            }
            n = abs(i2 - i_threat) + i - 1;
            for(; i < n; i++, j++)
                if(isalpha(board[i][j]))
                    return 0;
                return 1;
        }
        // from upper right to lower left
        else if ((i_threat > i2 && j_threat < j2) || (i_threat < i2 && j_threat > j2))
            {
                if (i_threat > i2)
                {
                    i = i2 + 1;
                    j = j2 - 1;
                }
                else
                {
                    i = i_threat + 1;
                    j = j_threat - 1;
                }
                n = abs(i2 - i_threat) + i - 1;
                for(; i < n; i++, j--)
                    if(isalpha(board[i][j]))
                        return 0;
                    return 1;
            }
    }
    // check the road for queens
    else if (board[i_threat][j_threat] == 'q' || board[i_threat][j_threat] == 'Q')
    {
        // moving horizontally
        if (j_threat == j2)
        {
            j = j_threat;
            if (i_threat > i2)
                i = i2 + 1;
            else
                i = i_threat + 1;
            n = abs(i_threat - i2) + i - 1;
            for (; i < n; i++)
                if (isalpha(board[i][j]))
                    return 0;
                return 1;
        }
        // moving vertically
        else if (i_threat == i2)
        {
            i = i_threat;
            if (j_threat > j2)
                j = j2 + 1;
            else
                j = j_threat + 1;
            n = abs(j_threat - j2) + j - 1;
            for (; j < n; j++)
                if (isalpha(board[i][j]))
                    return 0;
                return 1;
        }
        // from upper left to lower right
        else if((i2 > i_threat && j2 > j_threat) || (i2 < i_threat && j2 < j_threat))
        {
            if (i_threat > i2)
            {
                i = i2 + 1;
                j = j2 + 1;
            }
            else
            {
                i = i_threat + 1;
                j = j_threat + 1;
            }
            n = abs(i2 - i_threat) + i - 1;
            for(; i < n; i++, j++)
                if(isalpha(board[i][j]))
                    return 0;
                return 1;
        }
        // from upper right to lower left
        else if((i_threat > i2 && j_threat < j2) || (i_threat < i2 && j_threat > j2))
            {
                if (i_threat > i2)
                {
                    i = i2 + 1;
                    j = j2 - 1;
                }
                else
                {
                    i = i_threat + 1;
                    j = j_threat - 1;
                }
                n = abs(i2 - i_threat) + i - 1;
                for(; i < n; i++, j--)
                    if(isalpha(board[i][j]))
                        return 0;
                    return 1;
            }
    }
    return 1;
}

int which_piece(int i1, int j1, int i2, int j2)
{
    if (p_turn % 2 == 0 && board[i1][j1] == 'r')
        if ((i1 == i2 || j1 == j2) && !islower(board[i2][j2]) && check_road(i1, j1, i2, j2))
            return 2;
    else if (p_turn % 2 == 1 && board[i1][j1] == 'R')
        if ((i1 == i2 || j1 == j2) && !isupper(board[i2][j2]) && check_road(i1, j1, i2, j2))
            return 2;
    return 0;
}*/
