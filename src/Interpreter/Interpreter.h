#pragma once
#include "ClassFile/Classfile.h"

#include "Opcode.h"
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
};
struct StackFrame{
	private:
		std::vector<long> LocalVariableArray;
	public:
		std::stack<Operand> operands;
		StackFrame(){
			//CHANGE THIS
			LocalVariableArray=std::vector<long>(8);
		}
		void pushInt(int x){
			PRINTF(GRN "PUSHING [%d]\n" RST,x);
			operands.push({OPERANDS::INT,x});
		}
		int popInt(){
			auto _x=operands.top();
			if(_x.type!=OPERANDS::INT)PANIC("EXPECTED INT GOT:"+std::to_string(_x.type));
			int x=_x.value;
			operands.pop();
			PRINTF(RED "POPPING [%d]\n" RST,x);
			return x;
		}
		Operand popGeneric(){
			auto x=operands.top();
			operands.pop();
			return x;
		}
		void pushGeneric(int type,long x){
			operands.push({type,x});
		}
		void setRegisterInt(int index,int x){
			PRINTF(YEL "SETTING REGISTER[%d] -> %d\n",index,x);
			if(index>=8)PANIC("TRYING TO SET REGISTER:"+std::to_string(index));
			LocalVariableArray[index]=x;
		}
		int getRegisterInt(int index){
			if(index>=8)PANIC("TRYING TO SET REGISTER:"+std::to_string(index));
			return LocalVariableArray[index];
		}
};
class VirtualMachine{
		ClassFile& clazz;
		unsigned int pc=0;
		std::stack<StackFrame> frames;
	public:
		void runMain();
		inline unsigned char readByte(std::vector<unsigned char>& ref){
			return ref[pc++];
		}
		inline unsigned short readShort(std::vector<unsigned char>& ref){
			unsigned short s= COMBINE_BYTE(ref[pc], ref[pc+1]);
			pc+=2;
			return s;
		}
		StackFrame& getFrame(){return frames.top();}
		VirtualMachine(ClassFile& clazz):clazz(clazz){}

		inline void LDC(StackFrame& frame,std::vector<unsigned char>& data,int opcode=OPCODE::LDC);
		inline void invokeStatic(StackFrame& frame,std::vector<unsigned char>& data,int opcode=OPCODE::INVOKE_STATIC);
		inline void BIPUSH(StackFrame& frame,std::vector<unsigned char>& data,int opcode=OPCODE::BIPUSH);
		inline void IF_CMP_GE(StackFrame& frame,std::vector<unsigned char>& data,int opcode=OPCODE::IF_CMP_GE);
		//RETURNS FIN
		inline int RET(StackFrame& frame,std::vector<unsigned char>& data,int opcode=OPCODE::RET);
		inline void GOTO(StackFrame& frame,std::vector<unsigned char>& data,int opcode=OPCODE::ISTORE);

		//INTEGER STUFF
		inline void IADD(StackFrame& frame,std::vector<unsigned char>& data,int opcode=OPCODE::IADD);
		inline void ICONST_N(StackFrame& frame,std::vector<unsigned char>& data,int opcode);
		inline void ISTORE_N(StackFrame& frame,std::vector<unsigned char>& data,int opcode);
		inline void ILOAD_N(StackFrame& frame,std::vector<unsigned char>& data,int opcode);
		inline void ILOAD(StackFrame& frame,std::vector<unsigned char>& data,int opcode=OPCODE::ILOAD);
		inline void ISTORE(StackFrame& frame,std::vector<unsigned char>& data,int opcode=OPCODE::ISTORE);
		inline void IINC(StackFrame& frame,std::vector<unsigned char>& data,int opcode=OPCODE::ISTORE);

};

