//#include <stdio.h>
//#include <stdlib.h>
//
//void fun(int* *q) //p������Ϊ int *  ����p�ĵ�ַ��Ҫ�ö���ָ��int **  
//{
//	*q = NULL; //qָ����p  q==&p ��p������&a  Ҫ�ı�p��ָ�� ��Ҫ�ı�p��ĵ�ַ��Ϣ  ���ı�*p
//}
//
//int main(void)
//{
//	int a = 2;
//	int* p = &a;
//	printf("%p\n", p);   //pָ��a��p�洢�ľ���a�ĵ�ַ
//	fun(&p);			//Ҫ�޸�p��ָ�� ��Ҫ����p�ĵ�ַ  �ú���ͨ������p�ĵ�ַ���ı�p��ֵ
//	printf("%p\n", p);
//	system("pause");
//	return 0;
//}