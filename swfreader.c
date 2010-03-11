/*
 * Mallumo --- SWF Malkompilero
 *
 * swfreader.c : Legilo de SWF Dosiero
 *
 * Lasta Modifo: 2009/12/21
 *
 * Copyright (C) oksoftware. All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include "swfreader.h"
#include "binreader.h"

#define CheckReadError(br) if(br->err){\
	fprintf(stderr, "Mallumo: SWFReader: La dosiero estas tro mallonga.\n");\
	return NULL;\
}


void *SWFReader_Malloc(size_t size){
	void *ret = NULL;

	if((ret = malloc(size)) == NULL){
		fprintf(stderr, "Mallumo: SWFReader: Ne povis atribi memorojn.\n");
		exit(EXIT_FAILURE);
	}

	return ret;
}

SWFRect *SWFReader_ReadRect(BinaryReader *src){
	SWFRect *rect;
	unsigned int n_bits = 0;
	
	rect = SWFReader_Malloc(sizeof(SWFRect));

	n_bits = ReadUB(src, 5);
	rect->x_min = ReadSB(src, n_bits);
	rect->x_max = ReadSB(src, n_bits);
	rect->y_min = ReadSB(src, n_bits);
	rect->y_max = ReadSB(src, n_bits);

	SkipBits(src);

	return rect;
}

SWFRGB *SWFReader_ReadRGB(BinaryReader *src){
	SWFRGB *rgb;
	
	rgb = SWFReader_Malloc(sizeof(SWFRGB));
	rgb->red = ReadUI8(src);
	rgb->green = ReadUI8(src);
	rgb->blue = ReadUI8(src);
	CheckReadError(src);

	return rgb;
}

SWFMATRIX *SWFReader_ReadMATRIX(BinaryReader *src){
	SWFMATRIX *matrix;
	unsigned int n_bits;
	
	matrix = SWFReader_Malloc(sizeof(SWFMATRIX));
	if((matrix->has_scale = ReadUB(src, 1)) == 1){
		n_bits = ReadUB(src, 5);
		matrix->scale_x = ReadFB(src, n_bits);
		matrix->scale_y = ReadFB(src, n_bits);
	}
	if((matrix->has_rotate = ReadUB(src, 1)) == 1){
		n_bits = ReadUB(src, 5);
		matrix->rotate_skew_0 = ReadFB(src, n_bits);
		matrix->rotate_skew_1 = ReadFB(src, n_bits);
	}

	n_bits = ReadUB(src, 5);
	matrix->translate_x = ReadFB(src, n_bits);
	matrix->translate_y = ReadFB(src, n_bits);

SkipBits(src);

	CheckReadError(src);
	
	return matrix;
}

SWFCXFORM *SWFReader_ReadCXFORM(BinaryReader *src){
	SWFCXFORM *cxform;
	unsigned int n_bits;

	cxform = SWFReader_Malloc(sizeof(SWFCXFORM));

	cxform->has_add_terms = ReadUB(src, 1);
	cxform->has_mult_terms = ReadUB(src, 1);

	n_bits = ReadUB(src, 4);

	if(cxform->has_mult_terms){
		cxform->red_mult_term = ReadSB(src, n_bits);
		cxform->green_mult_term = ReadSB(src, n_bits);
		cxform->blue_mult_term = ReadSB(src, n_bits);
	}

	if(cxform->has_add_terms){
		cxform->red_add_term = ReadSB(src, n_bits);
		cxform->green_add_term = ReadSB(src, n_bits);
		cxform->blue_add_term = ReadSB(src, n_bits);
	}

	SkipBits(src);

	CheckReadError(src);

	return cxform;
}

SWFCXFORMWITHALPHA *SWFReader_ReadCXFORMWITHALPHA(BinaryReader *src){
	SWFCXFORMWITHALPHA *cxform_with_alpha;
	unsigned int n_bits;

	cxform_with_alpha = SWFReader_Malloc(sizeof(SWFCXFORMWITHALPHA));

	cxform_with_alpha->has_add_terms = ReadUB(src, 1);
	cxform_with_alpha->has_mult_terms = ReadUB(src, 1);

	n_bits = ReadUB(src, 4);

	if(cxform_with_alpha->has_mult_terms){
		cxform_with_alpha->red_mult_term = ReadSB(src, n_bits);
		cxform_with_alpha->green_mult_term = ReadSB(src, n_bits);
		cxform_with_alpha->blue_mult_term = ReadSB(src, n_bits);
		cxform_with_alpha->alpha_mult_term = ReadSB(src, n_bits);
	}

	if(cxform_with_alpha->has_add_terms){
		cxform_with_alpha->red_add_term = ReadSB(src, n_bits);
		cxform_with_alpha->green_add_term = ReadSB(src, n_bits);
		cxform_with_alpha->blue_add_term = ReadSB(src, n_bits);
		cxform_with_alpha->alpha_mult_term = ReadSB(src, n_bits);
	}

	SkipBits(src);

	CheckReadError(src);

	return cxform_with_alpha;
}

SWFCLIPEVENTFLAGS *SWFReader_ReadCLIPEVENTFLAGS(BinaryReader *src, unsigned char swf_version){
	unsigned int tmp;
	SWFCLIPEVENTFLAGS *clip_event_flags;
	clip_event_flags = SWFReader_Malloc(sizeof(SWFCLIPEVENTFLAGS));

	clip_event_flags->key_up = ReadUB(src, 1);
	clip_event_flags->key_down = ReadUB(src, 1);
	clip_event_flags->mouse_up = ReadUB(src, 1);
	clip_event_flags->mouse_down = ReadUB(src, 1);
	clip_event_flags->mouse_move = ReadUB(src, 1);
	clip_event_flags->unload = ReadUB(src, 1);
	clip_event_flags->enter_frame = ReadUB(src, 1);
	clip_event_flags->load = ReadUB(src, 1);
	clip_event_flags->drag_over = ReadUB(src, 1);
	clip_event_flags->roll_out = ReadUB(src, 1);
	clip_event_flags->roll_over = ReadUB(src, 1);
	clip_event_flags->release_outside = ReadUB(src, 1);
	clip_event_flags->release = ReadUB(src, 1);
	clip_event_flags->press = ReadUB(src, 1);
	clip_event_flags->initialize = ReadUB(src, 1);
	clip_event_flags->data = ReadUB(src, 1);

	if(swf_version >= 6){
		tmp = ReadUB(src, 5);
		clip_event_flags->construct = ReadUB(src, 1);
		clip_event_flags->key_press = ReadUB(src, 1);
		clip_event_flags->drag_out = ReadUB(src, 1);
		tmp = ReadUB(src, 8);
	}

	SkipBits(src);

	CheckReadError(src);

	return clip_event_flags;
}

SWFCLIPACTIONRECORD *SWFReader_ReadCLIPACTIONRECORD(BinaryReader *src){
	SWFCLIPACTIONRECORD *clip_action_record;
	clip_action_record = SWFReader_Malloc(sizeof(SWFCLIPACTIONRECORD));
	if((clip_action_record->event_flags = SWFReader_ReadCLIPEVENTFLAGS(src)) == NULL){
		return NULL;
	}

	/**** TODAY'S CODING IS END OF THERE ***/
	/**** HODIAUXA FONTPROGRAMI FINIGXAS EN TIE ***/ 

	return clip_action_record;
}

SWFCLIPACTIONS *SWFReader_ReadCLIPACTIONS(BinaryReader *src, unsigned char swf_version){
	unsigned int tmp;
	SWFCLIPACTIONS *clip_actions;
	clip_actions = SWFReader_Malloc(sizeof(SWFCLIPACTIONS));
	if((clip_actions->all_event_flags = SWFReader_ReadCLIPEVENTFLAGS(src, swf_version)) == NULL){
		return NULL;
	}

	if((clip_actions->clip_action_records = SWFReader_ReadCLIPACTIONRECORD(src)) == NULL){
		return NULL;
	}

	if(swf_version <= 5){
		tmp = ReadUI16(src);
	} else if(swf_version >= 6){
		tmp = ReadUI32(src);
	}
		
	return clip_actions;
}

/* Tags Begin */
SWFTagUnknown *SWFReader_ReadTagUnknown(BinaryReader *src, int len){
	SWFTagUnknown *unknown;

	unknown = SWFReader_Malloc(sizeof(SWFTagUnknown));

	unknown->len = (unsigned int)len;
	unknown->data = SWFReader_Malloc((unsigned int)len);

	ReadBytes(src, unknown->data, (unsigned int)len);
	CheckReadError(src);
	return unknown;
}

/* Display List Tags */
SWFTagPlaceObject *SWFReader_ReadTagPlaceObject(BinaryReader *src, int len){
	SWFTagPlaceObject *place_object;
	unsigned int tag_onset;

	tag_onset = src->cur;
	place_object = SWFReader_Malloc(sizeof(SWFTagPlaceObject));

	place_object->character_id = ReadUI16(src);
	place_object->depth = ReadUI16(src);
	CheckReadError(src);

	if((place_object->matrix = SWFReader_ReadMATRIX(src)) == NULL){
		return NULL;
	}

	if((src->cur - tag_onset) < len){
		if((place_object->color_transform = SWFReader_ReadCXFORM(src)) == NULL){
			return NULL;
		}
	}

	return place_object;
}

SWFTagPlaceObject2 *SWFReader_ReadTagPlaceObject2(BinaryReader *src, unsigned char swf_version){
	SWFTagPlaceObject2 *place_object_2;
	place_object_2 = SWFReader_Malloc(sizeof(SWFTagPlaceObject2));

	/* flags */
	place_object_2->has_clip_actions = ReadUB(src, 1);
	place_object_2->has_clip_depth = ReadUB(src, 1);
	place_object_2->has_name = ReadUB(src, 1);
	place_object_2->has_ratio = ReadUB(src, 1);
	place_object_2->has_color_transform = ReadUB(src, 1);
	place_object_2->has_matrix = ReadUB(src, 1);
	place_object_2->has_character = ReadUB(src, 1);
	place_object_2->move = ReadUB(src, 1);
	SkipBits(src);

	place_object_2->depth = ReadUI16(src);

	if(place_object_2->has_character){
		place_object_2->character_id = ReadUI16(src);
	}

	CheckReadError(src);

	if(place_object_2->has_matrix){
		if((place_object_2->matrix = SWFReader_ReadMATRIX(src)) == NULL){
			return NULL;
		}
	}

	if(place_object_2->has_color_transform){
		if((place_object_2->color_transform = SWFReader_ReadCXFORMWITHALPHA(src)) == NULL){
			return NULL;
		}
	}

	if(place_object_2->has_ratio){
		place_object_2->ratio = ReadUI16(src);
	}

	if(place_object_2->has_name){
		place_object_2->name = ReadString(src);
	}

	if(place_object_2->has_clip_depth){
		place_object_2->clip_depth = ReadUI16(src);
	}

	CheckReadError(src);

	if(place_object_2->has_clip_actions){
		if((place_object_2->clip_actions = SWFReader_ReadCLIPACTIONS(src, swf_version)) == NULL){
			return NULL;
		}
	}

	return place_object_2;
}
/* Control Tags */
SWFTagSetBackgroundColor *SWFReader_ReadTagSetBackgroundColor(BinaryReader *src){
	SWFTagSetBackgroundColor *set_background_color;
	set_background_color = SWFReader_Malloc(sizeof(SWFTagSetBackgroundColor));

	if((set_background_color->background_color = SWFReader_ReadRGB(src)) == NULL){
		return NULL;
	}

	return set_background_color;
}


	
SWFTag *SWFReader_ReadTag(BinaryReader *src, SWFHeader *header){
	unsigned short tag_code_and_length;
	SWFTag *tag;
	int len = 0;
	tag = SWFReader_Malloc(sizeof(SWFTag));
	
	tag_code_and_length = ReadUI16(src);

	tag->original_tag_type = (tag_code_and_length & 0xFFC0) >> 6;/*0xFFC0 == 0b 1111 1111 1100 0000*/
	len = tag_code_and_length & 0x003F;/*0x003F == 0b 11 1111*/
	if(len == 0x3F){
		len = ReadSI32(src);
	}

	switch(tag->original_tag_type){
		/* Display List Tags */
		case 4:/* PlaceObject */
			tag->tag_type = TAGTYPE_PLACEOBJECT;
			tag->tag_entity = (void *)SWFReader_ReadTagPlaceObject(src, len);
			break;
		case 26:/* PlaceObject2 */
			tag->tag_type = TAGTYPE_PLACEOBJECT2;
			tag->tag_entity = (void *)SWFReader_ReadTagPlaceObject2(src, header->version);
			break;
		/* Control Tags */
		case 9:/* SetBackgroundColor */
			tag->tag_type = TAGTYPE_SETBACKGROUNDCOLOR;
			tag->tag_entity = (void *)SWFReader_ReadTagSetBackgroundColor(src);
			break;
		default:/* Unknown */
			tag->tag_type = TAGTYPE_UNKNOWN;
			tag->tag_entity = (void *)SWFReader_ReadTagUnknown(src, len);
			break;
	}

	tag->next = NULL;
	return tag;
}

SWFHeader *SWFReader_ReadHeader(BinaryReader *src){
	SWFHeader *header;
	unsigned int tmp;

	header = SWFReader_Malloc(sizeof(SWFHeader));

	tmp = ReadUI8(src);
	CheckReadError(src);

	if(tmp == 'F'){
		header->is_compressed = 0;
	}else if(tmp == 'C'){
		header->is_compressed = 1;
	}else{
		CheckReadError(src);
		fprintf(stderr, "Mallumo: SWFReader: La dosiero ne estas SWF dosiero.\n");
		return NULL;
	}

	if((ReadUI8(src) != 'W') || (ReadUI8(src) != 'S')){
		CheckReadError(src);
		fprintf(stderr, "Mallumo: SWFReader: La dosiero ne estas SWF dosiero.\n");
		return NULL;
	}

	header->version = ReadUI8(src);
	CheckReadError(src);

	header->file_length = ReadUI32(src);
	CheckReadError(src);

	if(header->is_compressed){
		if(SwapBufferToZlib(src, header->file_length - 8)){
			fprintf(stderr, "Mallumo: SWFReader: Ne povis malpaki SWFn dosieron.\n");
			return NULL;
		}
	}

	header->frame_size = SWFReader_ReadRect(src);

	header->frame_rate = ReadFIXED8(src);
	CheckReadError(src);

	header->frame_count = ReadUI16(src);
	CheckReadError(src);

	return header;
}

SWFFile *SWFReader_Parse(unsigned char *src, unsigned int len){
	SWFFile *result = NULL;
	BinaryReader *br;
	SWFTag **prev;

	result = SWFReader_Malloc(sizeof(SWFFile));

	br = CreateBinaryReader(src, len);

	if((result->header = SWFReader_ReadHeader(br)) == NULL){
		fprintf(stderr, "Mallumo: SWFReader: Ne povis analizi titolon de SWFn dosieron.\n");
		return NULL;
	}

	prev = &result->tags;
	while(br->len - br->cur){
		if((*prev = SWFReader_ReadTag(br, result->header)) == NULL){
			fprintf(stderr, "Mallumo: SWFReader: Ne povis legi etikedon de SWFn dosieron.\n");
			return NULL;
		}
		prev = &((*prev)->next);
	}
	
	return result;
}

void SWFReader_Free(SWFFile *sf){
	free(sf->header);
	free(sf);
	return;
}
