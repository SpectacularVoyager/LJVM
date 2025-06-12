#pragma once
#include <cstdint>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>
#include "ClassMembers/ClassMember.h"

class ClassInfo;
class Constant;
class Attribute;


struct ACCESS{
};

class ClassFile{

	public:
		int major;
		int minor;
		std::vector<Constant*> constants;
		int access;
		ClassInfo* clazz;
		ClassInfo* super;
		std::vector<int> interfaces;
		std::map<std::string,Field*> fields;
		std::map<std::string,Method*> methods;
		std::vector<Attribute*> attributes;

		int parse(std::ifstream& file);
		Constant* get(std::ifstream& file);
		~ClassFile();
		Method* getMethod(std::string name)const ;
};
