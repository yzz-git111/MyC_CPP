//#include "stdio.h"
//#include "stdlib.h"
//
////在main外声明结构体  所有函数都可以使用
//struct stu
//{
//	char name[10];
//	int age;
//	char ID[10];
//};  //结构体最后要加；
//
////struct stu
////{
////	char name[10];
////	int age;
////	char ID[10];
////}stu2,stu3;  //也可以在声明结构体stu的时候直接声明两个结构体变量stu2 stu3
//
//int main(void)
//{	
//	struct stu stu1;   //定义了一个stu1的stu结构体
//	stu1.age = 78;		//给结构体的各个成员赋值
//						//对字符串类型的成员赋值要使用  strcpy(stu.name,"s1")！！！
//	printf("%d\n", stu1.age);
//
//
//	struct stu stu2 = { "john",18,"1806425" };//初始化了一个叫stu2的stu的结构体
//	printf("%s,%s", stu2.name, stu2.ID);
//
//
//	system("pause");
//	return 0;
//}