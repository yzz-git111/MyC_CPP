#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	//数组指针 即数组的指针；指针的类型是数组
	int arr[5] = { 8,7,2,4,5 };
	//int* p = &arr;  //“初始化”:“int *”与“int (*)[5]”的间接级别不同
	//表示arr的首地址可以 arr,可以 &arr[0];但是 &arr就需要数组指针
	int(*p)[5] = &arr; //理解：(*p)是指针  而所指的变量是int[] 数组类型的

	int arr2[3] = { 1,2 };
	int(*q)[3] = &arr2; //要跟着数据个数改变

	//p指向了arr  那么*p就是arr本身
	printf("%d,%d", arr[1], (*p)[1]);


	return 0;
}