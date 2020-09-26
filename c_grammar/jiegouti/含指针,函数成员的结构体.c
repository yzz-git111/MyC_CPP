//#include "stdio.h"
//#include "stdlib.h"
//
//struct stu1
//{
//	int* p;
//};  //结构体最后要加；
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
//	//c不允许函数类型的成员  c++可以
//	//但是可以定义一个函数指针成员
//	void(*p)(void);           //此处是函数指针
//};
//
//
//int main(void)
//{
//	struct stu1 a = { NULL };  //给结构体的指针成员*p初始化指向NULL  避免了野指针
//	int arr[5] = { 1,2,3,4,5 };
//	a.p = arr;			//让a的成员p指向arr的首地址  *a.p==arr[]
//	printf("%d,%d\n", a.p[0], *(a.p+3));  //这样就和利用指针访问数组没有差别
//
//	//当然p也可以指向一个堆区的空间（即利用malloc申请的空间）
//	//a.p = (int *)malloc(20);
//
//	struct stu2 b = { &fun };  //fun和&fun都可以 都表示函数的首地址
//	(b.p)();       //第二个()是指参数列表
//				//	函数调用的本质是  函数地址+参数列表   此处的函数地址就是（b.p）参数列表就是（）
//
//	system("pause");
//	return 0;
//}