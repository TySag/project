MDCoreUpdate

1. ������ܣ�
�����ṩ3��Դ�ļ���main.F, complexproc.F��complexproc1.F���Լ�һ��Makefile�ļ���
����main.F �ṩ���ݳ�ʼ�������ù��ܼ��㺯���������֤��
complexproc.F ��complexproc1.F �ǹ��ܼ��㺯����Ŀǰ�ļ�����һ�£�������֤ʹ�á�
ֱ��make�������ɿ�ִ���ļ� main��ֱ�����п�ִ���ļ��ó������ʱ��

$ make
ifort -g -O2    -c -o main.o main.F
ifort -g -O2    -c -o complexproc1.o complexproc1.F
ifort -g -O2    -c -o complexproc.o complexproc.F
ifort main.o complexproc1.o complexproc.o -o main $ ./main 
 total time(s):    5.637655    
 verification: correct

2. ��������
a) �����޸� main.F �� complexprof1.F �������ļ������޸ģ������ڳɼ�ֱ�����ϡ�
b) �����Գ����ڵ���COMPLEXPROC����ʱ����������ֵֻ����һ�ֳ��������
ѡ����Ҫ���ݳ����߼������Ż�������ֻ��Դ��ݵĲ���ֵ���ض��Ż������򱾻��ڳɼ����ϡ�
c) ��������֤ͨ�����ɼ���Ч�����򱾻��ڳɼ����ϡ�
d) �����Ż��Ƚϣ��Գ����������Ϊ�������ݡ�

