#include"ExecuteInstruction.h"
#include"ErrorList.h"
#include"init.h"
#include<iostream>
int _ins_len(char c) {
	switch (c)
	{
	case '\x00': {
		return 2;
	}
	case '\x01': {
		return 5;
	}
	case '\x02': {
		return 2;
	}
	case '\x03': {
		return 2;
	}
	case '\x04': {
		return 2;
	}
	}
	return -100;
}
/*
*��ȡָ���
*/
int get_ins_len(char f, char s) {
	switch (f)
	{
	case INS_MOV: {
		if (s <= '\x04')
		return	2+_ins_len(s);
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case INS_LEA: {
		if (s <= '\x04')
			return	2 + _ins_len(s);
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case INS_INT: {
		//
		return 2;
		break;
	}
    default: {
		break;
	}
		
	}
	return -1;
}
/*
*ִ��movָ��
*/
int do_mov() {
	int8_t high = code_ptr[registe_ptr->IP + 2] / 0x10;
	int8_t low= code_ptr[registe_ptr->IP + 2] % 0x10;

	switch (code_ptr[registe_ptr->IP+1])//��������
	{
	case '\x00': {
		//
		break;
	}
	case '\x01': {//
		if (low == 0)//��λ����
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//���������
			char *R = (char *)register_list[high];
			*R= temp;
		}else if (low==1)//��λ����
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//���������
			short *R = (short *)register_list[high];
			*R = temp;
		}else if(low ==2)//ȫ����
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//���������
			unsigned *R = (unsigned *)register_list[high];
			*R = temp;
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
	int8_t high = code_ptr[registe_ptr->IP + 2] / 0x10;
	int8_t low = code_ptr[registe_ptr->IP + 2] % 0x10;
	switch (code_ptr[registe_ptr->IP + 1])//��������
	{
	case '\x00': {
		//
		break;
	}
	case '\x01': {//
		if (low == 0)//��λ����
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//���������
			char *R = (char *)register_list[high];
			*R = temp;
		}
		else if (low == 1)//��λ����
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//���������
			short *R = (short *)register_list[high];
			*R = temp;
		}
		else if (low == 2)//ȫ����
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//���������
			unsigned *R = (unsigned *)register_list[high];
			*R = temp;
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
		printf("%s", data_ptr + registe_ptr->R3);
		break;
	}
	case LVM_EXIT: {
		printf("\n\n������ɹ��˳�\n\n");
	}
	}
	return LVM_SUCCESS;
}
/*
*ָ��ִ��ѡ����
*/
int do_ins(){
	if(ins_list[code_ptr[registe_ptr->IP]]!=nullptr)
	ins_list[code_ptr[registe_ptr->IP]]();
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
int exectue_ins() {
	int ins_len;
	ins_len=get_ins_len(code_ptr[registe_ptr->IP], code_ptr[registe_ptr->IP + 1]);//��ȡָ��� Ϊ��ַָ�������׼��
	if(ins_len>0)
	do_ins();
	else throw(LVM_EXECUTE_ERROR);
	registe_ptr->IP += ins_len;
	return LVM_SUCCESS;
}