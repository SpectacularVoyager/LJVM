#pragma once
#include "ClassFile/Classfile.h"

#include "Opcode.h"
#include <ostream>
#include <stack>
#include <string>
#include <vector>
#include "Utils/Utils.h"
#include "Operands.h"
void run(ClassFile& file);
#define PRINTF

struct Operand{
	int type;
	long value;
	friend std::ostream& operator<<(std::ostream& os,Operand& op){
		os<<"["<<op.type<<"] -> ["<<op.value<<"]";
		return os;
	}
};
struct StackFrame{
	private:
		std::vector<long> LocalVariableArray;
	public:
		std::vector<unsigned char> data;
		unsigned int pc=0;
		std::stack<Operand> operands;
		StackFrame(std::vector<unsigned char> data):data(data){
			//CHANGE THIS
			LocalVariableArray=std::vector<long>(8);
			PRINTF("NEW FRAME\n");
			//hexdump(data);
		}
		void pushInt(int x){
			pushGeneric({OPERANDS::INT,x});
		}
		int popInt(){
			auto _x=popGeneric();
			if(_x.type!=OPERANDS::INT)PANIC("EXPECTED INT GOT:"+std::to_string(_x.type));
			return _x.value;
		}
		Operand popGeneric(){
			auto x=operands.top();
			operands.pop();
			PRINTF(RED "POPPING [%ld]\n" RST,x.value);
			return x;
		}
		void pushGeneric(Operand op){
			operands.push(op);
			PRINTF(GRN "PUSHING [%ld]\n" RST,op.value);
		}
		void pushGeneric(int type,long x){
			pushGeneric({type,x});
		}
		inline void setRegisterLong(int index,long x){
			PRINTF(YEL "SETTING REGISTER[%d] -> %ld\n",index,x);
			if(index>=8)PANIC("TRYING TO SET REGISTER:"+std::to_string(index));
			LocalVariableArray[index]=x;
		}
		inline long getRegisterLong(int index){
			if(index>=8)PANIC("TRYING TO SET REGISTER:"+std::to_string(index));
			return LocalVariableArray[index];
		}
		inline void setRegisterInt(int index,int x){
			setRegisterLong(index,x);
		}
		inline int getRegisterInt(int index){
			return (int)getRegisterLong(index);
		}
		inline void setRegister(int index,Operand x){
			if(index>=8)PANIC("TRYING TO SET REGISTER:"+std::to_string(index));
			switch(x.type){
				case OPERANDS::INT:
					setRegisterInt(index, x.value);
					break;
				default:PANIC("HANDLE FOR:\t"+std::to_string(x.type));
			}
		}
		inline long getRegister(int index){
			if(index>=8)PANIC("TRYING TO SET REGISTER:"+std::to_string(index));
			return LocalVariableArray[index];
		}

};
class VirtualMachine{
		ClassFile& main_clazz;
		std::stack<StackFrame> frames;
	public:
		void runMain();
		StackFrame& getFrame(){return frames.top();}
		unsigned int& pc(){return getFrame().pc;}
		inline unsigned char readByte(){
			return getFrame().data[pc()++];
		}
		inline unsigned short readShort(){
			unsigned short s= COMBINE_BYTE(getFrame().data[pc()], getFrame().data[pc()+1]);
			pc()+=2;
			return s;
		}
		ClassFile& getClass(std::string& name);
		VirtualMachine(ClassFile& clazz):main_clazz(clazz){}

		inline void LDC(StackFrame& frame,int opcode=OPCODE::LDC);
		inline void invokeStatic(StackFrame& frame,int opcode=OPCODE::INVOKE_STATIC);
		inline void BIPUSH(StackFrame& frame,int opcode=OPCODE::BIPUSH);

		inline void IF_CMP_GE(StackFrame& frame,int opcode=OPCODE::IF_CMP_GE);
		inline void IF_CMP_GT(StackFrame& frame,int opcode=OPCODE::IF_CMP_GT);
		//RETURNS FIN
		inline int IRET(StackFrame& frame,int opcode=OPCODE::RET);
		inline int RET(StackFrame& frame,int opcode=OPCODE::RET);
		inline void GOTO(StackFrame& frame,int opcode=OPCODE::ISTORE);

		//INTEGER STUFF
		inline void IADD(StackFrame& frame,int opcode=OPCODE::IADD);
		inline void ISUB(StackFrame& frame,int opcode=OPCODE::ISUB);
		inline void IMUL(StackFrame& frame,int opcode=OPCODE::IMUL);
		inline void ICONST_N(StackFrame& frame,int opcode);
		inline void ISTORE_N(StackFrame& frame,int opcode);
		inline void ILOAD_N(StackFrame& frame,int opcode);
		inline void ILOAD(StackFrame& frame,int opcode=OPCODE::ILOAD);
		inline void ISTORE(StackFrame& frame,int opcode=OPCODE::ISTORE);
		inline void IINC(StackFrame& frame,int opcode=OPCODE::ISTORE);

};

