#pragma once

#include <fstream>
#include "Constants/Constants.h"
class ClassFile;
class Attribute{
	public:
		UTF8String** name;
		std::string getName();
		Attribute(UTF8String** name):name(name){}
};
class IgnoredAttribute : public Attribute{
	public:
		std::vector<unsigned char> bytes;
		IgnoredAttribute(UTF8String** str,std::ifstream& file);
};

namespace Attributes{
	Attribute* getAttribute(ClassFile& clazz,std::ifstream& file);
};
