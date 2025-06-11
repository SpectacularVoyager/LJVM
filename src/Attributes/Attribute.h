#pragma once

#include <fstream>
#include "Constants/Constants.h"
class ClassFile;
class Attribute{
	public:
		UTF8String** name;
		std::string getName();
		Attribute(UTF8String** name):name(name){}
};
class IgnoredAttribute : public Attribute{
	public:
		std::vector<unsigned char> bytes;
		IgnoredAttribute(ClassFile& clazz,UTF8String** str,std::ifstream& file);
};
class SourceFileAttribute : public Attribute{
	public:
		UTF8String** tag;
		SourceFileAttribute(ClassFile& clazz,UTF8String** str,std::ifstream& file);
};
class ExceptionTable{
	public:
		unsigned int startPC;
		unsigned int endPC;
		unsigned int handlerPC;
		unsigned int catchType;
		ExceptionTable(std::ifstream& file);
};

class CodeAttribute : public Attribute{
	public:
		int maxStacks;
		int maxLocals;
		std::vector<unsigned char> code;
		std::vector<ExceptionTable> exceptions;
		std::vector<Attribute*> attributes;
		CodeAttribute(ClassFile& clazz,UTF8String** str,std::ifstream& file);
		~CodeAttribute();
};

namespace Attributes{
	Attribute* getAttribute(ClassFile& clazz,std::ifstream& file);
};
