#pragma once
#include <fstream>
#include <ostream>
#include "ConstantDefs.h"

#include "ClassFile/Classfile.h"

template <typename T>
T* cast(Constant* c);
//INTERFACE
class Constant{
	public:
		int tag;
		friend std::ostream& operator<<(std::ostream& os, const Constant& val);
		virtual void print(std::ostream& os) const = 0;
		virtual void resolve(ClassFile& clazz) = 0;
};

class UTF8String : public Constant{
	public:
		std::string str;
		UTF8String(ClassFile& clazz,std::ifstream& file);
		virtual void print(std::ostream& os) const override;
		operator std::string(){return str;}
		virtual void resolve(ClassFile& clazz) override {}
};
class NameAndType : public Constant {
	int name_index;
	int type_index;
	public:
		NameAndType(ClassFile& clazz,std::ifstream& file);
		UTF8String* name;
		UTF8String* type;
		virtual void print(std::ostream& os) const override;
		std::string getName();
		std::string getType();
		virtual void resolve(ClassFile& clazz) override; 

};
class ClassInfo : public Constant{
	int name_index;
	public:
		UTF8String* name;
		ClassInfo(ClassFile& clazz,std::ifstream& file);
		virtual void print(std::ostream& os) const override;
		virtual void resolve(ClassFile& clazz) override; 
};
class ConstantString : public Constant{
	int str_index;
	public:
		UTF8String* str;
		ConstantString(ClassFile& clazz,std::ifstream& file);
		virtual void print(std::ostream& os) const override;
		virtual void resolve(ClassFile& clazz) override; 
		operator std::string(){return (std::string)*str;}
};


class GenericRef : public Constant{
	int clazz_index;
	int name_index;
	public:
		ClassInfo* clazz;
		NameAndType* name;
		GenericRef(ClassFile& clazz,std::ifstream& file);
		virtual void print(std::ostream& os) const override;
		virtual void resolve(ClassFile& clazz) override; 
};

class MethodRef: public GenericRef{
	public:
		using GenericRef::GenericRef;
};
class FieldRef: public GenericRef{
	public:
		using GenericRef::GenericRef;
};
namespace Constants{
	Constant* getConstant(ClassFile& clazz,std::ifstream& file);
}
