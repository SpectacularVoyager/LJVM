#pragma once
#include "Interpreter/Interpreter.h"

inline void VirtualMachine::IF_CMP_GE(StackFrame& frame,int){
	int branch=readShort();
	int op2=frame.popInt();
	int op1=frame.popInt();
	PRINTF("CHECK [%d] >= [%d]\n",op1,op2);
	if(op1>=op2){
		//OPCODE [SHORT]
		pc()+=branch-3;
	}
}
inline void VirtualMachine::IF_CMP_GT(StackFrame& frame,int){
	int branch=readShort();
	int op2=frame.popInt();
	int op1=frame.popInt();
	PRINTF("CHECK [%d] > [%d]\n",op1,op2);
	if(op1>op2){
		//OPCODE [SHORT]
		pc()+=branch-3;
	}
}
