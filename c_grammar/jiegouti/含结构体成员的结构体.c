#include "stdio.h"
#include "stdlib.h"

struct charofbook
{
	char name[20];
};

struct book
{
	char name[10];
	struct charofbook char1;  //结构体struct book里含有一个类型为结构体struct charofbook的变量char1
};  //结构体最后要加；

int main(void)
{
	struct book book1 = { "algorithm",{"algorithm_greedy"} };
	printf("%s , %s\n", book1.name, book1.char1.name);

	system("pause");
	return 0;
}