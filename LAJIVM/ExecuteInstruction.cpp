#include"ExecuteInstruction.h"
#include"ErrorList.h"
#include"init.h"
#include<iostream>
#include<string>
#include<ios>
#include<sstream>
bool debug_flag{ false };
bool uasm_flag{ false };
/*
*��λ��ʾ�Ĵ�������
*��λ��ʾ�����Ĵ����ĳ���
*/
int _ins_len(char c) {
	switch (c)
	{
	case '\x00': {
		if (uasm_flag )printf("R,R\n");
		return 2;
	}
	case '\x01': {
		if (uasm_flag)printf("R,i\n");
		return 5;
	}
	case '\x02': {
		if (uasm_flag)printf("R,[R]\n");
		return 2;
	}
	case '\x03': {
		if (uasm_flag)printf("[R],R\n");
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
	}
	return -100;
}
/*
*��ȡָ���
*/
int get_ins_len(char f, char s) {
	uint8_t f1 = (uint8_t)f;
	switch (f1)
	{
	case INS_MOV: {
		if (uasm_flag)printf("50 mov ");
		if (s <= '\x04')
		return	2+_ins_len(s);
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case INS_LEA: {
		if (uasm_flag)printf("51 lea ");
		if (s <= '\x04')
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
		if (s <= '\x02')
		{
			if (uasm_flag) printf("32 cmp ");
			return	2 + _ins_len(s);
		}
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case INS_ADD: case INS_SUB:case INS_XOR:case INS_AND:case INS_OR: {
		if (s <= '\x02')
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
    default: {
		break;
	}
		
	}
	return -1;
}
/*
*���ñ�־�Ĵ��� ��������
*/
int set_flag(char pram) {
	char * ptr = registe_ptr->flag;//��ȡflagָ��
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
	char * ptr = registe_ptr->flag;//��ȡflagָ��
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
	char * ptr = registe_ptr->flag;//��ȡflagָ��
	if (temp == 0) {//�����������0λ��0
		ptr[0] = 0;
		return LVM_SUCCESS;
	}
	else if (temp > 0) {//������0 0λ��1
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
*ִ��movָ��
*/
int do_mov() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low= (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;

	switch (code_ptr[registe_ptr->IP+1])//��������
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
		if (low == 0)//��λ����
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//���������
			char *R = (char *)register_list[high];
			*R= temp;
			set_flag(*R);
			break;
		}else if (low==1)//��λ����
		{
			unsigned temp = *(short*)(code_ptr+registe_ptr->IP + 2 + 1);//���������
			int16_t *R = (int16_t *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}else if(low ==2)//ȫ����
		{
			unsigned temp = *(unsigned*)(code_ptr+registe_ptr->IP + 2 + 1);//���������
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
		if (low == 0)//��λ����
		{
			unsigned value = *(unsigned*)register_list[highb];//���Rb�Ĵ����е�ֵ
			char temp = data_ptr[value];//��ȡ���ݶε�����
			char *R = (char *)register_list[high];
			*R = temp;
			break;
		}
		else if (low == 1)//��λ����
		{
			unsigned value = *(unsigned*)register_list[highb];//���Rb�Ĵ����е�ֵ
			int16_t temp = *(int16_t*)(data_ptr+value);//��ȡ���ݶε�����
			int16_t *R = (int16_t *)register_list[high];
			*R = temp;
			break;
		}
		else if (low == 2)//ȫ����
		{
			unsigned value = *(unsigned*)register_list[highb];//���Rb�Ĵ����е�ֵ
			unsigned temp = *(unsigned*)(data_ptr+value);//��ȡ���ݶε�����
			unsigned *R = (unsigned *)register_list[high];
			*R = temp;
			break;
		}
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case '\x03': {//[R],R----->3
		uint8_t lowb = (uint8_t)code_ptr[registe_ptr->IP + 3] % 0x10;
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		if (lowb == 0)//��λ����
		{
			unsigned value = *(unsigned*)register_list[high];//���Ra�Ĵ����е�ֵ
			char *temp = data_ptr+value;//��ȡ����ָ��
			char *R = (char *)register_list[highb];//���Rb�Ĵ����е�ֵ
			*temp=*R;//д��
			break;
		}
		else if (lowb == 1)//��λ����
		{
			unsigned value = *(unsigned*)register_list[high];//���Ra�Ĵ����е�ֵ
			int16_t *temp = (int16_t*)(data_ptr + value);//��ȡ����ָ��
			int16_t *R = (int16_t*)register_list[highb];//���Rb�Ĵ����е�ֵ
			*temp = *R;//д��
			break;
		}
		else if (lowb == 2)//ȫ����
		{
			unsigned value = *(unsigned*)register_list[high];//���Ra�Ĵ����е�ֵ
			unsigned *temp = (unsigned*)(data_ptr + value);//��ȡ����ָ��
			unsigned *R = (unsigned*)register_list[highb];//���Rb�Ĵ����е�ֵ
			*temp = *R;//д��
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
*ִ��leaָ��
*/
int do_lea() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	switch (code_ptr[registe_ptr->IP + 1])//��������
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
		if (low == 0)//��λ����
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//���������
			char *R = (char *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}
		else if (low == 1)//��λ����
		{
			unsigned temp = *(short*)(code_ptr+registe_ptr->IP + 2 + 1);//���������
			int16_t *R = (int16_t *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}
		else if (low == 2)//ȫ����
		{
			unsigned temp = *(unsigned*)(code_ptr+registe_ptr->IP + 2 + 1);//���������
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
*ִ��intָ��
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
		printf("\n\n������ɹ��˳�");
		exit(0);
	}
	}
	return LVM_SUCCESS;
}
/*
*ָ��ִ��ѡ����
*/
int do_ins(){
	if(ins_list[(uint8_t)code_ptr[registe_ptr->IP]]!=nullptr)
	ins_list[(uint8_t)code_ptr[registe_ptr->IP]]();
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*ִ��JMP
*/
int do_jmp() {
	if (code_ptr[registe_ptr->IP + 1] == 5) {//�Ĵ�����ת
		uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
		unsigned adr = *register_list[high];
		registe_ptr->IP = adr;
	}else if (code_ptr[registe_ptr->IP + 1] == 6){//��������ת
		unsigned adr = *(unsigned*)(code_ptr+registe_ptr->IP + 2);
		registe_ptr->IP = adr;
		
	}else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*ִ��JZ
*/
int do_jz() {
	if (registe_ptr->flag[0] == 0){
		if (code_ptr[registe_ptr->IP + 1] == 5) {//�Ĵ�����ת
			uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
			unsigned adr = *register_list[high];
			registe_ptr->IP = adr;
		}
		else if (code_ptr[registe_ptr->IP + 1] == 6) {//��������ת
			unsigned adr = *(unsigned*)(code_ptr + registe_ptr->IP + 2);
			registe_ptr->IP = adr;

		}
		else throw(LVM_EXECUTE_ERROR);
	}
	return LVM_SUCCESS;
}
/*
*ִ��JNZ
*/
int do_jnz() {
	if (registe_ptr->flag[0] == 1) {//�жϷ�����ת����
		if (code_ptr[registe_ptr->IP + 1] == 5) {//�Ĵ�����ת
			uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
			unsigned adr = *register_list[high];
			registe_ptr->IP = adr;
		}
		else if (code_ptr[registe_ptr->IP + 1] == 6) {//��������ת
			unsigned adr = *(unsigned*)(code_ptr+registe_ptr->IP + 2);
			registe_ptr->IP = adr;

		}else throw(LVM_EXECUTE_ERROR);
	}
	return LVM_SUCCESS;
}
/*
*ִ��JH
*/
int do_jh() {
	if (registe_ptr->flag[1] == 1) {
		if (code_ptr[registe_ptr->IP + 1] == 5) {//�Ĵ�����ת
			uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
			unsigned adr = *register_list[high];
			registe_ptr->IP = adr;
		}
		else if (code_ptr[registe_ptr->IP + 1] == 6) {//��������ת
			unsigned adr = *(unsigned*)(code_ptr + registe_ptr->IP + 2);
			registe_ptr->IP = adr;

		}
		else throw(LVM_EXECUTE_ERROR);
	}
	return LVM_SUCCESS;
}
/*
*ִ��JL
*/
int do_jl() {
	if (registe_ptr->flag[1] == 0) {
		if (code_ptr[registe_ptr->IP + 1] == 5) {//�Ĵ�����ת
			uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
			unsigned adr = *register_list[high];
			registe_ptr->IP = adr;
		}
		else if (code_ptr[registe_ptr->IP + 1] == 6) {//��������ת
			unsigned adr = *(unsigned*)(code_ptr + registe_ptr->IP + 2);
			registe_ptr->IP = adr;

		}
		else throw(LVM_EXECUTE_ERROR);
	}
	return LVM_SUCCESS;
}
/*
*ִ��INC
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
*ִ��DEC
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
*ִ��CMP
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
*ִ��ADD
*/
int do_add() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1]==0)//0������
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
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1������
		if (low == 0)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr+registe_ptr->IP + 2 + 1);//���������
			char *R = (char *)register_list[high];
			*R += temp;
			set_flag(*R);
		}
		else if (low == 1)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			int16_t *R = (int16_t *)register_list[high];
			*R += temp;
			set_flag(*R);
		}
		else if (low == 2)//ȫ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
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
*ִ��SUB
*/
int do_sub() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1] == 0)//0������
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
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1������
		if (low == 0)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			char *R = (char *)register_list[high];
			*R -= temp;
			set_flag(*R);
		}
		else if (low == 1)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			int16_t *R = (int16_t *)register_list[high];
			*R -= temp;
			set_flag(*R);
		}
		else if (low == 2)//ȫ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
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
*ִ��XOR
*/
int do_xor() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1] == 0)//0������
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
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1������
		if (low == 0)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			char *R = (char *)register_list[high];
			*R ^= temp;
			set_flag(*R);
		}
		else if (low == 1)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			int16_t *R = (int16_t *)register_list[high];
			*R ^= temp;
			set_flag(*R);
		}
		else if (low == 2)//ȫ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
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
*ִ��AND
*/
int do_and() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1] == 0)//0������
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
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1������
		if (low == 0)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			char *R = (char *)register_list[high];
			*R &= temp;
			set_flag(*R);
		}
		else if (low == 1)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			int16_t *R = (int16_t *)register_list[high];
			*R &= temp;
			set_flag(*R);
		}
		else if (low == 2)//ȫ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
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
*ִ��OR
*/
int do_or() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1] == 0)//0������
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
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1������
		if (low == 0)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			char *R = (char *)register_list[high];
			*R |= temp;
			set_flag(*R);
		}
		else if (low == 1)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			int16_t *R = (int16_t *)register_list[high];
			*R |= temp;
			set_flag(*R);
		}
		else if (low == 2)//ȫ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
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
*ִ��NOT
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
*ִ��NOP
*/
int do_nop() {
	return LVM_SUCCESS;
}
/*
*����ѡ��
*/
void lvm_debug(int ins_len) {
	debug_flag = true;
	printf("------------------------����ģʽ------------------------\n");
	printf("IP--------->%08X\n", registe_ptr->IP);
FI:
	std::string str{""};
	std::cout << ">>>>";
YC:
	std::getline(std::cin, str);
	if (str.empty()) goto YC;
	if (str == "n")//����
	{
		registe_ptr->R6 = registe_ptr->IP;
		registe_ptr->R6 += ins_len;
		registe_ptr->flag[15] = 1;
		do_ins();
		printf("IP--------->%08X\n", registe_ptr->IP);
		return;
	}
	else if (str=="e") {//�˳�����
		debug_flag = false;
		registe_ptr->R6 = (unsigned)-1;
		registe_ptr->R7 = (unsigned)-1;
		registe_ptr->flag[15] = 0;
		do_ins();
		return;
	}else if (str == "c") {//���г���
		registe_ptr->flag[15] = 0;
		registe_ptr->R6 = (unsigned)-1;//�����ϵ�ر�
		do_ins();
		return;
	}
	else if (str=="u") {
		uasm_flag = true;
		unsigned temp = registe_ptr->IP;//���浱ǰIP Ϊ�������׼��
		while (registe_ptr->IP!=m_code_length)
		{
			printf("%08X   ", registe_ptr->IP);
			int ins_len;
			ins_len = get_ins_len(code_ptr[registe_ptr->IP], code_ptr[registe_ptr->IP + 1]);//��ȡָ��� Ϊ��ַָ�������׼��
			registe_ptr->IP += ins_len;
		}
		registe_ptr->IP = temp;
		uasm_flag = false;
		goto FI;

	}else if (str == "p")//��ӡ������Ϣ
	{
		int count{ 0 };
		for (int i = 0; i < 8; i++)
		{
			++count;
			std::cout << "R" << i << "--------->" ;
			printf("%08X	", *register_list[i]);
			if (count == 3) {
				count = 0;
				printf("\n");
			}
		}
		printf("IP--------->%08X\n", registe_ptr->IP);
		printf("SP--------->%08X	", registe_ptr->SP);
		printf("BP--------->%08X\n", registe_ptr->BP);
		printf("flag[0]--------->%02x	", registe_ptr->flag[0]);
		printf("flag[1]--------->%02x\n", registe_ptr->flag[1]);
		goto FI;//���ض���
	}else if (*str.begin() == 'b') {
		auto temp=str.find(' ', 1);
		if (temp != std::string::npos) {
			std::string break_adr = str.substr(temp + 1);
			std::stringstream ss;
			ss.str("");
			unsigned break_num{ 0 };
			if (break_adr.substr(0, 2)=="0x")//16�����ַ���
			{
				ss<< std::hex << break_adr.substr(2);
				ss >> break_num;
			}else
			{
				ss <<  break_adr;
				ss >> break_num;
			}
			registe_ptr->R7 = break_num;
			printf("R7--------->%08X\n", registe_ptr->R7);
			goto FI;
		}else
		{
			printf("��������ȷ�ĸ�ʽ\n");
			goto FI;
		}
	}else if(str[0]=='d' && str[1]=='b')
	{
		auto temp = str.find(' ', 2);
		if (temp != std::string::npos) {
			std::string data_adr = str.substr(temp + 1);
			std::stringstream ss;
			ss.str("");
			unsigned data_num{ 0 };
			if (data_adr.substr(0, 2) == "0x")//16�����ַ���
			{
				ss << std::hex << data_adr.substr(2);
				ss >> data_num;
			}
			else
			{
				ss << data_adr;
				ss >> data_num;
			}
			int m{ 0 };
			for (int i = 0; i < 3; i++)
			{
				printf("%08X	", data_num+m);
				for (int h=0; h< 16; h++)
				{
					printf("%02X ",data_ptr[data_num+m]);
					++m;
				}
				int p = m-16;
				putchar('|');
				for (int o = p; o< m; o++)
				{
					if(data_ptr[data_num + o]=='\x0a' || data_ptr[data_num + o] == '\x0d') putchar('*');
					else putchar(data_ptr[data_num + o]);
				}
				printf("\n");
			}
			goto FI;
		}
		else
		{
			printf("��������ȷ�ĸ�ʽ\n");
			goto FI;
		}
	}
	else{
		printf("��������ȷ��ָ��\n");
		goto FI;
	}

}
int exectue_ins() {
	int ins_len;
	ins_len=get_ins_len(code_ptr[registe_ptr->IP], code_ptr[registe_ptr->IP + 1]);//��ȡָ��� Ϊ��ַָ�������׼��
	unsigned tempIP = registe_ptr->IP;
	if (ins_len > 0)
	{
		if (registe_ptr->R6 == registe_ptr->IP || registe_ptr->R7 == registe_ptr->IP)//�������״̬
		{
			lvm_debug(ins_len);
		}
		else do_ins();
	}
	else throw(LVM_EXECUTE_ERROR);
	if (tempIP == registe_ptr->IP)//����Ƿ�����ת
	{
		registe_ptr->IP += ins_len;
	}
	else
	{
		if (debug_flag && registe_ptr->flag[15]) registe_ptr->R6 = registe_ptr->IP;
	}
		
	
	return LVM_SUCCESS;
}