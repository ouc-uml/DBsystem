# DBsystem

## 功能
这一层实现了一个简单的小型关系数据库。

+ 提供了两个数据结构：元组和表，实现了在外存上的基本的数据库操作；
+ 提供了一个数据库管理工具，暂为雏形。

## 环境说明
+ Linux/Ubuntu 16.04

## 实现方法
### item （元组）类
元组类存放单个元组的信息；

+ 包含一个map，存放键—值对；
+ 其中键均为字符串，值可存放64个字节，可为无符号整型，也可为字符串；

### table.h : table ( 表 ) 类
表存放多个元组类的信息；

同时维护该表上的索引；

+ 表中含3个map，1个list；

	+ map_items为主体部分，存放 元组编号——元组名字 关系；
	+ map_index为索引链接，存放 具有索引的属性列——索引的名字；
	+ map_column为属性备忘，存放 属性名——属性类型（无符号整型或字符串）；
	+ list_empty为删除链，存放暂时被删除而空置的编号，用于提供下一个插入元组的编号。

+ 每个索引中包含一个map，值指向的元组编号则由一个list存放。即每个索引对应一个map与若干个list。

	+ map_表名+属性名 为索引链接，存放 值——对应该值的元组编号列；
	+ list_表名+属性名+值 为结果列，存放 对应某个值的元组编号。


## 函数使用说明
###  item.h : 

+ `item(const item & I)`

	+ 重构复制函数；

+ `item(unsigned char name[])`

	+ 新建一个名字为name的空元组，名字唯一标识一个元组；
	+ 若已存在则直接返回该元组；

+ `bool add_val(unsigned char k[],unsigned int n)`

	+ 添加键—值对，值为整型；
	+ 成功返回1；

+ `bool add_val(unsigned char k[],unsigned char s[])`

	+ 添加键—值对，值为字符串；
	+ 成功返回1；

+ `bool get_val(unsigned char k[],unsigned int &i)`

	+ 获取键为k的值到整型i上；
	+ 成功返回1；

+ `bool get_val(unsigned char k[],unsigned char s[])`

	+ 获取键为k的值字符串s上；
	+ 成功返回1；

+ `bool modify_val(unsigned char k[],int i)`

	+ 修改键为k的值为i；
	+ 该数据类型应为unsigned　int（但不检查）；
	+ 成功返回1; 

+ `bool modify_val(unsigned char k[],unsigned char s[])`

	+ 修改键为k的值为s，该数据类型应为字符串（但不检查）；
	+ 成功返回1；

+ `bool delete_val(unsigned char k[])`

	+ 将键为k的键值对删除；
	+ 成功返回1；

+ `void clear_all()`

	+ 清空该元组，但不删除；

+ `void delete_all()`

	+ 删除该元组；

___

### table.h : table ( 表 ) 类
+ `table(unsigned char name[])`

	+ 新建一个名字为name的空表，名字唯一标识一个表；
	+ 若已存在则直接返回该表；

+ `bool add_column(unsigned char s[]，char type)`

	+ 为所有元组在末尾添加名为s的新列；
	+ type为'd'添加unsigned int型，值为0，否则添加字符串，值为空；
	+ 属性编号即为添加顺序；
	+ 成功返回1；

+ `bool add_column(unsigned char s[]，char type，unsigned int ini)`

	+ 为所有元组在末尾添加名为s的新列；
	+ type为'd'添加unsigned int型，值为 ini，否则添加字符串，值为空；	
	+ 属性编号即为添加顺序；
	+ 成功返回1；

+ `bool add_column(unsigned char s[]，char type，unsigned char ini[])`

	+ 为所有元组在末尾添加名为s的新列；
	+ type为'd'添加unsigned int型，值为0，否则添加字符串，值为ini[]；	
	+ 属性编号即为添加顺序；
	+ 成功返回1；

+ `unsigned int add_item()`

	+ 添加新的空元组；
	+ 返回元组编号；

+ `unsigned int add_item(unsigned int len,...)`

	+ 添加新的空元组并编辑len个元组；
	+ 返回元组编号；

+ `unsigned int add_item(vector<string> v)`

	+ 添加新的空元组并编辑若干个元组；
	+ 数值也需要以字符串形式压入vector；
	+ 返回元组编号；

+ `vector<string> get_column\_s(unsigned char s[])`

	+ 返回名字为 s 的string型属性列；

+ `vector<unsigned int> get_column\_s(unsigned char s[])`

	+ 返回名字为 s 的unsigned int型属性列；

+ `int get_all\_no(unsigned int i[])`

	+ 获取现有的所有元组编号至数组 i 中；
	+ 返回元组个数；

+ `int get_all\_column(unsigned char i[][k_len])`

	+ 获取所有属性名至i中；
	+ 返回个数；

+ `char get_type(unsigned char i[])`

	+ 返回属性 i 的类型；
	+ 若 i 不存在返回'd'；

+ `item get_item(unsigned int i)`

	+ 获取编号为 i 的元组；
	+ 注意！不检查是否存在！

+ `bool set_index(unsigned int i)`

	+ 为第 i 列建立索引；
	+ 若该索引已建立则更新；
	+ 成功返回1；

+ `vector<unsigned int> find_by\_index(unsigned int i,unsigned val)`

	+ 在第i列上搜索值为val的元组；
	+ 返回满足条件的item的编号的vector；

+  `vector<unsigned int> find_by\_index(unsigned int i,unsigned char s[])`

	+ 在第i列上搜索包含s的元组；
	+ 返回满足条件的item的编号的vector；

+  `bool edit_item(unsigned int i)`

	+ 直接编辑第 i 条元组（键入）；
	+ 成功返回1；

+ `bool edit_item(unsigned int i,unsigned int len,...)`

	+ 直接编辑第 i 条元组的前len个元组（参数输入）；
	+ 成功返回1；

+ `bool edit_item(unsigned int i,unsigned int ii,vector<string> v)`

	+ 直接编辑第 i 条元组从 ii 个属性开始的若干个元组；
	+ 成功返回1；

+ `bool delete_item(unsigned int i)`

	+删除编号为 i 的元组；

+ `void delete_talbe()`

	+删除该表及表上所有元组，所有索引；

+ `void show_all()`
	
	+ 在屏幕上打印该表；

## 保留字表：
+ table名
+ table名+"_items"
+ table名+"_items\_index"
+ table名+"_items\_empty"
+ table名+"_column"
+ table名+"_column\_type"
+ table名+"_index"
+ table名+列名
+ table名+列名+列值

+ item名
+ item名+“_l”
