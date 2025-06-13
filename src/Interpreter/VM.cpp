#include "ClassMembers/ClassMember.h"
#include "Constants/ConstantDefs.h"
#include "Interpreter.h"
#include "Interpreter/Opcode.h"
#include "Constants/Constants.h"
#include "Arguments/Arguments.h"

#include"jni/jni.h"

#include <functional>
#include <iostream>
#include <string>
#include "dlfcn.h"
#include "Instructions/Compare.h"
#include "Instructions/Const.h"
#include "Instructions/Flow.h"
#include "Instructions/Integer.h"
#include "Instructions/References.h"

template<typename Signature>
std::function<Signature> VirtualMachine::fromDLL(){

}

VirtualMachine::VirtualMachine(std::vector<ClassFile>& classes,ClassFile& clazz,void* dllHandler)
:classes(classes),main_clazz(clazz),dllHandler(dllHandler)
{
	for(ClassFile& f:classes){
		classMap[f.clazz->getName()]=&f;
	}
}
ClassFile& VirtualMachine::getClass(std::string& name){
	auto x=classMap.find(name);
	if(x==classMap.end())PANIC("COULD NOT FIND CLASS:\t"+name);
	return *x->second;
}
#define PRINTF
void VirtualMachine::runMain(){
	std::string __MAIN="main";
	auto code = (CodeAttribute*)main_clazz.getMethod(__MAIN)->getAttribute("Code");
	if(code==NULL)PANIC("NO CODE FOUND");
	std::vector<unsigned char> data=code->code;

#define CASE(NAME,INST) case NAME: \
	INST;break;
	frames.push(StackFrame(data));
	while(pc()<data.size()){
		unsigned char opcode=readByte();
		//printf("%d\n",opcode);
		StackFrame& frame=getFrame();
		switch(opcode){
			CASE(OPCODE::LDC,LDC(frame));
			CASE(OPCODE::INVOKE_STATIC,invokeStatic(frame));
			CASE(OPCODE::BIPUSH,BIPUSH(frame));
			CASE(OPCODE::ISTORE,ISTORE(frame));
			CASE(OPCODE::IADD,IADD(frame));
			CASE(OPCODE::ISUB,ISUB(frame));
			CASE(OPCODE::IMUL,IMUL(frame));
			CASE(OPCODE::ILOAD,ILOAD(frame));
			CASE(OPCODE::IINC,IINC(frame));

			CASE(OPCODE::RET,RET(frame));
			CASE(OPCODE::IRET,IRET(frame));
			CASE(OPCODE::GOTO,GOTO(frame));

			CASE(OPCODE::IF_CMP_GE,IF_CMP_GE(frame));
			CASE(OPCODE::IF_CMP_GT,IF_CMP_GT(frame));
				
			case OPCODE::ISTORE_N:
			case OPCODE::ISTORE_N+1:
			case OPCODE::ISTORE_N+2:
			case OPCODE::ISTORE_N+3:
				ISTORE_N(frame,opcode);
				break;
			case OPCODE::ILOAD_N:
			case OPCODE::ILOAD_N+1:
			case OPCODE::ILOAD_N+2:
			case OPCODE::ILOAD_N+3:
				//FROM STACK TOP
				ILOAD_N(frame,opcode);
				break;
			case OPCODE::PUSH_INT_CONST-1:
			case OPCODE::PUSH_INT_CONST:
			case OPCODE::PUSH_INT_CONST+1:
			case OPCODE::PUSH_INT_CONST+2:
			case OPCODE::PUSH_INT_CONST+3:
			case OPCODE::PUSH_INT_CONST+4:
			case OPCODE::PUSH_INT_CONST+5:
				ICONST_N(frame,opcode);
				break;
			default:
				PANIC("UNSUPORTED OPCODE:\t"+opcodes[opcode]+" ["+std::to_string(opcode)+"]");
		}
	}
#undef CASE
}
#define COLOR_CONST YEL
#define COLOR_REG GRN

