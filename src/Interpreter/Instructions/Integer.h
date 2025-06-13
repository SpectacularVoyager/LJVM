#pragma once

#include "Interpreter/Interpreter.h"

//INTEGER
inline void VirtualMachine::ILOAD(StackFrame& frame,int){
	int reg=readByte();
	int val=frame.getRegisterInt(reg);
	frame.pushInt(val);
	PRINTF("LOADING %d FROM REG[%d]\n",reg,val);
}
inline void VirtualMachine::ISTORE(StackFrame& frame,int){
	int val=frame.popInt();
	int reg=readByte();
	PRINTF("STORING %d IN " COLOR_REG "REG[%d]" RST "\n",val,reg);
	frame.setRegisterInt(reg,val);
}
inline void VirtualMachine::IADD(StackFrame& frame,int){
	int op2=frame.popInt();
	int op1=frame.popInt();
	PRINTF("ADDING [%d] ,[%d]\n",op1,op2);
	frame.pushInt(op1+op2);
}
inline void VirtualMachine::ISUB(StackFrame& frame,int){
	int op2=frame.popInt();
	int op1=frame.popInt();
	PRINTF("SUBTRACTING [%d] ,[%d]\n",op1,op2);
	frame.pushInt(op1-op2);
}
inline void VirtualMachine::IMUL(StackFrame& frame,int){
	int op2=frame.popInt();
	int op1=frame.popInt();
	PRINTF("MULTIPLYING [%d] ,[%d]\n",op1,op2);
	frame.pushInt(op1*op2);
}

inline void VirtualMachine::IINC(StackFrame& frame,int){
	int op1=readByte();
	int op2=readByte();
	PRINTF("INCREMENTING LOCAL[%d] BY %d\n",op1,op2);
	frame.setRegisterInt(op1,frame.getRegisterInt(op1)+op2);
}
inline void VirtualMachine::ISTORE_N(StackFrame& frame,int opcode){
	int val=frame.popInt();
	PRINTF("STORING " COLOR_CONST "%d " RST " IN " COLOR_REG "REG[%d]" RST "\n",val,opcode-OPCODE::ISTORE_N);
	frame.setRegisterInt(opcode-OPCODE::ISTORE_N,val);
}
inline void VirtualMachine::ILOAD_N(StackFrame& frame,int opcode){
	int val=frame.getRegisterInt(opcode-OPCODE::ILOAD_N);
	frame.pushInt(val);
	PRINTF("LOADING " COLOR_CONST "%d " RST " FROM " COLOR_REG "REG[%d]" RST "\n",val,opcode-OPCODE::ILOAD_N);
}
inline void VirtualMachine::ICONST_N(StackFrame& frame,int opcode){
	PRINTF("CONST INT " COLOR_CONST "%d" RST "\n",(int)opcode-(int)OPCODE::PUSH_INT_CONST);
	frame.pushInt((int)opcode-(int)OPCODE::PUSH_INT_CONST);
}
