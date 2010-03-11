typedef signed char SI8;
typedef signed short SI16;
typedef signed int SI32;

typedef unsigned char UI8;
typedef unsigned short UI16;
typedef unsigned int UI32;

typedef double FIXED;
typedef double FIXED8;

typedef signed int SB;
typedef unsigned int UB;
typedef double FB;

typedef struct {
	unsigned char *buf;
	unsigned int cur;
	unsigned int len;
	int err;
	unsigned char cur_byte;
	unsigned int cur_bit_pos;
} BinaryReader;

BinaryReader *CreateBinaryReader(unsigned char *src, unsigned int len);
void ReadBytes(BinaryReader *br, unsigned char *dst, unsigned int len);
void SkipBytes(BinaryReader *br, unsigned len);
SI8 ReadSI8(BinaryReader *br);
SI16 ReadSI16(BinaryReader *br);
SI32 ReadSI32(BinaryReader *br);
UI8 ReadUI8(BinaryReader *br);
UI16 ReadUI16(BinaryReader *br);
UI32 ReadUI32(BinaryReader *br);
FIXED ReadFIXED(BinaryReader *br);
FIXED8 ReadFIXED8(BinaryReader *br);
UB ReadUB(BinaryReader *br, unsigned int len);
SB ReadSB(BinaryReader *br, unsigned int len);
FB ReadFB(BinaryReader *br, unsigned int len);
void SkipBits(BinaryReader *br);
char *ReadString(BinaryReader *br);
int SwapBufferToZlib(BinaryReader *src, unsigned int after_len);

