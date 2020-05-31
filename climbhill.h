#ifndef CLIMBHILL_H
#define CLIMBHILL_H
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#define random(x) (rand()%x)  // 定义产生随机数的函数

using namespace std;
class climbHill {
private:
    int N; // 棋盘大小
    int step;				   // 记录运行的步数
    int maxSteps;         // 达到最大扫描次数后，仍未解决则重启
    int rowP;           // 在该行位置检测最优位置
public:
    climbHill(int N) {
        step = 0;
        maxSteps = N * N *2;
        this->N = N;
        rowP = 0;
    }

    //随机重启，达到一步的步数后，仍未求解那么就重新打乱棋盘
    //先生成对角线的皇后棋子，然后随机交换两行
    void reset(vector<int>& chessboard) {
        chessboard.clear();
        for (int i = 0; i < N; i++) {
            chessboard.push_back(i);
        }
        srand((unsigned)time(0));
        for (int row1 = 0; row1 < N; row1++) {
            int row2 = random(N);
            // 随机交换行，打散棋盘，但保证皇后都在不同列
            swap(chessboard[row1], chessboard[row2]);
        }
    }

    // 计算当前棋盘存在的相互攻击的皇后对数
    int getattackNum(vector<int> *chessboard) {
        int tacketNum = 0;
        int width = this->N;
        for (int i = 0; i < width; i++) {
            for (int j = i + 1; j < width; j++) {
                // 当存在皇后位于对角线的时候 冲突数+1
                if (abs(j - i) == abs((*chessboard)[i] - (*chessboard)[j])) {
                    tacketNum++;
                }
                // 当存在皇后位于同一列的时候，冲突数+1
                if ((*chessboard)[i] == (*chessboard)[j]) {
                    tacketNum++;
                }
            }
        }
        return tacketNum;
    }

    // 计算某一行的最优位置
    int ChoseBest(vector<int>* chessboard, int row) {
        // 记录每一个位置对应的棋盘状态所对应的冲突数
        vector<int> tackData;
        for (int i = 0; i < N; i++) {
            (*chessboard)[row] = i;
            tackData.push_back(getattackNum(chessboard));
        }
        //记录冲突数最小的几个位置，然后随机然后一个位置
        int less = 0;
        vector<int> bestPoint;
        for (int i = 0; i < N; i++) {
            if (i == 0) {
                bestPoint.push_back(0);
                less = tackData[0];
                continue;
            }
            // 判断i位置冲突数小于最优位置,清空向量，存储更小冲突的位置
            if (tackData[i] < less) {
                bestPoint.clear();
                bestPoint.push_back(i);
                less = tackData[i];
            }
            // 将同样最小的冲突位置也添加到向量中
            else if(tackData[i] == less) {
                bestPoint.push_back(i);
            }
        }
        if (bestPoint.size() == 1)
            return bestPoint[0];

        // 最佳位置有多个，则随机筛选
        srand((unsigned)time(0));
        return bestPoint[rand() % bestPoint.size()];
    }

    // 每行选择最优位置
    vector<int>* chosechess(vector<int>* chessboard, int row){
        (*chessboard)[row] = ChoseBest(chessboard, row);
        return chessboard;
    }

    // 求解
    vector<int>* solve(vector<int> *chessboard) {
        srand(time(NULL));
        int resetTime = 0;  // 重启步数
        step = 0;   // 统计运行步数
        // 当冲突数为0时终止爬山
        while (getattackNum(chessboard) > 0) {
            if (step >= maxSteps) {
                reset(*chessboard);
                resetTime++;
                step = 0;
            }
            // 将rowPostion行的皇后移到同一行的最优位置
            chessboard = chosechess(chessboard,rowP++);
            // 判断rowP是否归零，防止越界
            rowP = rowP >= N ? rowP % N : rowP;
            step++;
        }
        return chessboard;
    }

};

#endif //
