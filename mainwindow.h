#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QTime>
#include <cstdlib>
#include "zhujuan.h"
#include <QProgressBar>
#include <QDebug>
#include <QString>
#include <QtCharts>
#include <QBarSet>
#include <QFile>
#include <QMessageBox>
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
    void chushihuachart();//刷新显示猪圈体重信息的chart
    void chushihuachart1();//刷新显示猪圈种类与个数信息的chart
    void chushihuachart2();//刷新显示猪圈饲养分布信息的chart
    void getfeedtime_all();//获取所有猪的饲养分布情况
    void qt_write_txt();//将最近一年的销售情况写入文件
    void qt_read_txt();//读取最近五年的销售情况
    void save_game();//保存当前游戏数据并写入txt文件
    void read_game();//读取txt文件初始化游戏内容
    void initplague();//设置猪瘟模式
    int MyMessageBox(QString title, QString message);//获取隔离栏的对话框的返回值
    int MyMessageBoxx(QString title, QString message);//询问是否出圈购猪的对话框

private:
    int day;//进行的总时间
    int month,fakemonth;
    int year;
    int pass_day;//每一跳度过的时间
    double sellprice;//一次卖的价钱
    double allprice;//玩家拥有的总价钱
    int maxprice0,maxprice1,maxprice2;
    int all0,all1,all2,allplague;//种类个数统计
    int sellpig[3][5] ,buypig[3][5];//五年销售记录
    int sellpriceyear[5] ;//五年销售记录
    double weight0,weight1,weight2;//种类体重统计
    int feed3,feed6,feed9,feed12;//饲养时间分布
    zhujuan *hogpen[100];//100个猪圈
    int flag;//新猪平均分配
    int tm_startID;
    bool isplaue;//判断是否是猪瘟模式
    int price0,price1,price2;
    QChart *chart,*chart1,*chart2;//实时显示猪圈状态信息的图表

    Ui::MainWindow *ui;
private slots:
    void updateprogress();//模拟游戏时间的进行和猪的成长
    void chujuangouzhu();//将符合出圈条件的猪卖出并用随机数购买猪仔



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

    void on_plague_pig_clicked();

    void on_geli_clicked();

    void on_closebutton_clicked();

    void on_savebutton_2_clicked();

    void on_closebutton_2_clicked();

protected:
    void timerEvent(QTimerEvent *event);

};

#endif // MAINWINDOW_H
