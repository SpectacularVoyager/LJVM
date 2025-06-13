#pragma once
#include "Interpreter/Interpreter.h"
#include "Constants/Constants.h"
#include "jni/jni.h"

//TODO FIX
inline void VirtualMachine::LDC(StackFrame& frame,int){
	int val=readByte()-1;
	Constant* c=main_clazz.constants[val];
	jstring* s;
	std::string str;
	switch(c->tag){
		case Constants::CONSTANT_STRING:
			//POINTER TO JSTRING
			str=cast<ConstantString>(c)->str->str;
			s=new jstring{str};
			frame.pushGeneric({OPERANDS::STRING,(long)s});
			break;
		default:
			PANIC("CANNOT HANDLE TAG:\t"+std::to_string(c->tag));
	}
}

inline void VirtualMachine::BIPUSH(StackFrame& frame,int){
	int val=readByte();
	PRINTF("PUSHING BYTE(" COLOR_CONST "%d" RST ")\n",val);
	frame.pushInt(val);
}
