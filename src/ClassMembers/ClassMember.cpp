#include "ClassMember.h"
#include "Attributes/Attribute.h"
#include "ClassFile/Classfile.h"
#include "Utils/Utils.h"
#include "Constants/ConstantDefs.h"
#include <iostream>
template <typename T>
T* cast(Constant* c);

ClassMember::ClassMember(ClassFile& clazz,std::ifstream& file){
	access=readU16(file);
	this->name=cast<UTF8String>(clazz.constants[readU16(file)-1]);
	this->desc=cast<UTF8String>(clazz.constants[readU16(file)-1]);
	int n_attrib=readU16(file);
	for(int i=0;i<n_attrib;i++){
		auto a=Attributes::getAttribute(clazz,file);
		attributes[a->getName()]=a;
	}
}
//TODO FIX
ClassMember::~ClassMember(){
	for(auto x:attributes){
		//delete x.second;
	}
}

Attribute* ClassMember::getAttribute(std::string name){
	auto x=attributes.find(name);
	if(x==attributes.end()){return NULL;}
	return x->second;
}
UTF8String& ClassMember::getName(){
	return *name;
}
