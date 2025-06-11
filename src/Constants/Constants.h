#pragma once
#include <fstream>
#include <ostream>
#include "ConstantDefs.h"

#include "ClassFile/Classfile.h"
//INTERFACE
class Constant{
	public:
		int tag;
		friend std::ostream& operator<<(std::ostream& os, const Constant& val);
		virtual void print(std::ostream& os) const = 0;
};

class UTF8String : public Constant{
	public:
		std::string str;
		UTF8String(ClassFile& clazz,std::ifstream& file);
		virtual void print(std::ostream& os) const override;
		operator std::string(){return str;}
};
class NameAndType : public Constant {
	public:
		UTF8String** name;
		UTF8String** type;
		NameAndType(ClassFile& clazz,std::ifstream& file);
		virtual void print(std::ostream& os) const override;
};
class ClassInfo : public Constant{
	public:
		NameAndType** name;
		ClassInfo(ClassFile& clazz,std::ifstream& file);
		virtual void print(std::ostream& os) const override;
};
class MethodRef : public Constant{
	public:
		ClassInfo** clazz;
		NameAndType** name;
		MethodRef(ClassFile& clazz,std::ifstream& file);
		virtual void print(std::ostream& os) const override;
};
namespace Constants{
	Constant* getConstant(ClassFile& clazz,std::ifstream& file);
}
