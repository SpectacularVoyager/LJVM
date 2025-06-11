#pragma once

#include <unordered_map>
#include "fstream"
#include "Attributes/Attribute.h"
class ClassFile;
class NameAndType;
class UTF8String;


class ClassMember{
	public:
		int access;	
		UTF8String** name;
		UTF8String** desc;
		std::unordered_map<std::string,Attribute*> attributes;
		ClassMember(ClassFile& clazz,std::ifstream& file);
		~ClassMember();
		Attribute* getAttribute(std::string name);
		UTF8String& getName();
};

class Field :public ClassMember{
	public:
		Field(ClassFile& clazz,std::ifstream& file):ClassMember(clazz,file){}

};
class Method :public ClassMember{
	public:
		Method(ClassFile& clazz,std::ifstream& file):ClassMember(clazz,file){}
};
