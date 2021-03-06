#include "mainwindow.h"
#include "ui_mainwindow.h"
#define cout qDebug() << "{" << __FILE__ << ":" << __LINE__ << "}"//方便debug

MainWindow::MainWindow(QWidget *parent) ://构造函数----------------------------------------------------------------------------------------
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("养猪模拟");
    this->setFixedSize(800,770);
    this->setWindowIcon(QIcon(":/pig_image/pigicon.png"));
    ui->stackedWidget->setFixedSize(800,770);
    day = 0;
    month = 0;
    year = 0;
    fakemonth = 0;
    flag = 0;
    sellprice = 0;
    isplaue = false;

    memset(sellpig,0,sizeof(sellpig));
    memset(buypig,0,sizeof(buypig));
    memset(sellpriceyear,0,sizeof(sellpriceyear));
    allprice = 100000 ;
    pass_day = 10;
    all0 = 0; all1 =0; all2 = 0; allplague = 0;
    weight0 =0;weight1 =0; weight2 =0;
    for(int i=0;i<100;i++){//初始化hogpen指针数组
        hogpen[i]=new zhujuan;
        hogpen[i]->setjuanpig_no(i);
        hogpen[i]->setspread(0);
    }
    for(int i=0;i<100;i++){//异常处理
        if(hogpen[i] == NULL)
             exit(1);
    }

    //第一次初始化，加点猪猪
    srand(time(NULL));
    QTime time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    price0 = qrand()% 10 +20;
    price1 = qrand()% 12 +8 ;
    price2 = qrand()%8  +8;
    maxprice0 = price0;
    maxprice1 = price1;
    maxprice2 = price2;
    int a = qrand()%20+10;//开始拥有随机10-30头猪
    qDebug() << "初始有" << a << "头猪" ;
    for(int i = 0; i < a;i++){
        piglist *p = new piglist;
        p->next = NULL;
        p->czday = 0;
        p->czmonth = 0;
        p->species = qrand()%3;
        p->weight = qrand()%30+10;
        hogpen[flag]->add_pig(p,hogpen[flag]->getjuanpig_num(),day,month);//初始化不用判断是否为黑猪圈
        if(hogpen[i]->getspecies(0) == 0){
            all0++;
            weight0 += p->weight;
            buypig[0][year]++;
        }
        else if (hogpen[i]->getspecies(0) == 1) {
            all1++;
             weight1 += p->weight;
             buypig[1][year]++;
        }
        else if (hogpen[i]->getspecies(0) == 2) {
            all2++;
             weight2 += p->weight;
             buypig[2][year]++;
        }
        flag++;
    }

    //瘟疫数组初始化
    for(int i=0;i<100;i++)
    {
        for(int j=0;j<10;j++){
            if(j<hogpen[i]->getjuanpig_num())
                hogpen[i]->setplague(j,0);
            else {
                hogpen[i]->setplague(j,-1);
            }
        }
    }

//    for(int i=0;i<100;i++)
//    {
//        for(int j=0;j<10;j++){
//            qDebug() << hogpen[i]->getisplague(j);
//        }
//        cout << i;
//    }


    ui->startgame->setStyleSheet("QPushButton{border-image: url(:/pig_image/start.png)}"
                                 "QPushButton:hover{border-image: url(:/pig_image/start_hover.png)}");
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
    ui->stoppig->setVisible(false);
    ui->geli->setVisible(false);
    ui->mainlabel_up->setText(tr( "现在是%1年 %2月  %3日\n 金钱：%4元" ).arg( year ).arg( month ).arg( day ).arg(allprice));
    ui->mainlabel_right->setText(tr( "你拥有100个猪圈\n开场你有10000元\n%1只黑猪崽儿\n%2只小花猪崽儿\n%3只大花白猪崽儿\n"
                                     "只有在游戏时间进行中\n猪猪们才会增长体重\n每种品种的肉价\n会随着时间流逝随机波动\n挑选最好的时机出圈吧！" ).arg(all0).arg(all1).arg(all2));
    ui->mainlabel_down->setText(tr("         市场价目表：\n 黑猪肉：%1        元/公斤\n 小花猪肉：%2      元/公斤\n 大花白猪肉：%3    元/公斤")
                                .arg(price0).arg(price1).arg(price2));
    ui->pig_image->setScaledContents(true);
    ui->end_image->setScaledContents(true);
    ui->title_lable->setScaledContents(true);
    QImage *image = new QImage;
    image->load(":/pig_image/title.png");
    ui->title_lable->setPixmap(QPixmap::fromImage(*image));
    setAutoFillBackground(true);
           QPalette palette = this->palette();
           palette.setBrush(QPalette::Window,
           QBrush(QPixmap(":/pig_image/background.png").scaled( // 缩放背景图.
                                       size(),
                                       Qt::IgnoreAspectRatio,
                                       Qt::SmoothTransformation))); // 使用平滑的缩放方式
          setPalette(palette); // 至此, 已给widget加上了背景图.

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
        ui->startpig->setVisible(true);
        ui->stoppig->setVisible(false);
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
        cout;
        QString str = "养猪场经过了" +QString::number(tm_now)+ "天，\n猪猪正在茁壮成长\n";
        for(int i=0;i<100;i++){
            hogpen[i]->pig_grow(pass_day,hogpen[i]->getjuanpig_num());//改时间
            for(int j=0;j<hogpen[i]->getjuanpig_num();j++){
              switch (hogpen[i]->getspecies(j)) {//记录此次购入猪的种类
                case 0:{
                    weight0 += hogpen[i]->getweight(j);
                    break;
                }
                case 1:{
                    weight1 += hogpen[i]->getweight(j);
                    break;
                }
                case 2:{
                    weight2 += hogpen[i]->getweight(j);
                    break;
               }

             }

           }
          if(isplaue == true && hogpen[i]->getspread()!=0){//专注本猪圈的传播，并且若传播成功改变周围猪圈传播模式
              int a = qrand()%100;
              cout << "a" <<a;
              if(hogpen[i]->getspread() == 2)//50%传播几率
                  for(int j = 0;hogpen[i]->getisplague(j)!=-1;j++){
                      if(hogpen[i]->getisplague(j) == 1)
                          continue;
                      a = qrand()%100;
                      if(a > 50){
                          hogpen[i]->setplague(j,1);
                          allplague++;
                          str += QString::number(i) + "号圈" + QString::number(j) +"号猪感染瘟疫\n";
                          cout << i <<"号圈" << j << "号猪因50%的几率感染了瘟疫" <<a;
                      }
                  }
              else if (hogpen[i]->getspread() == 1) {
                 for(int j = 0;hogpen[i]->getisplague(j)!=-1;j++){
                     a = qrand()%100;
                      cout << "a" <<a;
                     if(a > 65){
                         hogpen[i]->setplague(j,1);
                         allplague++;
                         str += QString::number(i) + "号圈" + QString::number(j) +"号猪感染瘟疫\n";
                         cout << i <<"号圈" << j << "号猪感染瘟疫" << a ;
                         if(i == 0 ){//对第一只瘟猪的猪圈传播模式作初始化
                             hogpen[i]->setspread(2);
                             if(hogpen[i+1]->getspread() == 0)
                                 hogpen[i+1]->setspread(1);
                         }
                         else if (i == 99) {
                             hogpen[i]->setspread(2);
                             if(hogpen[i-1]->getspread() == 0)
                                 hogpen[i-1]->setspread(1);
                         }
                         else {
                             if(hogpen[i+1]->getspread() == 0)
                                   hogpen[i+1]->setspread(1);
                             hogpen[i]->setspread(2);
                             if(hogpen[i-1]->getspread() == 0)
                                   hogpen[i-1]->setspread(1);
                         }
                     }
                 }
              }
             if(allplague >= all0+all1+all2){
                 ui->stackedWidget->setCurrentWidget(ui->pageend);
                 QImage *image = new QImage;
                 image->load(":/pig_image/zhuwen.png");
                 ui->end_image->setPixmap(QPixmap::fromImage(*image));
                 ui->endlabel_down->setText(tr("猪场开业后的第%1 年的 %2 月 %3日 所有的猪都感染了\n非洲瘟疫 \n游戏结束┭┮﹏┭┮").arg(year).arg(month).arg(day));
                 killTimer(tm_startID);
             }
           }
        }
        int a0,a1,a2;
        a0 = qrand()%30+1;
        a1 = qrand()%24+1;
        a2 = qrand()% 16+1;
        price0 = qrand()% a0 +20;
        price1 = qrand()% a1 +8 ;
        price2 = qrand()% a2 + 8;
        if (price0 > maxprice0)
            maxprice0 = price0;
        else if (price1 > maxprice1) {
            maxprice1 = price1;
        }
        else if (price2 > maxprice2) {
           maxprice2 = price2;
        }
        qDebug() << "时间过去了" << tm_now << "天";
        qDebug() << "现在是第"  << year  << "年，" << "第" << month << "个月，第" << day << "天" ;
        ui->mainlabel_right->setText(str);
       ui->mainlabel_down->setText(tr("         市场价目表：\n 黑猪肉：%1    元/公斤\n历史最高价：%4    元/公斤\n 小花猪肉：%2    元/公斤\n历史最高价：%5     元/公斤\n"
                                      " 大花白猪肉：%3    元/公斤\n历史最高价：    %6元/公斤").arg(price0).arg(price1).arg(price2).arg(maxprice0).arg(maxprice1).arg(maxprice2));
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
        QString str;
        for(int i=0;i<100;i++){
            sellprice   +=   hogpen[i]->returnprice_true(hogpen[i]->getjuanpig_num(),price0,price1,price2);
        }
        str += "此次卖猪一共可以卖"+QString::number(sellprice) + "元\n 是否要卖猪，当前市场价格仍在波动，或许等一等会卖出更好的价钱哦~\n";
        sellprice = 0;
        int pd = MyMessageBoxx("可以卖猪了！",str);
        if (pd == 1){
            for(int i=0;i<100;i++){
                sellprice   +=   hogpen[i]->returnprice(hogpen[i]->getjuanpig_num(),all0,all1,all2,price0,price1,price2);
            }
        }
        int a = qrand()%40+40;//每次新购入40-80头猪
        int black=0,small=0,big=0,buyprice=0;
        for(int i = 0;i < a;i++){
            piglist *p = new piglist;
            p->next = NULL;
            p->czday = 0;
            p->czmonth = 0;
            p->species = qrand()%3;
            p->weight = qrand()%30+20;
            switch (p->species) {//记录此次购入猪的种类
            case 0:{
                black++;
                all0++;
                weight0 += p->weight;
                buyprice += 1000;//黑猪幼崽600元
                break;
            }
            case 1:{
                small++;
                all1++;
                weight1 += p->weight;
                buyprice += 400;//小花猪幼崽400元
                break;
            }
            case 2:{
                big++;
                all2++;
                weight2 += p->weight;
                buyprice += 250;
                break;
            }

            }
            if(hogpen[flag]->getspecies(0) == 3){//如果是空猪圈，直接入圈
                hogpen[flag]->add_pig(p,hogpen[flag]->getjuanpig_num(),day,month);
                flag = (flag+1)%100;

            }
            else {//否则判断是否为黑猪圈，flag移到合适的位置
                if(hogpen[flag]->getjuanpig_num() >= 10)//一个猪圈最多10只猪
                    while(hogpen[flag]->getjuanpig_num() >= 10)
                        flag = (flag+1)%100;
                if(hogpen[flag]->getspecies(0) == 0 && p->species != 0){//黑猪只能进黑猪圈
                    while(hogpen[flag]->getspecies(0) == 0)
                        flag = (flag+1)%100;
                    hogpen[flag]->add_pig(p,hogpen[flag]->getjuanpig_num(),day,month);
                    flag = (flag+1)%100;
                }
                else if (hogpen[flag]->getspecies(0) != 0 && p->species == 0) {
                    while(hogpen[flag]->getspecies(0) ==1 || hogpen[flag]->getspecies(0) == 2)
                        flag = (flag+1)%100;
                    hogpen[flag]->add_pig(p,hogpen[flag]->getjuanpig_num(),day,month);
                    flag = (flag+1)%100;
                }
                else if (hogpen[flag]->getspecies(0) == 0 && p->species == 0) {
                    hogpen[flag]->add_pig(p,hogpen[flag]->getjuanpig_num(),day,month);
                    flag = (flag+1)%100;
                }
                else {
                    hogpen[flag]->add_pig(p,hogpen[flag]->getjuanpig_num(),day,month);
                    flag = (flag+1)%100;
                }
            }
        }
        for(int i=0;i<100;i++)//初始化
        {
            for(int j=0;j<10;j++){
                if(j<hogpen[i]->getjuanpig_num())
                    if(hogpen[i]->getisplague(j) == 1)
                        continue;
                    else {
                       hogpen[i]->setplague(j,0);
                    }
                else {
                    hogpen[i]->setplague(j,-1);
                }
            }
        }
        allprice += sellprice;
        allprice -= buyprice;

        sellpriceyear[year] += sellprice;
        buypig[0][year] += black;
        buypig[1][year] += small;
        buypig[2][year] += big  ;
        ui->mainlabel_right->setText(tr( "这次卖了%1元\n此次购入%2头猪\n其中%3只黑猪崽儿\n%4只小花猪崽儿\n%5只大花白猪崽儿\n共花费 %6 元" )
                                     .arg(sellprice).arg(a).arg(black).arg(small).arg(big).arg(buyprice));
        ui->mainlabel_up->setText(tr( "现在是%1年 %2月  %3日\n 金钱：%4元" ).arg( year ).arg( month ).arg( day ).arg(allprice));
        sellprice =0;
        fakemonth -= 3;
    }
    if(allprice <0 ){
        ui->stackedWidget->setCurrentWidget(ui->pageend);
        QImage *image = new QImage;
        image->load(":/pig_image/pochan.png");
        ui->end_image->setPixmap(QPixmap::fromImage(*image));
        ui->endlabel_down->setText(tr("猪场开业后的第%1 年的 %2 月 %3日，负债破产┭┮﹏┭┮").arg(year).arg(month).arg(day));
    }
    else if (allprice > 1000000) {
        ui->stackedWidget->setCurrentWidget(ui->pageend);
        QImage *image = new QImage;
        image->load(":/pig_image/baiwanfuweng.png");
        ui->end_image->setPixmap(QPixmap::fromImage(*image));
        ui->endlabel_down->setText(tr("恭喜你，成为了百万富翁，你的人生从此走上巅峰！！"));

    }
}
MainWindow::~MainWindow()
{
    delete ui;
    //delete []hogpen; qt对象树构造特性，只要继承于QWeidge就不用写delete，自动delete
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
    if(isplaue == false)
    *set0 << all0 << all1 << all2 ;
    else {
       *set0 << all0 << all1 << all2 << allplague;
    }
    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
    series->setLabelsVisible(true);//设置serier的值在柱状图上显示

    chart1 = new QChart();
    chart1->addSeries(series);
    chart1->setTitle("三种猪的个数");
    chart1->setAnimationOptions(QChart::NoAnimation);


    QStringList categories;
    if (isplaue == false)
    categories << "黑猪" << "小花猪" << "大花白猪";
    else {
       categories << "黑猪" << "小花猪" << "大花白猪"<<"瘟猪";
    }
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    chart1->createDefaultAxes();//创建默认的左侧的坐标轴（根据 QBarSet 设置的值）
    chart1->setAxisX(axis, series);//设置坐标轴

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
        for(int j = 0;j<hogpen[i]->getjuanpig_num();j++){
            switch (hogpen[i]->getczmonth(j)) {
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
    QFile f("S:\\qt\\Examples\\xiaobaichengxu\\mypig\\5yearlog.csv");
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
    QString str;
    QFile f("S:\\qt\\Examples\\xiaobaichengxu\\mypig\\5yearlog.csv");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << ("打开文件失败");
    }
    QTextStream txtInput(&f);
    QString lineStr;
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        str += lineStr + "\n";
        ui->pig_image->setText(str);
    }
    f.close();
}

void MainWindow::save_game()//保存当前文件的最基本的重要数据，需要下一次打开时点击读取上次数据后可以使用
{
    QFile f("S:\\qt\\Examples\\xiaobaichengxu\\mypig\\save.txt");
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << ("打开文件失败");
    }
    int allpig = all0+all1+all2;
    int flag=0;//此flag非彼flag
    piglist *p = hogpen[0]->head;
    QTextStream txtOutput(&f);
    QString str = QString::number(day)+"\n" +QString::number(month)+"\n"+QString::number(year)+"\n"+QString::number(fakemonth)+"\n"+QString::number(allprice)+"\n"
            +QString::number(all0)+"\n"+QString::number(all1)+"\n"+QString::number(all2)+"\n"+QString::number(weight0)+"\n"+QString::number(weight1)+"\n"
            +QString::number(weight2)+"\n"+QString::number(feed3)+"\n"+QString::number(feed6)+"\n"+QString::number(feed9)+"\n"+QString::number(feed12)+"\n";

    for(int i=0;i<allpig;i++){//读现有的猪的个数
         while(p == NULL){
             flag++;
             p = hogpen[flag]->head;
         }
        str += QString::number(p->weight)+"\n"+QString::number(p->species)+"\n"+QString::number(p->czday)+"\n"+QString::number(p->czmonth)+"\n"+
                QString::number(p->inday)+"\n"+QString::number(p->inmonth)+"\n";
        if(p->next != NULL){
            p = p->next;
        }
        else {
            if(flag <99)
              flag++;
            p = hogpen[flag]->head;
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
    QFile f("S:\\qt\\Examples\\xiaobaichengxu\\mypig\\save.txt");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << ("打开文件失败");
        ui->readbutton->setText("打开文件失败");
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
      if(hogpen[i]->head == NULL)
            continue;
      else {
          piglist *p0 = hogpen[flag]->head,*p ;
          while(p0->next)
          {
              p = p0->next;
              delete p0;
              p0 =p;
          }
          delete p0;
         hogpen[i]->head = NULL;
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
    if(hogpen[flag]->getspecies(0) == 3){//如果是空猪圈，直接入圈
        hogpen[flag]->add_pig(p,hogpen[flag]->getjuanpig_num(),p->inday,p->inmonth);
        flag = (flag+1)%100;
    }
    else {
        //piglist *p0;//否则判断是否为黑猪圈，flag移到合适的位置
        if(hogpen[flag]->getjuanpig_num() >= 10)//一个猪圈最多10只猪
            while(hogpen[flag]->getjuanpig_num() >= 10)
                flag = (flag+1)%100;
        if(hogpen[flag]->getspecies(0) == 0 && p->species != 0){//黑猪只能进黑猪圈
            while(hogpen[flag]->getspecies(0) == 0)
                flag = (flag+1)%100;
            hogpen[flag]->add_pig(p,hogpen[flag]->getjuanpig_num(),p->inday,p->inmonth);
            flag = (flag+1)%100;
        }
        else if (hogpen[flag]->getspecies(0) != 0 && p->species == 0) {
            while(hogpen[flag]->getspecies(0) == 1 || hogpen[flag]->getspecies(0) == 2)
                flag = (flag+1)%100;
            hogpen[flag]->add_pig(p,hogpen[flag]->getjuanpig_num(),p->inday,p->inmonth);
            flag = (flag+1)%100;
        }
        else if (hogpen[flag]->getspecies(0) == 0 && p->species == 0) {
           hogpen[flag]->add_pig(p,hogpen[flag]->getjuanpig_num(),p->inday,p->inmonth);
            flag = (flag+1)%100;
        }
        else {
            hogpen[flag]->add_pig(p,hogpen[flag]->getjuanpig_num(),p->inday,p->inmonth);
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

void MainWindow::initplague()
{
    isplaue = true;
    allplague += 1;
    int plague =0;//随机选一只猪得瘟疫，plague等于1的时候跳出循环
    int num0 = all0+all1+all2;
    int num = qrand()%(num0);
    if(num == 0)
        num++;//不能让0圈的猪成为第一头瘟猪，会有bug。。
    cout <<num;
    int flag,no;
    for (int i =0;i<100;i++){
        for(int j=0;j<10;j++){
            if(num == 0){
                hogpen[i]->setplague(j,1);
                flag = i;
                no =j;
                plague =1;
                break;
            }
            else if(hogpen[i]->getisplague(j) == 0){
                num--;
                if(j == hogpen[i]->getjuanpig_num()-1)
                    break;
            }
            else if (hogpen[i]->getisplague(j) == -1) {
                j = 10;
            }

        }
        if(plague == 1)
            break;
    }
    if(flag == 0 ){//对第一只瘟猪的猪圈传播模式作初始化
        hogpen[flag]->setspread(2);
        if(hogpen[flag+1]->getspread() == 0)
            hogpen[flag+1]->setspread(1);
    }
    else if (flag == 99) {
        hogpen[flag]->setspread(2);
        if(hogpen[flag-1]->getspread() == 0)
            hogpen[flag-1]->setspread(1);
    }
    else {
        if(hogpen[flag+1]->getspread() == 0)
              hogpen[flag+1]->setspread(1);
        hogpen[flag]->setspread(2);
        if(hogpen[flag-1]->getspread() == 0)
              hogpen[flag-1]->setspread(1);
    }
    QString str = "警告⚠\n";
    str += QString::number(flag) + "圈"  + QString::number(no) + "号猪，得了猪瘟。\n"
           "瘟猪肉不可出售\n每一次时间的流过\n相邻猪圈的猪有15%\n的概率被传染\n"
           "同一猪圈的猪有50%\n的几率被传染\n请尽快采取隔离措施!!";
    ui->mainlabel_right->setText(str);
    cout << flag << "号圈" << no << "号猪得了猪瘟" ;
    cout << hogpen[flag-1]->getspread();
    cout << hogpen[flag]->getspread();
    cout << hogpen[flag+1]->getspread();

        chushihuachart1();
}

int MainWindow::MyMessageBox(QString title, QString message)
{
     QMessageBox mymessage(QMessageBox::Information, title, message);
     QPushButton *btnYes = mymessage.addButton(("猪圈隔离"), QMessageBox::YesRole);
     QPushButton *btnNo = mymessage.addButton(("**猪栏隔离**"), QMessageBox::NoRole);
     QPushButton *btnrej = mymessage.addButton(("返回"), QMessageBox::RejectRole);
     mymessage.resize(400,300);
     mymessage.exec();
     if ((QPushButton*)mymessage.clickedButton() == btnYes)
             return 1;
     else if ((QPushButton*)mymessage.clickedButton() == btnNo) {
         return 0;
     }
     else if((QPushButton*)mymessage.clickedButton() == btnrej){
        return -1;
     }
     return -1;
}

int MainWindow::MyMessageBoxx(QString title, QString message)
{
    QMessageBox mymessage(QMessageBox::Information, title, message);
    QPushButton *btnYes = mymessage.addButton(("卖猪！"), QMessageBox::YesRole);
    QPushButton *btnNo = mymessage.addButton(("不卖了，再等等"), QMessageBox::NoRole);
    mymessage.resize(400,300);
    mymessage.exec();
    if ((QPushButton*)mymessage.clickedButton() == btnYes)
            return 1;
    else if ((QPushButton*)mymessage.clickedButton() == btnNo) {
        return 0;
    }
    return 0;
}



void MainWindow::on_startpig_clicked()
{
    tm_startID = this->startTimer(500);
    ui->startpig->setVisible(false);
    ui->stoppig->setVisible(true);
}
void MainWindow::on_stoppig_clicked()
{
    killTimer(tm_startID);
    ui->startpig->setVisible(true);
    ui->stoppig->setVisible(false);
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
        cout;
        updateprogress();
        chushihuachart2();
        chushihuachart();
        chushihuachart1();
    }
}

void MainWindow::on_checkjuan_clicked()
{
    int juan_no ;
    QImage *image = new QImage;
    juan_no = ui->juanpig_no->text().toInt();
    if(ui->juanpig_no->text().isEmpty())
        cout << "请输入0-99的猪圈编号";
    else{
        hogpen[juan_no]->show_zhujuan(hogpen[juan_no]->getjuanpig_num());
        ui->checklabel_right->setText(hogpen[juan_no]->show_zhujuan(hogpen[juan_no]->getjuanpig_num()));
        image->load(":/pig_image/juan_01.png");
        ui->pig_image->setPixmap(QPixmap::fromImage(*image));
    }

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
        QImage *image = new QImage;
        if(pig_no > hogpen[juan_no]->getjuanpig_num()-1){
            ui->checklabel_right->setText("查无此猪");
            image->load(":/pig_image/none.png");
            ui->pig_image->setPixmap(QPixmap::fromImage(*image));
        }
        else{
            //hogpen[juan_no]->show_zhuzhu(pig_no);

            if(hogpen[juan_no]->getspecies(pig_no) == 0){
                if(hogpen[juan_no]->getweight(pig_no) <=40){
                    image->load(":/pig_image/0_1.png");
                    ui->pig_image->setPixmap(QPixmap::fromImage(*image));
                }
                else if (hogpen[juan_no]->getweight(pig_no)>40&&hogpen[juan_no]->getweight(pig_no)<=90) {
                    image->load(":/pig_image/0_2.png");
                    ui->pig_image->setPixmap(QPixmap::fromImage(*image));
                }
                else if (hogpen[juan_no]->getweight(pig_no) >90) {
                    image->load(":/pig_image/0_3.png");
                    ui->pig_image->setPixmap(QPixmap::fromImage(*image));
                }
            }
            else if (hogpen[juan_no]->getspecies(pig_no) == 1) {
                if(hogpen[juan_no]->getweight(pig_no) <=40){
                    image->load(":/pig_image/1_1.png");
                    ui->pig_image->setPixmap(QPixmap::fromImage(*image));
                }
                else if (hogpen[juan_no]->getweight(pig_no)>40&&hogpen[juan_no]->getweight(pig_no)<=90) {
                    image->load(":/pig_image/1_2.png");
                    ui->pig_image->setPixmap(QPixmap::fromImage(*image));
                }
                else if (hogpen[juan_no]->getweight(pig_no) >90) {
                    image->load(":/pig_image/1_3.png");
                    ui->pig_image->setPixmap(QPixmap::fromImage(*image));
                }
            }
            else if (hogpen[juan_no]->getspecies(pig_no) == 2) {
                if(hogpen[juan_no]->getweight(pig_no) <=40){
                    image->load(":/pig_image/2_2.png");
                    ui->pig_image->setPixmap(QPixmap::fromImage(*image));
                }
                else if (hogpen[juan_no]->getweight(pig_no)>40&&hogpen[juan_no]->getweight(pig_no)<=90) {
                    image->load(":/pig_image/2_1.png");
                    ui->pig_image->setPixmap(QPixmap::fromImage(*image));
                }
                else if (hogpen[juan_no]->getweight(pig_no) >90) {
                    image->load(":/pig_image/2_3.png");
                    ui->pig_image->setPixmap(QPixmap::fromImage(*image));
                }
            }


            ui->checklabel_right->setText(hogpen[juan_no]->show_zhuzhu(pig_no));
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
    ui->checklabel_right->setText("请在下方输入猪圈\n编号(0-99)\n和猪编号(0-9)");
    ui->pig_image->setText("请在下方输入猪圈编号(0-99)\n和猪编号(0-9)");
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

void MainWindow::on_plague_pig_clicked()
{
    qDebug() << "猪瘟模式启动！！！，有一只瘟猪加入了你的猪圈，并在传播瘟疫。";
    initplague();
    ui->geli->setVisible(true);
    ui->savebutton->setVisible(false);
    ui->savebutton_2->setVisible(false);
    ui->plague_pig->setVisible(false);
}

void MainWindow::on_geli_clicked()
{
    int fifteen=0,fifty=0;
    for(int i = 0;i<100;i++)
        if(hogpen[i]->getspread() == 1)
            fifteen++;
        else if (hogpen[i]->getspread() == 2) {
            fifty++;
        }
    int fifteencash,allcash,allfif;
    allfif = fifteen+fifty;
    fifteencash = fifteen*888;
    allcash = allfif*3888;
    QString str;
    str = "可以选择的隔离措施有：\n ***猪圈隔离***（瘟疫不会再传入设置隔离栏的猪圈）\n ***猪栏隔离***（瘟疫不再会在猪与猪之间传播）\n猪圈隔离栏：888元/个  猪栏隔离 3888元/圈\n";
    str += "瘟猪圈   " + QString::number(fifteen) + "个，共需要" + QString::number(fifteencash) + "元\n"
           "瘟猪栏   " + QString::number(allfif) + "个，共需要" + QString::number(allcash)+"元";
    int a = MyMessageBox("选择隔离措施",str);
    if (a == 1){
          for(int i = 0;i<100;i++){
             if(hogpen[i]->getspread() == 1)
                  hogpen[i]->setgeli();
          }
          allprice -= fifteencash;

    }
    else if(a == 0){
         for(int i = 0;i<100;i++){
             if(hogpen[i]->getspread() == 1 || hogpen[i]->getspread() == 2)
                  hogpen[i]->setgeli();
        }
        allprice -= allcash;
    }
    if(allprice <0 ){
        ui->stackedWidget->setCurrentWidget(ui->pageend);
        QImage *image = new QImage;
        image->load(":/pig_image/pochan.png");
        ui->end_image->setPixmap(QPixmap::fromImage(*image));
        ui->endlabel_down->setText(tr("猪场开业后的第%1 年的 %2 月 %3日，负债破产┭┮﹏┭┮").arg(year).arg(month).arg(day));
    }
    ui->mainlabel_up->setText(tr( "现在是%1年 %2月  %3日\n 金钱：%4元" ).arg( year ).arg( month ).arg( day ).arg(allprice));
}

void MainWindow::on_closebutton_clicked()
{
    this->close();
}

void MainWindow::on_savebutton_2_clicked()
{
    save_game();
    ui->checklabel_right->setText("保存当前数据成功！");
}

void MainWindow::on_closebutton_2_clicked()
{
    this->close();
}
