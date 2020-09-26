//#include <stdio.h>
//#include <stdlib.h>
//
//int main(void)
//{
//	int* p = (int*)malloc(sizeof(int));	//malloc中（sizeof(int)）代表申请空间的大小为4B，int *表示将地址的类型转化为int*
//								//malloc函数申请内存中连续的空间，并返回空间首地址
//							 //用int类型的指针指向了返回空间的首地址
//	if (p == NULL) //判断申请空间是否成功
//	{
//		printf("fail!");
//	}
//
//	int* q = (int*)malloc(sizeof(int)*5); //申请了一个大小为5的一维数组空间
//
//	free(p);   //牢记要释放空间！！

//  system("pause");
//	return 0;
//}