#pragma once
#include "Interpreter/Interpreter.h"
#include "Constants/Constants.h"
#include "jni/jni.h"
#include "Arguments/Arguments.h"
#include <iostream>

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
