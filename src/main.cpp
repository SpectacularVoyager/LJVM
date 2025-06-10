#include <cstdio>
#include <fstream>
#include <ios>
using std::ios;

int readClass(std::ifstream& file);
int main(int argc,char** argv){
	if(argc<=1){
		printf("USAGE:\tJAVA [CLASS]\n");
		return -1;
	}
	auto in=std::ifstream(argv[1],ios::binary | ios::in);
	return readClass(in);
}
