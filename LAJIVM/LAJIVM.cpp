#include<iostream>
#include"init.h"
#include"ErrorList.h"
#include"bin_read.h"
#include"ExecuteInstruction.h"
int main(int argc, char argv) {
Begin:
	std::string str;
	try { init_all(); }
	catch (int) {
		printf("��ʼ��ʧ��\n");
		goto End;
	}
	printf("��ʼ�����������������ļ�·��,����ctrl+z�˳���\n");
	if (!(std::cin >> str)) exit(0);
	try { write_all(); }
	catch (int){
		printf("�򿪶������ļ�ʧ��\n");
		goto End;
	}
	printf("***************************************************\n");
	printf("ִ�ж������ļ�>>>>>>>>>>\n\n");
	try{
		while (registe_ptr->IP<m_code_length) exectue_ins();
		goto Begin;
	}
	catch (int)
	{
		printf("���й����г���\n");
		goto End;
	}
	End: system("pause");
}