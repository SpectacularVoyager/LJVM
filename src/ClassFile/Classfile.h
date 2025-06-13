#pragma once
#include <cstdint>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>
#include "Access.h"
#include "ClassMembers/ClassMember.h"


class StaticObject;

class ClassInfo;
class Constant;
class Attribute;



class ClassFile{

	public:
		int major;
		int minor;
		std::vector<Constant*> constants;
		ACCESS access;
		ClassInfo* clazz;
		ClassInfo* super;
		std::vector<int> interfaces;
		std::map<std::string,Field*> fields;
		std::map<std::string,Method*> methods;
		std::vector<Attribute*> attributes;

		StaticObject* staticObject;

		int parse(const std::string& file_name);
		int parse(std::ifstream& file);
		Constant* get(std::ifstream& file);
		~ClassFile();
		Method* getMethod(std::string name)const ;
};
