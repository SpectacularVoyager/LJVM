#pragma once
#include <fstream>
#include <unordered_map>
#include <vector>

class ClassInfo;
class Constant;
class Attribute;

class Field;
class Method;

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
		std::vector<Field> fields;
		std::vector<Method> methods;
		std::unordered_map<std::string,Attribute*> attributes;

		int parse(std::ifstream& file);
		Constant* get(std::ifstream& file);
		~ClassFile();
};
