#pragma once

#include <fstream>
#include <ostream>
#include <vector>
class ClassFile;
class UTF8String;
class Attribute{
	public:
		UTF8String& name;
		std::string getName();
		Attribute(UTF8String& name):name(name){}
		virtual ~Attribute() = default;
		virtual void print(std::ostream& os) const = 0;
		friend std::ostream& operator<<(std::ostream& os, const Attribute& val){
			val.print(os);
			return os;
		}
};
class IgnoredAttribute : public Attribute{
	public:
		std::vector<unsigned char> bytes;
		IgnoredAttribute(ClassFile& clazz,UTF8String& str,std::ifstream& file);
		virtual void print(std::ostream& os) const override;
};
class SourceFileAttribute : public Attribute{
	public:
		UTF8String* tag;
		SourceFileAttribute(ClassFile& clazz,UTF8String& str,std::ifstream& file);

		virtual void print(std::ostream& os) const override;
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
		CodeAttribute(ClassFile& clazz,UTF8String& str,std::ifstream& file);
		~CodeAttribute();
		virtual void print(std::ostream& os) const override;
};
//TODO
struct LineNumberTableEntry{
	int startPC;
	int lineNumber;
};
class LineNumberAttribute :public Attribute{
	public:
		UTF8String* name;
		std::vector<LineNumberTableEntry> table;
		LineNumberAttribute(ClassFile& clazz,UTF8String& str,std::ifstream& file);
		virtual void print(std::ostream& os) const override;
		std::string& getName();
};
namespace Attributes{
	Attribute* getAttribute(ClassFile& clazz,std::ifstream& file);
};
