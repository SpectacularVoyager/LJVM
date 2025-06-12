#pragma once

#include <cstddef>
#include <string>
typedef int jint;
typedef bool jboolean;

struct jstring{
	std::string str;
};

class JNIEnv{
	const char* GetStringUTFChars(jstring str, jboolean *isCopy);
	void ReleaseStringUTFChars(jstring str, const char *chars);
};
