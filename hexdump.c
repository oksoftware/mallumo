/*
 * hexdump.c : Forjxetilo de Bitara Dosiero
 *
 * La fontprogrammo estas parto de atingajxo de dishsp.
 *
 * Lasta Modifo: 2009/12/24
 *
 * Copyright (C) oksoftware. All rights reserved.
 *
 */

#include <stdio.h>
#include "hexdump.h"

void dumpHex(unsigned int lenData, unsigned char *pData){
	int i = 0, j = 0;
	int posData = 0;
	unsigned char lineData[16];
	unsigned char charNext;
	unsigned char skip = 0;

	if(!lenData) return;

	printf("ADDRESS   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 0123456789ABCDEF\n");
	for(i = 0; i < ((lenData / 16) + 1); i++){
		if(!(lenData % 16)) break;

		printf("%08x:", posData);

		for(j = 0; j < 16; j++){
			if(posData < lenData){
				lineData[j] = *(pData + posData);
				printf(" %02x", lineData[j]);

				posData++;
			}else{
				lineData[j] = 0;
				printf("   ");
			}
		}

		printf(" ");

		for(j = 0; j < 16; j++){
			if(skip){
				printf(" ");
				skip = 0;
			}else if((0x20 <= lineData[j]) && (0x7F >= lineData[j])){
				/* ASCII */
				printf("%c", lineData[j]);
			}else if((0xA1 <= lineData[j]) && (0xDF >= lineData[j])){
				/* Hankaku Kana */
				printf("%c", lineData[j]);
			}else if(((0x81 <= lineData[j]) && (0x9F >= lineData[j])) ||
					((0xE0 <= lineData[j]) && (0xFC >= lineData[j]))){
				/* Shift JIS first byte */
				/* Check the next byte */
				if(j != 15){
					charNext = lineData[j + 1];
				}else{
					if(posData == (lenData - 1)){
						printf(".");
						break;
					}else{
						charNext = *(pData + ((i * 16) + j + 1));
						skip = 1;
					}
				}

				if(((0x40 <= charNext) && (0xFC >= charNext)) && (charNext != 0x7F)){
						printf("%c%c", lineData[j], charNext);
						j++;
				}
			}else{
				printf(".");
			}
		}

		printf("\n");
	}
	printf("\n\n");
	return;
}

