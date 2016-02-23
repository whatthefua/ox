#include <stdio.h>
#include <cmath>

#include "NN-mid.h"
#include "NN-late.h"

typedef struct
{
    int macroBoard[3][3];
    int board[3][3][3][3];

    int timeBank, timePerMove;
    char playerNames[2][10];
    char name[10];
    int id, round, move, time;
} gameinfo;

typedef struct
{
    int moveId;
    float eval;
} mmReturn;

gameinfo info;

char str[200];

void getTimeBank();
void getTimePerMove();
void getPlayerNames();
void getName();
void getId();

void updateMove();
void updateRound();
void updateMacroBoard();
void updateBoard();
void actionRequest();
void showStatus();
float sigmoid(float x);
float eval(gameinfo);
gameinfo place(gameinfo, int, int, int, int, int);
mmReturn minimax(gameinfo, int, int);
void play(int t);

float L1[180], L2[70], L3[10], L4[1];

void getTimeBank()
{
    int err;
    err = scanf("%d",&info.timeBank);
}

void getTimePerMove()
{
    int err;
    err = scanf("%d",&info.timePerMove);
}

void getPlayerNames()
{
    int err;
    err = scanf("%s%s",info.playerNames[0], info.playerNames[1]);
}

void getName()
{
    int err;
    err = scanf("%s",info.name);
}

void getId()
{
    int err;
    err = scanf("%d",&info.id);
}

void updateRound()
{
    int err;
    err = scanf("%d",&info.round);
}

void updateMove()
{
    int err;
    err = scanf("%d",&info.move);
}

void updateMacroBoard()
{
    int i, j;
    int c = 0;
    int err;

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            scanf("%d",info.macroBoard[i] + j);
        }
    }
}

void updateBoard()
{
    int i, j, k, l, c = 0;
    int err;

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            for(k = 0; k < 3; k++)
            {
                for(l = 0; l < 3; l++)
                {
                    scanf("%d",info.board[i][j][k] + l);
                }
            }
        }
    }
}

void actionRequest()
{
    int err;
    err = scanf("%d",&info.time);
    play(info.time);
}

void showStatus(gameinfo board)
{
    int i, j, k, l;

    printf("Macroboard\n");

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            printf("%d ",board.macroBoard[i][j]);
        }

        printf("\n");
    }

    printf("board\n");

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            for(k = 0; k < 3; k++)
            {
                for(l = 0; l < 3; l++)
                {
                    printf("%d ",board.board[i][k][j][l]);
                }
            }

            printf("\n");
        }
    }
}

float sigmoid(float x)
{
    return (float)1 / ((float)1 + exp((double)-x));
}

float eval(gameinfo board)
{
    int i, j, k, l, c = 0;

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            L1[c] = (board.macroBoard[i][j] == 1)? 1: 0;
            L1[c + 9] = (board.macroBoard[i][j] == 2)? 1: 0;
            c++;
        }
    }

    c = 18;

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            for(k = 0; k < 3; k++)
            {
                for(l = 0; l < 3; l++)
                {
                    L1[c] = (board.board[i][k][j][l] == 1)? 1: 0;
                    L1[c + 81] = (board.board[i][k][j][l] == 2)? 1: 0;
                    c++;
                }
            }
        }
    }

    if(board.move <= 20)
    {
        for(j = 0; j < 50; j++)
        {
            L2[j] = B2mid[j];

            for(i = 0; i < 180; i++)
            {
                L2[j] += L1[i] * W2mid[i][j];
            }

            L2[j] = sigmoid(L2[j]);
        }

        for(j = 0; j < 10; j++)
        {
            L3[j] = B3mid[j];

            for(i = 0; i < 50; i++)
            {
                L3[j] += L2[i] * W3mid[i][j];
            }

            L3[j] = sigmoid(L3[j]);
        }

        L4[0] = B4mid[0];

        for(i = 0; i < 10; i++)
        {
            L4[0] += L3[i] * W4mid[i][0];
        }
    } else {
        for(j = 0; j < 50; j++)
        {
            L2[j] = B2late[j];

            for(i = 0; i < 180; i++)
            {
                L2[j] += L1[i] * W2late[i][j];
            }

            L2[j] = sigmoid(L2[j]);
        }

        for(j = 0; j < 10; j++)
        {
            L3[j] = B3late[j];

            for(i = 0; i < 50; i++)
            {
                L3[j] += L2[i] * W3late[i][j];
            }

            L3[j] = sigmoid(L3[j]);
        }

        L4[0] = B4late[0];

        for(i = 0; i < 10; i++)
        {
            L4[0] += L3[i] * W4late[i][0];
        }
    }

    if(info.id == 1)
    {
        return L4[0];
    } else {
        return -L4[0];
    }
}

gameinfo place(gameinfo board, int x1, int y1, int x2, int y2, int p)
{
    int i, j;

    board.board[x1][y1][x2][y2] = p;

    if(board.board[x1][y1][x2][0] == board.board[x1][y1][x2][1] && board.board[x1][y1][x2][1] == board.board[x1][y1][x2][2])
    {
        board.macroBoard[x1][y1] = p;
    } else if(board.board[x1][y1][0][y2] == board.board[x1][y1][1][y2] && board.board[x1][y1][1][y2] == board.board[x1][y1][2][y2]) {
        board.macroBoard[x1][y1] = p;
    } else if(x2 == y2 && board.board[x1][y1][0][0] == board.board[x1][y1][1][1] && board.board[x1][y1][1][1] == board.board[x1][y1][2][2]) {
        board.macroBoard[x1][y1] = p;
    } else if(2 - x2 == y2 && board.board[x1][y1][2][0] == board.board[x1][y1][1][1] && board.board[x1][y1][1][1] == board.board[x1][y1][0][2]) {
        board.macroBoard[x1][y1] = p;
    }

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            if(board.macroBoard[i][j] == -1)
            {
                board.macroBoard[i][j] = 0;
            }
        }
    }

    if(board.macroBoard[x2][y2] == 0)
    {
        board.macroBoard[x2][y2] = -1;
    } else {
        for(i = 0; i < 3; i++)
        {
            for(j = 0; j < 3; j++)
            {
                if(board.macroBoard[i][j] == 0)
                {
                    board.macroBoard[i][j] = -1;
                }
            }
        }
    }

    if(board.move % 2 == 1)
    {
        board.round++;
    }

    board.move++;

    return board;
}

int end(gameinfo board)
{
    int i;

    for(i = 0; i < 3; i++)
    {
        if(board.macroBoard[i][0] == board.macroBoard[i][1] && board.macroBoard[i][1] == board.macroBoard[i][2])
        {
            if(board.macroBoard[i][0] == info.id)
            {
                return 1000000;
            } else if(board.macroBoard[i][0] == (info.id ^ 3)) {
                return -1000000;
            }
        }

        if(board.macroBoard[0][i] == board.macroBoard[1][i] && board.macroBoard[1][i] == board.macroBoard[2][i])
        {
            if(board.macroBoard[0][i] == info.id)
            {
                return 1000000;
            } else if(board.macroBoard[0][i] == (info.id ^ 3)) {
                return -1000000;
            }
        }
    }

    if(board.macroBoard[0][0] == board.macroBoard[1][1] && board.macroBoard[1][1] == board.macroBoard[2][2])
    {
        if(board.macroBoard[0][0] == info.id)
        {
            return 1000000;
        } else if(board.macroBoard[0][0] == (info.id ^ 3)) {
            return -1000000;
        }
    }

    if(board.macroBoard[2][0] == board.macroBoard[1][1] && board.macroBoard[1][1] == board.macroBoard[0][2])
    {
        if(board.macroBoard[2][0] == info.id)
        {
            return 1000000;
        } else if(board.macroBoard[2][0] == (info.id ^ 3)) {
            return -1000000;
        }
    }

    return 0;
}

mmReturn minimax(gameinfo board, int rem_depth, int hero)
{
    int i, j, k, l;
    mmReturn res,tmp;
    gameinfo gtmp;

    i = end(board);

    if(i != 0)
    {
        res.eval = (float)i;
        return res;
    }

    if(rem_depth == 0)
    {
        res.eval = eval(board);
        return res;
    }

    if(hero == 1)
    {
        res.eval = (float)-1000000000;

        for(i = 0; i < 3; i++)
        {
            for(j = 0; j < 3; j++)
            {
                if(board.macroBoard[i][j] == -1)
                {
                    for(k = 0; k < 3; k++)
                    {
                        for(l = 0; l < 3; l++)
                        {
                            if(board.board[i][j][k][l] == 0)
                            {
                                gtmp = place(board, i, j, k, l, info.id);

                                tmp = minimax(gtmp, rem_depth - 1, 0);

                                if(tmp.eval > res.eval)
                                {
                                    res.eval = tmp.eval;
                                    res.moveId = (j * 3 + l) * 9 + i * 3 + k;
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        res.eval = (float)1000000000;

        for(i = 0; i < 3; i++)
        {
            for(j = 0; j < 3; j++)
            {
                if(board.macroBoard[i][j] == -1)
                {
                    for(k = 0; k < 3; k++)
                    {
                        for(l = 0; l < 3; l++)
                        {
                            if(board.board[i][j][k][l] == 0)
                            {
                                gtmp = place(board, i, j, k, l, info.id ^ 3);

                                tmp = minimax(gtmp, rem_depth - 1, 1);

                                if(tmp.eval < res.eval)
                                {
                                    res.eval = tmp.eval;
                                    res.moveId = (j * 3 + l) * 9 + i * 3 + k;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return res;
}

void play(int t)
{
    int i, j, k, l;
    mmReturn res;

    res = minimax(info, 4, 1);

    printf("%d %d\n",res.moveId / 9, res.moveId % 9);
    fflush(stdout);
}

int main()
{
    printf("k");
    fflush(stdout);
    getId();
    updateMacroBoard();
    updateBoard();

    play(0);
}

/*
settings timebank 10000
settings time_per_move 500
settings player_names player1,player2
settings your_bot player1
settings your_botid 1
update game round 1
update game move 1
update game field 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
update game macroboard -1,-1,-1,-1,-1,-1,-1,-1,-1
action move 10000
*/
