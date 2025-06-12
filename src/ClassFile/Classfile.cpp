#include "Classfile.h"
#include <fstream>
#include "Utils/Utils.h"
#include "Constants/Constants.h"
#include "ClassMembers/ClassMember.h"
#include "Attributes/Attribute.h"
#include "iostream"

#define readAll(file) for(unsigned int i=0,count=readU16(file);i<count;i++)

Constant** ClassFile::get(std::ifstream& file){
	int idx=readU16(file)-1;
	return &constants[idx];
}

int ClassFile::parse(std::ifstream& file){
	unsigned int magic=readU32(file);	
	if(magic!=0xCAFEBABE){
		return 0;
	}
	
	minor=readU16(file);
	major=readU16(file);
	unsigned int n_consts=readU16(file)-1;
	constants.resize(n_consts);
	for(size_t i=0;i<n_consts;i++){
		Constant* c=Constants::getConstant(*this, file);
		constants[i]=c;
	}
	int i=0;
	for(auto x:constants){
		//std::cout<<i++<<"\t"<<x->tag<<"\n";
	}
	i=0;
	for(auto x:constants){
		//std::cout<<x->tag<<"\n";
		x->resolve(*this);
	}
	i=0;
	for(auto x:constants){
		std::cout<<i++<<" ( "<<*x<<" )\n";	
	}
	std::cout<<"\n\n";

	access=readU16(file);
	clazz=(ClassInfo*)constants[readU16(file)-1];
	super=(ClassInfo*)constants[readU16(file)-1];

	readAll(file){
		readU16(file);
	}
	readAll(file){
		Field f(*this,file);
		fields.push_back(f);
	}
	readAll(file){
		Method m(*this,file);
		//std::cout<<m.getName()<<std::endl;
		methods.push_back(m);
	}
	readAll(file){
		Attribute* a=Attributes::getAttribute(*this,file);
		attributes.push_back(a);
	}

	// for(auto x:constants){
	// 	std::cout<<*x<<"\n";
	// }
	return 1;
}
ClassFile::~ClassFile(){
	for(auto x:constants){
		delete x;
	}
	for(auto x:attributes){
		delete x;
	}
}


