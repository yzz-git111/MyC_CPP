//#include <stdio.h>
//#include <stdlib.h>
//
//void fun1(int *p,int arrlen)  //p==&arr[0]
//{
//	for (int j = 0; j < arrlen; j++)
//	{
//		printf("%d  ", p[j]); // p[]即下标运算
//	}
//}
//
//void fun2(int* p,int arrlen) //p==&arr[0]
//{
//	for (int j = 0; j < arrlen; j++)
//	{
//		printf("%d  ", *(p+j)); //p+j表示首地址进行偏移  然后再进行* 取地址内容
//	}
//}
//
//void fun3(int* p, int arrlen) 
//{
//	for (int j = 0; j < arrlen; j++)
//	{
//		printf("%d  ", *(p + j)); //p+j表示首地址进行偏移  然后再进行* 取地址内容
//	}
//}
//
//
//void fun4(int (*p)[3]) //数组指针 p指向了arr3 *p==arr3[0]
//{
//	for (int j = 0; j < 3; j++)
//	{
//		printf("%d  ",(*p)[j]); 
//	}
//}
//
//int main(void)
//{
//	//一维数组作为参数
//	int arr1[5] = { 8,5,3,7,6 };
//	int arr2[7] = { 1,2,3,4,5,6,7 };
//	fun1(&arr1[0],sizeof(arr1)/sizeof(arr1[0]));  //传递的是数组arr的首地址
//	printf("\n");
//	fun2(arr2,sizeof(arr2) / sizeof(arr2[0]));    //单个数组名 arr 也代表的是数组的首地址
//	printf("\n");
//
//	//二维数组作为参数
//	int arr3[2][3] = { 88,75,36,41,25,11 }; 
//	fun3(arr3[0], 6); // arr3[0]==&arr3[0][0]
//	printf("\n");
//	fun4(arr3);
//	system("pause");
//	return 0;
//}