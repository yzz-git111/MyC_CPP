//#include "stdio.h"
//#include "stdlib.h"
//
//struct stu
//{
//	char name[10];
//	int age;
//	char ID[10];
//};  //�ṹ�����Ҫ�ӣ�
//
//
//int main(void)
//{
//	struct stu a = { "john",18,"1806425" };
//	printf("%d,%s,%s\n", a.age,a.ID,a.name);   // ���� . �����ʽṹ�����a�ĳ�Ա
//
//	struct stu* p = &a; //������һ������Ϊstruct stu��ָ��ָ��ṹ�����a
//	printf("%d,%s,%s\n", p->age, p->ID, p->name);//���ڽṹ��ָ��  Ҫʹ��->�����ʽṹ���Ա
//	printf("%d,%s,%s\n", (*p).age, (*p).ID, (*p).name); //pָ����a  *p==a
//
//	//�ܵ���˵ �����õ�ַ���ʽṹ��Ԫ����->����ʵ�����ʽṹ��Ԫ���� . 
//
//	system("pause");
//	return 0;
//}