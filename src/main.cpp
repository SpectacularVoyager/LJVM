#include <cstdio>
#include <fstream>
#include <ios>
#include "Utils/Utils.h"

#include "ClassFile/Classfile.h"
#include "Interpreter/Interpreter.h"
using std::ios;

int main(int argc,char** argv){
	if(argc<=1){
		printf("USAGE:\tJAVA [CLASS]\n");
		return -1;
	}
	auto in=std::ifstream(argv[1],ios::binary | ios::in);
	ClassFile clazz;
	if(!clazz.parse(in)){
		PANIC("INVALID CLASS FILE\n");
	}
	run(clazz);

}
