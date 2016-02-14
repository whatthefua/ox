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

char info[1000],inp[1000],out[1000];
vector<vector<vector<db> > > W;
vector<vector<db> > Wb;
vector<vector<db> > nodes;
vector<int> nodesN;

db sigmoid(db x)
{
    return (db)1 / ((db)1 + exp(-x));
}

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        return 0;
    }

    FILE *fp1,*fp2;
    int layer,i,j,k,n;
    db tmp;

    strcpy(info,argv[1]);
    strcpy(inp,argv[2]);
    strcpy(out,argv[3]);

    fp1 = fopen(info,"r");

    fscanf(fp1,"%d",&layer);

    nodesN.resize(layer);
    W.resize(layer);
    Wb.resize(layer);
    nodes.resize(layer);

    for(i = 0; i < layer; i++)
    {
        fscanf(fp1,"%d",&nodesN[i]);

        nodes[i].resize(nodesN[i]);
    }

    for(i = 1; i < layer; i++)
    {
        Wb[i].resize(nodesN[i]);
        W[i].resize(nodesN[i - 1]);

        for(j = 0; j < nodesN[i - 1]; j++)
        {
            W[i][j].resize(nodesN[i]);
        }
    }

    for(i = 1; i < layer; i++)
    {
        for(j = 0; j < nodesN[i - 1]; j++)
        {
            for(k = 0; k < nodesN[i]; k++)
            {
                fscanf(fp1,"%lf",&W[i][j][k]);
            }
        }

        for(j = 0; j < nodesN[i]; j++)
        {
            fscanf(fp1,"%lf",&Wb[i][j]);
        }
    }

    fclose(fp1);


    fp1 = fopen(inp,"r");
    fp2 = fopen(out,"w");

    fscanf(fp1,"%d",&n);
    fprintf(fp2,"%d\n",n);

    for(; n > 0; n--)
    {
        for(i = 0; i < nodesN[0]; i++)
        {
            fscanf(fp1,"%lf",&nodes[0][i]);
        }

        for(i = 1; i < layer; i++)
        {
            for(k = 0; k < nodesN[i]; k++)
            {
                tmp = Wb[i][k];

                for(j = 0; j < nodesN[i - 1]; j++)
                {
                    tmp += nodes[i - 1][j] * W[i][j][k];
                }

                nodes[i][k] = sigmoid(tmp);
            }
        }

        for(i = 0; i < nodesN.back(); i++)
        {
            fprintf(fp2,"%lf ",nodes.back()[i]);
        }

        fprintf(fp2,"\n");
    }

    fclose(fp1);
    fclose(fp2);
}
