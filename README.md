# mypig
# c++个人课设迭代记录，选题：养猪模拟
`
题目
养猪模拟
小明开了个养猪场，一共有100个猪圈（按照0-99编号），每个猪圈最多养10头猪（按照0到9编号）
，猪一共有三个品种：黑猪、小花猪和大花白猪，其中黑猪不能与其他两种猪放在一个猪圈里，会打架。
规定每3个月，养猪场要出圈一批猪，包括：体重超过150斤的和饲养超过1年的；同时补充一批猪崽儿（猪的品种、数量录入或者选择随机生成）。
试利用随机数方法构造猪崽儿的初始体重等信息（20-50kg），利用随机数方法按照秒或次数刷新每头猪随时间的体重增长函数（一秒/一次表示一天，一天增重0.0 ~1.2kg）。
试利用结构体链表形式管理猪圈的猪，为新进的猪自动分配猪圈（要求猪均匀分布在各个猪圈中），自动统计本次出圈的猪的总体售价（黑猪15元一斤，小花猪7元一斤，大花白猪6元一斤）；

（1）可任意查询当前某一猪圈的猪的数量和种类；

（2）可任意查询当前某一猪圈某头猪的状态信息；

（3）可任意统计当前猪场每个品种猪的数量和体重、饲养时间分布情况；

（4）可任意查询近5年猪的销售记录和猪崽儿的购入记录（记录用文件形式存储）；

（5）所有当前猪的信息使用文件形式保存，程序再启动时，自动读取养猪场信息并显示。

（6）模拟养猪场某一只猪得了猪瘟，猪瘟的扩展几率是，同一个猪圈的猪每天被传染几率是50%，相邻猪圈的猪每天被传染的几率是15%，不相邻的猪圈的猪不传染。编写程序模拟多久后养猪场的猪死光光。

（7）当发现有猪得了猪瘟已经发病后，请采取合理的隔离措施，进行风险控制。

提高部分：（1）可使用MFC 或者QT等IDE设计用户界面；（2）显示每个品种猪的数量和体重、饲养时间分布图；（3）可根据自己的理解，加入功能（比如转换为一个养猪游戏）。
`
## 2020-9-15
上传第一个不是很成熟的版本
### 2020-09-16 10:10:00  
`debug`
>>>问题：

如果删除猪的数量超过两个，就会程序异常报错。

>>原因：

移动指针p，p0在函数del_pig（piglist *p）内定义的是局部变量，没有办法改变chujuangouzhu（）函数里的移动指针地址

>解决方法：

将移动指针p，p0从局部变量改为了类的私有成员，解决了删除猪的时候以为指针的地址移到下一只猪而实际没有改变的问题。


### 2020-09-16 17:42:09
`debug`

>>>问题： 

1:保存数据如果猪的个数超过猪圈的个数就会程序异常，导致读取数据也会异常

2： 查询不存在的猪的时候会程序异常并退出

>>原因

1： 在pjuan【99】圈完成猪的数据读取后，又执行了一次flag++，导致下一次访问了pjuan【100】，超过指针数组的范围

2： 当时更改`show_zhuzhu();`为 `QString`类型时没有更改查询不存在的猪的时候的处理，导致访问空指针。

>解决办法

1：加一个if判断防止flag++到100

2：将查询不存在的猪的判断移出到查询按钮的槽函数里面，直接改变textedit的输出，与查询存在的猪形成正确的if（）else（）逻辑。

### 2020-09-18 23:36:20
 
 >>> 加入新功能，模拟瘟猪
 
### 2020-09-19 22:40:13
基本完成要求

### 2020-09-27 16:06:03
进行了进一步的界面优化，添加了游戏背景，和一些猪的图片。

### 2020-10-7 

* 增添游戏胜利条件：资产到达100万
* 出圈购猪时新增对话框询问用户是否卖猪
* 增添游戏结束条件： 资产为负
* 调整猪的成长函数，使其成长曲线更为合理。


