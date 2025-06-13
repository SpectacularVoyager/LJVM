#pragma once
#include "Constants/Constants.h"
#include <vector>
#include "Interpreter/Interpreter.h"


class StaticObject{
	public:
		ClassFile& clazz;
		std::map<std::string,Operand> fields;
		StaticObject(ClassFile& file);
};

class ObjectRef{
	ClassInfo& clazz;
	public:
		std::map<std::string,Operand> fields;
		ObjectRef(ClassInfo& clazz);
};

namespace Objects{
	ObjectRef* newInstance(ClassInfo& info);
}
