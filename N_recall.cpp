#include"N_recall.h"
N_recall::N_recall()
{

}
/*将所得的数据保存到ves二维数组中*/
void N_recall::print(int n){
    int i,j;
       for(i=1;i<=n;i++)
       {
           for(j=1;j<=n;j++)
           {
               if(x[i]==j)q.push_back(j);
           }
       }
   ves.push_back(q);
   q.clear();
};
//判断是否符合n皇后的规则
int N_recall::find(int i, int k){
    int j=1;
       while(j<i)  //j=1~i-1是已经放置了皇后的行
       {
           //第j行的皇后是否在k列或(j,q[j])与(i,k)是否在斜线上
           if(x[j]==k || abs(j-i)==abs(x[j]-k))
               return 0;
           j++;
       }
       return 1;
};
//递归主函数
void N_recall::place(int k, int n){
    int j;
        if(k>n)
            print(n); //递归出口
        else
        {
            for(j=1;j<=n;j++)   //试探第k行的每一个列
            {
                if(find(k,j))
                {
                   x[k]=j;
                    place(k+1,n);  //接着下一行
                }
            }
        }
}
