#include "Classfile.h"
namespace ACCESS_MASK {
	static const int PUBLIC = 0x0001;
	static const int PRIVATE = 0x0002;
	static const int PROTECTED = 0x0004;
	static const int STATIC = 0x0008;
	static const int FINAL = 0x0010;
	//DUPLICATE HERE
	static const int SYNCHRONISED = 0x0020;
	static const int SUPER = 0x0020;
	//
	static const int BRIDGE = 0x0040;
	static const int VARARGS = 0x0080;
	static const int NATIVE = 0x0100;
	static const int INTERFACE = 0x0200;
	static const int ABSTRACT = 0x0400;
	static const int STRICT = 0x0800;
	static const int SYNTHETIC = 0x1000;
	static const int ANNOTATION = 0x2000;
	static const int ENUM = 0x4000;
}
bool ACCESS::isPublic() const {
	return (access & ACCESS_MASK::PUBLIC) > 0;
}

bool ACCESS::isPrivate() const {
	return (access & ACCESS_MASK::PRIVATE) > 0;
}

bool ACCESS::isProtected() const {
	return (access & ACCESS_MASK::PROTECTED) > 0;
}

bool ACCESS::isStatic() const {
	return (access & ACCESS_MASK::STATIC) > 0;
}

bool ACCESS::isBridge() const {
	return (access & ACCESS_MASK::BRIDGE) > 0;
}

bool ACCESS::isVarArgs() const {
	return (access & ACCESS_MASK::VARARGS) > 0;
}

bool ACCESS::isNative() const {
	return (access & ACCESS_MASK::NATIVE) > 0;
}

bool ACCESS::isStrict() const {
	return (access & ACCESS_MASK::STRICT) > 0;
}
bool ACCESS::isFinal() const {
	return (access & ACCESS_MASK::FINAL) > 0;
}

bool ACCESS::isSuper() const {
	return (access & ACCESS_MASK::SUPER) > 0;
}
bool ACCESS::isSynchronised() const {
	return (access & ACCESS_MASK::SYNCHRONISED) > 0;
}

bool ACCESS::isInterface() const {
	return (access & ACCESS_MASK::INTERFACE) > 0;
}

bool ACCESS::isAbstract() const {
	return (access & ACCESS_MASK::ABSTRACT) > 0;
}

bool ACCESS::isSynthetic() const {
	return (access & ACCESS_MASK::SYNTHETIC) > 0;
}

bool ACCESS::isAnnotation() const {
	return (access & ACCESS_MASK::ANNOTATION) > 0;
}

bool ACCESS::isEnum() const {
	return (access & ACCESS_MASK::ENUM) > 0;
}
