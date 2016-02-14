/**************************************************
    WhatTheFua
    Anan Schuett
    arnan_s@msn.com
**************************************************/

#define BK back()
#define BL for(int K = 1; K <= T; K++)
#define F first
#define INF 2147483647LL
#define LNF 8000000000000000000LL
#define P107 1000000007LL
#define P109 1000000009LL
#define PB push_back
#define PF push_front
#define I insert
#define E erase
#define S second
#define SZ size()
#define IT iterator
#define db double
#define ll long long int
#define mp make_pair

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>
using namespace std;

char fileName[300];

typedef struct
{
    int macroBoard[3][3];
    int board[3][3][3][3];

    int timeBank, timePerMove;
    char playerNames[2][10];
    char name[10];
    int id, round, move, time;
} gameinfo;

gameinfo info;

float boardEvalRow(int board[3][3])
{
    float res = (float)0;
    int i;

    for(i = 0; i < 3; i++)
    {
        if(board[i][0] == board[i][1] && board[i][1] == board[i][2])
        {
            if(board[i][0] == info.id)
            {
                return (float)1;
            } else if(board[i][0] == (info.id ^ 3)) {
                return (float)-1;
            }
        }

        if(board[0][i] == board[1][i] && board[1][i] == board[2][i])
        {
            if(board[0][i] == info.id)
            {
                return (float)1;
            } else if(board[0][i] == (info.id ^ 3)) {
                return (float)-1;
            }
        }
    }

    if(board[0][0] == board[1][1] && board[1][1] == board[2][2])
    {
        if(board[0][0] == info.id)
        {
            return (float)1;
        } else if(board[0][0] == (info.id ^ 3)) {
            return (float)-1;
        }
    }

    if(board[2][0] == board[1][1] && board[1][1] == board[0][2])
    {
        if(board[2][0] == info.id)
        {
            return (float)1;
        } else if(board[2][0] == (info.id ^ 3)) {
            return (float)-1;
        }
    }

    return res;
}

float boardEvalRowDraw(int board[3][3])
{
    float res = (float)0;
    int i, j;

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            if(board[i][j] <= 0)
            {
                board[i][j] = info.id;

                if(board[i][0] == board[i][1] && board[i][1] == board[i][2])
                {
                    res += (float)1;
                } else if(board[0][j] == board[1][j] && board[1][j] == board[2][j]) {
                    res += (float)1;
                } else if(i == j && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
                    res += (float)1;
                } else if(2 - i == j && board[2][0] == board[1][1] && board[1][1] == board[0][2]) {
                    res += (float)1;
                }

                board[i][j] ^= 3;

                if(board[i][0] == board[i][1] && board[i][1] == board[i][2])
                {
                    res -= (float)1;
                } else if(board[0][j] == board[1][j] && board[1][j] == board[2][j]) {
                    res -= (float)1;
                } else if(i == j && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
                    res -= (float)1;
                } else if(2 - i == j && board[2][0] == board[1][1] && board[1][1] == board[0][2]) {
                    res -= (float)1;
                }

                board[i][j] = 0;
            }
        }
    }

    return res;
}

float boardEvalCell(int board[3][3])
{
    float res = (float)0;
    int i, j;

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            if(board[i][j] >= 0)
            {
                if(board[i][j] == info.id)
                {
                    res += (float)1;
                } else if(board[i][j] == (info.id ^ 3)) {
                    res -= (float)1;
                }
            }
        }
    }

    return res;
}

int main()
{
    FILE *fp = fopen("train_short_lim.txt","w"),*fpr;
    int x,c;

    fprintf(fp,"%d\n",0);

    info.id = 1;

    for(int I = 1; I <= 235; I++)
    {
        for(int J = 1; J <= 50; J++)
        {
            sprintf(fileName,"gamedata\\game-%d-%d.txt",I,J);
            printf("%s\n",fileName);
            fpr = fopen(fileName,"r");

            if(!fpr)
            {
                continue;
            }

            c = 0;

            while(fscanf(fpr,"%d",&info.macroBoard[0][0]) != EOF)
            {
                if(c > 20)
                {
                    fprintf(fp,"0.05 0.2 ");
                }

                for(int i = 0; i < 3; i++)
                {
                    for(int j = 0; j < 3; j++)
                    {
                        if(i != 0 || j != 0)
                        {
                            fscanf(fpr,"%d",&info.macroBoard[i][j]);
                        }
                    }
                }

                /*for(int i = 0; i < 3; i++)
                {
                    for(int j = 0; j < 3; j++)
                    {
                        if(info.macroBoard[i][j] == 1)
                        {
                            fprintf(fp,"1 ");
                        } else {
                            fprintf(fp,"0 ");
                        }
                    }
                }

                for(int i = 0; i < 3; i++)
                {
                    for(int j = 0; j < 3; j++)
                    {
                        if(info.macroBoard[i][j] == 2)
                        {
                            fprintf(fp,"1 ");
                        } else {
                            fprintf(fp,"0 ");
                        }
                    }
                }*/

                for(int i = 0; i < 3; i++)
                {
                    for(int j = 0; j < 3; j++)
                    {
                        for(int k = 0; k < 3; k++)
                        {
                            for(int l = 0; l < 3; l++)
                            {
                                fscanf(fpr,"%d",&info.board[i][k][j][l]);
                            }
                        }
                    }
                }

                /*for(int i = 0; i < 3; i++)
                {
                    for(int j = 0; j < 3; j++)
                    {
                        for(int k = 0; k < 3; k++)
                        {
                            for(int l = 0; l < 3; l++)
                            {
                                if(info.board[i][k][j][l] == 1)
                                {
                                    fprintf(fp,"1 ");
                                } else {
                                    fprintf(fp,"0 ");
                                }
                            }
                        }
                    }
                }

                for(int i = 0; i < 3; i++)
                {
                    for(int j = 0; j < 3; j++)
                    {
                        for(int k = 0; k < 3; k++)
                        {
                            for(int l = 0; l < 3; l++)
                            {
                                if(info.board[i][k][j][l] == 2)
                                {
                                    fprintf(fp,"1 ");
                                } else {
                                    fprintf(fp,"0 ");
                                }
                            }
                        }
                    }
                }*/

                fscanf(fpr,"%d",&x);

                if(c > 20)
                {
                    fprintf(fp,"%f %f %f ",boardEvalRow(info.macroBoard), boardEvalRowDraw(info.macroBoard), boardEvalCell(info.macroBoard));

                    for(int i = 0; i < 3; i++)
                    {
                        for(int j = 0; j < 3; j++)
                        {
                            fprintf(fp,"%f %f %f ",boardEvalRow(info.board[i][j]), boardEvalRowDraw(info.board[i][j]), boardEvalCell(info.board[i][j]));
                        }
                    }

                    fprintf(fp,"%d\n",x);
                }

                c++;
            }

            fclose(fpr);
        }
    }
}
