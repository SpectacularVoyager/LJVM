#pragma once
#include "Interpreter/Interpreter.h"
#include <string>

inline int VirtualMachine::RET(StackFrame&,int){
	if(frames.size()<=1)return 1;
	frames.pop();
	PRINTF("RET\n");
	return 0;
}
inline int VirtualMachine::IRET(StackFrame& f,int){
	if(frames.size()<=1)return 1;
	int val=f.popInt();
	frames.pop();
	StackFrame &next=getFrame();
	next.pushInt(val);
	PRINTF("IRET -> %d\n",std::to_string(val).c_str());
	return 0;
}

inline void VirtualMachine::GOTO(StackFrame&,int){
	short branch=readShort()-3;

	pc()+=(short)(branch);
	PRINTF("GOTO[%d]\n",pc());
}
