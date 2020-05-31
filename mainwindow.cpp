#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"N_recall.h"
#include<QDebug>
#include <synchapi.h>
#include "allh.h"
#include "climbhill.h"
#include "genetic.h"
#include "outhot.h"
#define random(x) (rand()%x)  // 定义产生随机数的函数
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(recall()));
    connect(ui->pushButton_x,SIGNAL(clicked()),this,SLOT(nextN()));
    connect(ui->pushButton_c,SIGNAL(clicked()),this,SLOT(climb()));
    connect(ui->pushButton_g,SIGNAL(clicked()),this,SLOT(gent()));
    connect(ui->pushButton_h,SIGNAL(clicked()),this,SLOT(outh()));
    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,QBrush(QPixmap(":/new/img/view.jpg").scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));// 使用平滑的缩放方式
    this->setPalette(palette);// 给widget加上背景图
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::resizeEvent(QResizeEvent *){
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,QBrush(QPixmap(":/new/img/view.jpg").scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));// 使用平滑的缩放方式
    this->setPalette(palette);// 给widget加上背景图
    int nw = width();
   // int nh = height();
    ui->label->move(nw-170,ui->label->y());
    ui->label_b->move(nw-160,ui->label_b->y());
    ui->label_t->move(nw-160,ui->label_t->y());
    ui->pushButton->move(nw-130,ui->pushButton->y());
    ui->pushButton_c->move(nw-130,ui->pushButton_c->y());
    ui->pushButton_g->move(nw-130,ui->pushButton_g->y());
    ui->pushButton_h->move(nw-130,ui->pushButton_h->y());
    ui->pushButton_x->move(nw-130,ui->pushButton_x->y());
    ui->lineEdit->move(nw-100,ui->lineEdit->y());
}

void MainWindow::paintEvent(QPaintEvent *)
{
    paint=new QPainter;
    paint->begin(this);//主窗口
    int  n = ui->lineEdit->text().toInt();
    SIZE = n;
    paint->setPen(QPen(Qt::darkGreen,2,Qt::SolidLine));//钢笔工具：颜色，线号，实线
    //画SIZE+1条横线

    for(int i=0;i<SIZE+1;i++)
      {
        paint->drawLine(x,y+WIDTH*i,x+WIDTH*(SIZE),y+WIDTH*i);//画线函数：x1,y1,x2,y2:画从(x1,y1)到(x2,y2)的线
    }
    //画SIZE+1条竖线
    for(int i=0;i<SIZE+1;i++)
      {
        paint->drawLine(x+WIDTH*i,y,x+WIDTH*i,y+WIDTH*(SIZE));
      }
    paint->setBrush(QBrush(Qt::red,Qt::SolidPattern));//毛刷：颜色，实图案
   //绘制皇后
    if(ves.size()!=0){
    std::vector<int>vec_imp;
            vec_imp=ves[0];
            paint->setPen(QPen(Qt::red,2,Qt::SolidLine));

       for(int i = 0;i<vec_imp.size();i++){/*
           paint->drawLine(x+WIDTH*i,y+WIDTH*(vec_imp[i]-1)+WIDTH/2,x+WIDTH*i+WIDTH/4,y+WIDTH*(vec_imp[i]-1)+WIDTH/2);
           paint->drawLine(x+WIDTH*i+WIDTH/4,y+WIDTH*(vec_imp[i]-1)+WIDTH/2,x+WIDTH*i+WIDTH/2,y+WIDTH*(vec_imp[i]-1));
           paint->drawLine(x+WIDTH*i+WIDTH/2,y+WIDTH*(vec_imp[i]-1),x+WIDTH*i+WIDTH*3/4,y+WIDTH*(vec_imp[i]-1)+WIDTH/2);
           paint->drawLine(x+WIDTH*i+WIDTH*3/4,y+WIDTH*(vec_imp[i]-1)+WIDTH/2,x+WIDTH*i+WIDTH,y+WIDTH*(vec_imp[i]-1)+WIDTH/2);
           paint->drawLine(x+WIDTH*i+WIDTH,y+WIDTH*(vec_imp[i]-1)+WIDTH/2,x+WIDTH*i+WIDTH*3/4,y+WIDTH*(vec_imp[i]-1)+WIDTH);
           paint->drawLine(x+WIDTH*i+WIDTH*3/4,y+WIDTH*(vec_imp[i]-1)+WIDTH,x+WIDTH*i+WIDTH/4,y+WIDTH*(vec_imp[i]-1)+WIDTH);
           paint->drawLine(x+WIDTH*i+WIDTH/4,y+WIDTH*(vec_imp[i]-1)+WIDTH,x+WIDTH*i,y+WIDTH*(vec_imp[i]-1)+WIDTH/2);
           paint->drawEllipse(x+WIDTH*i+WIDTH/4,y+WIDTH*(vec_imp[i]-1)+WIDTH/2,WIDTH/2,WIDTH/2);
           */
           //将re放在rex中 x y h h
           QRect re(0,0,40,40);
           QRect rex(x+WIDTH*i,y+WIDTH*(vec_imp[i]-1),40,40);
           QImage img(":/new/img/hgic.png");
           paint->drawImage(rex,img,re);
       }
    }
  paint->end();
}

void MainWindow::initC(std::vector<int> &cmap){
    int N = ui->lineEdit->text().toInt();
    for (int i = 0; i < N; i++) {
            cmap.push_back(i);
        }
        srand((unsigned)time(0));
        for (int row1 = 0; row1 < N; row1++) {
            int row2 = random(N);
            // 随机交换行，打散棋盘，但保证皇后都在不同列
            swap(cmap[row1], cmap[row2]);
        }
};


void MainWindow::recall(){
   q.clear();
   ves.clear();
   start = clock();
   N_recall n1;
   n1.place(1,ui->lineEdit->text().toInt());
   update();
   endt = clock();
   allt = (double)(endt-start)/CLOCKS_PER_SEC;
   ui->label_t->setText(tr("time is %1 s").arg(QString::number(allt,10,3)));
   ui->label_b->setText("回溯法");
}
void MainWindow::nextN(){
    if(ves.size()!=0){
        ves.erase(ves.begin(),ves.begin()+1);
        update();
    }
}
void MainWindow::climb(){
    q.clear();
    ves.clear();
    start = clock();
    climbHill cmb(ui->lineEdit->text().toInt());
    initC(q);
    q = *cmb.solve(&q);
    for(int i=0;i<q.size();i++){
        q[i]++;
    }
    ves.push_back(q);
    update();
    endt = clock();
    allt = (double)(endt-start)/CLOCKS_PER_SEC;
    ui->label_t->setText(tr("time is %1 s").arg(QString::number(allt,10,3)));
    ui->label_b->setText("随机重启爬山法");
}

void MainWindow::gent(){
    q.clear();
    ves.clear();
    start = clock();
    genetic  gen(ui->lineEdit->text().toInt());
    initC(q);
    q = *gen.solve(q);
    for(int i=0;i<q.size();i++){
        q[i]++;
    }
    ves.push_back(q);
    update();
    endt = clock();
    allt = (double)(endt-start)/CLOCKS_PER_SEC;
    ui->label_t->setText(tr("time is %1 s").arg(QString::number(allt,10,3)));
    ui->label_b->setText("遗传算法");
}
void MainWindow::outh(){
    q.clear();
    ves.clear();
    start = clock();
    outhot  ou(ui->lineEdit->text().toInt());
    initC(q);
    q = *ou.solve(&q);
    for(int i=0;i<q.size();i++){
        q[i]++;
    }
    ves.push_back(q);
    update();
    endt = clock();
    allt = (double)(endt-start)/CLOCKS_PER_SEC;
    ui->label_t->setText(tr("time is %1 s").arg(QString::number(allt,10,3)));
    ui->label_b->setText("退火法");
}

