#pragma once
#include "ClassFile/Classfile.h"

#include "Opcode.h"
#include <functional>
#include <ostream>
#include <stack>
#include <string>
#include <vector>
#include "Utils/Utils.h"
#include "Operands.h"
void run(ClassFile& file);
#define PRINTF 
// #define PRINTF printf

#define COLOR_CONST YEL
#define COLOR_REG GRN

struct Operand{
	int type=0;
	long value=0;
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
		StackFrame(std::vector<unsigned char> data);
		void pushInt(int x);
		int popInt();
		Operand popGeneric();
		void pushGeneric(Operand op);
		void pushGeneric(int type,long x);
		void setRegisterLong(int index,long x);
		long getRegisterLong(int index);
		void setRegisterInt(int index,int x);
		int getRegisterInt(int index);
		void setRegister(int index,Operand x);
		long getRegister(int index);

};
class VirtualMachine{
		std::stack<StackFrame> frames;
		std::vector<ClassFile*>& classes;
		ClassFile& main_clazz;
		void* dllHandler;
		std::map<std::string,ClassFile*> classMap;

		template<typename Signature>
		std::function<Signature> fromDLL();

		void callFunction(void* thiz,Method& m,StackFrame& frame,bool fromStack=0);
	public:
		VirtualMachine(std::vector<ClassFile*>& classes,ClassFile& clazz,void* dllHandler);
		void reset();
		int runMethod(std::string n);
		StackFrame& getFrame(){return frames.top();}
		unsigned int& pc(){return getFrame().pc;}
		inline unsigned char readByte(){
			return getFrame().data[pc()++];
		}
		inline unsigned short readShort(){
			unsigned short s= COMBINE_BYTE(readByte(),readByte());
			return s;
		}
		ClassFile& getClass(std::string& name);

		inline void LDC(StackFrame& frame,int opcode=OPCODE::LDC);
		inline void BIPUSH(StackFrame& frame,int opcode=OPCODE::BIPUSH);

		//REFERENCES
		inline void putStatic(StackFrame& frame,int opcode=OPCODE::INVOKE_STATIC);
		inline void getStatic(StackFrame& frame,int opcode=OPCODE::INVOKE_STATIC);
		inline void putField(StackFrame& frame,int opcode=OPCODE::INVOKE_STATIC);
		inline void getField(StackFrame& frame,int opcode=OPCODE::INVOKE_STATIC);
		inline void invokeStatic(StackFrame& frame,int opcode=OPCODE::INVOKE_STATIC);
		inline void invokeSpecial(StackFrame& frame,int opcode=OPCODE::INVOKE_SPECIAL);
		inline void invokeVirtual(StackFrame& frame,int opcode=OPCODE::INVOKE_SPECIAL);
		inline void NEW(StackFrame& frame,int opcode=OPCODE::NEW);

		//STACK
		inline void dup(StackFrame& frame,int opcode=OPCODE::DUP);
		inline void POP(StackFrame& frame,int opcode=OPCODE::DUP);

		//COMPARISONS
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

		//REFERENCE STUFF
		inline void ALOAD_N(StackFrame& frame,int opcode);
		inline void ASTORE_N(StackFrame& frame,int opcode);

};

