//#include <stdio.h>
//#include <stdlib.h>
//
//void fun1(int *p,int arrlen)  //p==&arr[0]
//{
//	for (int j = 0; j < arrlen; j++)
//	{
//		printf("%d  ", p[j]); // p[]���±�����
//	}
//}
//
//void fun2(int* p,int arrlen) //p==&arr[0]
//{
//	for (int j = 0; j < arrlen; j++)
//	{
//		printf("%d  ", *(p+j)); //p+j��ʾ�׵�ַ����ƫ��  Ȼ���ٽ���* ȡ��ַ����
//	}
//}
//
//void fun3(int* p, int arrlen) 
//{
//	for (int j = 0; j < arrlen; j++)
//	{
//		printf("%d  ", *(p + j)); //p+j��ʾ�׵�ַ����ƫ��  Ȼ���ٽ���* ȡ��ַ����
//	}
//}
//
//
//void fun4(int (*p)[3]) //����ָ�� pָ����arr3 *p==arr3[0]
//{
//	for (int j = 0; j < 3; j++)
//	{
//		printf("%d  ",(*p)[j]); 
//	}
//}
//
//int main(void)
//{
//	//һά������Ϊ����
//	int arr1[5] = { 8,5,3,7,6 };
//	int arr2[7] = { 1,2,3,4,5,6,7 };
//	fun1(&arr1[0],sizeof(arr1)/sizeof(arr1[0]));  //���ݵ�������arr���׵�ַ
//	printf("\n");
//	fun2(arr2,sizeof(arr2) / sizeof(arr2[0]));    //���������� arr Ҳ�������������׵�ַ
//	printf("\n");
//
//	//��ά������Ϊ����
//	int arr3[2][3] = { 88,75,36,41,25,11 }; 
//	fun3(arr3[0], 6); // arr3[0]==&arr3[0][0]
//	printf("\n");
//	fun4(arr3);
//	system("pause");
//	return 0;
//}