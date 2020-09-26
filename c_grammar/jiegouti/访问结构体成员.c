//#include "stdio.h"
//#include "stdlib.h"
//
//struct stu
//{
//	char name[10];
//	int age;
//	char ID[10];
//};  //结构体最后要加；
//
//
//int main(void)
//{
//	struct stu a = { "john",18,"1806425" };
//	printf("%d,%s,%s\n", a.age,a.ID,a.name);   // 利用 . 来访问结构体变量a的成员
//
//	struct stu* p = &a; //定义了一个类型为struct stu的指针指向结构体变量a
//	printf("%d,%s,%s\n", p->age, p->ID, p->name);//对于结构体指针  要使用->来访问结构体成员
//	printf("%d,%s,%s\n", (*p).age, (*p).ID, (*p).name); //p指向了a  *p==a
//
//	//总的来说 就是用地址访问结构体元素用->，用实例访问结构体元素用 . 
//
//	system("pause");
//	return 0;
//}