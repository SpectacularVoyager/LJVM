#include "ClassMembers/ClassMember.h"
#include "Interpreter.h"
#include "Interpreter/Opcode.h"
#include "Constants/Constants.h"
#include <iostream>
#include <string>

#define PRINTF 
void VirtualMachine::runMain(){
	std::string __MAIN="main";
	auto code = (CodeAttribute*)clazz.getMethod(__MAIN)->getAttribute("Code");
	if(code==NULL)PANIC("NO CODE FOUND");
	std::vector<unsigned char> data=code->code;
	hexdump(data);

#define CASE(NAME,INST) case NAME: \
	INST;break;
	frames.push(StackFrame());
	while(pc<data.size()){
		unsigned char opcode=readByte(data);
		//printf("%d\n",opcode);
		StackFrame& frame=getFrame();
		switch(opcode){
			CASE(OPCODE::LDC,LDC(frame,data));
			CASE(OPCODE::INVOKE_STATIC,invokeStatic(frame,data));
			CASE(OPCODE::BIPUSH,BIPUSH(frame,data));
			CASE(OPCODE::IF_CMP_GE,IF_CMP_GE(frame,data));
			CASE(OPCODE::RET,RET(frame,data));
			CASE(OPCODE::ISTORE,ISTORE(frame,data));
			CASE(OPCODE::IADD,IADD(frame,data));
			CASE(OPCODE::ILOAD,ILOAD(frame,data));
			CASE(OPCODE::IINC,IINC(frame,data));
			CASE(OPCODE::GOTO,GOTO(frame,data));
				
			case OPCODE::ISTORE_N:
			case OPCODE::ISTORE_N+1:
			case OPCODE::ISTORE_N+2:
			case OPCODE::ISTORE_N+3:
				ISTORE_N(frame,data,opcode);
				break;
			case OPCODE::ILOAD_N:
			case OPCODE::ILOAD_N+1:
			case OPCODE::ILOAD_N+2:
			case OPCODE::ILOAD_N+3:
				//FROM STACK TOP
				ILOAD_N(frame,data,opcode);
				break;
			case OPCODE::PUSH_INT_CONST-1:
			case OPCODE::PUSH_INT_CONST:
			case OPCODE::PUSH_INT_CONST+1:
			case OPCODE::PUSH_INT_CONST+2:
			case OPCODE::PUSH_INT_CONST+3:
			case OPCODE::PUSH_INT_CONST+4:
			case OPCODE::PUSH_INT_CONST+5:
				ICONST_N(frame,data,opcode);
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
inline void VirtualMachine::invokeStatic(StackFrame& frame,std::vector<unsigned char>& data,int){
	int index=readShort(data)-1;
	MethodRef* method=cast<MethodRef>(clazz.constants[index]);
	std::string clazz_name=method->clazz->getName();
	std::string method_name=method->name->getName();
	std::string name="Java_"+clazz_name+"_"+method_name;
	if(method_name=="printInt" && clazz_name == "main"){
		printf("%d\n",frame.popInt());
	}else{
		std::cout<<name<<"\n";
	}
}
//TODO FIX
inline void VirtualMachine::LDC(StackFrame& frame,std::vector<unsigned char>& data,int){
	int val=readByte(data)-1;
	frame.pushGeneric(OPERANDS::STRING,val);
}

inline void VirtualMachine::IF_CMP_GE(StackFrame& frame,std::vector<unsigned char>& data,int){
	int branch=readShort(data);
	int op2=frame.popInt();
	int op1=frame.popInt();
	PRINTF("CHECK [%d] >= [%d]\n",op1,op2);
	if(op1>=op2){
		//OPCODE [SHORT]
		pc+=branch-3;
	}
}
inline void VirtualMachine::BIPUSH(StackFrame& frame,std::vector<unsigned char>& data,int){
	int val=readByte(data);
	PRINTF("PUSHING BYTE(" COLOR_CONST "%d" RST ")\n",val);
	frame.pushInt(val);
}
inline int VirtualMachine::RET(StackFrame&,std::vector<unsigned char>&,int){
	if(frames.size()<=1)return 1;
	frames.pop();
	return 0;
}

inline void VirtualMachine::GOTO(StackFrame&,std::vector<unsigned char>& data,int){
	short branch=readShort(data)-3;

	pc+=(short)(branch);
	PRINTF("GOTO[%d]\n",pc);
}
//INTEGER
inline void VirtualMachine::ILOAD(StackFrame& frame,std::vector<unsigned char>& data,int){
	int reg=readByte(data);
	int val=frame.getRegisterInt(reg);
	frame.pushInt(val);
	PRINTF("LOADING %d FROM REG[%d]\n",reg,val);
}
inline void VirtualMachine::ISTORE(StackFrame& frame,std::vector<unsigned char>& data,int){
	int val=frame.popInt();
	int reg=readByte(data);
	PRINTF("STORING %d IN " COLOR_REG "REG[%d]" RST "\n",val,reg);
	frame.setRegisterInt(reg,val);
}
inline void VirtualMachine::IADD(StackFrame& frame,std::vector<unsigned char>& data,int){
	int op2=frame.popInt();
	int op1=frame.popInt();
	PRINTF("ADDING [%d] ,[%d]\n",op1,op2);
	frame.pushInt(op1+op2);
}

inline void VirtualMachine::IINC(StackFrame& frame,std::vector<unsigned char>& data,int){
	int op1=readByte(data);
	int op2=readByte(data);
	PRINTF("INCREMENTING LOCAL[%d] BY %d\n",op1,op2);
	frame.setRegisterInt(op1,frame.getRegisterInt(op1)+op2);
}
inline void VirtualMachine::ISTORE_N(StackFrame& frame,std::vector<unsigned char>&,int opcode){
	int val=frame.popInt();
	PRINTF("STORING " COLOR_CONST "%d " RST " IN " COLOR_REG "REG[%d]" RST "\n",val,opcode-OPCODE::ISTORE_N);
	frame.setRegisterInt(opcode-OPCODE::ISTORE_N,val);
}
inline void VirtualMachine::ILOAD_N(StackFrame& frame,std::vector<unsigned char>&,int opcode){
	int val=frame.getRegisterInt(opcode-OPCODE::ILOAD_N);
	frame.pushInt(val);
	PRINTF("LOADING " COLOR_CONST "%d " RST " FROM " COLOR_REG "REG[%d]" RST "\n",val,opcode-OPCODE::ILOAD_N);
}
inline void VirtualMachine::ICONST_N(StackFrame& frame,std::vector<unsigned char>&,int opcode){
	PRINTF("CONST INT " COLOR_CONST "%d" RST "\n",(int)opcode-(int)OPCODE::PUSH_INT_CONST);
	frame.pushInt((int)opcode-(int)OPCODE::PUSH_INT_CONST);
}
