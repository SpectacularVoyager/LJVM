#pragma once
#include "Constants/ConstantDefs.h"
#include "Interpreter/Interpreter.h"
#include "Constants/Constants.h"
#include "Utils/Utils.h"
#include "jni/jni.h"
#include "Arguments/Arguments.h"
#include "Objects/Objects.h"
#include <iostream>
#include <string>

inline void VirtualMachine::getStatic(StackFrame& frame,int){
	int index=readShort()-1;
	auto c=main_clazz.constants[index];
	//std::cout<<*c<<"\n";
	auto ref=cast<FieldRef>(c);
	frame.pushGeneric(main_clazz.staticObject->fields[ref->name->getName()]);
}
inline void VirtualMachine::putStatic(StackFrame& frame,int){
	int index=readShort()-1;
	auto c=main_clazz.constants[index];
	//std::cout<<*c<<"\n";
	auto ref=cast<FieldRef>(c);
	//CHECK TYPE
	main_clazz.staticObject->fields[ref->name->getName()]=frame.popGeneric();
}
inline void VirtualMachine::getField(StackFrame& frame,int){
	Operand ref=frame.popGeneric();
	int index=readShort()-1;
	if(ref.type!=OPERANDS::OBJECT_REF)PANIC("EXPECTED OBJECT REF GOT:\t"+std::to_string(ref.type));
	ObjectRef* obj=(ObjectRef*)ref.value;
	auto c=cast<FieldRef>(main_clazz.constants[index]);
	frame.pushGeneric(obj->fields[c->name->getName()]);
}
inline void VirtualMachine::putField(StackFrame& frame,int){
	Operand val=frame.popGeneric();
	Operand ref=frame.popGeneric();
	if(ref.type!=OPERANDS::OBJECT_REF)PANIC("EXPECTED OBJECT REF GOT:\t"+std::to_string(ref.type));
	ObjectRef* obj=(ObjectRef*)ref.value;
	int index=readShort()-1;
	auto c=cast<FieldRef>(main_clazz.constants[index]);
	obj->fields[c->name->getName()]=val;
}
void VirtualMachine::callFunction(void* thiz,Method& m,StackFrame& frame,bool fromStack){
	StackFrame next(((CodeAttribute*)m.getAttribute("Code"))->code);
	auto p=getArgs(m.desc->str);
	auto args=p.first;
	std::string ret=p.second;
	if((thiz!=NULL)||(fromStack)){
		for(int i=args.size();i>=1;i--){
			next.setRegister(i,getFrame().popGeneric());
		}
		if(fromStack){
			Operand op=frame.popGeneric();
			if(op.type!=OPERANDS::OBJECT_REF)PANIC("NEED OBJECT GOT:\t"+std::to_string(op.type));
			next.setRegister(0,{OPERANDS::OBJECT_REF,op.value});
		}else{
			next.setRegister(0,{OPERANDS::OBJECT_REF,(long)thiz});
		}
	}else{
		for(int i=args.size()-1;i>=0;i--){
			next.setRegister(i,getFrame().popGeneric());
		}

	}
	frames.push(next);
}
//TODO READ THIS https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.invokespecial
inline void VirtualMachine::invokeSpecial(StackFrame& frame,int){
	int index=readShort()-1;
	MethodRef* method=cast<MethodRef>(main_clazz.constants[index]);
	std::string clazz_name=method->clazz->getName();
	if(clazz_name=="java/lang/Object"){
		printf("IGNORING CALLS TO java/lang/Object\n");
		return;
	}
	std::string method_name=method->name->getName();
	Method* m=getClass(clazz_name).getMethod(method_name);
	if(!m)PANIC("METHOD NOT FOUND:\t"+method_name);
	if(m->access.isNative())PANIC("DO NOT SUPPORT NATIVE FUNCTIONS");
	if(method->name->getName()=="<init>"){
		Operand op=frame.popGeneric();
		if(op.type!=OPERANDS::OBJECT_REF)PANIC("NEED OBJECT GOT:\t"+std::to_string(op.type));
		callFunction((void*)op.value,*m,frame);
	}else{
		TODO;
	}
}
inline void VirtualMachine::invokeVirtual(StackFrame& frame,int){
	int index=readShort()-1;
	MethodRef* method=cast<MethodRef>(main_clazz.constants[index]);
	std::string clazz_name=method->clazz->getName();
	// if(clazz_name=="java/lang/Object"){
	// 	printf("IGNORING CALLS TO java/lang/Object\n");
	// 	return;
	// }
	std::string method_name=method->name->getName();
	ClassFile& info=getClass(clazz_name);
	Method* m=info.getMethod(method_name);
	while(info.super&&!m){
		m=info.getMethod(method_name);
		if(info.clazz->getName()=="java/lang/Object")PANIC("METHOD NOT FOUND IN java/lang/Object");
		std::string s=info.super->getName();
		info=getClass(s);
	}
	if(!m)PANIC("METHOD NOT FOUND:\t"+method_name);
	if(m->access.isNative())PANIC("DO NOT SUPPORT NATIVE FUNCTIONS");
	callFunction(NULL,*m,frame,true);
}
//TODO FIX
inline void VirtualMachine::invokeStatic(StackFrame& frame,int){
	int index=readShort()-1;
	MethodRef* method=cast<MethodRef>(main_clazz.constants[index]);
	std::string clazz_name=method->clazz->getName();
	std::string method_name=method->name->getName();
	Method* m=getClass(clazz_name).getMethod(method_name);
	if(!m)PANIC("METHOD NOT FOUND:\t"+method_name);
	if(m->access.isNative()){
		std::string name="Java_"+clazz_name+"_"+method_name;
		PRINTF("INVOKING STATIC:\t%s\n",name.c_str());
		if(method_name=="printInt" && clazz_name == "Main"){
			printf("%d\n",frame.popInt());
		}else if(method_name=="println" && clazz_name == "Main"){
			auto o=frame.popGeneric();
			if(o.type!=OPERANDS::STRING)PANIC("EXPECTED STRING GOT:"+std::to_string(o.type));
			jstring* jstr=(jstring*)o.value;
			printf("%s",jstr->str.c_str());
		}else{
			std::cout<<name<<"\n";
		}
	}else{
		callFunction(NULL,*m,frame);
	}
}
inline void VirtualMachine::NEW(StackFrame& frame,int){
	int index=readShort()-1;
	Constant* c=main_clazz.constants[index];
	if(c->tag==Constants::CLASS_INFO){
		ClassInfo* obj=cast<ClassInfo>(c);
		ObjectRef* x=Objects::newInstance(*obj);
		frame.pushGeneric({OPERANDS::OBJECT_REF,(long)x});
	}else{
		PANIC("CANNOT WORK WITH TAG:\t"+std::to_string(c->tag));
	}

}
