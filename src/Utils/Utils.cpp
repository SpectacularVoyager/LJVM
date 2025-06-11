#include <cstddef>
#include <cstdio>
#include <string>
#include "Utils.h"
#include <stdnoreturn.h>
#include <vector>

[[noreturn]] void PANIC_FUNC(std::string str,std::string file,int line){
	printf(RED "PANIC" RST " AT:[%s:%d]\t%s\n",file.c_str(),line,str.c_str());
	exit(1);
}

void hexdump(std::vector<unsigned char>& vec,int d){
	for(size_t i=0;i<vec.size();i++){
		if((i+1)%d==0)printf("\n");
		printf("%02x ",vec[i]);
	}
	printf("\n");
}
