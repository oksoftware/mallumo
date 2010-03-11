/*
 * Mallumo --- SWF Malkompilero
 *
 * binreader.c : Legilo de Bitara Dateno
 *
 * Lasta Modifo: 2009/12/21
 *
 * Copyright (C) oksoftware. All rights reserved.
 *
 */

#include <malloc.h>
#include <zlib.h>
#include "binreader.h"

#define ZLIB_BUF_SIZE 4096

BinaryReader *CreateBinaryReader(unsigned char *src, unsigned int len){
	BinaryReader *br;
	if((br = malloc(sizeof(BinaryReader))) == NULL){
		return NULL;
	}
	br->buf = src;
	br->cur = 0;
	br->len = len;
	br->err = 0;

	br->cur_byte = 0;
	br->cur_bit_pos = 8;
	return br;
}

void ReadBytes(BinaryReader *br, unsigned char *dst, unsigned int len){
	int i;

	if((br->len - br->cur) < len){
		br->err = 1;
		for(i = 0; i < len; i++){
			*(dst + i) = 0;
		}
		return;
	}

	for(i = 0;i < len; i++){
		*(dst + i) = *(br->buf + br->cur + i);
	}

	br->cur += len;
	return;
}

void SkipBytes(BinaryReader *br, unsigned len){
	if((br->len - br->cur) < len){
		br->err = 1;
		return;
	}

	br->cur += len;
	return;
}

SI8 ReadSI8(BinaryReader *br){
	SI8 tmp;
	ReadBytes(br, (unsigned char *)&tmp, 1);
	return tmp;
}

SI16 ReadSI16(BinaryReader *br){
	SI16 tmp;
	ReadBytes(br, (unsigned char *)&tmp, 2);
	return tmp;
}

SI32 ReadSI32(BinaryReader *br){
	SI32 tmp;
	ReadBytes(br, (unsigned char *)&tmp, 4);
	return tmp;
}

UI8 ReadUI8(BinaryReader *br){
	UI8 tmp;
	ReadBytes(br, (unsigned char *)&tmp, 1);
	return tmp;
}

UI16 ReadUI16(BinaryReader *br){
	UI16 tmp;
	ReadBytes(br, (unsigned char *)&tmp, 2);
	return tmp;
}

UI32 ReadUI32(BinaryReader *br){
	UI32 tmp;
	ReadBytes(br, (unsigned char *)&tmp, 4);
	return tmp;
}

FIXED ReadFIXED(BinaryReader *br){
	FIXED tmp;
	tmp = ReadUI32(br) / 0x10000;
	return tmp;
}

FIXED8 ReadFIXED8(BinaryReader *br){
	FIXED8 tmp;
	tmp = ReadUI16(br) / 0x100;
	return tmp;
}

unsigned char SubBits(unsigned char src, unsigned int from, unsigned int len){
	unsigned char ret = 0;
	ret = src << from;
	ret = ret >> (8 - len);
	return ret;
}

unsigned int ReadBits(BinaryReader *br, unsigned int len){
	unsigned int tmp = 0;
	unsigned int bits_have_to_read;
	unsigned int cur_rem_bits;
	unsigned int rem_bits;

	rem_bits = len;

	// cur_rem_bits : Remaining bits of current byte現バイトに残り何ビットあるか
	// rem_bits : Remaining bits of total bytesあと何ビット読まなあかんか

	while(rem_bits){
		if(br->cur_bit_pos > 7){
			br->cur_bit_pos = 0;
			ReadBytes(br, &(br->cur_byte), 1);
			if(br->err){
				return 0;
			}
		}

		cur_rem_bits = 8 - br->cur_bit_pos;
		if(cur_rem_bits > rem_bits){
			bits_have_to_read = rem_bits;
		}else{
			bits_have_to_read = cur_rem_bits;
		}

		rem_bits -= bits_have_to_read;
		tmp += SubBits(br->cur_byte, br->cur_bit_pos, bits_have_to_read) << rem_bits;

		br->cur_bit_pos += bits_have_to_read;
	}

	return tmp;
}
void SkipBits(BinaryReader *br){
	br->cur_bit_pos = 8;
	br->cur_byte = 0;
	return;
}

UB ReadUB(BinaryReader *br, unsigned int len){
	return ReadBits(br, len);
}

SB ReadSB(BinaryReader *br, unsigned int len){
	SB tmp = 0;
	tmp = ReadBits(br, len);
	if(tmp >> (len - 1)){
		tmp += 0xFFFFFFFF << len;
	}
	return tmp;
}

FB ReadFB(BinaryReader *br, unsigned int len){
	FB tmp = 0;
	tmp = ReadSB(br, len) / 0x10000;
	return tmp;
}

char *ReadString(BinaryReader *br){
	unsigned int i;
	unsigned int len;
	char *tmp;

	for(i = 0; ; i++){
		if((br->len - (br->cur + i)) < 1){
			br->err = 1;
			return NULL;
		}
		if(*(br->buf + br->cur + i) == '\0'){
			len = i + 1;
			break;
		}
	}

	if((tmp = malloc(len)) == NULL){
		return NULL;
	}

	ReadBytes(br, tmp, len);

	return tmp;
}

int SwapBufferToZlib(BinaryReader *src, unsigned int after_len){
	z_stream zs;
	unsigned char *after_buf;
	int ret = 0;

	zs.zalloc = Z_NULL;
	zs.zfree = Z_NULL;
	zs.opaque = Z_NULL;

	if(inflateInit(&zs) != Z_OK){
		return 1;
	}

	zs.next_in = src->buf + src->cur;
	zs.avail_in = src->len - src->cur;
	
	
	if((after_buf = (unsigned char *)malloc(after_len)) == NULL){
		return 1;
	}

	zs.next_out = after_buf;
	zs.avail_out = after_len;

	ret = inflate(&zs, Z_FINISH);
	if(ret != Z_OK && ret != Z_STREAM_END){
		return 1;
	}

	free(src->buf);
	src->buf = after_buf;
	src->cur = 0;
	src->len = after_len;
	src->err = 0;
	src->cur_byte = 0;
	src->cur_bit_pos = 8;

	inflateEnd(&zs);

	return 0;
}
