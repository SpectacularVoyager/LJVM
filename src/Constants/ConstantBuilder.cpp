#include "Constants.h"
#include "../Utils/Utils.h"
Constant* Constants::getConstant(ClassFile& clazz,std::ifstream& file){
	int tag=readU8(file);
	Constant* c;
	switch(tag){
		case Constants::METHOD_REF:
			c = new MethodRef(clazz,file);
			break;
		case Constants::FIELD_REF:
			c = new FieldRef(clazz,file);
			break;
		case Constants::CLASS_INFO:
			c = new ClassInfo(clazz,file);
			break;
		case Constants::NAME_AND_TYPE:
			c = new NameAndType(clazz,file);
			break;
		case Constants::UTF8_STRING:
			c = new UTF8String(clazz,file);
			break;
		case Constants::CONSTANT_STRING:
			c = new ConstantString(clazz,file);
			break;
		default:
			PANIC("INVALID TAG:\t"+std::to_string(tag));
	}
	c->tag=tag;
	return c;
}
