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

//TODO FIX
inline void VirtualMachine::invokeStatic(StackFrame& frame,int){
	int index=readShort()-1;
	MethodRef* method=cast<MethodRef>(main_clazz.constants[index]);
	std::string clazz_name=method->clazz->getName();
	std::string method_name=method->name->getName();
	std::string name="Java_"+clazz_name+"_"+method_name;
	Method* m=getClass(clazz_name).getMethod(method_name);
	if(!m)PANIC("METHOD NOT FOUND:\t"+method_name);
	PRINTF("INVOKING STATIC:\t%s\n",name.c_str());
	if(m->access.isNative()){
		if(method_name=="printInt" && clazz_name == "main"){
			printf("%d\n",frame.popInt());
		}else if(method_name=="println" && clazz_name == "main"){
			auto o=frame.popGeneric();
			if(o.type!=OPERANDS::STRING)PANIC("EXPECTED STRING GOT:"+std::to_string(o.type));
			jstring* jstr=(jstring*)o.value;
			printf("%s",jstr->str.c_str());
		}else{
			std::cout<<name<<"\n";
		}
	}else{
		//GET ARGS
		StackFrame next(((CodeAttribute*)m->getAttribute("Code"))->code);
		auto p=getArgs(m->desc->str);
		auto args=p.first;
		std::string ret=p.second;
		for(int i=args.size()-1;i>=0;i--){
			next.setRegister(i,getFrame().popGeneric());
		}
		frames.push(next);

		//PANIC("ONLY NATIVE METHODS SUPPORTED");
	}
}
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
inline void VirtualMachine::BIPUSH(StackFrame& frame,int){
	int val=readByte();
	PRINTF("PUSHING BYTE(" COLOR_CONST "%d" RST ")\n",val);
	frame.pushInt(val);
}
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
