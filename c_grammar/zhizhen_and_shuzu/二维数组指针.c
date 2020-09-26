#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	//二维数组指针
	int a[2][3] = { 6,8,1,5,4 };
	int* r = &a[0][0];
	int(*s)[3] = &a[0];
	int(*t)[2][3] = &a;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("%d  ", (*t)[i][j]);  //t指向了a，那么*t==a  利用*t遍历二维数组a
		}
	}
	printf("\n");


	for (int i = 0; i < 6; i++)
	{
			printf("%d  ", *((*s)+i));  //	*s==a[0]==&a[0][0]
	}
	printf("\n");

	printf("%d\n", (*s)[0]);  //输出6

	for (int i = 0; i < 6; i++)
	{
		printf("%d  ", *(&*r+i));  //	*r==a[0][0]
	}


	return 0;
}
