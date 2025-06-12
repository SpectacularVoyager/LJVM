#include "jni.h"
#include <cstdlib>

const char* JNIEnv::GetStringUTFChars(jstring str, jboolean *isCopy){
	return &str.str[0];
	*isCopy=false;
}
void JNIEnv::ReleaseStringUTFChars(jstring str, const char *chars){
}
