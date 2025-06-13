#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include "Utils/Utils.h"

#include "ClassFile/Classfile.h"
#include "Interpreter/Interpreter.h"
#include "Constants/Constants.h"
using std::ios;

int main(int argc,char** argv){
	std::vector<ClassFile> classes;
	if(argc<=1){
		printf("USAGE:\tJAVA [CLASS]\n");
		return -1;
	}
	for(int i=1;i<argc;i++){
		std::cout<<"PARSING:"<<std::string(argv[i])<<"\n";
		classes.push_back(ClassFile());
		if(classes.back().parse(argv[i])==0)PANIC("CANNOT PARSE:\t"+std::string(argv[i]));
	}
	// auto in=std::ifstream(argv[1],ios::binary | ios::in);
	// ClassFile& clazz=classes[0];
	// if(!clazz.parse(argv[1])){
	// 	PANIC("INVALID CLASS FILE\n");
	// }
	VirtualMachine machine(classes,classes[0],NULL);
	for(auto c=classes.begin();c!=classes.end();c++){
		//int res=machine.runMethod("<clinit>");
		machine.reset();
		//if(!res)std::cout<<"NO <clinit> in "<<*c->clazz<<"\n";
	}
	//std::cout<<machine.frames.size()<<"\n";
	machine.runMethod("main");
	//run(clazz);

}
