#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include "Structures.h"

#define JAVA_CLASS_MAGIC 0xCAFEBABE
#define PANIC(err) printf("\e[0;31mPANIC\e[0m AT %s:%d\t\e[0;33m%s\e[0m\n",__FILE__,__LINE__,err);exit(1);
#define TODO PANIC("TODO");
void readTag(std::ifstream& file){
	std::vector<Incomplete> inc;
	int t=readU8(file);
	Incomplete tag;
	switch(t){
		case CLASS_CONSTANT_METHOD_REF:
			tag=MethodRefIncomplete(file);
			break;
		case CLASS_CONSTANT_CLASS:
			tag=ClassIncomplete(file);
			break;
		case CLASS_CONSTANT_NAME_TYPE:
			tag=NameAndTypeIncomplete(file);
			break;
		case CLASS_CONSTANT_UTF8:
			tag=UTF8Incomplete(file);
			break;
		default:PANIC(("TODO: IMPLEMENT FOR["+std::to_string(t)+"]").c_str());
	}
	inc.push_back(tag);
}

int readClass(std::ifstream& file){
	printf("MAGIC:\t\t0x%x\n",readU32(file));
	printf("VERSION:\t%d:%d\n",readU16(file),readU16(file));
	unsigned int constants=readU16(file);
	printf("CONSTANTS:\t%d\n",constants);
	for(size_t i=0;i<constants-1;i++){
		readTag(file);
	}
	return 0;
}
