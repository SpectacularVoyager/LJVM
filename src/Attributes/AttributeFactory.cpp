#include "Attribute.h"
#include <fstream>
#include <iostream>
#include "Utils/Utils.h"
#include "Constants/Constants.h"

Attribute* Attributes::getAttribute(ClassFile& clazz,std::ifstream& file){
	auto a=(UTF8String**)&clazz.constants[readU16(file)-1];
	//std::cout<<"Attribute("<<(*a)->str<<")\n";

	Attribute* attr;
	if((*a)->str=="Code"){
		attr= new IgnoredAttribute(clazz,a,file);
	}else if((*a)->str=="SourceFile"){
		attr= new SourceFileAttribute(clazz,a,file);
	}else{
		PANIC("UNIMPLEMENTED ATTRIBUTE:"+(*a)->str);
	}
	attr->name=a;
	return attr;
}

std::string Attribute::getName(){
	return (*name)->str;
}
IgnoredAttribute::IgnoredAttribute(ClassFile& clazz,UTF8String** str,std::ifstream& file):Attribute(str){
	std::cout<<"IGNORED("<<(*str)->str<<")\n";
	unsigned int len=readU32(file);
	bytes.resize(len);
	file.read((char*)&bytes[0],len);
}
SourceFileAttribute::SourceFileAttribute(ClassFile& clazz,UTF8String** str,std::ifstream& file):Attribute(str){
	volatile unsigned int len=readU32(file);
	tag=((UTF8String**)clazz.get(file));
}
