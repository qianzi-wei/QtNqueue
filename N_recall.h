#ifndef N_RECALL_H
#define N_RECALL_H
#include<stdio.h>
#include<vector>
#include<stdlib.h>
#include"allh.h"
class N_recall
{
public:
    N_recall();
    void place (int k,int n);
    int x[225];
private:
    void print(int n);
    int find(int i,int k);
};

#endif // N_RECALL_H
