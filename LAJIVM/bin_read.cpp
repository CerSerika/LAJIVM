#include"bin_read.h"
#include"ErrorList.h"
#include"BinType.h"
#include"init.h"
#include<fstream>
#include<ios>
#include<iostream>
using std::ifstream;
using std::ios;
char origin_bin[0xffff];
int index{0};
unsigned m_code_length;
/*
*�Ƚ��ֽ��Ƿ���ͬ
*/
bool bincmp(char * a, char *b, int len) {
	bool flag{ true };
	for (int i = 0; i < len; i++)
	{
		flag =(a[i] == b[i]);
		if (!flag) return flag;
	}
	return flag;
}
/*
*�����ַ������س��� ʧ�ܷ���-1
*��Ϊû��ʱ�� ���Բ��������㷨
*/
int find_str(char* a, char* b,int lena,int lenb) {
	for (int i = 0; i < lena - lenb; i++)
	{
		if (bincmp(a + i, b, lenb))
			return i;
	}

	return -1;
}
/*
*��ȡ�������ļ�
*/
int read_bin(std::string str) {
	ifstream bin(str,ios::in|ios::binary);
	if (bin) {
		std::cout << "���ļ��ɹ���" << std::endl;
		bin.read(origin_bin, BIN_LENGTH);
		bin.close();
		return LVM_SUCCESS;
	}else throw(LVM_BIN_OPEN_ERROR);
}
/*
*�Ѷ������ļ��е����ݶ�д���ڴ�
*/
int write_to_data() {
	int data_len;
	if (!bincmp(origin_bin+index,LVM_SIGNE,BIN_HEAD_LEN))
	{
		std::cout << "�������������Ŀ�ִ�ж������ļ�" << std::endl;
		throw(LVM_BIN_READ_ERROR);
	}else index = BIN_HEAD_LEN;
	if (!bincmp(origin_bin+index,LVM_DATA,BIN_DATA_LEN))
	{
		std::cout << "�������������Ŀ�ִ�ж������ļ�" << std::endl;
		throw(LVM_BIN_READ_ERROR);
	}else index += BIN_DATA_LEN;
	data_len = find_str(origin_bin + index, LVM_CODE, BIN_LENGTH - index, BIN_CODE_LEN);
	if (data_len == -1){
		std::cout << "�������������Ŀ�ִ�ж������ļ�" << std::endl;
		throw(LVM_BIN_READ_ERROR);
	}else
	{
		memcpy(data_ptr, origin_bin + index, data_len);
		std::cout << "���Ƶ����ݶγɹ���" << std::endl;
	}
	index += data_len;
	return LVM_SUCCESS;
}
/*
*�Ѷ������ļ��еĴ����д���ڴ�
*/
int write_to_code(){
	int code_len;
	index += BIN_CODE_LEN;//�±�ָ����������
	code_len = find_str(origin_bin + index, LVM_END, BIN_LENGTH - index, BIN_END_LEN);
	m_code_length = (unsigned)code_len;
	if (code_len == -1) {
		std::cout << "�������������Ŀ�ִ�ж������ļ�" << std::endl;
		throw(LVM_BIN_READ_ERROR);
	}else
	{
		memcpy(code_ptr, origin_bin + index, code_len);
		std::cout << "���Ƶ�����γɹ���" << std::endl;
	}
	return LVM_SUCCESS;
}
void write_all(std::string str) {
	read_bin(str);
	write_to_data();
	write_to_code();
	index = 0;
}