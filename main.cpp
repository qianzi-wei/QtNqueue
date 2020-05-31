#include "mainwindow.h"
#include"N_recall.h"
#include <QApplication>
#include<iostream>
  std::vector<int>q;
  std::vector<std::vector<int>> ves;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("N皇后");
    w.setWindowIcon(QIcon(":/new/img/hg.png"));
    w.show();
    return a.exec();
}
