#include "Constants.h"
#include "Utils/Utils.h"

MethodRef::MethodRef(ClassFile& clazz,std::ifstream& file){
	// printf("METHOD REF\n");
	this->clazz=(ClassInfo**)&clazz.constants[readU16(file)-1];
	this->name=(NameAndType**)&clazz.constants[readU16(file)-1];
}

ClassInfo::ClassInfo(ClassFile& clazz,std::ifstream& file){
	// printf("CLASS INFO\n");
	this->name=(NameAndType**)&clazz.constants[readU16(file)-1];
}
NameAndType::NameAndType(ClassFile& clazz,std::ifstream& file){
	// printf("NAME AND TYPE\n");
	int i_name=readU16(file)-1;
	int i_type=readU16(file)-1;
	this->name=(UTF8String**)&clazz.constants[i_name];
	this->type=(UTF8String**)&clazz.constants[i_type];
}
UTF8String::UTF8String(ClassFile& clazz,std::ifstream& file){
	// printf("UTF8 STRING\n");
	int len=readU16(file);
	str.resize(len);
	file.read(&str[0],len);
}

std::ostream& operator<<(std::ostream& os, const Constant& val){
	val.print(os);
	return os;
}


void UTF8String::print(std::ostream& os) const{
	os<<str;
}
void NameAndType::print(std::ostream& os) const{
	os<<"("<<**name<<","<<**type<<")";
}
void ClassInfo::print(std::ostream& os) const{
	os<<"Class("<<**name<<")";
}
void MethodRef::print(std::ostream& os) const{
	os<<"MethodRef("<<**((*clazz)->name)<<","<<**name<<")";
}
