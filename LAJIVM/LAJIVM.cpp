#include<iostream>
#include<sstream>
#include"init.h"
#include"ErrorList.h"
#include"bin_read.h"
#include"ExecuteInstruction.h"
int main(int argc, char **argv) {
Begin:
	std::string str;
	std::string r_str;
	try { init_all(); }
	catch (int) {
		printf("��ʼ��ʧ��\n");
		goto End;
	}
	if (argc == 2) {
		r_str = argv[1];
		goto DOIT;
	}else if(argc == 3)
	{
		std::string temp = argv[1];
		if (temp == "d") {
			registe_ptr->R6 = 0;
			r_str = argv[2];
			goto DOIT;
		}else{
			printf("�����в�������\n");
			exit(1);
		}
	}
	printf("�������ʼ�����������������ļ�·��,����ctrl+z�˳���\n");
	if (!std::getline(std::cin,str)) exit(0);
	if (*str.begin() == 'd') {
		auto temp = str.find(' ', 1);
		if (temp != std::string::npos) {
			r_str = str.substr(temp + 1);
			registe_ptr->R6 = 0;
		}else r_str = str;
	}else r_str = str;
	DOIT:
	try { write_all(r_str); }
	catch (int){
		printf("�򿪶������ļ�ʧ��\n");
		goto End;
	}
	printf("ִ�ж������ļ�>>>>>>>>>>\n");
	try{
		while (registe_ptr->IP<=m_code_length) exectue_ins();
		goto Begin;
	}
	catch (int)
	{
		printf("���й����г���\n");
		goto End;
	}
End:
	return 0;
}