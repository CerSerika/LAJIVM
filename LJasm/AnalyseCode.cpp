#include"LJasm.h"
#include"AnalyseCode.h"
#include<cctype>
#include<sstream>
std::regex reg0("^R([0-9]{1,2})([LH]?)$");//ƥ��0 1 2 12 13 ��ʽ����
std::regex reg1("^(0X)?([0-9]*)$");//ƥ��������
std::regex reg2("^\\[R([0-9]{1,2})\\]$");//ƥ��[R]
std::regex reg1x("^S\\[R([0-9]{1,2})\\]$");//ƥ��S[R]
std::regex reg7("^\\[R([0-9]{1,2})\\+(0X)?([0-9]*)\\]$");//ƥ��[R+i]
std::regex reg17("^S\\[R([0-9]{1,2})\\+(0X)?([0-9]*)\\]$");//ƥ��s[R+i]
std::regex reg9("^\\[R([0-9]{1,2})\\+R([0-9]{1,2})\\]$");//ƥ��[R+R]
std::regex reg19("^S\\[R([0-9]{1,2})\\+R([0-9]{1,2})\\]$");//ƥ��[R+R]
std::regex reglabel("^([a-zA-Z_][a-zA-Z0-9_]+):$");//ƥ���ǩ
std::regex reglabel2("^([a-zA-Z_][a-zA-Z0-9_]+)$");//ƥ���ǩ
unsigned immed_to_unsgned(std::string is16, std::string str){
	if (is16 == "0X") {//Ϊ16������
		int data_num{ -1 };
		std::stringstream ss;
		ss << std::hex << str;
		ss >> data_num;
		return (unsigned)data_num;
	}
	else if (is16 == "") {//10������
		int data_num{ -1 };
		std::stringstream ss;
		ss << str;
		ss >> data_num;
		return (unsigned)data_num;
	}
}
uint8_t registernum_to_num(std::string str) {
	std::regex reg("([0-9]{1,2})[LH]?$");
	std::smatch match;
	if (std::regex_match(str, match, reg)) {
		return atoi(match.str().c_str());
	}
	else throw(now_index);
}
uint8_t lowhigh_to_num(std::string str) {
	if (str == "L") return 0;
	else if (str == "H") return 1;
	else if (str == "") return 2;
	else throw (now_index);
}
std::string& trim(std::string &s)
{
	if (s.empty())
	{
		return s;
	}

	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}
int Analyse_code(std::string str) {//mov R0L,0 ;123456789
	unsigned comment_index{ str.find(';') };
	str = str.substr(0, comment_index);//��ȥע��
	for (auto &c:str) c=toupper(c);//ȫ��ת��д
	unsigned blank_index{ str.find_first_of(' ') };
	std::smatch opmatch;//op��ƥ��
	std::string op_str = str.substr(0, blank_index);//��ȡָ���ַ���
	std::string ins_str = str.substr(++blank_index, str.length()-blank_index);//��ȡ����ָ���ַ���
	if (op_str == "MOV") Analyse_mov(ins_str);
	else if(op_str == "ADD") Analyse_add(ins_str);
	else if (op_str == "NOP") {
		out_ptr[bin_length] = INS_NOP;
		++bin_length;
	}
	else if (op_str == "LEA") Analyse_lea(ins_str);
	else if (op_str == "INT") Analyse_int(ins_str);
	else if (op_str == "CMP") Analyse_cmp(ins_str);
	else if (op_str == "JNZ") Analyse_jnz(ins_str);
	else if (op_str == "JMP") Analyse_jmp(ins_str);
	else if (std::regex_match(op_str,opmatch,reglabel) ) {//ƥ���ǩ
		if (is_label_define(op_str, label_index) == -1)
		{
			label_s[label_index].name = opmatch[1];
			label_s[label_index].now_bin_length = bin_length - data_long;
			++label_index;
		}else {
			std::cout << "�ظ�����ı�ǩ" << std::endl;
			throw(now_index);
		}
	}
	else
	{
		std::cout << "δ�����ָ������" << std::endl;
		throw(now_index);
	}
	return 0;
}
int Analyse_mov(std::string str) {
	unsigned slip_index{ str.find(',') };
	std::string a_str{ str.substr(0,slip_index) };
	std::string b_str{ str.substr(++slip_index,str.length()-slip_index) };
	trim(a_str);
	trim(b_str);
	std::smatch amatch;
	std::smatch bmatch;
	bool f1=std::regex_match(a_str, amatch, reg0);
	if (f1) {//ƥ��0 1 2 12  ��ʽ����ɹ�
		bool ftemp = std::regex_match(b_str, bmatch, reg0);
		if (ftemp) {//ƥ��0�ͳɹ� mov R,R  ---->0
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a1 = lowhigh_to_num(amatch[2]);
			uint8_t a = a0 * 0x10 | a1;
			uint8_t b0 = registernum_to_num(bmatch[1]);
			uint8_t b1 = lowhigh_to_num(bmatch[2]);
			uint8_t b = b0 * 0x10 | b1;
			/*д��out_ptr*/
			out_ptr[bin_length] = INS_MOV;
			++bin_length;
			out_ptr[bin_length] = 0x20;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			out_ptr[bin_length] = b;
			++bin_length;
			return 0;
		}else if(std::regex_match(b_str, bmatch, reg1))//ƥ��1�ͳɹ� mov R,i   ----->1
		{
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a1 = lowhigh_to_num(amatch[2]);
			uint8_t a = a0 * 0x10 | a1;
			unsigned i = immed_to_unsgned(bmatch[1],bmatch[2]);
			out_ptr[bin_length] = INS_MOV;
			++bin_length;
			out_ptr[bin_length] = 0x21;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			*(unsigned *)(out_ptr + bin_length) = i;
			bin_length += 4;
			return 0;
		}else if(std::regex_match(b_str, bmatch, reg2))//ƥ��2�ͳɹ� mov R,[R] ----->2
		{
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a1 = lowhigh_to_num(amatch[2]);
			uint8_t a = a0 * 0x10 | a1;
			uint8_t b0 = registernum_to_num(bmatch[1]);
			uint8_t b1 = lowhigh_to_num(bmatch[2]);
			uint8_t b = b0 * 0x10 | b1;
			/*д��out_ptr*/
			out_ptr[bin_length] = INS_MOV;
			++bin_length;
			out_ptr[bin_length] = 0x22;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			out_ptr[bin_length] = b;
			++bin_length;
			return 0;
		}
		else if (std::regex_match(b_str, bmatch, reg1x)) {//ƥ��12�ͳɹ� mov R,s[R] ----->12
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a1 = lowhigh_to_num(amatch[2]);
			uint8_t a = a0 * 0x10 | a1;
			uint8_t b0 = registernum_to_num(bmatch[1]);
			uint8_t b1 = lowhigh_to_num(bmatch[2]);
			uint8_t b = b0 * 0x10 | b1;
			/*д��out_ptr*/
			out_ptr[bin_length] = INS_MOV;
			++bin_length;
			out_ptr[bin_length] = 0x12;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			out_ptr[bin_length] = b;
			++bin_length;
			return 0;
		}
		else if (std::regex_match(b_str, bmatch, reg7)) {//ƥ��7�ͳɹ� mov R,[R+i] ----->7
				uint8_t a0 = registernum_to_num(amatch[1]);
				uint8_t a1 = lowhigh_to_num(amatch[2]);
				uint8_t a = a0 * 0x10 | a1;
				uint8_t b0 = registernum_to_num(bmatch[1]);
				uint8_t b = b0 * 0x10 | 0x2;
				unsigned i = immed_to_unsgned(bmatch[2], bmatch[3]);
				out_ptr[bin_length] = INS_MOV;
				++bin_length;
				out_ptr[bin_length] = 0x7;
				++bin_length;
				out_ptr[bin_length] = a;
				++bin_length;
				out_ptr[bin_length] = b;
				++bin_length;
				out_ptr[bin_length] = 0;
				++bin_length;
				*(uint16_t*)(out_ptr + bin_length) = (uint16_t)i;
				bin_length += 2;
		}
		else if (std::regex_match(b_str, bmatch, reg17)) {//ƥ��17�ͳɹ� mov R,s[R+i] ----->7
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a1 = lowhigh_to_num(amatch[2]);
			uint8_t a = a0 * 0x10 | a1;
			uint8_t b0 = registernum_to_num(bmatch[1]);
			uint8_t b = b0 * 0x10 | 0x2;
			unsigned i = immed_to_unsgned(bmatch[2], bmatch[3]);
			out_ptr[bin_length] = INS_MOV;
			++bin_length;
			out_ptr[bin_length] = 0x17;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			out_ptr[bin_length] = b;
			++bin_length;
			out_ptr[bin_length] = 0;
			++bin_length;
			*(uint16_t*)(out_ptr + bin_length) = (uint16_t)i;
			bin_length += 2;
		}
		else throw (now_index);
	}
	else if (std::regex_match(a_str, amatch, reg2)){//ƥ��3 9��
		if (std::regex_match(b_str, bmatch, reg0)) {//ƥ��3�ͳɹ� mov [R],R----->3
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a1 = lowhigh_to_num(amatch[2]);
			uint8_t a = a0 * 0x10 | a1;
			uint8_t b0 = registernum_to_num(bmatch[1]);
			uint8_t b1 = lowhigh_to_num(bmatch[2]);
			uint8_t b = b0 * 0x10 | b1;
			/*д��out_ptr*/
			out_ptr[bin_length] = INS_MOV;
			++bin_length;
			out_ptr[bin_length] = 0x03;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			out_ptr[bin_length] = b;
			++bin_length;
			return 0;
		}else throw (now_index);
	}else if(std::regex_match(a_str, amatch, reg7))//ƥ��8 18��
	{
		if (std::regex_match(b_str, bmatch, reg0)) {//ƥ��8�ͳɹ� [R+i],R----->08
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a = a0 * 0x10 | 0x2;
			unsigned i = immed_to_unsgned(amatch[2], amatch[3]);
			uint8_t b0 = registernum_to_num(amatch[1]);
			uint8_t b1 = lowhigh_to_num(amatch[2]);
			uint8_t b = b0 * 0x10 | b1;
			out_ptr[bin_length] = INS_MOV;
			++bin_length;
			out_ptr[bin_length] = 0x08;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			out_ptr[bin_length] = 0;
			++bin_length;
			*(uint16_t*)(out_ptr + bin_length) = (uint16_t)i;
			bin_length += 2;
			out_ptr[bin_length] = b;
			++bin_length;
			return 0;
		}else throw (now_index);
	}
	else if (std::regex_match(a_str, amatch, reg17)) {
		if (std::regex_match(b_str, bmatch, reg0)) {//ƥ��18�ͳɹ� s[R+i],R----->18
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a = a0 * 0x10 | 0x2;
			unsigned i = immed_to_unsgned(amatch[2], amatch[3]);
			uint8_t b0 = registernum_to_num(amatch[1]);
			uint8_t b1 = lowhigh_to_num(amatch[2]);
			uint8_t b = b0 * 0x10 | b1;
			out_ptr[bin_length] = INS_MOV;
			++bin_length;
			out_ptr[bin_length] = 0x18;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			out_ptr[bin_length] = 0;
			++bin_length;
			*(uint16_t*)(out_ptr + bin_length) = (uint16_t)i;
			bin_length += 2;
			out_ptr[bin_length] = b;
			++bin_length;
			return 0;
		}else throw (now_index);
	}
	else if (std::regex_match(a_str, amatch, reg9)) {//ƥ��9��
		if (std::regex_match(b_str, bmatch, reg0)) {//ƥ��9�ͳɹ� mov [R+R],R----->09
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a = a0 * 0x10 | 0x2;
			uint8_t b0 = registernum_to_num(amatch[2]);
			uint8_t b = b0 * 0x10 | 0x2;
			uint8_t c0= registernum_to_num(bmatch[1]);
			uint8_t c1 = lowhigh_to_num(bmatch[2]);
			uint8_t c = c0 * 0x10 | c1;
			out_ptr[bin_length] = INS_MOV;
			++bin_length;
			out_ptr[bin_length] = 0x09;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			out_ptr[bin_length] = b;
			++bin_length;
			out_ptr[bin_length] = c;
			++bin_length;
			return 0;
		}
	}
	else if (std::regex_match(a_str, amatch, reg19)) {//ƥ��9��
		if (std::regex_match(b_str, bmatch, reg0)) {//ƥ��9�ͳɹ� mov [R+R],R----->09
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a = a0 * 0x10 | 0x2;
			uint8_t b0 = registernum_to_num(amatch[2]);
			uint8_t b = b0 * 0x10 | 0x2;
			uint8_t c0 = registernum_to_num(bmatch[1]);
			uint8_t c1 = lowhigh_to_num(bmatch[2]);
			uint8_t c = c0 * 0x10 | c1;
			out_ptr[bin_length] = INS_MOV;
			++bin_length;
			out_ptr[bin_length] = 0x19;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			out_ptr[bin_length] = b;
			++bin_length;
			out_ptr[bin_length] = c;
			++bin_length;
			return 0;
		}
	}
	else throw (now_index);
	return 0;
}
int Analyse_add(std::string str) {
	unsigned slip_index{ str.find(',') };
	std::string a_str{ str.substr(0,slip_index) };
	std::string b_str{ str.substr(++slip_index,str.length() - slip_index) };
	trim(a_str);
	trim(b_str);
	std::smatch amatch;
	std::smatch bmatch;
	bool f1 = std::regex_match(a_str, amatch, reg0);
	if (f1) {//ƥ��0 1 2 12  ��ʽ����ɹ�
		bool ftemp = std::regex_match(b_str, bmatch, reg0);
		if (ftemp) {//ƥ��0�ͳɹ� mov R,R  ---->0
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a1 = lowhigh_to_num(amatch[2]);
			uint8_t a = a0 * 0x10 | a1;
			uint8_t b0 = registernum_to_num(bmatch[1]);
			uint8_t b1 = lowhigh_to_num(bmatch[2]);
			uint8_t b = b0 * 0x10 | b1;
			/*д��out_ptr*/
			out_ptr[bin_length] = INS_ADD;
			++bin_length;
			out_ptr[bin_length] = 0x0;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			out_ptr[bin_length] = b;
			++bin_length;
			//test_out_ptr(bin_length);
			return 0;
		}
		else if (std::regex_match(b_str, bmatch, reg1))//ƥ��1�ͳɹ� mov R,i   ----->1
		{
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a1 = lowhigh_to_num(amatch[2]);
			uint8_t a = a0 * 0x10 | a1;
			unsigned i = immed_to_unsgned(bmatch[1], bmatch[2]);
			out_ptr[bin_length] = INS_ADD;
			++bin_length;
			out_ptr[bin_length] = 0x1;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			*(unsigned *)(out_ptr + bin_length) = i;
			bin_length += 4;
			//test_out_ptr(bin_length);
			return 0;
		}
	}
	else throw (now_index);
	return 0;
}
int Analyse_lea(std::string str) {
	unsigned slip_index{ str.find(',') };
	std::string a_str{ str.substr(0,slip_index) };
	std::string b_str{ str.substr(++slip_index,str.length() - slip_index) };
	std::smatch amatch;
	std::smatch bmatch;
	trim(a_str);
	trim(b_str);
	bool f1 = std::regex_match(a_str, amatch, reg0);
	if (f1) {//ƥ��0 1 2 12  ��ʽ����ɹ�
		bool ftemp = std::regex_match(b_str, bmatch, reg0);
		if (ftemp) {//ƥ��0�ͳɹ� lea R,R  ---->0
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a1 = lowhigh_to_num(amatch[2]);
			uint8_t a = a0 * 0x10 | a1;
			uint8_t b0 = registernum_to_num(bmatch[1]);
			uint8_t b1 = lowhigh_to_num(bmatch[2]);
			uint8_t b = b0 * 0x10 | b1;
			/*д��out_ptr*/
			out_ptr[bin_length] = INS_LEA;
			++bin_length;
			out_ptr[bin_length] = 0x0;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			out_ptr[bin_length] = b;
			++bin_length;
			//test_out_ptr(bin_length);
			return 0;
		}
		else if (std::regex_match(b_str, bmatch, reg1))//ƥ��1�ͳɹ� lea R,i   ----->1
		{
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a1 = lowhigh_to_num(amatch[2]);
			uint8_t a = a0 * 0x10 | a1;
			unsigned i = immed_to_unsgned(bmatch[1], bmatch[2]);
			out_ptr[bin_length] = INS_LEA;
			++bin_length;
			out_ptr[bin_length] = 0x1;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			*(unsigned *)(out_ptr + bin_length) = i;
			bin_length += 4;
			//test_out_ptr(bin_length);
			return 0;
		}
	}
	else throw (now_index);
	return 0;
}
int Analyse_int(std::string str){
	trim(str);
	std::smatch bmatch;
	if (std::regex_match(str, bmatch, reg1))//ƥ��1�ͳɹ� mov R,i   ----->1
	{
		unsigned i = immed_to_unsgned(bmatch[1], bmatch[2]);
		out_ptr[bin_length] = INS_INT;
		++bin_length;
		out_ptr[bin_length] = (uint8_t)i;
		++bin_length;
		return 0;
	}else throw (now_index);
	return 0;

}
int Analyse_cmp(std::string str) {
	unsigned slip_index{ str.find(',') };
	std::string a_str{ str.substr(0,slip_index) };
	std::string b_str{ str.substr(++slip_index,str.length() - slip_index) };
	trim(a_str);
	trim(b_str);
	std::smatch amatch;
	std::smatch bmatch;
	if (std::regex_match(a_str, amatch, reg0)) {//ƥ��0 1��
		if (std::regex_match(b_str, bmatch, reg0)) {//ƥ��0�ͳɹ�
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a1 = lowhigh_to_num(amatch[2]);
			uint8_t a = a0 * 0x10 | a1;
			uint8_t b0 = registernum_to_num(bmatch[1]);
			uint8_t b1 = lowhigh_to_num(bmatch[2]);
			uint8_t b = b0 * 0x10 | b1;
			/*д��out_ptr*/
			out_ptr[bin_length] = INS_CMP;
			++bin_length;
			out_ptr[bin_length] = 0x0;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			out_ptr[bin_length] = b;
			++bin_length;
			return 0;
		}
		else if (std::regex_match(b_str, bmatch, reg1)) {//ƥ��1�ͳɹ�
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a1 = lowhigh_to_num(amatch[2]);
			uint8_t a = a0 * 0x10 | a1;
			unsigned i = immed_to_unsgned(bmatch[1], bmatch[2]);
			out_ptr[bin_length] = INS_CMP;
			++bin_length;
			out_ptr[bin_length] = 0x1;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			*(unsigned *)(out_ptr + bin_length) = i;
			bin_length += 4;
			return 0;
		}
		else throw (now_index);
	}
	else throw (now_index);
	return 0;
}
int Analyse_jmp(std::string str) {
	trim(str);
	std::smatch match;
	if (std::regex_match(str, match, reglabel2)) {//ƥ��Ϊ��ǩ
		write_label_d[write_label_index].name = str;
		write_label_d[write_label_index].op = INS_JMP;
		write_label_d[write_label_index].bin_to_write = bin_length;
		bin_length += 6;
		++write_label_index;
		return 0;
	}
	if (std::regex_match(str, match, reg1))//ƥ��6�ͳɹ� 06�� ��������
	{
		unsigned i = immed_to_unsgned(match[1], match[2]);
		out_ptr[bin_length] = INS_JMP;
		++bin_length;
		out_ptr[bin_length] = 0x6;
		++bin_length;
		*(unsigned *)(out_ptr + bin_length) = i;
		bin_length += 4;
		return 0;
	}
	else if (std::regex_match(str, match, reg0)) {//ƥ��5�ͳɹ� 05�� ���Ĵ���
		uint8_t a0 = registernum_to_num(match[1]);
		uint8_t a = a0 * 0x10 | 0x2;
		out_ptr[bin_length] = INS_JMP;
		++bin_length;
		out_ptr[bin_length] = 0x5;
		++bin_length;
		out_ptr[bin_length] = a;
		++bin_length;
		return 0;
	}
	else throw (now_index);
	return 0;
}
int Analyse_jnz(std::string str) {
	trim(str);
	std::smatch match;
	if (std::regex_match(str, match, reglabel2)) {//ƥ��Ϊ��ǩ
		write_label_d[write_label_index].name = str;
		write_label_d[write_label_index].op = INS_JNZ;
		write_label_d[write_label_index].bin_to_write = bin_length;
		bin_length += 6;
		++write_label_index;
		return 0;
	}
	if (std::regex_match(str, match, reg1))//ƥ��6�ͳɹ� 06�� ��������
	{
		unsigned i = immed_to_unsgned(match[1], match[2]);
		out_ptr[bin_length] = INS_JNZ;
		++bin_length;
		out_ptr[bin_length] = 0x6;
		++bin_length;
		*(unsigned *)(out_ptr+bin_length) = i;
		bin_length+=4;
		return 0;
	}
	else if (std::regex_match(str, match, reg0)) {//ƥ��5�ͳɹ� 05�� ���Ĵ���
		uint8_t a0 = registernum_to_num(match[1]);
		uint8_t a = a0 * 0x10 | 0x2;
		out_ptr[bin_length] = INS_JNZ;
		++bin_length;
		out_ptr[bin_length] = 0x5;
		++bin_length;
		out_ptr[bin_length] = a;
		++bin_length;
		return 0;
	}
	else throw (now_index);
	return 0;
}