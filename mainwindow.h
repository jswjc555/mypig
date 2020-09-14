#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QProgressBar>//养猪进度条
#include <cstdlib>
#include <QDebug>
#include <QString>
#include "zhujuan.h"
#include <QtCharts>
#include <QBarSet>
#include <QFile>
QT_CHARTS_USE_NAMESPACE
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int getday();
    void chushihuachart();//初始化chart
    void chushihuachart1();
    void chushihuachart2();
    void getfeedtime_all();
    void qt_write_txt();
    void qt_read_txt();
    void save_game();
    void read_game();
private:
    int day;//进行的总时间
    int month,fakemonth;
    int year;
    int pass_day;//每一跳度过的时间
    double sellprice;
    double allprice;
    int all0,all1,all2;//种类个数统计
    int sellpig[3][5] ,buypig[3][5];
    int sellpriceyear[5] ;
    double weight0,weight1,weight2;//种类体重统计
    int feed3,feed6,feed9,feed12;//饲养时间分布
    zhujuan *pjuan[100];//100个猪圈
    int flag;//新猪平均分配
    int tm_startID;
    int price0,price1,price2;
    QChart *chart,*chart1,*chart2;

    Ui::MainWindow *ui;
private slots:
    void updateprogress();
    void chujuangouzhu();



    void on_startpig_clicked();
    void on_stoppig_clicked();
    void on_sellpig_clicked();

    void on_checkjuan_clicked();

    void on_checkpig_clicked();

    void on_startgame_clicked();

    void on_to_check_clicked();

    void on_back_main_clicked();

    void on_back_start_clicked();

    void on_searchbutton_clicked();

    void on_savebutton_clicked();

    void on_readbutton_clicked();

protected:
    void timerEvent(QTimerEvent *event);

};

#endif // MAINWINDOW_H
