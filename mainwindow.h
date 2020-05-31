#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPixmap>
#include <vector>
#include <time.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int SIZE=1;
    const int WIDTH=40;
    const int x=20,y=20;
 private slots:
     void recall();
     void nextN();
     void climb();
     void gent();
     void outh();
private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    QPainter *paint;
    void initC(std::vector<int>&cmap);
    clock_t start;
    clock_t endt;
    double allt;
};
#endif // MAINWINDOW_H
