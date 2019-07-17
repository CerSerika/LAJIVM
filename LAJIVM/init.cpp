#include "ErrorList.h"
#include "init.h"
#include<iostream>
vm_register* registe_ptr;
char* code_ptr;
char * data_ptr;
unsigned* register_list[REG_NUM]{0};//寄存器数组
int(*ins_list[0xff])() {0};//指令数组
vm_register* init_register() {
	int temp{ sizeof(vm_register) };
	vm_register *ptr{ nullptr };
	ptr = (vm_register*)malloc(temp);
	if (ptr!=nullptr)
	{
		printf("寄存器初始化成功!\n");
		memset(ptr, 0, temp);//内存清空
		ptr->R6 = unsigned(-1);//调试寄存器值0xffffffff
		return ptr;
	}
	else throw(LVM_RIGISTER_SEGEMENT_ERROR);
}
char* init_code_segment() {
	char *ptr{ nullptr };
	ptr = (char *)malloc(code_length);
	if (ptr != nullptr) {
		printf("代码段初始化成功!\n");
		memset(ptr, 0, code_length);//内存清空
		return ptr;
	}
	else throw(LVM_CODE_SEGEMENT_ERROR);
}
char* init_data_segement() {
	char *ptr{ nullptr };
	ptr = (char *)malloc(data_length);
	if (ptr != nullptr) {
		printf("数据段初始化成功!\n");
		memset(ptr, 0, data_length);//内存清空
		return ptr;
	}
	else throw(LVM_DATA_SEGEMENT_ERROR);
}
int init_list() {
	/*
	*初始化寄存器的地址指针
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
	*初始化指令函数数组
	*/
	ins_list[INS_MOV] = do_mov;
	ins_list[INS_LEA] = do_lea;
	ins_list[INS_INT] = do_int;
	ins_list[INS_JMP] = do_jmp;
	ins_list[INS_JZ] = do_jz;
	ins_list[INS_JNZ] = do_jnz;
	ins_list[INS_JH] = do_jh;
	ins_list[INS_JL] = do_jl;
	ins_list[INS_INC] = do_inc;
	ins_list[INS_DEC] = do_dec;
	ins_list[INS_CMP] = do_cmp;
	return LVM_SUCCESS;
}
void init_all() {
	registe_ptr = init_register();
	code_ptr = init_code_segment();
	data_ptr = init_data_segement();
	init_list();
}