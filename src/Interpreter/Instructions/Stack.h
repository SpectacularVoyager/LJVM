#pragma once
#include "Interpreter/Interpreter.h"

inline void VirtualMachine::dup(StackFrame& frame,int opcode){
	Operand o=frame.popGeneric();
	frame.pushGeneric(o);
	frame.pushGeneric(o);
}
//CATEGORY 1 INSTRUCTIONS ONLY
inline void VirtualMachine::POP(StackFrame& frame,int opcode){
	Operand o=frame.popGeneric();
}
