//#include <stdio.h>
//#include <stdlib.h>
//
//int* func(void);
//
//int main(void)
//{
//	int* a = func(); //��������ֵ��ָ�� ����Ҫ��int *a��ס����ֵ
//	printf("%d,%d,%d\n", a[0], a[1], *(a + 2));
//	free(a);
//
//
//	system("pause");
//	return 0;
//}
//
////���ض��ֵ��ԭ������һ�οռ��׵�ַ
////ֻ���� malloc ������
//int* func() //����ֵ��ָ��  ��������Ҫ��Ϊint *
//{
//	int* p = (int*)malloc(sizeof(int)*3); //malloc������һ��12���ֽڵĿռ� ���Դ��3��int����������pָ�򷵻ص��׵�ַ
//	*p = 4;			//pΪ����Ŀռ���׵�ַ����*p��ȡ���޸ĵ�ַ�洢�ı�����ֵ
//	*(p + 1) = 5;
//	p[2] = 6;
//	return p; //���ص�p��һ��ָ��  ��ַ���͵�
//}