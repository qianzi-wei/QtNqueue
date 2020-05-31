#ifndef OUTHOT_H
#define OUTHOT_H
#include<iostream>
#include<vector>
#include<string>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#define random(x) (rand()%x)  // 定义产生随机数的函数
using namespace std;
class outhot {
private:
    int N;              // 皇后的数目
    int maxSteps;         // 达到最大扫描次数后，仍未解决则重启
public:
    // 默认构造函数
    outhot(int N) {
        this->N = N;
        maxSteps = N * N * 2;
    }
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

    //利用模拟退火算法得到该行的合适位置
    int outHot(const vector<int> *chessboard, int row) {
        double T = 5.0;          // 当前温度
        double Tmin = 0.1;         // 最小温度
        double decreaseRate = 0.7; // 降温速率

        // 当前状态和下一状态
        vector<int>* currentC = new vector<int>(N);
        vector<int>* nextC = new vector<int>(N);

        for (int i = 0; i < this->N; i++) {
            (*currentC)[i] = ((*chessboard)[i]);
            (*nextC)[i] = ((*chessboard)[i]);
        }
        // 开始模拟退火
        while (T > Tmin) {
            // 修改下一状态，改变row行的皇后位置
            for (int queeP = 0; queeP < this->N; queeP++) {
                (*nextC)[row] = queeP;
                // 得到前后状态的能量差
                double dE = getattackNum(nextC) - getattackNum(currentC);
                // 如果下一步更优则，接受该移动
                if (dE <= 0) {
                    (*currentC)[row] = (*nextC)[row];
                }
                //  如果下一步更差，则一定几率接受该移动，几率逐渐变小
                else {
                    if (exp((-1) * dE / T) > random(100)* 1.0 / 100) {
                        (*currentC)[row] = (*nextC)[row];
                    }
                }
            }
            //  降温
                T = decreaseRate * T;

        }
        return (*currentC)[row];
    }

    // 利用模拟退火算法修改该行，得到一个合适的位置
    vector<int>* nextState(vector<int> *chessboard, int row) {
        (*chessboard)[row] = outHot(chessboard, row);
        return chessboard;
    }

    // 求解主函数，在不同的行中不断搜寻下一步合适的位置
    vector<int>* solve(vector<int> *chessboard) {
        int rowP = 0;
        int step = 0;
        srand(time(NULL));
        // 当冲突数为0时终止
        while (getattackNum(chessboard)) {
            if (step >= maxSteps) {
                reset(*chessboard);
                step = 0;
            }
            if (rowP == this->N) {
                rowP %= rowP;
            }
            chessboard = nextState(chessboard, rowP++);
            step++;
        }
        return chessboard;
    }

};
#endif // OUTHOT_H
