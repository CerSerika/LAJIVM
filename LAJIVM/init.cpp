#include "ErrorList.h"
#include "init.h"
#include<iostream>
vm_register* registe_ptr;
char* code_ptr;
char * data_ptr;
vm_register* init_register() {
	int temp{ sizeof(vm_register) };
	vm_register *ptr{ nullptr };
	ptr = (vm_register*)malloc(temp);
	if (ptr!=nullptr)
	{
		printf("�Ĵ�����ʼ���ɹ�!\n");
		memset(ptr, 0, temp);//�ڴ����
		return ptr;
	}
	else throw(LVM_RIGISTER_SEGEMENT_ERROR);
}
char* init_code_segment() {
	char *ptr{ nullptr };
	ptr = (char *)malloc(code_length);
	if (ptr != nullptr) {
		printf("����γ�ʼ���ɹ�!\n");
		memset(ptr, 0, code_length);//�ڴ����
		return ptr;
	}
	else throw(LVM_CODE_SEGEMENT_ERROR);
}
char* init_data_segement() {
	char *ptr{ nullptr };
	ptr = (char *)malloc(data_length);
	if (ptr != nullptr) {
		printf("���ݶγ�ʼ���ɹ�!\n");
		memset(ptr, 0, data_length);//�ڴ����
		return ptr;
	}
	else throw(LVM_DATA_SEGEMENT_ERROR);
}
void init_all() {
	registe_ptr = init_register();
	code_ptr = init_code_segment();
	data_ptr = init_data_segement();
}