#include "mainwindow.h"
#include "ui_mainwindow.h"
#define cout qDebug() << "{" << __FILE__ << ":" << __LINE__ << "}"//方便debug

MainWindow::MainWindow(QWidget *parent) ://构造函数----------------------------------------------------------------------------------------
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("养猪模拟");
    resize(800,750);
    ui->stackedWidget->resize(800,750);
    day = 0;
    month = 0;
    year = 0;
    fakemonth = 0;
    flag = 0;
    sellprice = 0;

    memset(sellpig,0,sizeof(sellpig));
    memset(buypig,0,sizeof(buypig));
    memset(sellpriceyear,0,sizeof(sellpriceyear));
    allprice = 10000 ;
    pass_day = 10;
    all0 = 0; all1 =0; all2 = 0;
    weight0 =0;weight1 =0; weight2 =0;
    for(int i=0;i<100;i++){//初始化zhujuan指针数组
        pjuan[i]=new zhujuan;
        pjuan[i]->setjuanpig_no(i);
    }
    for(int i=0;i<100;i++){//异常处理
        if(pjuan[i] == NULL)
             exit(1);
    }

    //第一次初始化，加点猪猪
    srand(time(NULL));
    price0 = rand()% 10 +20;
    price1 = rand()% 12 +8 ;
    price2 = rand()%8  +8;
    int a = rand()%20+10;//开始拥有随机10-30头猪
    qDebug() << "初始有" << a << "头猪" ;
    for(int i = 0; i < a;i++){
        piglist *p = new piglist;
        p->next = NULL;
        p->czday = 0;
        p->czmonth = 0;
        p->species = rand()%3;
        p->weight = rand()%30+10;
        pjuan[flag]->add_pig(p,pjuan[flag]->getjuanpig_num(),day,month);//初始化不用判断是否为黑猪圈
        if(pjuan[i]->getspecies(0) == 0){
            all0++;
            weight0 += p->weight;
            buypig[0][year]++;
        }
        else if (pjuan[i]->getspecies(0) == 1) {
            all1++;
             weight1 += p->weight;
             buypig[1][year]++;
        }
        else if (pjuan[i]->getspecies(0) == 2) {
            all2++;
             weight2 += p->weight;
             buypig[2][year]++;
        }
        flag++;
    }

    ui->stackedWidget->setCurrentWidget(ui->pagestart);
    ui->juanpig_no->setPlaceholderText("猪圈编号0-99");
    ui->juanpig_no->setAlignment(Qt::AlignCenter);
    ui->juanpig_no->setValidator(new QIntValidator(0,99,this));//设置只能输入0-99的整数
    ui->pig_no->setPlaceholderText("猪猪编号0-9");
    ui->pig_no->setAlignment(Qt::AlignCenter);
    ui->pig_no->setValidator(new QIntValidator(0,9,this));//设置只能输入0-99的整数
    ui->daytbar->setRange(0,90);
    ui->daytbar->setValue(0);
    ui->sellpig->setVisible(false);
    ui->mainlabel_up->setText(tr( "现在是%1年 %2月  %3日\n 金钱：%4元" ).arg( year ).arg( month ).arg( day ).arg(allprice));
    ui->mainlabel_right->setText(tr( "你拥有100个猪圈\n开场你有10000元\n%1只黑猪崽儿\n%2只小花猪崽儿\n%3只大花白猪崽儿\n"
                                     "只有在游戏时间进行中\n猪猪们才会增长体重\n每种品种的肉价\n会随着时间流逝随机波动\n挑选最好的时机出圈吧！" ).arg(all0).arg(all1).arg(all2));
    ui->mainlabel_down->setText(tr("         市场价目表：\n 黑猪肉：%1        元/公斤\n 小花猪肉：%2      元/公斤\n 大花白猪肉：%3    元/公斤")
                                .arg(price0).arg(price1).arg(price2));
    ui->pig_image->setScaledContents(true);


    chushihuachart();
    chushihuachart1();
    chushihuachart2();
    qt_write_txt();
}
Q_DECLARE_METATYPE(piglist)
void MainWindow::updateprogress(){

    int tm_now ;
    tm_now = ui->daytbar->value();
    tm_now += pass_day;
    if( tm_now <=90){
        weight0 = 0;
        weight1 = 0;
        weight2 = 0;
    }
    ui->daytbar->setValue(tm_now);

    if(tm_now > 90){
        ui->daytbar->setValue(0);
        tm_now = 0;
        killTimer(tm_startID);
        qDebug() << "时间暂停,已经过去了3个月\n可以选择卖猪啦！";
        ui->mainlabel_right->setText(tr( "时间暂停\n已经过去了3个月\n可以选择卖猪啦！" ));
        ui->sellpig->setVisible(true);
        chushihuachart();
        chushihuachart1();
    }
    else{
        day+=pass_day;//改时间
        if(day >= 30){
            month++;
            fakemonth++;
            day -= 30;
        }
        if (month >= 12){
            sellpig[0][year] = buypig[0][year] - all0;
            sellpig[1][year] = buypig[1][year] - all1;
            sellpig[2][year] = buypig[2][year] - all2;
            qt_write_txt();
            year++;
            month -= 12;
        }
        for(int i=0;i<100;i++){
            pjuan[i]->pig_grow(pass_day,pjuan[i]->getjuanpig_num());//改时间
            for(int j=0;j<pjuan[i]->getjuanpig_num();j++){
              switch (pjuan[i]->getspecies(j)) {//记录此次购入猪的种类
                case 0:{
                    weight0 += pjuan[i]->getweight(j);
                    break;
                }
                case 1:{
                    weight1 += pjuan[i]->getweight(j);
                    break;
                }
                case 2:{
                    weight2 += pjuan[i]->getweight(j);
                    break;
               }

             }

           }

        }
        price0 = rand()% 10 +20;
        price1 = rand()% 12 +8 ;
        price2 = rand()%8  + 8;
        qDebug() << "时间过去了" << tm_now << "天";
//        qDebug() << "all0=" << all0 << "all1" << all1 << "all2" << all2<<endl;
//        qDebug() << "weight0=" << weight0 << "weight1" << weight1 << "weight2" << weight2 << endl;
            qDebug() << "现在是第"  << year  << "年，" << "第" << month << "个月，第" << day << "天" ;
       ui->mainlabel_right->setText(tr( "养猪场经过了%1天，\n猪猪正在茁壮成长" ).arg( tm_now ));
       ui->mainlabel_down->setText(tr("         市场价目表：\n 黑猪肉：%1    元/公斤\n 小花猪肉：%2    元/公斤\n 大花白猪肉：%3    元/公斤").arg(price0).arg(price1).arg(price2));
       ui->mainlabel_up->setText(tr( "现在是%1年 %2月  %3日\n 金钱：%4元" ).arg( year ).arg( month ).arg( day ).arg(allprice));


   }


}


void MainWindow::chujuangouzhu()
{
    if(fakemonth < 3){
        cout << "时间未过三个月，请耐心等待猪猪长大后再出圈哦~";
    ui->mainlabel_right->setText(tr( "时间未过三个月，请耐心\n等待猪猪们长大后再出圈哦~" ));
    }
    else {
        for(int i=0;i<100;i++){
            cout << i <<"开始卖猪";
            sellprice   +=   pjuan[i]->returnprice(pjuan[i]->getjuanpig_num(),all0,all1,all2,price0,price1,price2);
            //           qDebug() << i << "圈卖猪" << sellprice;
        }

       // int a = rand()%30+70;//每次新购入70-100头猪
        int a = 100;
        int black=0,small=0,big=0;
        for(int i = 0;i < a;i++){
            piglist *p = new piglist;
            p->next = NULL;
            p->czday = 0;
            p->czmonth = 0;
            p->species = rand()%3;
            p->weight = rand()%30+20;
            switch (p->species) {//记录此次购入猪的种类
            case 0:{
                black++;
                all0++;
                weight0 += p->weight;
                break;
            }
            case 1:{
                small++;
                all1++;
                weight1 += p->weight;
                break;
            }
            case 2:{
                big++;
                all2++;
                weight2 += p->weight;
                break;
            }

            }
            qDebug() << "准备在" << flag << "圈加新猪" <<p->species<< "该圈是" << pjuan[flag]->getspecies(0);

            if(pjuan[flag]->getspecies(0) == 3){//如果是空猪圈，直接入圈
                pjuan[flag]->add_pig(p,pjuan[flag]->getjuanpig_num(),day,month);
                qDebug() << "购猪（空猪圈）"<< flag ;
                flag = (flag+1)%100;

            }
            else {//否则判断是否为黑猪圈，flag移到合适的位置
                if(pjuan[flag]->getjuanpig_num() >= 10)//一个猪圈最多10只猪
                    while(pjuan[flag]->getjuanpig_num() >= 10)
                        flag = (flag+1)%100;
                if(pjuan[flag]->getspecies(0) == 0 && p->species != 0){//黑猪只能进黑猪圈
                    while(pjuan[flag]->getspecies(0) == 0)
                        flag = (flag+1)%100;
                    qDebug() << "购猪，推移(花猪圈)"<< flag ;
                    pjuan[flag]->add_pig(p,pjuan[flag]->getjuanpig_num(),day,month);
                    flag = (flag+1)%100;
                }
                else if (pjuan[flag]->getspecies(0) != 0 && p->species == 0) {
                    while(pjuan[flag]->getspecies(0) ==1 || pjuan[flag]->getspecies(0) == 2)
                        flag = (flag+1)%100;
                    qDebug() << "购猪，推移(黑猪圈)"<< flag ;
                    pjuan[flag]->add_pig(p,pjuan[flag]->getjuanpig_num(),day,month);
                    flag = (flag+1)%100;
                }
                else if (pjuan[flag]->getspecies(0) == 0 && p->species == 0) {
                    qDebug() << "购猪，(黑猪圈)"<< flag ;
                    pjuan[flag]->add_pig(p,pjuan[flag]->getjuanpig_num(),day,month);
                    flag = (flag+1)%100;
                }
                else {
                    qDebug() << "购猪，(花猪圈)"<< flag ;
                    pjuan[flag]->add_pig(p,pjuan[flag]->getjuanpig_num(),day,month);
                    flag = (flag+1)%100;
                }
            }
        }
        allprice += sellprice;
        sellpriceyear[year] += sellprice;
        qDebug() << "这次卖了" << sellprice << "元,你现在一共有"<< allprice<<"元请继续加油哦";
        buypig[0][year] += black;
        buypig[1][year] += small;
        buypig[2][year] += big  ;
        qDebug() << "此次购入" << a << "头猪其中" << black << "只黑猪" << small << "只小花猪" << big << "只大花白猪" ;
        ui->mainlabel_right->setText(tr( "这次卖了%1元\n此次购入%2头猪\n其中%3只黑猪崽儿\n%4只小花猪崽儿\n%5只大花白猪崽儿" )
                                     .arg(sellprice).arg(a).arg(black).arg(small).arg(big));
        ui->mainlabel_up->setText(tr( "现在是%1年 %2月  %3日\n 金钱：%4元" ).arg( year ).arg( month ).arg( day ).arg(allprice));
        sellprice =0;
        fakemonth -= 3;
    }

}
MainWindow::~MainWindow()
{
    delete ui;
    //delete []pjuan; qt对象树构造特性，只要继承于QWeidge就不用写delete，自动delete
}

int MainWindow::getday()
{
    return day;
}

void MainWindow::chushihuachart()
{
    QBarSet *set0 = new QBarSet("体重");
//
    *set0 << weight0 << weight1 << weight2 ;
//
    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
    series->setLabelsVisible(true);
//    series->append(set1);

    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("三种猪的体重情况");
    chart->setAnimationOptions(QChart::NoAnimation);

    QStringList categories;
    categories << "黑猪" << "小花猪" << "大花白猪" ;
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart->createDefaultAxes();//创建默认的左侧的坐标轴（根据 QBarSet 设置的值）
    chart->setAxisX(axis, series);//设置坐标轴

    chart->legend()->setVisible(true); //设置图例为显示状态
    chart->legend()->setAlignment(Qt::AlignBottom);//设置图例的显示位置在底部
    ui->chartweight->setChart(chart);
}

void MainWindow::chushihuachart1()
{
    QBarSet *set0 = new QBarSet("个数");
    *set0 << all0 << all1 << all2 ;
    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
    series->setLabelsVisible(true);//设置serier的值在柱状图上显示

    chart1 = new QChart();
    chart1->addSeries(series);
    chart1->setTitle("三种猪的个数");
    chart1->setAnimationOptions(QChart::NoAnimation);


    QStringList categories;
    categories << "黑猪" << "小花猪" << "大花白猪";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart1->createDefaultAxes();//创建默认的左侧的坐标轴（根据 QBarSet 设置的值）
    chart1->setAxisX(axis, series);//设置坐标轴
//![4]

//![5]
    chart1->legend()->setVisible(true); //设置图例为显示状态
    chart1->legend()->setAlignment(Qt::AlignBottom);//设置图例的显示位置在底部
    ui->chartnum->setChart(chart1);
}

void MainWindow::chushihuachart2()
{
    QBarSet *set0 = new QBarSet("饲养分布情况");
    getfeedtime_all();
    if(feed6+feed9+feed12 == 0)
        *set0 << feed3 ;
    else if (feed6!= 0 &&feed9 == 0) {
        *set0 << feed3 << feed6;
    }
    else if (feed9!= 0&& feed12==0) {
        *set0 << feed3 << feed6 <<feed9;
    }
    else
        *set0 << feed3 << feed6 <<feed9 << feed12;

    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
    series->setLabelsVisible(true);

    chart2 = new QChart();
    chart2->addSeries(series);
    chart2->setTitle("饲养分布情况");
    chart2->setAnimationOptions(QChart::NoAnimation);


    QStringList categories;
    if(feed6+feed9+feed12 == 0)
        categories << "0-3个月" ;
    else if (feed6!= 0 &&feed9 == 0) {
        categories << "0-3个月"<< "3-6个月" ;
    }
    else if (feed9!= 0&& feed12==0) {
        categories << "0-3个月"<< "3-6个月" << "6-9个月";
    }
    else
        categories << "0-3个月"<< "3-6个月" << "6-9个月"<<"9-12个月";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart2->createDefaultAxes();//创建默认的左侧的坐标轴（根据 QBarSet 设置的值）
    chart2->setAxisX(axis, series);//设置坐标轴

    chart2->legend()->setVisible(true); //设置图例为显示状态
    chart2->legend()->setAlignment(Qt::AlignBottom);//设置图例的显示位置在底部
    ui->charfeed->setChart(chart2);
}

void MainWindow::getfeedtime_all()
{
    feed3 = 0;feed6 = 0;
    feed9 = 0;feed12 = 0;
    for(int i=0;i<100;i++){
        for(int j = 0;j<pjuan[i]->getjuanpig_num();j++){
            switch (pjuan[i]->getczmonth(j)) {
            case 0:case 1: case 2:
            {feed3++;break;}
            case 3:case 4:case 5:
            {feed6++;break;}
            case 6:case 7:case 8:
            {feed9++;break;}
            default:
            {feed12++;break;}
            }
        }
    }
}

void MainWindow::qt_write_txt()
{
    QFile f("S:\\qt\\Examples\\xiaobaichengxu\\mypig\\123.csv");
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << ("打开文件失败");
    }
    QTextStream txtOutput(&f);
    QString str = "年份,销售额,黑猪售出,黑猪购入,小花猪售出,小花猪购入,大花白猪售出,大花白猪购入\n";
    for(int i = 0;i<5;i++)
    str += QString::number(i)+",  "+QString::number(sellpriceyear[i])+",    "+QString::number(sellpig[0][i])+",     "
            +QString::number(buypig[0][i])+",        "+QString::number(sellpig[1][i])+",        "+
            QString::number(buypig[1][i])+",      "+QString::number(sellpig[2][i])+",        "+QString::number(buypig[2][i])+"\n";
    txtOutput << str << endl;
    f.close();
}

void MainWindow::qt_read_txt()
{
    QFile f("S:\\qt\\Examples\\xiaobaichengxu\\mypig\\123.csv");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << ("打开文件失败");
    }
    QTextStream txtInput(&f);
    QString lineStr;
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        qDebug() << (lineStr);
    }
    f.close();
}

void MainWindow::save_game()//保存当前文件的最基本的重要数据，需要下一次打开时点击读取上次数据后可以使用
{
    QFile f("S:\\qt\\Examples\\xiaobaichengxu\\mypig\\456.txt");
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << ("打开文件失败");
    }
    int allpig = all0+all1+all2;
    int flag=0;//此flag非彼flag
    piglist *p = pjuan[0]->head;
    QTextStream txtOutput(&f);
    QString str = QString::number(day)+"\n" +QString::number(month)+"\n"+QString::number(year)+"\n"+QString::number(fakemonth)+"\n"+QString::number(allprice)+"\n"
            +QString::number(all0)+"\n"+QString::number(all1)+"\n"+QString::number(all2)+"\n"+QString::number(weight0)+"\n"+QString::number(weight1)+"\n"
            +QString::number(weight2)+"\n"+QString::number(feed3)+"\n"+QString::number(feed6)+"\n"+QString::number(feed9)+"\n"+QString::number(feed12)+"\n";

    for(int i=0;i<allpig;i++){//读现有的猪的个数
         while(p == NULL){
             flag++;
             p = pjuan[flag]->head;
         }
        str += QString::number(p->weight)+"\n"+QString::number(p->species)+"\n"+QString::number(p->czday)+"\n"+QString::number(p->czmonth)+"\n"+
                QString::number(p->inday)+"\n"+QString::number(p->inmonth)+"\n";
        if(p->next != NULL){
            p = p->next;
        }
        else {
            if(flag <99)
              flag++;
            p = pjuan[flag]->head;    
        }

    }
    ui->mainlabel_right->setText(tr( "保存当前游戏数据成功" ));
    txtOutput << str << endl;
    f.close();
}

void MainWindow::read_game()
{
    flag = 0;//部分初始化
    pass_day = 10;
    QFile f("S:\\qt\\Examples\\xiaobaichengxu\\mypig\\456.txt");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << ("打开文件失败");
    }
    QTextStream txtInput(&f);
    QString lineStr;
    lineStr = txtInput.readLine();
    day = lineStr.toInt();
    lineStr = txtInput.readLine();
    month = lineStr.toInt();
    lineStr = txtInput.readLine();
    year = lineStr.toInt();
    lineStr = txtInput.readLine();
    fakemonth = lineStr.toInt();
    lineStr = txtInput.readLine();
    allprice = lineStr.toInt();
    lineStr = txtInput.readLine();
    all0 = lineStr.toInt();
    lineStr = txtInput.readLine();
    all1 = lineStr.toInt();
    lineStr = txtInput.readLine();
    all2 = lineStr.toInt();
    lineStr = txtInput.readLine();
    weight0 = lineStr.toInt();
    lineStr = txtInput.readLine();
    weight1 = lineStr.toInt();
    lineStr = txtInput.readLine();
    weight2 = lineStr.toInt();
    lineStr = txtInput.readLine();
    feed3 = lineStr.toInt();
    lineStr = txtInput.readLine();
    feed6 = lineStr.toInt();
    lineStr = txtInput.readLine();
    feed9 = lineStr.toInt();
    lineStr = txtInput.readLine();
    feed12 = lineStr.toInt();
    for(int i;i<100;i++)//释放初始化的猪圈动态地址
      if(pjuan[i]->head == NULL)
            continue;
      else {
          piglist *p0 = pjuan[flag]->head,*p ;
          while(p0->next)
          {
              p = p0->next;
              delete p0;
              p0 =p;
          }
          delete p0;
         pjuan[i]->head = NULL;
      }
    int flag=0;
    int allpig = all0+all1+all2;
    for(int i =0;i<allpig;i++){
        piglist *p =new piglist;
        lineStr = txtInput.readLine();
        p->weight = lineStr.toInt();
        lineStr = txtInput.readLine();
        p->species = lineStr.toInt();
        lineStr = txtInput.readLine();
        p->czday = lineStr.toInt();
        lineStr = txtInput.readLine();
        p->czmonth = lineStr.toInt();
        lineStr = txtInput.readLine();
        p->inday = lineStr.toInt();
        lineStr = txtInput.readLine();
        p->inmonth = lineStr.toInt();
    if(pjuan[flag]->getspecies(0) == 3){//如果是空猪圈，直接入圈
        pjuan[flag]->add_pig(p,pjuan[flag]->getjuanpig_num(),p->inday,p->inmonth);
        flag = (flag+1)%100;
    }
    else {
        //piglist *p0;//否则判断是否为黑猪圈，flag移到合适的位置
        if(pjuan[flag]->getjuanpig_num() >= 10)//一个猪圈最多10只猪
            while(pjuan[flag]->getjuanpig_num() >= 10)
                flag = (flag+1)%100;
        if(pjuan[flag]->getspecies(0) == 0 && p->species != 0){//黑猪只能进黑猪圈
            while(pjuan[flag]->getspecies(0) == 0)
                flag = (flag+1)%100;
            pjuan[flag]->add_pig(p,pjuan[flag]->getjuanpig_num(),p->inday,p->inmonth);
            flag = (flag+1)%100;
        }
        else if (pjuan[flag]->getspecies(0) != 0 && p->species == 0) {
            while(pjuan[flag]->getspecies(0) == 1 || pjuan[flag]->getspecies(0) == 2)
                flag = (flag+1)%100;
            pjuan[flag]->add_pig(p,pjuan[flag]->getjuanpig_num(),p->inday,p->inmonth);
            flag = (flag+1)%100;
        }
        else if (pjuan[flag]->getspecies(0) == 0 && p->species == 0) {
           pjuan[flag]->add_pig(p,pjuan[flag]->getjuanpig_num(),p->inday,p->inmonth);
            flag = (flag+1)%100;
        }
        else {
            pjuan[flag]->add_pig(p,pjuan[flag]->getjuanpig_num(),p->inday,p->inmonth);
            flag = (flag+1)%100;
        }
      }
    }
    ui->mainlabel_right->setText(tr( "你拥有100个猪圈\n现在你有%4元\n%1只黑猪崽儿\n%2只小花猪崽儿\n%3只大花白猪崽儿\n"
                                     "只有在游戏时间进行中\n猪猪们才会增长体重\n每种品种的肉价\n会随着时间流逝随机波动\n挑选最好的时机出圈吧！" ).arg(all0).arg(all1).arg(all2).arg(allprice));
    ui->mainlabel_down->setText(tr("         市场价目表：\n 黑猪肉：%1    元/公斤\n 小花猪肉：%2    元/公斤\n 大花白猪肉：%3    元/公斤").arg(price0).arg(price1).arg(price2));
    ui->mainlabel_up->setText(tr( "现在是%1年 %2月  %3日\n 金钱：%4元" ).arg( year ).arg( month ).arg( day ).arg(allprice));
    chushihuachart();
    chushihuachart1();
    chushihuachart2();

    f.close();
}



void MainWindow::on_startpig_clicked()
{
    tm_startID = this->startTimer(500);
}
void MainWindow::on_stoppig_clicked()
{
    killTimer(tm_startID);
}
void MainWindow::on_sellpig_clicked()
{
    chujuangouzhu();
    chushihuachart();
    chushihuachart1();
    chushihuachart2();

}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == tm_startID){
        updateprogress();
        chushihuachart2();
        chushihuachart();
    }
}

void MainWindow::on_checkjuan_clicked()
{
    int juan_no ;
    juan_no = ui->juanpig_no->text().toInt();
    if(ui->juanpig_no->text().isEmpty())
        cout << "请输入0-99的猪圈编号";
    else
    pjuan[juan_no]->show_zhujuan(pjuan[juan_no]->getjuanpig_num());

}

void MainWindow::on_checkpig_clicked()
{
    int juan_no,pig_no;
    juan_no = ui->juanpig_no->text().toInt();
    pig_no = ui->pig_no->text().toInt();
    if(ui->juanpig_no->text().isEmpty()||ui->pig_no->text().isEmpty()){
        ui->checklabel_right->setText("查询失败！\n请正确输入猪圈编号(0-99)，和猪编号(0-9)");
    }
    else{
        qDebug() << juan_no << "号猪圈" << pig_no << "号猪"  ;
        cout;
        if(pig_no > pjuan[juan_no]->getjuanpig_num()-1){
            ui->checklabel_right->setText("查无此猪");
        }
        else{
            //pjuan[juan_no]->show_zhuzhu(pig_no);
            QImage *image = new QImage;
            if(pjuan[juan_no]->getspecies(pig_no) == 0){
                if(pjuan[juan_no]->getweight(pig_no) <=40){
                    image->load(":/pig_image/0_1.png");
                    ui->pig_image->setPixmap(QPixmap::fromImage(*image));
                }
                else if (pjuan[juan_no]->getweight(pig_no)>40&&pjuan[juan_no]->getweight(pig_no)<=90) {
                    image->load(":/pig_image/0_2.png");
                    ui->pig_image->setPixmap(QPixmap::fromImage(*image));
                }
                else if (pjuan[juan_no]->getweight(pig_no) >90) {
                    image->load(":/pig_image/0_3.png");
                    ui->pig_image->setPixmap(QPixmap::fromImage(*image));
                }
            }

            ui->checklabel_right->setText(pjuan[juan_no]->show_zhuzhu(pig_no));
        }

    }

}

void MainWindow::on_startgame_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pagemain);
}


void MainWindow::on_to_check_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pagecheck);
    ui->checklabel_right->setText("请输入猪圈编号(0-99)\n和猪编号(0-9)");
}

void MainWindow::on_back_main_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pagemain);
}

void MainWindow::on_back_start_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pagestart);
}

void MainWindow::on_searchbutton_clicked()
{
   qt_read_txt();
}

void MainWindow::on_savebutton_clicked()
{
    save_game();
}

void MainWindow::on_readbutton_clicked()
{
   read_game();
   ui->stackedWidget->setCurrentWidget(ui->pagemain);
}
