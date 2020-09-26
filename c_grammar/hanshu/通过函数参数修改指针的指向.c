//#include <stdio.h>
//#include <stdlib.h>
//
//void fun(int* *q) //p的类型为 int *  调用p的地址就要用二级指针int **  
//{
//	*q = NULL; //q指向了p  q==&p 而p里存的是&a  要改变p的指向 就要改变p存的地址信息  即改变*p
//}
//
//int main(void)
//{
//	int a = 2;
//	int* p = &a;
//	printf("%p\n", p);   //p指向a；p存储的就是a的地址
//	fun(&p);			//要修改p的指向 就要传递p的地址  让函数通过操作p的地址来改变p的值
//	printf("%p\n", p);
//	system("pause");
//	return 0;
//}