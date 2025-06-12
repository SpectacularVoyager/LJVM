#pragma once

#include <vector>
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define RST "\e[0m"

#include <cstdint>
#include <fstream>

inline uint32_t readU32(std::istream& file){
	int u32;
	file.read((char*)&u32,4);
	return __builtin_bswap32(u32);
}
inline uint16_t readU16(std::istream& file){
	int u16;
	file.read((char*)&u16,2);
	return __builtin_bswap16(u16);
}
inline uint8_t readU8(std::istream& file){
	int u8;
	file.read((char*)&u8,1);
	return u8;
}
#define COMBINE_BYTE(a,b) (((a)<<8)|(b))
[[noreturn]] void PANIC_FUNC(std::string str,std::string file,int line);

void hexdump(std::vector<unsigned char>& vec,int d=30);

#define PANIC(e) PANIC_FUNC(e,__FILE__,__LINE__)
#define TODO PANIC("TODO: COMPLETE FEATURE")
