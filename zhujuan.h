#ifndef ZHUJUAN_H
#define ZHUJUAN_H
#include <cstdlib>
#include "time.h"
#include <QDebug>
#include <QTime>
struct piglist
{
   double weight;//猪的体重
   int species;//'0'代表黑猪，‘1’代表小花猪，‘2’代表大花白猪
   int zhu_no;//猪的编号0-9
   int czday,czmonth;//猪的成长时间
   int inday,inmonth=0;//猪进入猪圈的时间
   piglist *next;//链表指针

};

class zhujuan
{
private:
    int juanpig_no;//猪圈的编号0-99
    int juanpig_num;//本圈中猪的数量
    int spe0,spe1,spe2;//'0'代表黑猪，‘1’代表小花猪，‘2’代表大花白猪
    piglist *p,*p0;//方便对链表进行操作的指针
    int plaguepig[10];//瘟疫猪的标记数组，0代表健康，1代表瘟疫猪，-1代表没有猪
    int spread;//标志本猪圈的猪瘟传播模式，0代表不传播，1代表15%概率传播，2代表50%概率传播
public:
    zhujuan();//构造函数
    piglist *head;//猪圈链表头节点
    void add_pig(piglist *p , int b,int day,int month);//b对应juanpig_num，在本猪圈链表尾增加一头猪
    void del_pig(piglist *p);//删除猪猪
    void setjuanpig_no(int a);
    void pig_grow(int d, int b);//随即成长函数，b对应juanpig_num
    double returnprice(int a,int &all0,int &all1,int &all2,int price0,int price1,int price2);//该猪圈该次出圈卖的钱数
    double returnprice_true(int a,int price0,int price1,int price2);//该猪圈该次出圈卖的钱数
    QString show_zhujuan(int a);//查询猪圈信息
    QString show_zhuzhu(int a);//查询某头猪的信息
    void plus_num();//修改猪圈猪的个数
    int getisplague(int a);//查询本圈a号坑位的猪的属性
    void setplague(int a,int b);//a号坑位的猪设置成b属性
    void setspread(int a);//设置传播模式，0代表不传播，1代表15%传播几率，2代表50%传播纪律
    int getspread();//查询该猪圈的传播几率
    void setgeli();//设置隔离
    int getspecies(int a);//查询编号a的猪的种类
    int getczday(int a);//查询编号a的猪的饲养天数
    int getczmonth(int a);//查询编号a的猪的饲养月数
    int getjuanpig_num();//返回该猪圈猪的个数
    double getweight(int a);//返回编号a的猪的体重
};

#endif // ZHUJUAN_H
