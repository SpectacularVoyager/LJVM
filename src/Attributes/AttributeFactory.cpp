#include "Attribute.h"
#include <fstream>
#include <iostream>
#include "Utils/Utils.h"
#include "Constants/Constants.h"

Attribute* Attributes::getAttribute(ClassFile& clazz,std::ifstream& file){
	UTF8String* a=((UTF8String*)clazz.get(file));
	std::cout<<"Attribute("<<a->str<<")\n";
	if(a->str=="Code"){
		return new IgnoredAttribute(&a,file);
	}else{
		PANIC("UNIMPLEMENTED ATTRIBUTE:"+a->str);
	}
}

std::string Attribute::getName(){
	return (*name)->str;
}
IgnoredAttribute::IgnoredAttribute(UTF8String** str,std::ifstream& file):Attribute(str){
	int len=readU32(file);
	bytes.resize(len);
	file.read((char*)&bytes[0],len);
}
