#ifndef VMREGISTER_H
#define VMREGISTER_H
#pragma pack(1)
struct vm_register
{
	/*
	*R0-R5ͨ�üĴ���
	*/
	unsigned R0;
	unsigned R1;
	unsigned R2;
	unsigned R3;
	unsigned R4;
	unsigned R5;
	//R6���ԼĴ���
	unsigned R6;

	unsigned SP;//ջ�Ĵ���
	unsigned BP;//ջ֡

	unsigned IP;//����ִ��ָ��
	char flag[8];//��־�Ĵ���
};
#pragma pack()
#define REG_NUM 11
#endif // !VMREGISTER_H

