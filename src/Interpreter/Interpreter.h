#pragma once
#include "ClassFile/Classfile.h"

#include "Opcode.h"
#include <stack>
#include <vector>

void run(ClassFile& file);

struct StackFrame{
	std::vector<long> LocalVariableArray;
	std::stack<int> operands;
	StackFrame(){
		//CHANGE THIS
		LocalVariableArray=std::vector<long>(8);
	}
	void pushInt(int x){
		operands.push(x);
	}
	int popInt(){
		int x=operands.top();
		operands.pop();
		return x;
	}
	void setRegisterInt(int index,int x){

	}
	int getRegisterInt(int index){

	}
};
