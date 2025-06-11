#pragma once

#include "Constants/Constants.h"
#include <unordered_map>

class ClassMember{
	public:
		int access;	
		NameAndType** name;
		UTF8String** desc;
		std::unordered_map<std::string,Attribute*> attributes;
		ClassMember(ClassFile& clazz,std::ifstream& file);
		~ClassMember();
};

class Field :public ClassMember{
	public:
		Field(ClassFile& clazz,std::ifstream& file):ClassMember(clazz,file){}

};
class Method :public ClassMember{
	public:
		Method(ClassFile& clazz,std::ifstream& file):ClassMember(clazz,file){}
};
