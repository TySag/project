MDCoreUpdate

1. 程序介绍：
程序提供3个源文件，main.F, complexproc.F和complexproc1.F，以及一个Makefile文件。
其中main.F 提供数据初始化，调用功能计算函数，结果验证。
complexproc.F 和complexproc1.F 是功能计算函数，目前文件内容一致，用于验证使用。
直接make可以生成可执行文件 main，直接运行可执行文件得出程序计时。

$ make
ifort -g -O2    -c -o main.o main.F
ifort -g -O2    -c -o complexproc1.o complexproc1.F
ifort -g -O2    -c -o complexproc.o complexproc.F
ifort main.o complexproc1.o complexproc.o -o main $ ./main 
 total time(s):    5.637655    
 verification: correct

2. 比赛规则：
a) 不得修改 main.F 和 complexprof1.F 这两个文件，若修改，本环节成绩直接作废。
b) 本测试程序在调用COMPLEXPROC函数时，参数传递值只是是一种常见情况，
选手需要根据程序逻辑进行优化，不得只针对传递的参数值做特定优化，否则本环节成绩作废。
c) 程序结果验证通过，成绩有效，否则本环节成绩作废。
d) 最终优化比较，以程序输出计算为评判依据。

