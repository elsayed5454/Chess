#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

char board[8][8] = {{'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}};
char p1_dead[16], p2_dead[16];
int p1_dead_c = 0, p2_dead_c = 0;
char swap;
int i, j, k, p_turn = 0, i_king_p1 = 7, j_king_p1 = 4, i_king_p2 = 0, j_king_p2 = 4;
COORD coord = {0, 0}; // sets coordinates to 0,0

void gotoxy (int x, int y);
void print(char arr[8][8]);
int check_char(char p[]);
void check_move(char arr[8][8]);
void move(char p_move[]);
void pawn(int i1, int j1, int i2, int j2);
void rook(int i1, int j1, int i2, int j2);
void knight(int i1, int j1, int i2, int j2);
void bishop(int i1, int j1, int i2, int j2);
void queen(int i1, int j1, int i2, int j2);
void king(int i1, int j1, int i2, int j2);
int check_road(int i1, int j1, int i2, int j2);
int check_check(int p_turn);


int main()
{
    // initializing board
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
    print(board);
    check_move(board);

    return 0;
}

void gotoxy (int x, int y)
{
    coord.X = x;
    coord.Y = y; // X and Y coordinates
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
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

void check_move(char arr[8][8])
{
    char p1_move[5];
    char p2_move[5];
    printf("\n");
    while (1)
    {
        print(board);
        if (p_turn % 2 == 0)
        {
            if (!check_check(p_turn))
                printf("Your king is in check, choose your move wisely\n");
            do
            {
                printf("Player 1 move: ");
                scanf("%s", p1_move);
            }
            while (!check_char(p1_move));
            move(p1_move);
        }
        else
        {
            if (!check_check(p_turn))
                printf("Your king is in check, choose your move wisely\n");
            do
            {
                printf("Player 2 move: ");
                scanf("%s", p2_move);
            }
            while (!check_char(p2_move));
            move(p2_move);
        }
        p_turn++;
    }
}

void move(char p_move[])
{
    int i_from = -(p_move[1] - '1') + 7, i_to = -(p_move[3] - '1') + 7;
    int j_from = p_move[0] - 'A', j_to = p_move[2] - 'A';

    if (board[i_from][j_from] == 'p' || board[i_from][j_from] == 'P')
        pawn(i_from, j_from, i_to, j_to);

    else if (board[i_from][j_from] == 'r' || board[i_from][j_from] == 'R')
        rook(i_from, j_from, i_to, j_to);

    else if (board[i_from][j_from] == 'n' || board[i_from][j_from] == 'N')
        knight(i_from, j_from, i_to, j_to);

    else if (board[i_from][j_from] == 'b' || board[i_from][j_from] == 'B')
        bishop(i_from, j_from, i_to, j_to);

    else if (board[i_from][j_from] == 'q' || board[i_from][j_from] == 'Q')
        queen(i_from, j_from, i_to, j_to);

    else if (board[i_from][j_from] == 'k' || board[i_from][j_from] == 'K')
        king(i_from, j_from, i_to, j_to);
}

void pawn(int i1, int j1, int i2, int j2)
{
    // player 1 pawn
    if (p_turn % 2 == 0 && board[i1][j1] == 'p')
    {
        // pawn is allowed to move two squares in the first move
        if (i1 == 6 && (i1 - i2 == 2 || i1 - i2 == 1) && j1 == j2 && !isalpha(board[i2][j2]))
        {
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        // normal pawn move
        else if (!isalpha(board[i2][j2]) && i1 - i2 == 1 && j1 == j2)
        {
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        // in case of capturing
        else if (isupper(board[i2][j2]) && i1 - i2 == 1 && abs(j1 - j2) == 1)
        {
            p2_dead[p2_dead_c++] = board[i2][j2];
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        else
            p_turn--;
    }
    // player 2 pawn
    else if (p_turn % 2 == 1 && board[i1][j1] == 'P')
    {
        // pawn is allowed to move two squares in the first move
        if (i1 == 1 && (i2 - i1 == 2 || i2 - i1 == 1) && j1 == j2 && !isalpha(board[i2][j2]))
        {
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        // normal pawn move
        else if (!isalpha(board[i2][j2]) && i2 - i1 == 1 && j1 == j2)
        {
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        // in case of capturing
        else if (islower(board[i2][j2]) && i2 - i1 == 1 && abs(j1 - j2) == 1)
        {
            p1_dead[p1_dead_c++] = board[i2][j2];
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        else
            p_turn--;
    }
    else
        p_turn--;
}

void rook(int i1, int j1, int i2, int j2)
{
    // player 1 rook
    if (p_turn % 2 == 0 && board[i1][j1] == 'r')
    {
        // normal rook move
        if ((i1 == i2 || j1 == j2) && !isalpha(board[i2][j2]) && check_road(i1, j1, i2, j2))
        {
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        // in case of capturing
        else if ((i1 == i2 || j1 == j2) && isupper(board[i2][j2]) && check_road(i1, j1, i2, j2))
        {
            p2_dead[p2_dead_c++] = board[i2][j2];
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        else
            p_turn--;
    }
    // player 2 rook
    else if (p_turn % 2 == 1 && board[i1][j1] == 'R')
    {
        // normal rook move
        if ((i1 == i2 || j1 == j2) && !isalpha(board[i2][j2]) && check_road(i1, j1, i2, j2))
        {
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        // in case of capturing
        else if ((i1 == i2 || j1 == j2) && islower(board[i2][j2]) && check_road(i1, j1, i2, j2))
        {
            p1_dead[p1_dead_c++] = board[i2][j2];
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        else
            p_turn--;
    }
    else
        p_turn--;
}

void knight(int i1, int j1, int i2, int j2)
{
    // player 1 knight
    if (p_turn % 2 == 0 && board[i1][j1] == 'n')
    {
        // normal knight move
        if (((abs(i1 - i2) == 1 && abs(j1 - j2) == 2) || (abs(i1 - i2) == 2 && abs(j1 - j2) == 1)) && !isalpha(board[i2][j2]))
        {
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        // in case of capturing
        else if (((abs(i1 - i2) == 1 && abs(j1 - j2) == 2) || (abs(i1 - i2) == 2 && abs(j1 - j2) == 1)) && isupper(board[i2][j2]))
        {
            p2_dead[p2_dead_c++] = board[i2][j2];
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        else
            p_turn--;
    }
    // player 2 knight
    else if (p_turn % 2 == 1 && board[i1][j1] == 'N')
    {
        // normal knight move
        if (((abs(i1 - i2) == 1 && abs(j1 - j2) == 2) || (abs(i1 - i2) == 2 && abs(j1 - j2) == 1)) && !isalpha(board[i2][j2]))
        {
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        // in case of capturing
        else if (((abs(i1 - i2) == 1 && abs(j1 - j2) == 2) || (abs(i1 - i2) == 2 && abs(j1 - j2) == 1)) && islower(board[i2][j2]))
        {
            p1_dead[p1_dead_c++] = board[i2][j2];
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        else
            p_turn--;
    }
    else
        p_turn--;
}

void bishop(int i1, int j1, int i2, int j2)
{
    // player 1 bishop
    if (p_turn % 2 == 0 && board[i1][j1] == 'b')
    {
        // normal bishop move
        if (abs(i1 - i2) == abs(j1 - j2) && !isalpha(board[i2][j2]) && check_road(i1, j1, i2, j2))
        {
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        // in case of capturing
        else if (abs(i1 - i2) == abs(j1 - j2) && isupper(board[i2][j2]) && check_road(i1, j1, i2, j2))
        {
            p2_dead[p2_dead_c++] = board[i2][j2];
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        else
            p_turn--;
    }
    // player 2 bishop
    else if (p_turn % 2 == 1 && board[i1][j1] == 'B')
    {
        // normal bishop move
        if (abs(i1 - i2) == abs(j1 - j2) && !isalpha(board[i2][j2]) && check_road(i1, j1, i2, j2))
        {
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        // in case of capturing
        else if (abs(i1 - i2) == abs(j1 - j2) && islower(board[i2][j2]) && check_road(i1, j1, i2, j2))
        {
            p1_dead[p1_dead_c++] = board[i2][j2];
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        else
            p_turn--;
    }
    else
        p_turn--;
}

void queen(int i1, int j1, int i2, int j2)
{
    // player 1 queen
    if (p_turn % 2 == 0 && board[i1][j1] == 'q')
    {
        // normal queen move
        if (((i1 == i2 || j1 == j2) || abs(i1 - i2) == abs(j1 - j2)) && !isalpha(board[i2][j2]) && check_road(i1, j1, i2, j2))
        {
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        // in case of capturing
        else if (((i1 == i2 || j1 == j2) || abs(i1 - i2) == abs(j1 - j2)) && isupper(board[i2][j2]) && check_road(i1, j1, i2, j2))
        {
            p2_dead[p2_dead_c++] = board[i2][j2];
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        else
            p_turn--;
    }
    // player 2 queen
    else if (p_turn % 2 == 1 && board[i1][j1] == 'Q')
    {
        // normal queen move
        if (((i1 == i2 || j1 == j2) || abs(i1 - i2) == abs(j1 - j2)) && !isalpha(board[i2][j2]) && check_road(i1, j1, i2, j2))
        {
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        // in case of capturing
        else if (((i1 == i2 || j1 == j2) || abs(i1 - i2) == abs(j1 - j2)) && islower(board[i2][j2]) && check_road(i1, j1, i2, j2))
        {
            p1_dead[p1_dead_c++] = board[i2][j2];
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        else
            p_turn--;
    }
    else
        p_turn--;
}

void king(int i1, int j1, int i2, int j2)
{
    // player 1 king
    if (p_turn % 2 == 0 && board[i1][j1] == 'k')
    {
        // normal king move
        if ((abs(i1 - i2) == 1 || abs(j1 - j2) == 1) && !isalpha(board[i2][j2]))
        {
            i_king_p1 = i2, j_king_p1 = j2;
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        // in case of capturing
        else if ((abs(i1 - i2) == 1 || abs(j1 - j2) == 1) && isupper(board[i2][j2]))
        {
            i_king_p1 = i2, j_king_p1 = j2;
            p2_dead[p2_dead_c++] = board[i2][j2];
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        else
            p_turn--;
    }
    // player 2 king
    else if (p_turn % 2 == 1 && board[i1][j1] == 'K')
    {
        // normal king move
        if ((abs(i1 - i2) == 1 || abs(j1 - j2) == 1) && !isalpha(board[i2][j2]))
        {
            i_king_p2 = i2, j_king_p2 = j2;
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        // in case of capturing
        else if ((abs(i1 - i2) == 1 || abs(j1 - j2) == 1) && islower(board[i2][j2]))
        {
            i_king_p2 = i2, j_king_p2 = j2;
            p1_dead[p1_dead_c++] = board[i2][j2];
            board[i2][j2] = board[i1][j1];
            if ((i1 + j1) % 2 != 0)
                board[i1][j1] = ' ';
            else
                board[i1][j1] = '\xdb';
        }
        else
            p_turn--;
    }
    else
        p_turn--;
}

int check_road(int i1, int j1, int i2, int j2)
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

int check_check(int p_turn)
{
    // check player 1 king in check
    if (p_turn % 2 == 0)
    {
        i = i_king_p1, j = j_king_p1;
        // check for pawns in front of the king
        if (board[i - 1][j + 1] == 'P' || board[i - 1][j - 1] == 'P')
            return 0;
        // check for knights around the king

        // check down the king
        for (i = i_king_p1 + 1, j = j_king_p1; i < 8; i++)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'R' || board[i][j] == 'Q')
                return 0;
        // check up the king
        for (i = i_king_p1 - 1, j = j_king_p1; i >= 0; i--)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'R' || board[i][j] == 'Q')
                return 0;
        // check right the king
        for (i = i_king_p1, j = j_king_p1 + 1; j < 8; j++)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'R' || board[i][j] == 'Q')
                return 0;
        // check left the king
        for (i = i_king_p1, j = j_king_p1 - 1; j >= 0; j--)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'R' || board[i][j] == 'Q')
                return 0;
        // check upper right the king
        for (i = i_king_p1 - 1, j = j_king_p1 + 1; i >= 0 && j < 8; i--, j++)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'B' || board[i][j] == 'Q')
                return 0;
        // check lower left the king
        for (i = i_king_p1 + 1, j = j_king_p1 - 1; i < 8 && j >= 0; i++, j--)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'B' || board[i][j] == 'Q')
                return 0;
        // check upper left the king
        for (i = i_king_p1 - 1, j = j_king_p1 - 1; i >= 0 && j >= 0; i--, j--)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'B' || board[i][j] == 'Q')
                return 0;
        // check lower right the king
        for (i = i_king_p1 + 1, j = j_king_p1 + 1; i < 8 && j < 8; i++, j++)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'B' || board[i][j] == 'Q')
                return 0;
        return 1;
    }
    // check player 2 king in check
    else if (p_turn % 2 == 1)
    {
        i = i_king_p2, j = j_king_p2;
        // check for pawns in front of the king
        if (board[i + 1][j + 1] == 'p' || board[i + 1][j - 1] == 'p')
            return 0;
        // check down the king
        for (i = i_king_p2 + 1, j = j_king_p2; i < 8; i++)
            if (isupper(board[i][j]))
                break;
            else if (board[i][j] == 'r' || board[i][j] == 'q')
                return 0;
        // check up the king
        for (i = i_king_p2 - 1, j = j_king_p2; i >= 0; i--)
            if (isupper(board[i][j]))
                break;
            else if (board[i][j] == 'r' || board[i][j] == 'q')
                return 0;
        // check right the king
        for (i = i_king_p2, j = j_king_p2 + 1; j < 8; j++)
            if (isupper(board[i][j]))
                break;
            else if (board[i][j] == 'r' || board[i][j] == 'q')
                return 0;
        // check left the king
        for (i = i_king_p2, j = j_king_p2 - 1; j >= 0; j--)
            if (isupper(board[i][j]))
                break;
            else if (board[i][j] == 'r' || board[i][j] == 'q')
                return 0;
        // check upper right the king
        for (i = i_king_p2 - 1, j = j_king_p2 + 1; i >= 0 && j < 8; i--, j++)
            if (isupper(board[i][j]))
                break;
            else if (board[i][j] == 'b' || board[i][j] == 'q')
                return 0;
        // check lower left the king
        for (i = i_king_p2 + 1, j = j_king_p2 - 1; i < 8 && j >= 0; i++, j--)
            if (isupper(board[i][j]))
                break;
            else if (board[i][j] == 'b' || board[i][j] == 'q')
                return 0;
        // check upper left the king
        for (i = i_king_p2 - 1, j = j_king_p2 - 1; i >= 0 && j >= 0; i--, j--)
            if (islower(board[i][j]))
                break;
            else if (board[i][j] == 'b' || board[i][j] == 'q')
                return 0;
        // check lower right the king
        for (i = i_king_p2 + 1, j = j_king_p2 + 1; i < 8 && j < 8; i++, j++)
            if (isupper(board[i][j]))
                break;
            else if (board[i][j] == 'b' || board[i][j] == 'q')
                return 0;
        return 1;
    }
    return 1;
}
