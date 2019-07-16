#include "ErrorList.h"
#include "init.h"
#include<iostream>
vm_register* registe_ptr;
char* code_ptr;
char * data_ptr;
unsigned* register_list[REG_NUM]{0};//�Ĵ�������
int(*ins_list[0xff])() {0};//ָ������
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
int init_list() {
	/*
	*��ʼ���Ĵ����ĵ�ַָ��
	*/
	register_list[0] = &registe_ptr->R0;
	register_list[1] = &registe_ptr->R1;
	register_list[2] = &registe_ptr->R2;
	register_list[3] = &registe_ptr->R3;
	register_list[4] = &registe_ptr->R4;
	register_list[5] = &registe_ptr->R5;
	register_list[6] = &registe_ptr->R6;
	register_list[7] = &registe_ptr->SP;
	register_list[8] = &registe_ptr->BP;
	register_list[9] = &registe_ptr->IP;

	/*
	*��ʼ��ָ�������
	*/
	ins_list[INS_MOV] = do_mov;
	ins_list[INS_LEA] = do_lea;
	ins_list[INS_INT] = do_int;
	return LVM_SUCCESS;
}
void init_all() {
	registe_ptr = init_register();
	code_ptr = init_code_segment();
	data_ptr = init_data_segement();
	init_list();
}