#pragma once

typedef int jint;
typedef bool jboolean;

struct jstring;

class JNIEnv{
	const char* GetStringUTFChars(jstring str, jboolean *isCopy);
	void ReleaseStringUTFChars(jstring str, const char *chars);
};
