#include "Constants.h"

#include "Utils/Utils.h"
#include <map>
#include <string>
#include <typeinfo>

void NameAndType::resolve(ClassFile& clazz) {
	name=cast<UTF8String>(clazz.constants[name_index]);
	type=cast<UTF8String>(clazz.constants[type_index]);
}
void GenericRef::resolve(ClassFile& clazz) {
	name=cast<NameAndType>(clazz.constants[name_index]);
	this->clazz=cast<ClassInfo>(clazz.constants[clazz_index]);
}
void ClassInfo::resolve(ClassFile& clazz) {
	name=cast<UTF8String>(clazz.constants[name_index]);
}
void ConstantString::resolve(ClassFile& clazz) {
	str=cast<UTF8String>(clazz.constants[str_index]);
}

GenericRef::GenericRef(ClassFile& clazz,std::ifstream& file){
	// printf("METHOD REF\n");
	clazz_index=readU16(file)-1;
	name_index=readU16(file)-1;
}
ClassInfo::ClassInfo(ClassFile& clazz,std::ifstream& file){
	this->name_index=readU16(file)-1;
}
NameAndType::NameAndType(ClassFile& clazz,std::ifstream& file){
	// printf("NAME AND TYPE\n");
	name_index=readU16(file)-1;
	type_index=readU16(file)-1;
}
UTF8String::UTF8String(ClassFile& clazz,std::ifstream& file){
	// printf("UTF8 STRING\n");
	int len=readU16(file);
	str.resize(len);
	file.read(&str[0],len);
}
ConstantString::ConstantString(ClassFile& clazz,std::ifstream& file){
	str_index=readU16(file)-1;
}

std::ostream& operator<<(std::ostream& os, const Constant& val){
	val.print(os);
	return os;
}
std::string NameAndType::getName(){
	return name->str;
}
std::string NameAndType::getType(){
	return type->str;
}


void ConstantString::print(std::ostream& os) const{
	os<<"\""<<*str<<"\"";
}
void UTF8String::print(std::ostream& os) const{
	os<<str;
}
void NameAndType::print(std::ostream& os) const{
	os<<"("<<*name<<","<<*type<<")";
}
void ClassInfo::print(std::ostream& os) const{
	os<<"Class("<<*name<<")";
}
void GenericRef::print(std::ostream& os) const{
	os<<"MethodRef("<<*(clazz->name)<<","<<*name<<")";
}
