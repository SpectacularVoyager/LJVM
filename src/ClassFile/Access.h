#pragma once

class ACCESS{
	unsigned short access;
	public:
		ACCESS(){}
		ACCESS(unsigned short access):access(access){}
		ACCESS(int access):access(access){}
		operator unsigned short(){return access;}
		operator int(){return access;}
		bool isPublic() const;
		bool isPrivate() const;
		bool isProtected() const;
		bool isStatic() const;
		bool isBridge() const;
		bool isVarArgs() const;
		bool isNative() const;
		bool isStrict() const;
		bool isFinal() const;
		bool isSuper() const;
		bool isSynchronised() const;
		bool isInterface() const;
		bool isAbstract() const;
		bool isSynthetic() const;
		bool isAnnotation() const;
		bool isEnum() const;
};
