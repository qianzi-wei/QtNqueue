#ifndef GENETIC_H
#define GENETIC_H
#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#define random(x) (rand()%x)  // 定义产生随机数的函数
using namespace  std;
class genetic {
private:
    int N;                          // 皇后的数目
    int numOfG;           //  种群个体数目
    int maxNumnoC;   // 每一个个体最大的不冲突个数
    vector<double> fitness;           // 每一个个体的适应度值列表
    vector<int> tChessboard;  // 目标棋盘
    vector<vector<int>> population;

public:
    genetic(int N) {
        this -> N = N;
        maxNumnoC = ccmaxNumnoC(N);
        numOfG = 4 * N;
    }

    // 计算个体最大的不冲突个数
    int ccmaxNumnoC(int N) {
        int num = 0;
        for (int i = N - 1; i > 0; i--) {
            num += i;
        }
        return num;
    }

    // 适应度函数，检查个体不冲突的皇后数
    int getNotattackNum(vector<int> *chessboard) {
        int NotattackNum = 0;
        int width = this->N;
        for (int i = 0; i < width; i++) {
            for (int j = i + 1; j < width; j++) {
                // 当存在皇后不位于对角线的时候 且不在同一列时
                if (abs(j - i) != abs((*chessboard)[i] - (*chessboard)[j]) &&  (*chessboard)[i] != (*chessboard)[j]) {
                    NotattackNum++;
                }
            }
        }
        return NotattackNum;
    }

    // 返回种群（棋盘）中不存在冲突的个体的位置，若不存在则返回-1
    int PnotmaxC(vector<vector<int>>& population) {
        for (int i = 0; i < numOfG; i++) {
            if (getNotattackNum(&(population[i])) == this->maxNumnoC) {
                return i;
            }
        }
        return -1;
    }

    // 计算种群中每一个个体的适应度,并且将比例保存在fitness中
    void ccFitness(vector<vector<int>>& population, vector<double>& fitness) {
        fitness.clear();
        int sufeNum = 0;
        vector<int> notConflict;
        for (int i = 0; i < numOfG; i++) {
            int n = getNotattackNum(&(population[i]));
            sufeNum += n;
            notConflict.push_back(n);
        }
        for (int i = 0; i < numOfG; i++) {
            fitness.push_back(1.0 * notConflict[i]/sufeNum);
        }

    }

    // 随机产生N个个体，形成一个种群
    vector<vector<int>>* creatP(vector<vector<int>>& population, int numOfI) {
        population.clear();
        vector<int> chessboard ;
        for (int i = 0; i < numOfI; i++) {
            for (int j = 0; j < N; j++) {
                chessboard.push_back(j);
            }
            for (int row1 = 0; row1 < N; row1++) {
                int row2 = random(N);
                // 随机交换行，打散棋盘，但保证皇后都在不同列
                swap(chessboard[row1], chessboard[row2]);
            }

            population.push_back(chessboard);
            chessboard.clear();
        }
        return &population;
    }

    // 随机选择两个个体,轮盘赌算法：思想是产生一个0~1的随机数，按适应度的比例挑选个体
    vector<vector<int>>* select(const vector<vector<int>>& population,const vector<double>& fitness, vector<vector<int>>& parents) {
        float m = 0;
        parents.clear();
        float p1 = (rand() % 100) * 1.0 / 100;
        float p2 = (rand() % 100 )* 1.0 / 100;
        for (int i = 0; i < numOfG; i++) {
            m += fitness[i];
            if (p1 <= m) {
                // 加入第一个个体
                // 产生的随机数p1 在 m ~m+fitness[i] 间则认为选择了i
                parents.push_back(population[i]);
                break;
            }
        }
        m = 0;
        for (int i = 0; i < numOfG; i++) {
            m += fitness[i];
            // 加入第二个个体
            if (p2 <= m) {
                parents.push_back(population[i]);
                break;
            }
        }
        return &parents;
    }

    // 对两个个体进行杂交，产生一个后代,双点+双侧杂交
    vector<int>* crossover(const vector<int>& chessboard1, const vector<int>& chessboard2, vector<int> &son) {
        int pos1 = 0, pos2 = 0;
        son.clear();
        while (pos1 >= pos2) {
            pos1 = rand() % N;
            pos2 = rand() % N;
        }

        for (int i = 0; i < this->N; i++) {
            if (i < pos1 || i > pos2) {
                son.push_back(chessboard1[i]);
            }
            else {
                son.push_back(chessboard2[i]);
            }
        }
        return &son;
    }

    // 对该产生的后代个体进行变异
    vector<int>* mutate(vector<int>& chessboard) {
        int row1 = rand() % N;
        int col1 = rand() % N;
        chessboard[row1] = col1;
        return &chessboard;
    }

    // 求解
    vector<int>* solve(vector<int>& chessboard) {
        /*
         设置种群参数
         pMutate:  变异发生的概率
         pCrossover: 交叉发生的概率
        */
        double pMutate = 0.2;
        double pCrossover = 0.9;

        vector<int> individual;
        population = *creatP(population,numOfG);  // 初始化种群
        int numOfGeneration = 0;   // 进化的代数
        srand((unsigned)time(NULL));
        // 开始进化
        do {
            // 计算种群中每一个个体的适应度值,结果保存在fitness
            ccFitness(population, fitness);
            vector<vector<int>> newPopulation;
            do {
                // 随机挑选两个个体作为父母
                vector<vector<int>> parents;
                parents = *select(population, fitness, parents);
                // 只产生更优的儿子
                while (1) {
                    // 防止不交叉，son为空
                    vector<int> son = parents[0];
                    // 随机播种，产生随机数
                    // 一定概率发生交叉，产生儿子,儿子适应度更高
                    float pc = rand() % 100 * 1.0 / 100;
                    if (pc < pCrossover) {
                        son = *(crossover((parents).at(0), (parents).at(1), son));
                    }
                    // 一定概率发生变异，改变儿子
                    float pm = rand() % 100 * 1.0 / 100;
                    if (pm < pMutate) {
                        son = *mutate(son);
                    }
                    // 若儿子优于或等于父母，则添加到种群中
                    if (getNotattackNum(&son) >= getNotattackNum(&parents[0]) && getNotattackNum(&son) >= getNotattackNum(&parents[1])) {
                        newPopulation.push_back(son);
                        son.clear();
                        parents.clear();
                        break;
                    }
                    else {
                        if(pMutate <= 0.98)
                            pMutate += 0.02;
                    }
                }
            } while(newPopulation.size()!= numOfG); // 直到儿子种群数目重新等于种群规模

            // 新种群替代旧种群
            population.clear();
            for (int i = 0; i < numOfG; i++) {
                population.push_back(newPopulation[i]);
            }
            numOfGeneration++;
        } while (PnotmaxC(population) == -1);
        // 得到种群中满足适应度的个体
        chessboard = population[PnotmaxC(population)];
        return &chessboard;
    }
};

#endif // GENETIC_H
