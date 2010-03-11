/*
 * Mallumo --- SWF Malkompilero
 *
 * mallumo.c : Dosiero de Cxefa Fontprogramo
 *
 * Lasta Modifo: 2009/12/21
 *
 * Copyright (C) oksoftware. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "swfreader.h"
#include "hexdump.h"

#define TwipsToPixel(x) (x/20)

/* Basic Data Types */
void DumpSWFRect(SWFRect *rect){
	printf("    Rect:\n"
	       "      X Min\t:%i twips (%i px)\n"
	       "      X Max\t:%i twips (%i px)\n"
	       "      Y Min\t:%i twips (%i px)\n"
	       "      Y Max\t:%i twips (%i px)\n",
	       rect->x_min, TwipsToPixel(rect->x_min),
	       rect->x_max, TwipsToPixel(rect->x_max),
	       rect->y_min, TwipsToPixel(rect->y_min),
	       rect->y_max, TwipsToPixel(rect->y_max));
	return;
}

void DumpSWFRGB(SWFRGB *rgb){
	printf("    RGB:\n"
	       "      Red\t: %u\n"
	       "      Green\t: %u\n"
	       "      Blue\t: %u\n",
	       rgb->red, rgb->green, rgb->blue);
	return;
}

/* Tags */
void DumpSWFTagUnknown(SWFTagUnknown *unknown){
	dumpHex(unknown->len, unknown->data);
	return;
}

/* Display List Tags */

/* Control Tags */
void DumpSWFTagSetBackgroundColor(SWFTagSetBackgroundColor *set_background_color){
	DumpSWFRGB(set_background_color->background_color);
	printf("\n");
	return;
}

void DumpSWFTag(SWFTag *tag){
	char *tag_name[]= {"Unknown",
		/* Display List Tags */
		"PlaceObject", "PlaceObject2", "PlaceObject3",
		"ClipEventFlags", "RemoveObject", "RemoveObject2",
		"ShowFrame",
		/* Control Tags */
		"SetBackgroundColor", "FrameLabel", "Protect", "End",
		"ExportAssets", "ImportAssets", "EnableDebugger",
		"EnableDebugger2", "ScriptLimits", "SetTabIndex",
		"FileAttributes", "ImportAssets2", "SymbolClass",
		"Metadata", "DefineScalingGrid", "DefineSceneAndFrameLabelData"
	};

	printf("SWF Tag:\n"
	       "  Tag Type\t: %s(%u)\n",
			*(tag_name + tag->tag_type), tag->original_tag_type);
	switch(tag->tag_type){
		case TAGTYPE_SETBACKGROUNDCOLOR:
			DumpSWFTagSetBackgroundColor(tag->tag_entity);
			break;
		case TAGTYPE_UNKNOWN:
			DumpSWFTagUnknown(tag->tag_entity);
			break;
	}
	return;
}

void DumpSWFTags(SWFTag *tags){
	SWFTag *cur;
	cur = tags;
	printf("SWF Tags:\n\n");
	while(1){
		DumpSWFTag(cur);
		if((cur = cur->next) == NULL){
			return;
		}
	}

	return;
}

void DumpSWFHeader(SWFHeader *header){
	printf("SWF Header:\n");
	printf("  Compressed\t:%s\n",
			header->is_compressed ? "True" : "False");
	printf("  Version\t:%u\n"
			"  File Length\t:%u\n",
			header->version,
			header->file_length);
	printf("  Frame Size:\n");
	DumpSWFRect(header->frame_size);

	printf("  Frame Rate\t:%f\n"
	       "  Frame Count\t:%u\n\n",
			header->frame_rate,
			header->frame_count);
	
	return;
}
void DumpSWFFile(SWFFile *swf){
	DumpSWFHeader(swf->header);
	DumpSWFTags(swf->tags);
	return;
}
int main(int argc, char *argv[]){
	FILE *swf_file = NULL;
	fpos_t swf_size = 0;
	unsigned char *swf_buffer = NULL;
	SWFFile *swf_result = NULL;

	if(argc < 2){
		fprintf(stderr, "Mallumo: Ne eniga dosiero.\n");
		return EXIT_FAILURE;
	}

	if((swf_file = fopen(argv[1], "rb")) == NULL){
		fprintf(stderr, "Mallumo: Ne povis malfermi la dosieron.\n");
		return EXIT_FAILURE;
	}

	fseek(swf_file, 0, SEEK_END);
	fgetpos(swf_file, &swf_size);

	if((swf_buffer = (unsigned char *)malloc(swf_size)) == NULL){
		fprintf(stderr, "Mallumo: Ne povis atribi memorojn.\n");
		return EXIT_FAILURE;
	}

	fseek(swf_file, 0, SEEK_SET);

	if(fread(swf_buffer, swf_size, 1, swf_file) != 1){
		fprintf(stderr, "Mallumo: Ne povis legi la dosieron.\n");
		return EXIT_FAILURE;
	}

	if((swf_result = SWFReader_Parse(swf_buffer, swf_size)) == NULL){
		fprintf(stderr, "Mallumo: Ne povis analizi la SWFn dosieron.\n");
		return EXIT_FAILURE;
	}

	DumpSWFFile(swf_result);

	SWFReader_Free(swf_result);

	return EXIT_SUCCESS;
}
