#include"ExecuteInstruction.h"
#include"ErrorList.h"
#include"init.h"
#include<iostream>
#include<string>
#include<ios>
#include<sstream>
/*
*高位表示寄存器类型
*地位表示操作寄存器的长度
*/
int _ins_len(char c) {
	uint8_t high = (uint8_t)c / 0x10;//获取操作类型高位
	uint8_t low = (uint8_t)c % 0x10;
	if (!uasm_flag) {
		if (high == 0) {//操纵数据段
			registe_ptr->flag[14] = 0;
		}
		else if (high == 1) {//操纵栈段
			registe_ptr->flag[14] = 1;
		}
		else if (high == 2);
		else throw(LVM_EXECUTE_ERROR);

	}
	switch (low)
	{
	case '\x00': {
		if (uasm_flag)printf("R,R\n");
		return 2;
	}
	case '\x01': {
		if (uasm_flag)printf("R,i\n");
		return 5;
	}
	case '\x02': {
		if (uasm_flag) {
			if (high == 1) {
				printf("R,s[R]\n");
			}
			else {
				printf("R,[R]\n");
			}
		}
		return 2;
	}
	case '\x03': {
		if (uasm_flag) {
			if (high == 1)
				printf("s[R],R\n");
			else {
				printf("[R],R\n");
			}
		}
		return 2;
	}
	case '\x04': {
		if (uasm_flag)printf("[R],[R]\n");
		return 2;
	}
	case '\x05': {
		if (uasm_flag)printf("R\n");
		return 1;
	}
	case '\x06': {
		if (uasm_flag)printf("i\n");
		return 4;
	}
	case '\x07': {
		if (uasm_flag)printf("R,[R+i]\n");
		return 5;
	}
	case '\x08': {
		if (uasm_flag)printf("[R+i],R\n");
		return 5;
	}
	case '\x09': {
		if (uasm_flag) {
			if (high == 1)
				printf("s[R+R],R\n");
			else {
				printf("[R+R],R\n");
			}
		}
			
		return 3;
	}
	}
	throw(LVM_EXECUTE_ERROR);
	return -1;
}
/*
*获取指令长度
*/
int get_ins_len(char f, char s) {
	uint8_t f1 = (uint8_t)f;
	switch (f1)
	{
	case INS_MOV: {
		if (uasm_flag)printf("50 mov ");
		if (s <= '\x44')
		return	2+_ins_len(s);
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case INS_LEA: {
		if (uasm_flag)printf("51 lea ");
		if (s <= '\x44')
			return	2 + _ins_len(s);
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case INS_INT: {
		//
		if (uasm_flag)printf("E0 int i\n");
		return 2;
		break;
	}
	case INS_JMP:case INS_JZ:case INS_JNZ:case INS_JH:case INS_JL: {
		if (uasm_flag) {
			if (f == INS_JMP) 
				printf("40 jmp ");
			else if(f == INS_JZ)
				printf("41 jz ");
			else if (f == INS_JNZ)
				printf("42 jnz ");
			else if (f == INS_JH)
				printf("43 jh ");
			else if (f == INS_JH)
				printf("44 jl ");
		}
		return 2+_ins_len(s);
		break;
	}
	case INS_INC:case INS_DEC: {
		if (uasm_flag) {
			if (f == INS_INC)
				printf("30 inc R\n");
			else if (f == INS_DEC)
				printf("31 dec R\n ");
		}
		return 2;
		break;
	}
	case INS_CMP: {
		if (s <= '\x42')
		{
			if (uasm_flag) printf("32 cmp ");
			return	2 + _ins_len(s);
		}
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case INS_ADD: case INS_SUB:case INS_XOR:case INS_AND:case INS_OR: {
		if (s <= '\x42')
		{
			if (uasm_flag) {
				if (f == INS_ADD)
					printf("20 add ");
				else if (f == INS_SUB)
					printf("21 sub ");
				else if (f == INS_XOR)
					printf("22 xor ");
				else if (f == INS_AND)
					printf("23 and ");
				else if (f == INS_OR)
					printf("24 or ");
			}
			return	2 + _ins_len(s);
		}
	}
	case INS_NOT: {
		if (uasm_flag) printf("25 not R\n");
		return 2;
	}
	case INS_NOP: {
		if (uasm_flag) printf("5F nop\n");
		return 1;
	}
	case INS_PUSH: {
		if (uasm_flag) {
			printf("10 push ");
		}
		return 2 + _ins_len(s);
	}
	case INS_POP: {
		if (uasm_flag) {
			printf("11 pop R\n");
		}
		return 2;
	}
    default: {
		break;
	}
		
	}
	throw(LVM_EXECUTE_ERROR);
	return -1;
}
/*
*设置标志寄存器 三次重载
*/
int set_flag(char pram) {
	char * ptr = registe_ptr->flag;//获取flag指针
	if(pram== 0){
		ptr[0] = 1;
		return LVM_SUCCESS;
	}
	else if (pram > 0) {
		ptr[0] = 0;
		ptr[1] = 1;
		return LVM_SUCCESS;
	}else if(pram<0)
	{
		ptr[0] = 0;
		ptr[1] = 0;
		return LVM_SUCCESS;
	}
	return LVM_FAILD;
}
int set_flag(int16_t pram) {
	char * ptr = registe_ptr->flag;//获取flag指针
	if (pram == 0) {
		ptr[0] = 0;
		return LVM_SUCCESS;
	}
	else if (pram > 0) {
		ptr[0] = 1;
		ptr[1] = 1;
		return LVM_SUCCESS;
	}
	else if (pram<0)
	{
		ptr[0] = 1;
		ptr[1] = 0;
		return LVM_SUCCESS;
	}
	return LVM_FAILD;
}
int set_flag(unsigned pram) {
	int temp = (int) pram;
	char * ptr = registe_ptr->flag;//获取flag指针
	if (temp == 0) {//操作结果等于0位置0
		ptr[0] = 0;
		return LVM_SUCCESS;
	}
	else if (temp > 0) {//不等于0 0位置1
		ptr[0] = 1;
		ptr[1] = 1;
		return LVM_SUCCESS;

	}
	else if (temp<0)
	{
		ptr[0] = 1;
		ptr[1] = 0;
		return LVM_SUCCESS;
	}
	return LVM_FAILD;
}
/*
*执行mov指令
*/
int do_mov() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low= (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;

	uint8_t o_low = (uint8_t)code_ptr[registe_ptr->IP + 1] % 0x10;

	switch (o_low)//操作类型
	{
	case '\x00': {//mov R,R  ---->0
		//
		uint8_t highb = code_ptr[registe_ptr->IP + 3] / 0x10;
		if (low == 0) {
			char*Ra= (char *)register_list[high];
			char*Rb=(char*)register_list[highb];
			*Ra = *Rb;
			set_flag(*Ra);
			break;
		}else if (low==1)
		{
			int16_t*Ra = (int16_t *)register_list[high];
			int16_t*Rb = (int16_t *)register_list[highb];
			*Ra = *Rb;
			set_flag(*Ra);
			break;
		}else if(low == 2)
		{
			unsigned*Ra = (unsigned *)register_list[high];
			unsigned*Rb = (unsigned*)register_list[highb];
			*Ra = *Rb;
			set_flag(*Ra);
			break;
		}else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case '\x01': {//mov R,i   ----->1
		if (low == 0)//低位运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			char *R = (char *)register_list[high];
			*R= temp;
			set_flag(*R);
			break;
		}else if (low==1)//高位运算
		{
			unsigned temp = *(short*)(code_ptr+registe_ptr->IP + 2 + 1);//获得立即数
			int16_t *R = (int16_t *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}else if(low ==2)//全运算
		{
			unsigned temp = *(unsigned*)(code_ptr+registe_ptr->IP + 2 + 1);//获得立即数
			unsigned *R = (unsigned *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}
		else throw(LVM_EXECUTE_ERROR);
			break;
	}
	case '\x02': {//R,[R] ----->2
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		char *data_temp_p = data_ptr;
		if (registe_ptr->flag[14]==1) data_ptr = stack_ptr;//操控栈段
		if (low == 0)//低位运算
		{
			unsigned value = *(unsigned*)register_list[highb];//获得Rb寄存器中的值
			char temp = data_ptr[value];//获取数据段的内容
			char *R = (char *)register_list[high];
			*R = temp;
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//还原操作
			break;
		}
		else if (low == 1)//高位运算
		{
			unsigned value = *(unsigned*)register_list[highb];//获得Rb寄存器中的值
			int16_t temp = *(int16_t*)(data_ptr+value);//获取数据段的内容
			int16_t *R = (int16_t *)register_list[high];
			*R = temp;
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//还原操作
			break;
		}
		else if (low == 2)//全运算
		{
			unsigned value = *(unsigned*)register_list[highb];//获得Rb寄存器中的值
			unsigned temp = *(unsigned*)(data_ptr+value);//获取数据段的内容
			unsigned *R = (unsigned *)register_list[high];
			*R = temp;
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//还原操作
			break;
		}
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case '\x03': {//[R],R----->3
		uint8_t lowb = (uint8_t)code_ptr[registe_ptr->IP + 3] % 0x10;
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		char *data_temp_p = data_ptr;
		if (registe_ptr->flag[14] == 1) data_ptr = stack_ptr;//操控栈段
		if (lowb == 0)//低位运算
		{
			unsigned value = *(unsigned*)register_list[high];//获得Ra寄存器中的值
			char *temp = data_ptr+value;//获取数据指针
			char *R = (char *)register_list[highb];//获得Rb寄存器中的值
			*temp=*R;//写入
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//还原操作
			break;
		}
		else if (lowb == 1)//高位运算
		{
			unsigned value = *(unsigned*)register_list[high];//获得Ra寄存器中的值
			int16_t *temp = (int16_t*)(data_ptr + value);//获取数据指针
			int16_t *R = (int16_t*)register_list[highb];//获得Rb寄存器中的值
			*temp = *R;//写入
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//还原操作
			break;
		}
		else if (lowb == 2)//全运算
		{
			unsigned value = *(unsigned*)register_list[high];//获得Ra寄存器中的值
			unsigned *temp = (unsigned*)(data_ptr + value);//获取数据指针
			unsigned *R = (unsigned*)register_list[highb];//获得Rb寄存器中的值
			*temp = *R;//写入
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//还原操作
			break;
		}
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case '\x07':{//R, [R + i]----->07
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		short i = *(short*)(code_ptr + registe_ptr->IP + 5);//获取立即数

		char *data_temp_p = data_ptr;
		if (registe_ptr->flag[14] == 1) data_ptr = stack_ptr;//操控栈段
		if (low == 0)//低位运算
		{
			unsigned value = *(unsigned*)register_list[highb];//获得Rb寄存器中的值
			char temp = data_ptr[i+value];//获取数据段的内容
			char *R = (char *)register_list[high];
			*R = temp;
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//还原操作
			break;
		}
		else if (low == 1)//高位运算
		{
			unsigned value = *(unsigned*)register_list[highb];//获得Rb寄存器中的值
			int16_t temp = *(int16_t*)(data_ptr + i+value);//获取数据段的内容
			int16_t *R = (int16_t *)register_list[high];
			*R = temp;
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//还原操作
			break;
		}
		else if (low == 2)//全运算
		{
			unsigned value = *(unsigned*)register_list[highb];//获得Rb寄存器中的值
			unsigned temp = *(unsigned*)(data_ptr + i+value);//获取数据段的内容
			unsigned *R = (unsigned *)register_list[high];
			*R = temp;
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//还原操作
			break;
		}
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case '\x08': {//[R+i],R----->08
		uint8_t lowb = (uint8_t)code_ptr[registe_ptr->IP + 6] % 0x10;
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 6] / 0x10;
		short i = *(short*)(code_ptr + registe_ptr->IP + 4);//获取立即数

		char *data_temp_p = data_ptr;
		if (registe_ptr->flag[14] == 1) data_ptr = stack_ptr;//操控栈段
		if (lowb == 0)//低位运算
		{
			unsigned value = *(unsigned*)register_list[high];//获得Ra寄存器中的值
			char *temp = data_ptr + i+value;//获取数据指针
			char *R = (char *)register_list[highb];//获得Rb寄存器中的值
			*temp = *R;//写入
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//还原操作
			break;
		}
		else if (lowb == 1)//高位运算
		{
			unsigned value = *(unsigned*)register_list[high];//获得Ra寄存器中的值
			int16_t *temp = (int16_t*)(data_ptr + i+value);//获取数据指针
			int16_t *R = (int16_t*)register_list[highb];//获得Rb寄存器中的值
			*temp = *R;//写入
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//还原操作
			break;
		}
		else if (lowb == 2)//全运算
		{
			unsigned value = *(unsigned*)register_list[high];//获得Ra寄存器中的值
			unsigned *temp = (unsigned*)(data_ptr + i+value);//获取数据指针
			unsigned *R = (unsigned*)register_list[highb];//获得Rb寄存器中的值
			*temp = *R;//写入
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//还原操作
			break;
		}
		break;
	}
	case'\x09': {
		uint8_t lowb1 = (uint8_t)code_ptr[registe_ptr->IP + 3] % 0x10;
		uint8_t highb1 = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		uint8_t lowb = (uint8_t)code_ptr[registe_ptr->IP + 4] % 0x10;
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 4] / 0x10;
		char *data_temp_p = data_ptr;
		if (registe_ptr->flag[14] == 1) data_ptr = stack_ptr;//操控栈段
		if (lowb == 0)//低位运算
		{
			unsigned value = *(unsigned*)register_list[high];//获得Ra寄存器中的值
			unsigned value1 = *(unsigned*)register_list[highb1];//获得Ra1寄存器中的值
			char *temp = data_ptr + value+value1;//获取数据指针
			char *R = (char *)register_list[highb];//获得Rb寄存器中的值
			*temp = *R;//写入
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//还原操作
			break;
		}
		else if (lowb == 1)//高位运算
		{
			unsigned value = *(unsigned*)register_list[high];//获得Ra寄存器中的值
			unsigned value1 = *(unsigned*)register_list[highb1];//获得Ra1寄存器中的值
			int16_t *temp = (int16_t*)(data_ptr + value+value1);//获取数据指针
			int16_t *R = (int16_t*)register_list[highb];//获得Rb寄存器中的值
			*temp = *R;//写入
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//还原操作
			break;
		}
		else if (lowb == 2)//全运算
		{
			unsigned value = *(unsigned*)register_list[high];//获得Ra寄存器中的值
			unsigned value1 = *(unsigned*)register_list[highb1];//获得Ra1寄存器中的值
			unsigned *temp = (unsigned*)(data_ptr + value+value1);//获取数据指针
			unsigned *R = (unsigned*)register_list[highb];//获得Rb寄存器中的值
			*temp = *R;//写入
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//还原操作
			break;
		}
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	default: {
		throw(LVM_EXECUTE_ERROR);
		break;
	}
	}

	return LVM_SUCCESS;
}
/*
*执行lea指令
*/
int do_lea() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	switch (code_ptr[registe_ptr->IP + 1])//操作类型
	{
	case '\x00': {
		//
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		if (low == 0) {
			char*Ra = (char *)register_list[high];
			char*Rb = (char*)register_list[highb];
			*Ra = *Rb;
			break;
		}
		else if (low == 1)
		{
			int16_t*Ra = (int16_t *)register_list[high];
			int16_t*Rb = (int16_t *)register_list[highb];
			*Ra = *Rb;
			break;
		}
		else if (low == 2)
		{
			unsigned*Ra = (unsigned *)register_list[high];
			unsigned*Rb = (unsigned*)register_list[highb];
			*Ra = *Rb;
			break;
		}
		break;
	}
	case '\x01': {//
		if (low == 0)//低位运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			char *R = (char *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}
		else if (low == 1)//高位运算
		{
			unsigned temp = *(short*)(code_ptr+registe_ptr->IP + 2 + 1);//获得立即数
			int16_t *R = (int16_t *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}
		else if (low == 2)//全运算
		{
			unsigned temp = *(unsigned*)(code_ptr+registe_ptr->IP + 2 + 1);//获得立即数
			unsigned *R = (unsigned *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	default: {
		throw(LVM_EXECUTE_ERROR);
		break;
	}
	}
	return LVM_SUCCESS;
}
/*
*执行int指令
*/
int do_int() {
	switch ((char)registe_ptr->R0){
	case LVM_PRINT: {
		//
		int temp{ 0 };
		temp=printf("%s", data_ptr + registe_ptr->R3);
		registe_ptr->R0 = temp;
		break;
	}
	case LVM_EXIT: {
		//printf("\n虚拟机成功退出");
		exit(0);
	}
	}
	return LVM_SUCCESS;
}
/*
*指令执行选择器
*/
int do_ins(){
	if(ins_list[(uint8_t)code_ptr[registe_ptr->IP]]!=nullptr)
	ins_list[(uint8_t)code_ptr[registe_ptr->IP]]();
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*执行JMP
*/
int do_jmp() {
	if (code_ptr[registe_ptr->IP + 1] == 5) {//寄存器跳转
		uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
		unsigned adr = *register_list[high];
		registe_ptr->IP = adr;
	}else if (code_ptr[registe_ptr->IP + 1] == 6){//立即数跳转
		unsigned adr = *(unsigned*)(code_ptr+registe_ptr->IP + 2);
		registe_ptr->IP = adr;
		
	}else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*执行JZ
*/
int do_jz() {
	if (registe_ptr->flag[0] == 0){
		if (code_ptr[registe_ptr->IP + 1] == 5) {//寄存器跳转
			uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
			unsigned adr = *register_list[high];
			registe_ptr->IP = adr;
		}
		else if (code_ptr[registe_ptr->IP + 1] == 6) {//立即数跳转
			unsigned adr = *(unsigned*)(code_ptr + registe_ptr->IP + 2);
			registe_ptr->IP = adr;

		}
		else throw(LVM_EXECUTE_ERROR);
	}
	return LVM_SUCCESS;
}
/*
*执行JNZ
*/
int do_jnz() {
	if (registe_ptr->flag[0] == 1) {//判断发生跳转条件
		if (code_ptr[registe_ptr->IP + 1] == 5) {//寄存器跳转
			uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
			unsigned adr = *register_list[high];
			registe_ptr->IP = adr;
		}
		else if (code_ptr[registe_ptr->IP + 1] == 6) {//立即数跳转
			unsigned adr = *(unsigned*)(code_ptr+registe_ptr->IP + 2);
			registe_ptr->IP = adr;

		}else throw(LVM_EXECUTE_ERROR);
	}
	return LVM_SUCCESS;
}
/*
*执行JH
*/
int do_jh() {
	if (registe_ptr->flag[1] == 1) {
		if (code_ptr[registe_ptr->IP + 1] == 5) {//寄存器跳转
			uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
			unsigned adr = *register_list[high];
			registe_ptr->IP = adr;
		}
		else if (code_ptr[registe_ptr->IP + 1] == 6) {//立即数跳转
			unsigned adr = *(unsigned*)(code_ptr + registe_ptr->IP + 2);
			registe_ptr->IP = adr;

		}
		else throw(LVM_EXECUTE_ERROR);
	}
	return LVM_SUCCESS;
}
/*
*执行JL
*/
int do_jl() {
	if (registe_ptr->flag[1] == 0) {
		if (code_ptr[registe_ptr->IP + 1] == 5) {//寄存器跳转
			uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
			unsigned adr = *register_list[high];
			registe_ptr->IP = adr;
		}
		else if (code_ptr[registe_ptr->IP + 1] == 6) {//立即数跳转
			unsigned adr = *(unsigned*)(code_ptr + registe_ptr->IP + 2);
			registe_ptr->IP = adr;

		}
		else throw(LVM_EXECUTE_ERROR);
	}
	return LVM_SUCCESS;
}
/*
*执行INC
*/
int do_inc(){
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 1] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 1] % 0x10;
	switch (low)
	{
	case 0: {
		char*Ra = (char *)register_list[high];
		++*Ra;
		set_flag(*Ra);
		break;
	}
	case 1: {
		int16_t *Ra = (int16_t *)register_list[high];
		++*Ra;
		set_flag(*Ra);
		break;
	}
	case 2: {
		unsigned *Ra = (unsigned *)register_list[high];
		++*Ra;
		set_flag(*Ra);
		break;
	}
	default:
	{
		throw(LVM_EXECUTE_ERROR);
		break;
	}
	}
	return LVM_SUCCESS;
}
/*
*执行DEC
*/
int do_dec() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 1] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 1] % 0x10;
	switch (low)
	{
	case 0: {
		char*Ra = (char *)register_list[high];
		--*Ra;
		set_flag(*Ra);
		break;
	}
	case 1: {
		int16_t *Ra = (int16_t *)register_list[high];
		--*Ra;
		set_flag(*Ra);
		break;
	}
	case 2: {
		unsigned *Ra = (unsigned *)register_list[high];
		--*Ra;
		set_flag(*Ra);
		break;
	}
	default:
	{
		throw(LVM_EXECUTE_ERROR);
		break;
	}
	}
	return LVM_SUCCESS;
}
/*
*执行CMP
*/
int do_cmp() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	if (code_ptr[registe_ptr->IP + 1] == '\x00') {
	}
	else if (code_ptr[registe_ptr->IP + 1] == '\x01') {
		unsigned i = code_ptr[registe_ptr->IP + 3];
		set_flag(*register_list[high] - i);
	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*执行ADD
*/
int do_add() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1]==0)//0型命令
	{
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		if (low == 0) {
			char*Ra = (char *)register_list[high];
			char*Rb = (char*)register_list[highb];
			*Ra += *Rb;
			set_flag(*Ra);
		}
		else if (low == 1)
		{
			int16_t*Ra = (int16_t *)register_list[high];
			int16_t*Rb = (int16_t *)register_list[highb];
			*Ra += *Rb;
			set_flag(*Ra);
		}
		else if (low == 2)
		{
			unsigned*Ra = (unsigned *)register_list[high];
			unsigned*Rb = (unsigned*)register_list[highb];
			*Ra += *Rb;
			set_flag(*Ra);
		}else throw(LVM_EXECUTE_ERROR);
	}
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1型命令
		if (low == 0)//低位运算
		{
			unsigned temp = *(unsigned*)(code_ptr+registe_ptr->IP + 2 + 1);//获得立即数
			char *R = (char *)register_list[high];
			*R += temp;
			set_flag(*R);
		}
		else if (low == 1)//高位运算
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//获得立即数
			int16_t *R = (int16_t *)register_list[high];
			*R += temp;
			set_flag(*R);
		}
		else if (low == 2)//全运算
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//获得立即数
			unsigned *R = (unsigned *)register_list[high];
			*R += temp;
			set_flag(*R);
		}
		else throw(LVM_EXECUTE_ERROR);

	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*执行SUB
*/
int do_sub() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1] == 0)//0型命令
	{
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		if (low == 0) {
			char*Ra = (char *)register_list[high];
			char*Rb = (char*)register_list[highb];
			*Ra -= *Rb;
			set_flag(*Ra);
		}
		else if (low == 1)
		{
			int16_t*Ra = (int16_t *)register_list[high];
			int16_t*Rb = (int16_t *)register_list[highb];
			*Ra -= *Rb;
			set_flag(*Ra);
		}
		else if (low == 2)
		{
			unsigned*Ra = (unsigned *)register_list[high];
			unsigned*Rb = (unsigned*)register_list[highb];
			*Ra -= *Rb;
			set_flag(*Ra);
		}
		else throw(LVM_EXECUTE_ERROR);
	}
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1型命令
		if (low == 0)//低位运算
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//获得立即数
			char *R = (char *)register_list[high];
			*R -= temp;
			set_flag(*R);
		}
		else if (low == 1)//高位运算
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//获得立即数
			int16_t *R = (int16_t *)register_list[high];
			*R -= temp;
			set_flag(*R);
		}
		else if (low == 2)//全运算
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//获得立即数
			unsigned *R = (unsigned *)register_list[high];
			*R -= temp;
			set_flag(*R);
		}
		else throw(LVM_EXECUTE_ERROR);

	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*执行XOR
*/
int do_xor() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1] == 0)//0型命令
	{
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		if (low == 0) {
			char*Ra = (char *)register_list[high];
			char*Rb = (char*)register_list[highb];
			*Ra ^= *Rb;
			set_flag(*Ra);
		}
		else if (low == 1)
		{
			int16_t*Ra = (int16_t *)register_list[high];
			int16_t*Rb = (int16_t *)register_list[highb];
			*Ra ^= *Rb;
			set_flag(*Ra);
		}
		else if (low == 2)
		{
			unsigned*Ra = (unsigned *)register_list[high];
			unsigned*Rb = (unsigned*)register_list[highb];
			*Ra ^= *Rb;
			set_flag(*Ra);
		}
		else throw(LVM_EXECUTE_ERROR);
	}
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1型命令
		if (low == 0)//低位运算
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//获得立即数
			char *R = (char *)register_list[high];
			*R ^= temp;
			set_flag(*R);
		}
		else if (low == 1)//高位运算
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//获得立即数
			int16_t *R = (int16_t *)register_list[high];
			*R ^= temp;
			set_flag(*R);
		}
		else if (low == 2)//全运算
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//获得立即数
			unsigned *R = (unsigned *)register_list[high];
			*R ^= temp;
			set_flag(*R);
		}
		else throw(LVM_EXECUTE_ERROR);

	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*执行AND
*/
int do_and() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1] == 0)//0型命令
	{
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		if (low == 0) {
			char*Ra = (char *)register_list[high];
			char*Rb = (char*)register_list[highb];
			*Ra &= *Rb;
			set_flag(*Ra);
		}
		else if (low == 1)
		{
			int16_t*Ra = (int16_t *)register_list[high];
			int16_t*Rb = (int16_t *)register_list[highb];
			*Ra &= *Rb;
			set_flag(*Ra);
		}
		else if (low == 2)
		{
			unsigned*Ra = (unsigned *)register_list[high];
			unsigned*Rb = (unsigned*)register_list[highb];
			*Ra &= *Rb;
			set_flag(*Ra);
		}
		else throw(LVM_EXECUTE_ERROR);
	}
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1型命令
		if (low == 0)//低位运算
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//获得立即数
			char *R = (char *)register_list[high];
			*R &= temp;
			set_flag(*R);
		}
		else if (low == 1)//高位运算
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//获得立即数
			int16_t *R = (int16_t *)register_list[high];
			*R &= temp;
			set_flag(*R);
		}
		else if (low == 2)//全运算
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//获得立即数
			unsigned *R = (unsigned *)register_list[high];
			*R &= temp;
			set_flag(*R);
		}
		else throw(LVM_EXECUTE_ERROR);

	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*执行OR
*/
int do_or() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1] == 0)//0型命令
	{
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		if (low == 0) {
			char*Ra = (char *)register_list[high];
			char*Rb = (char*)register_list[highb];
			*Ra |= *Rb;
			set_flag(*Ra);
		}
		else if (low == 1)
		{
			int16_t*Ra = (int16_t *)register_list[high];
			int16_t*Rb = (int16_t *)register_list[highb];
			*Ra |= *Rb;
			set_flag(*Ra);
		}
		else if (low == 2)
		{
			unsigned*Ra = (unsigned *)register_list[high];
			unsigned*Rb = (unsigned*)register_list[highb];
			*Ra |= *Rb;
			set_flag(*Ra);
		}
		else throw(LVM_EXECUTE_ERROR);
	}
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1型命令
		if (low == 0)//低位运算
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//获得立即数
			char *R = (char *)register_list[high];
			*R |= temp;
			set_flag(*R);
		}
		else if (low == 1)//高位运算
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//获得立即数
			int16_t *R = (int16_t *)register_list[high];
			*R |= temp;
			set_flag(*R);
		}
		else if (low == 2)//全运算
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//获得立即数
			unsigned *R = (unsigned *)register_list[high];
			*R |= temp;
			set_flag(*R);
		}
		else throw(LVM_EXECUTE_ERROR);

	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*执行NOT
*/
int do_not(){
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 1] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 1] % 0x10;
	if (low == 0) {
		char*Ra = (char *)register_list[high];
		*Ra = ~*Ra;
		set_flag(*Ra);
	}
	else if (low == 1)
	{
		int16_t*Ra = (int16_t *)register_list[high];
		*Ra = ~*Ra;
		set_flag(*Ra);
	}
	else if (low == 2)
	{
		unsigned*Ra = (unsigned *)register_list[high];
		*Ra = ~*Ra;
		set_flag(*Ra);
	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*执行NOP
*/
int do_nop() {
	return LVM_SUCCESS;
}
int exectue_ins() {
	int ins_len;
	ins_len=get_ins_len(code_ptr[registe_ptr->IP], code_ptr[registe_ptr->IP + 1]);//获取指令长度 为地址指针递增做准备
	unsigned tempIP = registe_ptr->IP;
	if (ins_len > 0)
	{
		if (registe_ptr->R6 == registe_ptr->IP || registe_ptr->R7 == registe_ptr->IP)//陷入调试状态
		{
			lvm_debug(ins_len);
		}
		else do_ins();
	}
	else throw(LVM_EXECUTE_ERROR);
	if (tempIP == registe_ptr->IP)//检查是否发生跳转
	{
		registe_ptr->IP += ins_len;
	}
	else
	{
		if (debug_flag && registe_ptr->flag[15]) registe_ptr->R6 = registe_ptr->IP;
	}
		
	
	return LVM_SUCCESS;
}