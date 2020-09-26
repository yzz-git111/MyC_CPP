//#include <stdio.h>
//#include <stdlib.h>
//
//int* func(void);
//
//int main(void)
//{
//	int* a = func(); //函数返回值是指针 所以要用int *a接住返回值
//	printf("%d,%d,%d\n", a[0], a[1], *(a + 2));
//	free(a);
//
//
//	system("pause");
//	return 0;
//}
//
////返回多个值的原理：返回一段空间首地址
////只能用 malloc ！！！
//int* func() //返回值是指针  函数类型要变为int *
//{
//	int* p = (int*)malloc(sizeof(int)*3); //malloc申请了一个12个字节的空间 可以存放3个int变量。并用p指向返回的首地址
//	*p = 4;			//p为申请的空间的首地址，用*p读取和修改地址存储的变量的值
//	*(p + 1) = 5;
//	p[2] = 6;
//	return p; //返回的p是一个指针  地址类型的
//}