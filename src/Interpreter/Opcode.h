namespace OPCODE{
	const unsigned int BIPUSH=0x10;			//PUSH BYTE
	const unsigned int ISTORE_N=0x3b;		//STORE INT
	const unsigned int ISTORE=0x36;			//STORE INT
	const unsigned int ILOAD_N=0x1a;		//LOAD INT
	const unsigned int ILOAD=0x15;			//LOAD INT
	const unsigned int RET=0xb1;			//RET
	const unsigned int LDC=0x12;			//LOAD CONSTANT
	const unsigned int INVOKE_STATIC=0xB8;	//INVOKE STATIC
	const unsigned int PUSH_INT_CONST=0x03;	//PUSH INT CONST [-1 -> 5]
	const unsigned int IF_CMP_GE=0xa2;
	const unsigned int IINC=0x84;			//INTEGER INCREMENT
	const unsigned int GOTO=0xa7;			//GOTO
	const unsigned int IADD=0x60;			//GOTO
}
