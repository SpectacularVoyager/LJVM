#include "Attribute.h"
#include <fstream>
#include <iostream>
#include "Utils/Utils.h"
#include "Constants/Constants.h"

Attribute* Attributes::getAttribute(ClassFile& clazz,std::ifstream& file){
	//auto a=(UTF8String**)&clazz.constants[readU16(file)-1];
	UTF8String& a=*cast<UTF8String>(clazz.constants[readU16(file)-1]);
	//std::cout<<"Attribute("<<a<<")\n";

	Attribute* attr;
	if((std::string)a=="Code"){
		attr= new CodeAttribute(clazz,a,file);
	}else if((std::string)a=="SourceFile"){
		attr= new SourceFileAttribute(clazz,a,file);
	}else if((std::string)a=="StackMapTable"){
		attr= new IgnoredAttribute(clazz,a,file);
	}else if((std::string)a=="LineNumberTable"){
		attr= new IgnoredAttribute(clazz,a,file);
	}else{
		PANIC("UNIMPLEMENTED ATTRIBUTE:"+(std::string)a);
	}
	return attr;
}

std::string Attribute::getName(){
	return (std::string)name;
}
IgnoredAttribute::IgnoredAttribute(ClassFile& clazz,UTF8String& str,std::ifstream& file):Attribute(str){
	std::cout<<"IGNORED("<<str<<")\n";
	unsigned int len=readU32(file);
	bytes.resize(len);
	file.read((char*)&bytes[0],len);
}
SourceFileAttribute::SourceFileAttribute(ClassFile& clazz,UTF8String& str,std::ifstream& file):Attribute(str){
	volatile unsigned int len=readU32(file);
	//tag=((UTF8String**)clazz.get(file));
	tag=cast<UTF8String>(clazz.constants[readU16(file)-1]);
}

CodeAttribute::CodeAttribute(ClassFile& clazz,UTF8String& str,std::ifstream& file):Attribute(str){
	volatile unsigned int len=readU32(file);
	maxStacks=readU16(file);
	maxLocals=readU16(file);
	unsigned int codelen=readU32(file);
	code.resize(codelen);
	file.read((char*)&code[0],codelen);
	for(size_t i=0;i<readU16(file);i++){
		exceptions.push_back(ExceptionTable(file));
	}
	unsigned int n_attrs=readU16(file);
	for(size_t i=0;i<n_attrs;i++){
		attributes.push_back(Attributes::getAttribute(clazz,file));
	}
}
CodeAttribute::~CodeAttribute(){
	for(auto x:attributes){
		delete x;
	}
}
ExceptionTable::ExceptionTable(std::ifstream& file){
	startPC=readU16(file);
	endPC=readU16(file);
	handlerPC=readU16(file);
	catchType=readU16(file);
}
void IgnoredAttribute::print(std::ostream& os) const {
	os<<"IGNORED("<<(std::string)name<<")";
}
void CodeAttribute::print(std::ostream& os) const {
	os<<"Code("<<name<<")";
}
void SourceFileAttribute::print(std::ostream& os) const {
	os<<"Source("<<*tag<<")";
}
