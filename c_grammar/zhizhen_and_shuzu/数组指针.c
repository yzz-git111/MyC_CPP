#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	//����ָ�� �������ָ�룻ָ�������������
	int arr[5] = { 8,7,2,4,5 };
	//int* p = &arr;  //����ʼ����:��int *���롰int (*)[5]���ļ�Ӽ���ͬ
	//��ʾarr���׵�ַ���� arr,���� &arr[0];���� &arr����Ҫ����ָ��
	int(*p)[5] = &arr; //��⣺(*p)��ָ��  ����ָ�ı�����int[] �������͵�

	int arr2[3] = { 1,2 };
	int(*q)[3] = &arr2; //Ҫ�������ݸ����ı�

	//pָ����arr  ��ô*p����arr����
	printf("%d,%d", arr[1], (*p)[1]);


	return 0;
}