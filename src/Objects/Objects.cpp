#include "Objects.h"
#include "Interpreter/Interpreter.h"

ObjectRef::ObjectRef(ClassInfo& clazz):clazz(clazz){
}

ObjectRef* Objects::newInstance(ClassInfo &info){
	return new ObjectRef(info);
}

StaticObject::StaticObject(ClassFile& clazz):clazz(clazz){
	for(auto x:clazz.fields){
		switch(x.second->desc->str[0]){
			case 'I':
				fields[x.first]={OPERANDS::INT,0};
				break;
			default:
					PANIC("CANNOT HANDLE:\t"+x.second->desc->str);
		}
	}
}
