#include"ExecuteInstruction.h"
#include"ErrorList.h"
#include"init.h"
#include<iostream>
#include<string>
#include<ios>
#include<sstream>
bool debug_flag{ false };
/*
*��λ��ʾ�Ĵ�������
*��λ��ʾ�����Ĵ����ĳ���
*/
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
	case INS_JMP:case INS_JZ:case INS_JNZ:case INS_JH:case INS_JL: {
		return 5;
		break;
	}
	case INS_INC:case INS_DEC: {
		return 2;
		break;
	}
	case INS_CMP: {
		if (s <= '\x02')
			return	2 + _ins_len(s);
		else throw(LVM_EXECUTE_ERROR);
		break;
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
	char * ptr = registe_ptr->flag;//��ȡflagָ��
	if (pram == 0) {//�����������0λ��0
		ptr[0] = 0;
		return LVM_SUCCESS;
	}
	else if (pram > 0) {//������0 0λ��1
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
/*
*ִ��movָ��
*/
int do_mov() {
	int8_t high = code_ptr[registe_ptr->IP + 2] / 0x10;
	int8_t low= code_ptr[registe_ptr->IP + 2] % 0x10;

	switch (code_ptr[registe_ptr->IP+1])//��������
	{
	case '\x00': {//mov R,R  ---->0
		//
		int8_t highb = code_ptr[registe_ptr->IP + 3] / 0x10;
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
		}
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
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//���������
			int16_t *R = (int16_t *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}else if(low ==2)//ȫ����
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//���������
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
*ִ��leaָ��
*/
int do_lea() {
	int8_t high = code_ptr[registe_ptr->IP + 2] / 0x10;
	int8_t low = code_ptr[registe_ptr->IP + 2] % 0x10;
	switch (code_ptr[registe_ptr->IP + 1])//��������
	{
	case '\x00': {
		//
		int8_t highb = code_ptr[registe_ptr->IP + 3] / 0x10;
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
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//���������
			int16_t *R = (int16_t *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}
		else if (low == 2)//ȫ����
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//���������
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
/*
*ִ��JMP
*/
int do_jmp() {
	unsigned adr = code_ptr[registe_ptr->IP + 1];
	registe_ptr->IP = adr;
	return LVM_SUCCESS;
}
/*
*ִ��JZ
*/
int do_jz() {
	unsigned adr = code_ptr[registe_ptr->IP + 1];
	if(registe_ptr->flag[0]==0)registe_ptr->IP = adr;
	return LVM_SUCCESS;
}
/*
*ִ��JNZ
*/
int do_jnz() {
	unsigned adr = code_ptr[registe_ptr->IP + 1];
	if (registe_ptr->flag[0] == 1)registe_ptr->IP = adr;
	return LVM_SUCCESS;
}
/*
*ִ��JH
*/
int do_jh() {
	unsigned adr = code_ptr[registe_ptr->IP + 1];
	if (registe_ptr->flag[1] == 1)registe_ptr->IP = adr;
	return LVM_SUCCESS;
}
/*
*ִ��JL
*/
int do_jl() {
	unsigned adr = code_ptr[registe_ptr->IP + 1];
	if (registe_ptr->flag[1] == 0)registe_ptr->IP = adr;
	return LVM_SUCCESS;
}
/*
*ִ��INC
*/
int do_inc(){
	int8_t high = code_ptr[registe_ptr->IP + 1] / 0x10;
	int8_t low = code_ptr[registe_ptr->IP + 1] % 0x10;
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
	int8_t high = code_ptr[registe_ptr->IP + 1] / 0x10;
	int8_t low = code_ptr[registe_ptr->IP + 1] % 0x10;
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
	int8_t high = code_ptr[registe_ptr->IP + 2] / 0x10;
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
*����ѡ��
*/
void lvm_debug(int ins_len) {
	debug_flag = true;
	printf("----------------------����ģʽ----------------------\n");
	printf("IP--------->%08X\n", registe_ptr->IP);
FI:
	std::string str{""};
	std::cout << ">>>>";
YC:
	std::getline(std::cin, str);
	if (str.empty()) goto YC;
	if (str == "n")//����
	{
		registe_ptr->R6 += ins_len;
		registe_ptr->flag[15] = 1;
		do_ins();
		printf("IP--------->%08X\n", registe_ptr->IP);
		return;
	}
	else if (str=="e") {//�˳�����
		debug_flag = false;
		registe_ptr->R6 = (unsigned)-1;
		registe_ptr->flag[15] = 0;
		do_ins();
		return;
	}else if (str == "c") {//���г���
		registe_ptr->flag[15] = 0;
		do_ins();
		return;
	}
	else if (str == "p")//��ӡ������Ϣ
	{
		for (int i = 0; i < 7; i++)
		{
			std::cout << "R" << i << "--------->" ;
			printf("%08X\n", *register_list[i]);
		}
		printf("IP--------->%08X\n", registe_ptr->IP);
		printf("flag[0]--------->%02x\n", registe_ptr->flag[0]);
		printf("flag[1]--------->%02x\n", registe_ptr->flag[1]);
		goto FI;//���ض���
	}if (*str.begin() == 'b') {
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
			registe_ptr->R6 = break_num;
			printf("R6--------->%08X\n", registe_ptr->R6);
			goto FI;
		}else
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
		if (registe_ptr->R6 == registe_ptr->IP)//�������״̬
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