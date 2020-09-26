//#include <stdio.h>
//#include <stdlib.h>
//
//void fun(int* p)   // int *p=&a
//{
//	*p = 33;    //p指向a 那么*p == a.  *p=2 等价于 将a=2
//				//由此实现了通过函数修改外部变量的值（实际就是操作参数所对应的地址）
//}
//
//int main(void)
//{
//	int a = 2;
//	printf("a = %d\n",a);
//	fun(&a);	 //&a 是地址（指针类型的）  所以fun（）的形式参数也要是int *；
//	printf("a = %d\n",a);
//
//	system("pause");
//	return 0;
//}