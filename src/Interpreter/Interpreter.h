#pragma once
#include "ClassFile/Classfile.h"

#include "Opcode.h"
#include <stack>
#include <vector>
#include "Utils/Utils.h"

void run(ClassFile& file);
#define PRINTF 

struct StackFrame{
	private:
		std::vector<long> LocalVariableArray;
	public:
		std::stack<int> operands;
		StackFrame(){
			//CHANGE THIS
			LocalVariableArray=std::vector<long>(8);
		}
		void pushInt(int x){
			PRINTF(GRN "PUSHING [%d]\n" RST,x);
			operands.push(x);
		}
		int popInt(){
			int x=operands.top();
			operands.pop();
			PRINTF(RED "POPPING [%d]\n" RST,x);
			return x;
		}
		void setRegisterInt(int index,int x){
			PRINTF(YEL "SETTING REGISTER[%d] -> %d\n" RST,index,x);
			LocalVariableArray[index]=x;
		}
		int getRegisterInt(int index){
			return LocalVariableArray[index];
		}
};
