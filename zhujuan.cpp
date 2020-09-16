#include "zhujuan.h"
#define cout qDebug() << "{" << __FILE__ << ":" << __LINE__ << "}"//方便debug


zhujuan::zhujuan()//猪圈的初始化构造
{
    juanpig_num = 0;
    head = NULL;
    p = NULL;
    p0 = NULL;
}

void zhujuan::add_pig(piglist *p9 ,int b,int day,int month)//新猪插入链尾并编号0-9
{
    juanpig_num++;
    p9->inday = day;
    p9->inmonth = month;
    if(head == NULL){
        head = p9;
        p9->zhu_no = 0;
    }
    else {
        piglist *p8;
        p8 = head;
        //        qDebug() << "下一句估计会报错";
        for(int j =1; j<b&&p8;j++){
            p8 = p8->next;
        }
        p8->next = p9;
        p9->zhu_no = juanpig_num-1;
    }
}

void zhujuan::del_pig(piglist *pc)//删除猪猪并重新为猪猪编号
{
    if (juanpig_num == 1) {
        cout ;
        head = NULL;
        delete p;
        p = NULL;
        cout;
    }
    else if (p == head){
        cout;
        head = p->next;
        delete p;
        p = head;
        cout;
    }

    else{
        piglist *p0 = head;
        cout;
        while(p0->next != pc)
            p0 = p0->next;
        cout;
        p0->next = pc->next;
        cout;
        delete pc;
        cout;
        p = p0->next;
        cout;
    }
    cout;
    juanpig_num--;
    piglist *p1 = head;
    for(int i = 0;i<juanpig_num;i++){
        p1->zhu_no = i;
        p1 = p1->next;
    }
}

void zhujuan::setjuanpig_no(int a)
{
    juanpig_no = a;
}

int zhujuan::getspecies(int a)//返回该猪圈第a头猪的种类 a(0-9)
{
    if (head == NULL)
        return 3;//如果首指针指向空，直接返回3允许新猪直接入圈
    piglist *p = head;
    for(int j = 0; j<a&&p; j++){
        p = p->next;
    }
    return p->species;
}

int zhujuan::getczday(int a)
{
    piglist *p = head;
    for(int j = 0; j<a&&p; j++){
        p = p->next;
    }
    return p->czday;
}

int zhujuan::getczmonth(int a)
{
    piglist *p = head;
    for(int j = 0; j<a&&p; j++){
        p = p->next;
    }
    return p->czmonth;
}

int zhujuan::getjuanpig_num()
{
    return juanpig_num;
}

double zhujuan::getweight(int a)
{
    piglist *p = head;
    for(int j = 0; j<a&&p; j++){
        p = p->next;
    }
    return p->weight;
}

void zhujuan::pig_grow(int d , int b)//小猪成长函数并刷新猪的成长数值
{
    srand(time(NULL));
    piglist *p = head;
    for(int j = 0; j<b&&p; j++){
//        p->weight += (double)(rand()%12)*d/10*(1-p->weight/75+0.3);
        p->weight += (double)(rand()%12)*d/10;
        p->czday += d;
        if(p->czday >= 30){
            p->czmonth++;
            p->czday -= 30;
        }
        p = p->next;
    }
}

double zhujuan::returnprice(int a,int &all0,int &all1, int &all2,int price0,int price1,int price2)//出圈卖钱，b是该猪圈的猪的个数，方便遍历链表
{
    double sellprice = 0;
    if (head == NULL )
        return 0;
    cout << "juanpig_num:"<<juanpig_num;
    //piglist *p =head , *p0 = p;
    p =head;
    p0 = p;
    for(int j = 0; j<a&&p; j++){
        cout << "j:" << j;
        if (p->czmonth >= 12 || p->weight > 130){//设置卖猪的条件
            if(p->species == 0){
                sellprice += price0*p->weight;
                all0--;
            }
            if(p->species == 1){
                sellprice += price1*p->weight;
                all1--;
            }
            if(p->species == 2){
                sellprice += price2*p->weight;
                all2--;
            }
            cout;
            del_pig(p);
            continue;
        }
        cout;
        p0 = p;
        cout;
        p  = p0->next;
    }
    return sellprice;
}

void zhujuan::show_zhujuan(int a)
{
    spe0 = 0;
    spe1 = 0;
    spe2 = 0;
    piglist *p = head;
    for(int i = 0; i<a&&p; i++){
        if (p->species == 0)
            spe0++;
        else if (p->species == 1) {
            spe1++;
        }
        else if(p->species == 2)
            spe2++;
        p = p->next;
    }

    cout << juanpig_no << "号猪圈   黑猪有" << spe0 << "头， 小花猪有" << spe1 << "头，大花白猪有" << spe2<< "头";
}

QString zhujuan::show_zhuzhu(int a)
{
    piglist *p =head;
    QString str;
    double localprice;
        for(int i = 0; i<a&&p; i++){
            p = p->next;
        }
        if(getspecies(a)==0){
            qDebug() << "该猪是黑猪   ";
            localprice = p->weight * 30;
            str = "品种： 黑猪\n\n\n";
        }
        else if (getspecies(a)==1) {
            qDebug() << "该猪是小花猪   ";
            localprice = p->weight * 14;
            str = "品种： 小花猪\n\n\n";
        }
        else if (getspecies(a)==2) {
           qDebug() << "该猪是大花白猪   ";
           localprice = p->weight * 12;
           str = "品种： 大花白猪\n\n\n";
        }
        qDebug() << "该猪入圈时间是第" << p->inmonth << "月，第" << p->inday << "天\n"
                 << "已经在这个猪圈里生活了" << p->czmonth << "个月" << p->czday << "天，该猪体重是" << p->weight << "公斤，可以卖" << localprice << "元";

    if(p->weight <=40)
        str +="时期 ： 幼崽期\n\n\n";
    else if (p->weight >40&&p->weight<=90) {
        str +="时期 ： 成长期\n\n\n";
    }
    else if (p->weight >90) {
        str +="时期 ： 成熟期（可出圈）\n\n\n";
    }
    str += "在某一年的"+ QString::number(p->inmonth)+"月" +QString::number(p->inday)+"日\n它与你相遇\n\n\n"
            +"饲养时间："+QString::number(p->czmonth)+"个月"+QString::number(p->czday)+"天\n\n\n"
            +"体重："+QString::number(p->weight)+"公斤\n\n\n"
            +"战斗力：5\n\n\n"
            +"建议零售价："+QString::number(localprice)+"元";
    return str ;
}

void zhujuan::plus_num()
{
    juanpig_num++;
}

