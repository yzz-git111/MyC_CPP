//#include "stdio.h"
//#include "stdlib.h"
//
//struct stu1
//{
//	int* p;
//};  //�ṹ�����Ҫ�ӣ�
//
//
//void fun(void)
//{
//	printf("hello");
//}
//struct stu2
//{
//	/*void fun()
//	{
//		printf("hello");
//	}*/
//	//c�����������͵ĳ�Ա  c++����
//	//���ǿ��Զ���һ������ָ���Ա
//	void(*p)(void);           //�˴��Ǻ���ָ��
//};
//
//
//int main(void)
//{
//	struct stu1 a = { NULL };  //���ṹ���ָ���Ա*p��ʼ��ָ��NULL  ������Ұָ��
//	int arr[5] = { 1,2,3,4,5 };
//	a.p = arr;			//��a�ĳ�Աpָ��arr���׵�ַ  *a.p==arr[]
//	printf("%d,%d\n", a.p[0], *(a.p+3));  //�����ͺ�����ָ���������û�в��
//
//	//��ȻpҲ����ָ��һ�������Ŀռ䣨������malloc����Ŀռ䣩
//	//a.p = (int *)malloc(20);
//
//	struct stu2 b = { &fun };  //fun��&fun������ ����ʾ�������׵�ַ
//	(b.p)();       //�ڶ���()��ָ�����б�
//				//	�������õı�����  ������ַ+�����б�   �˴��ĺ�����ַ���ǣ�b.p�������б���ǣ���
//
//	system("pause");
//	return 0;
//}