#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	//��ά����ָ��
	int a[2][3] = { 6,8,1,5,4 };
	int* r = &a[0][0];
	int(*s)[3] = &a[0];
	int(*t)[2][3] = &a;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("%d  ", (*t)[i][j]);  //tָ����a����ô*t==a  ����*t������ά����a
		}
	}
	printf("\n");


	for (int i = 0; i < 6; i++)
	{
			printf("%d  ", *((*s)+i));  //	*s==a[0]==&a[0][0]
	}
	printf("\n");

	printf("%d\n", (*s)[0]);  //���6

	for (int i = 0; i < 6; i++)
	{
		printf("%d  ", *(&*r+i));  //	*r==a[0][0]
	}


	return 0;
}
