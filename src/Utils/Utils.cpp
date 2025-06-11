#include <cstdio>
#include <string>
#include "Utils.h"
#include <stdnoreturn.h>

[[noreturn]] void PANIC_FUNC(std::string str,std::string file,int line){
	printf(RED "PANIC" RST " AT:[%s:%d]\t%s\n",file.c_str(),line,str.c_str());
	exit(1);
}
