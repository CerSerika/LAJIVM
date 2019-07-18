#ifndef EXECUTEINSTRUCTION_H
#define EXECUTEINSTRUCTION_H
#define INS_MOV 0x50
#define INS_LEA 0x51
#define INS_INT '\xe0'
#define LVM_PRINT '\x10'
#define LVM_EXIT '\x40'
#define INS_JMP 0x40
#define INS_JZ 0x41
#define INS_JNZ 0x42
#define INS_JH 0x43
#define INS_JL 0x44
#define INS_INC 0x30
#define INS_DEC 0x31
#define INS_CMP 0x32
#define INS_ADD 0x20
#define INS_SUB 0x21
#define INS_XOR 0x22
#define INS_AND 0x23
#define INS_OR 0x24
#define INS_NOT 0x25
int get_ins_len(char, char);
int _ins_len(char);
int exectue_ins();
void lvm_debug(int ins_len);
int set_flag(char);
int set_flag(short);
int set_flag(unsigned);
int do_ins();
int do_mov();
int do_lea();
int do_int();
int do_jmp();
int do_jz();
int do_jnz();
int do_jh();
int do_jl();
int do_inc();
int do_dec();
int do_cmp();
int do_add();
int do_sub();
int do_xor();
int do_and();
int do_or();
int do_not();
#endif // !EXECUTEINSTRUCTION_H

