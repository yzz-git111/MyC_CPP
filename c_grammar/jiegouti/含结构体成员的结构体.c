#include "stdio.h"
#include "stdlib.h"

struct charofbook
{
	char name[20];
};

struct book
{
	char name[10];
	struct charofbook char1;  //�ṹ��struct book�ﺬ��һ������Ϊ�ṹ��struct charofbook�ı���char1
};  //�ṹ�����Ҫ�ӣ�

int main(void)
{
	struct book book1 = { "algorithm",{"algorithm_greedy"} };
	printf("%s , %s\n", book1.name, book1.char1.name);

	system("pause");
	return 0;
}