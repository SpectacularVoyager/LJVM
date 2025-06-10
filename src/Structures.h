#include "unistd.h"
#include "stdint.h"
#include <fstream>
#include <iostream>
#include <vector>

inline uint32_t readU32(std::ifstream& file){
	uint32_t u32;
	file.read((char*)&u32,4);
	return __builtin_bswap32(u32);
}
inline uint16_t readU16(std::ifstream& file){
	uint16_t u16;
	file.read((char*)&u16,2);
	return __builtin_bswap16(u16);
}
inline uint8_t readU8(std::ifstream& file){
	uint8_t u8;
	file.read((char*)&u8,1);
	return u8;
}
struct MethodRefConstant{};

#define CLASS_CONSTANT_METHOD_REF	10
#define CLASS_CONSTANT_CLASS		7
#define CLASS_CONSTANT_NAME_TYPE	12
#define CLASS_CONSTANT_UTF8			1

struct Incomplete{
	int tag;
};
struct Complete{};

struct UTF8Incomplete:public Incomplete{
	std::string name;
	UTF8Incomplete(std::ifstream& file){
		tag=CLASS_CONSTANT_UTF8;
		int len=readU16(file);
		name.resize(len);
		file.read(&name[0],len);
		std::cout<<"\tSTRING:\t"<<name<<"\n";
	}
};

struct NameAndType{
	std::string name;
	std::string type;
	NameAndType(std::string name,std::string type):name(name),type(type){}
};
struct NameAndTypeIncomplete:public Incomplete{
	int name;
	int type;
	NameAndTypeIncomplete(std::ifstream& file){
		tag=CLASS_CONSTANT_NAME_TYPE;
		name=readU16(file)-1;
		type=readU16(file)-1;
		printf("PARSING NAME_AND_TYPE\n");
		printf("NAME:\t%d\n",name);
		printf("TYPE:\t%d\n",type);
	}
	NameAndType complete(std::vector<Incomplete> list){
		auto name_str=(UTF8Incomplete*)(&list[name]);
		auto type_str=(UTF8Incomplete*)(&list[name]);
		return NameAndType(name_str->name,type_str->name);
	}
};
struct MethodRefIncomplete:public Incomplete{
	int clazz;
	int name;
	MethodRefIncomplete(std::ifstream& file){
		tag=CLASS_CONSTANT_METHOD_REF;
		clazz=readU16(file)-1;
		name=readU16(file)-1;
		printf("PARSING METHOD REF\n");
		printf("CLASS:\t%d\n",clazz);
		printf("NAME:\t%d\n",name);
	}
};
struct ClassComplete:public Complete{
	NameAndType name;
	ClassComplete(NameAndType name):name(name){

	}
};
struct ClassIncomplete:public Incomplete{
	int name;
	ClassIncomplete(std::ifstream& file){
		tag=CLASS_CONSTANT_CLASS;
		name=readU16(file)-1;
		printf("PARSING CLASS\n");
		printf("NAME:\t%d\n",name);
	}
};
