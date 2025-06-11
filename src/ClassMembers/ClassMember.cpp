#include "ClassMember.h"
#include "Attributes/Attribute.h"
#include "ClassFile/Classfile.h"
#include "Utils/Utils.h"
#include <iostream>


ClassMember::ClassMember(ClassFile& clazz,std::ifstream& file){
	access=readU16(file);
	this->name=(NameAndType**)&clazz.constants[readU16(file)-1];
	this->desc=(UTF8String**)&clazz.constants[readU16(file)-1];
	int n_attrib=readU16(file);
	for(int i=0;i<n_attrib;i++){
		auto a=Attributes::getAttribute(clazz,file);
		attributes[a->getName()]=a;
	}
}
ClassMember::~ClassMember(){
	for(auto x:attributes){
		delete x.second;
	}
}
