#include "Interpreter.h"
#include "Attributes/Attribute.h"
#include "Constants/Constants.h"
#include "Utils/Utils.h"
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include "dlfcn.h"
#include "Arguments/Arguments.h"
typedef void (*dllfunc)();

#define PRINTF 
dllfunc getFunction(void* handler,std::string symbol){
	char* error;
	auto* object = (long*)dlsym(handler, symbol.c_str());
	if ((error = dlerror()) != nullptr && !object) {
		PANIC(error);
	}

	//* cast the variable "object" into the appropriate function pointer type
	return reinterpret_cast<dllfunc>(object);
}

void run(ClassFile& clazz){
	void* handler{};

	handler=dlopen("res/native/main.so",RTLD_LAZY);
	if (!handler) {
		PANIC(dlerror());
	}
	dlerror();
	dllfunc func;

	std::vector<unsigned char> data;

	std::string __MAIN="main";
	auto code = (CodeAttribute*)clazz.getMethod(__MAIN)->getAttribute("Code");
	if(code==NULL)PANIC("NO CODE FOUND");
	data=code->code;

	hexdump(data);
	unsigned int pc=0;
	std::stack<StackFrame> frames;
	frames.push(StackFrame(data));
	long _data;
	MethodRef* method;
	std::string class_name;
	std::string name;

	std::string method_desc;
	int op1,op2;
	while(pc<data.size()){
		int opcode=data[pc++];
		StackFrame& frame=frames.top();
		PRINTF("%d [0x%x]\t",pc-1,opcode);
		switch(opcode){
			case OPCODE::BIPUSH:
				//PUSH BYTE
				//std::cout<<"PUSHING BYTE("<<(int)data[pc]<<")\n";
				PRINTF("PUSHING BYTE(%d)\n",(int)data[pc]);
				frame.pushInt(data[pc++]);
				break;
			case OPCODE::ISTORE:
				op1=frame.popInt();
				op2=data[pc++];
				PRINTF("STORING %d IN REG[%d]\n",op1,op2);
				if(op2>=8) PANIC("CANNOT HANDLE MORE THAN 8 LOCAL VARIABLES GOT:"+std::to_string(op1));
				frame.setRegisterInt(op2,op1);
				break;
			case OPCODE::ISTORE_N:
			case OPCODE::ISTORE_N+1:
			case OPCODE::ISTORE_N+2:
			case OPCODE::ISTORE_N+3:
				//FROM STACK TOP
				_data=frame.popInt();
				PRINTF("STORING [%d] IN REG[%d]\n",(int)_data,opcode-OPCODE::ISTORE_N);
				frame.setRegisterInt(opcode-OPCODE::ISTORE_N,_data);
				break;
			case OPCODE::ILOAD:
				op2=data[pc++];
				op1=frame.getRegisterInt(op2);
				if(op2>=8) PANIC("CANNOT HANDLE MORE THAN 8 LOCAL VARIABLES GOT:"+std::to_string(op1));
				frame.pushInt(op1);
				PRINTF("LOADING %d FROM REG[%d]\n",op2,op1);
				break;
			case OPCODE::ILOAD_N:
			case OPCODE::ILOAD_N+1:
			case OPCODE::ILOAD_N+2:
			case OPCODE::ILOAD_N+3:
				//FROM STACK TOP
				_data=frame.getRegisterInt(opcode-OPCODE::ILOAD_N);
				frame.pushInt(_data);
				PRINTF("LOADING [%d] FROM REG[%d]\n",(int)_data,opcode-OPCODE::ILOAD_N);
				break;
			case OPCODE::PUSH_INT_CONST-1:
			case OPCODE::PUSH_INT_CONST:
			case OPCODE::PUSH_INT_CONST+1:
			case OPCODE::PUSH_INT_CONST+2:
			case OPCODE::PUSH_INT_CONST+3:
			case OPCODE::PUSH_INT_CONST+4:
			case OPCODE::PUSH_INT_CONST+5:
				PRINTF("CONST INT [%d]\n",(int)opcode-(int)OPCODE::PUSH_INT_CONST);
				frame.pushInt((int)opcode-(int)OPCODE::PUSH_INT_CONST);
				break;
			case OPCODE::IF_CMP_GE:
				_data=COMBINE_BYTE(data[pc], data[pc+1]);
				op2=frame.popInt();
				op1=frame.popInt();
				PRINTF("CHECK [%d] >= [%d]\t",op1,op2);
				if(op1>=op2){
					pc+=_data-1;
					PRINTF("JUMPING [%d]\n",pc);
				}else{
					pc+=2;
					PRINTF("NOT JUMPING [%d]\n",pc);
				}
				break;
			case OPCODE::GOTO:
				_data=COMBINE_BYTE(data[pc], data[pc+1]);
				//pc+=2;

				pc+=(short)_data-1;
				PRINTF("GOTO[%d]\n",pc);
				break;
			case OPCODE::RET:
				PRINTF("RET\n");
				//PUSH BYTE
				if(frames.size()<=1){
					goto exit;
				}else{
					frames.pop();
				}
				break;
			case OPCODE::LDC:
				op1=data[pc++]-1;
				frame.pushGeneric(OPERANDS::STRING,op1);
				//std::cout<<"LOADING:\t"<<*clazz.constants[op1];
				break;
			case OPCODE::IADD:
				op2=frame.popInt();
				op1=frame.popInt();
				PRINTF("ADDING [%d] ,[%d]\n",op1,op2);
				frame.pushInt(op1+op2);
				break;
			case OPCODE::IINC:
				op1=data[pc++];
				op2=data[pc++];
				PRINTF("INCREMENTING LOCAL[%d] BY %d\n",op1,op2);
				if(op1>=8) PANIC("CANNOT HANDLE MORE THAN 8 LOCAL VARIABLES GOT:"+std::to_string(op1));
				//frame.LocalVariableArray[op1]+=op2;
				frame.setRegisterInt(op1,frame.getRegisterInt(op1)+op2);
				//std::cout<<"LOADING:\t"<<*clazz.constants[op1];
				break;
			case OPCODE::INVOKE_STATIC:
				//CHECK NATIVE
				_data=COMBINE_BYTE(data[pc], data[pc+1])-1;
				method=cast<MethodRef>(clazz.constants[_data]);
				class_name=*method->clazz->name;
				name=method->name->getName();
				method_desc=method->name->getType();
				if(name=="printInt"){
					printf("%d\n",frame.popInt());
				}else{
					//FIX BUG WHERE STACK IS NOT POPPED CORRECTLY
					// for(auto x:getArgs(method->)){
					//
					// }
					for(auto x:getArgs(method_desc).first){
						frame.popGeneric();
						//std::cout<<"ARG:\t"<<x<<"\n";
					}
					func=getFunction(handler, "Java_"+class_name+"_"+name);
					func();
				}
				pc+=2;
				break;
			default:
				PANIC("UNSUPORTED OPCODE:\t"+std::to_string(opcode));
		}
	}
exit:
	dlclose(handler);
}
