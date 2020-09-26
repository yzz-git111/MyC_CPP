//#include <stdio.h>
//#include <stdlib.h>
//
//int fun1(void);  //函数声明  这样函数就可以不管位置的进行调用
//
//int main(void)
//{
//	//函数名即函数的地址。即fun()==&fun();
//	//调用时其实是对地址的内容操作  所以调用函数可以 fun（），也可以（&fun）（）
//	//调用的本质其实是  函数地址+（参数列表）
//	fun1();

//  system("pause");
//	return 0;
//}
//
//int fun1()
//{
//	printf("hello world!");
//	return 1;  //遇到return就跳出函数  下面的代码不执行
//				//return  还可以用于终止函数  用于无返回值的函数
//	printf("hello world!222");
//}
//
