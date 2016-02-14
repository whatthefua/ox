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

char info[1000],inp[1000];
vector<vector<vector<db> > > W;
vector<vector<vector<db> > > Wupdate;
vector<vector<db> > Wb;
vector<vector<db> > Wbupdate;
vector<vector<db> > nodes;
vector<vector<db> > deriv;
vector<int> nodesN;
vector<db> D;

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

    FILE *fp;
    int layer,i,j,k,n,K;
    db tmp,alpha,momentum;

    sscanf(argv[3],"%d",&K);
    strcpy(inp,argv[2]);
    strcpy(info,argv[1]);

    fp = fopen(info,"r");

    fscanf(fp,"%d",&layer);

    nodesN.resize(layer);
    W.resize(layer);
    Wupdate.resize(layer);
    Wb.resize(layer);
    Wbupdate.resize(layer);
    nodes.resize(layer);
    deriv.resize(layer);

    for(i = 0; i < layer; i++)
    {
        fscanf(fp,"%d",&nodesN[i]);

        nodes[i].resize(nodesN[i]);
    }

    D.resize(nodesN[layer - 1]);

    for(i = 1; i < layer; i++)
    {
        Wbupdate[i].resize(nodesN[i]);
        Wb[i].resize(nodesN[i]);
        Wupdate[i].resize(nodesN[i - 1]);
        W[i].resize(nodesN[i - 1]);
        deriv[i].resize(nodesN[i]);

        for(j = 0; j < nodesN[i - 1]; j++)
        {
            W[i][j].resize(nodesN[i]);
            Wupdate[i][j].resize(nodesN[i]);
        }
    }

    for(i = 1; i < layer; i++)
    {
        for(j = 0; j < nodesN[i - 1]; j++)
        {
            for(k = 0; k < nodesN[i]; k++)
            {
                fscanf(fp,"%lf",&W[i][j][k]);
                Wupdate[i][j][k] = (db)0;
            }
        }

        for(j = 0; j < nodesN[i]; j++)
        {
            fscanf(fp,"%lf",&Wb[i][j]);
            Wbupdate[i][j] = (db)0;
        }
    }

    fclose(fp);

    for(; K > 0; K--)
    {
        fp = fopen(inp,"r");

        fscanf(fp,"%lf%lf",&alpha,&momentum);

        for(i = 0; i < nodesN[0]; i++)
        {
            fscanf(fp,"%lf",&nodes[0][i]);
        }

        for(i = 0; i < nodesN[layer - 1]; i++)
        {
            fscanf(fp,"%lf",&D[i]);
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

        for(i = 0; i < nodesN[layer - 1]; i++)
        {
            deriv[layer - 1][i] = (D[i] - nodes[layer - 1][i]) * ((db)1 - nodes[layer - 1][i]) * nodes[layer - 1][i];
        }

        for(i = layer - 2; i > 0; i--)
        {
            for(j = 0; j < nodesN[i]; j++)
            {
                deriv[i][j] = (db)0;

                for(k = 0; k < nodesN[i + 1]; k++)
                {
                    deriv[i][j] += deriv[i + 1][k] * W[i + 1][j][k];
                }

                deriv[i][j] *= ((db)1 - nodes[i][j]) * nodes[i][j];
            }
        }

        for(i = layer - 1; i > 0; i--)
        {
            for(k = 0; k < nodesN[i]; k++)
            {
                for(j = 0; j < nodesN[i - 1]; j++)
                {
                    Wupdate[i][j][k] = alpha * deriv[i][k] * nodes[i - 1][j] + momentum * Wupdate[i][j][k];
                    W[i][j][k] += Wupdate[i][j][k];
                }

                Wbupdate[i][k] = alpha * deriv[i][k] + momentum * Wbupdate[i][k];
                Wb[i][k] += Wbupdate[i][k];
            }
        }

        fclose(fp);
    }

    fp = fopen(info,"w");

    fprintf(fp,"%d\n",layer);

    for(i = 0; i < layer; i++)
    {
        fprintf(fp,"%d ",nodesN[i]);
    }

    fprintf(fp,"\n");

    for(i = 1; i < layer; i++)
    {
        for(j = 0; j < nodesN[i - 1]; j++)
        {
            for(k = 0; k < nodesN[i]; k++)
            {
                fprintf(fp,"%lf ",W[i][j][k]);
            }
        }

        fprintf(fp,"\n");

        for(j = 0; j < nodesN[i]; j++)
        {
            fprintf(fp,"%lf ",Wb[i][j]);
        }

        fprintf(fp,"\n");
    }

    fclose(fp);
}
