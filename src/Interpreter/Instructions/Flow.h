#pragma once
#include "Interpreter/Interpreter.h"

inline int VirtualMachine::RET(StackFrame&,int){
	if(frames.size()<=1)return 1;
	frames.pop();
	return 0;
}
inline int VirtualMachine::IRET(StackFrame& f,int){
	if(frames.size()<=1)return 1;
	int val=f.popInt();
	frames.pop();
	StackFrame &next=getFrame();
	next.pushInt(val);
	return 0;
}

inline void VirtualMachine::GOTO(StackFrame&,int){
	short branch=readShort()-3;

	pc()+=(short)(branch);
	PRINTF("GOTO[%d]\n",pc());
}
