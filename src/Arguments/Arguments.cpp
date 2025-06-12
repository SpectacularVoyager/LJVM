#include "Arguments.h"
#include <iostream>
#include <queue>
#include <vector>
#include "Utils/Utils.h"

std::string getSingle(std::queue<char>& q);

std::pair<std::vector<std::string>,std::string> getArgs(std::string& data){
	std::vector<std::string> res;
	std::queue<char> q;
	for(auto x:data)q.push(x);
	if(q.front()!='(')PANIC("EXPECTED '(' GOT:"+std::string(1,q.front()));
	q.pop();
	while(!q.empty()&&q.front()!=')'){
		if(q.empty())PANIC("EXPECTED ')' GOT EOF");
		res.push_back(getSingle(q));
	}
	q.pop();
	return std::make_pair(res,getSingle(q));
}
std::string getSingle(std::queue<char>& q){
	char x=q.front();
	std::string str;
	switch(x){
		case 'B':
		case 'C':
		case 'D':
		case 'F':
		case 'I':
		case 'J':
		case 'S':
		case 'Z':
		case 'V':
			q.pop();
			return std::string(1,x);
			break;
		case 'L':
			while(!q.empty()&&x!=';'){
				x=q.front();
				q.pop();
				str+=x;
				if(q.empty())PANIC("EXPECTED ';' GOT EOF");
			}
			return str;
		case '[':
			q.pop();
			str="["+getSingle(q);
			return str;
		default:
			PANIC("UNEXPECTED:\t"+std::string(1,x));
		
	}

}
