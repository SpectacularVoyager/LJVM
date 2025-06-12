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
		UTF8String* name;
		UTF8String* desc;
		std::unordered_map<std::string,Attribute*> attributes;
		ClassMember(ClassFile& clazz,std::ifstream& file);
		~ClassMember();
		Attribute* getAttribute(std::string name);
		UTF8String& getName();
		virtual void print(std::ostream& os) const = 0;
		friend std::ostream& operator<<(std::ostream& os, const ClassMember& val){
			val.print(os);
			return os;
		}
};

class Field :public ClassMember{
	public:
		Field(ClassFile& clazz,std::ifstream& file):ClassMember(clazz,file){}
		virtual void print(std::ostream& os) const override;
};
class Method :public ClassMember{
	public:
		Method(ClassFile& clazz,std::ifstream& file):ClassMember(clazz,file){}
		virtual void print(std::ostream& os) const override;
};
