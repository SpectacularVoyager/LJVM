#include "Interpreter.h"
StackFrame::StackFrame(std::vector<unsigned char> data):data(data){
	//CHANGE THIS
	LocalVariableArray=std::vector<long>(8);
	PRINTF("NEW FRAME\n");
	//hexdump(data);
}
void StackFrame::pushInt(int x){
	pushGeneric({OPERANDS::INT,x});
}
int StackFrame::popInt(){
	auto _x=popGeneric();
	if(_x.type!=OPERANDS::INT)PANIC("EXPECTED INT GOT:"+std::to_string(_x.type));
	return _x.value;
}
Operand StackFrame::popGeneric(){
	auto x=operands.top();
	operands.pop();
	PRINTF(RED "POPPING [%ld]\n" RST,x.value);
	return x;
}
void StackFrame::pushGeneric(Operand op){
	operands.push(op);
	PRINTF(GRN "PUSHING [%ld]\n" RST,op.value);
}
void StackFrame::pushGeneric(int type,long x){
	pushGeneric({type,x});
}
inline void StackFrame::setRegisterLong(int index,long x){
	PRINTF(YEL "SETTING REGISTER[%d] -> %ld\n",index,x);
	if(index>=8)PANIC("TRYING TO SET REGISTER:"+std::to_string(index));
	LocalVariableArray[index]=x;
}
long StackFrame::getRegisterLong(int index){
	if(index>=8)PANIC("TRYING TO SET REGISTER:"+std::to_string(index));
	return LocalVariableArray[index];
}
void StackFrame::setRegisterInt(int index,int x){
	setRegisterLong(index,x);
}
int StackFrame::getRegisterInt(int index){
	return (int)getRegisterLong(index);
}
void StackFrame::setRegister(int index,Operand x){
	if(index>=8)PANIC("TRYING TO SET REGISTER:"+std::to_string(index));
	switch(x.type){
		case OPERANDS::INT:
			setRegisterInt(index, x.value);
			break;
		default:PANIC("HANDLE FOR:\t"+std::to_string(x.type));
	}
}
long StackFrame::getRegister(int index){
	if(index>=8)PANIC("TRYING TO SET REGISTER:"+std::to_string(index));
	return LocalVariableArray[index];
}

