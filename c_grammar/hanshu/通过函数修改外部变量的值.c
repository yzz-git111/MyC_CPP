//#include <stdio.h>
//#include <stdlib.h>
//
//void fun(int* p)   // int *p=&a
//{
//	*p = 33;    //pָ��a ��ô*p == a.  *p=2 �ȼ��� ��a=2
//				//�ɴ�ʵ����ͨ�������޸��ⲿ������ֵ��ʵ�ʾ��ǲ�����������Ӧ�ĵ�ַ��
//}
//
//int main(void)
//{
//	int a = 2;
//	printf("a = %d\n",a);
//	fun(&a);	 //&a �ǵ�ַ��ָ�����͵ģ�  ����fun��������ʽ����ҲҪ��int *��
//	printf("a = %d\n",a);
//
//	system("pause");
//	return 0;
//}