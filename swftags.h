#ifndef swftags
#define swftags
typedef enum {
	TAGTYPE_UNKNOWN,
	/* Display List Tags */
	TAGTYPE_PLACEOBJECT,
	TAGTYPE_PLACEOBJECT2,
	TAGTYPE_PLACEOBJECT3,
	TAGTYPE_CLIPEVENTFLAGS,
	TAGTYPE_REMOVEOBJECT,
	TAGTYPE_REMOVEOBJECT2,
	TAGTYPE_SHOWFRAME,
	/* Control Tags */
	TAGTYPE_SETBACKGROUNDCOLOR,
	TAGTYPE_FRAMELABEL,
	TAGTYPE_PROTECT,
	TAGTYPE_END,
	TAGTYPE_EXPORTASSETS,
	TAGTYPE_IMPORTASSETS,
	TAGTYPE_ENABLEDEBUGGER,
	TAGTYPE_ENABLEDEBUGGER2,
	TAGTYPE_SCRIPTLIMITS,
	TAGTYPE_SETTABINDEX,
	TAGTYPE_FILEATTRIBUTES,
	TAGTYPE_IMPORTASSETS2,
	TAGTYPE_SYMBOLCLASS,
	TAGTYPE_METADATA,
	TAGTYPE_DEFINESCALINGRID,
	TAGTYPE_DEFINESCENEANDFRAMELABELDATA
} SWFTagTypes;

/* Basic Data Types */
typedef struct {
	int x_min;
	int x_max;
	int y_min;
	int y_max;
} SWFRect;

typedef struct {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
} SWFRGB;

typedef struct {
	unsigned int has_scale;
	double scale_x;
	double scale_y;
	unsigned int has_rotate;
	double rotate_skew_0;
	double rotate_skew_1;
	double translate_x;
	double translate_y;
} SWFMATRIX;

typedef struct {
	unsigned int has_add_terms;
	unsigned int has_mult_terms;
	int red_mult_term;
	int green_mult_term;
	int blue_mult_term;
	int red_add_term;
	int green_add_term;
	int blue_add_term;
} SWFCXFORM;

typedef struct {
	unsigned int has_add_terms;
	unsigned int has_mult_terms;
	int red_mult_term;
	int green_mult_term;
	int blue_mult_term;
	int alpha_mult_term;
	int red_add_term;
	int green_add_term;
	int blue_add_term;
	int alpha_add_term;
} SWFCXFORMWITHALPHA;

typedef struct {
	unsigned char action_code;
	unsigned short length;
	unsigned char *data;
} SWFACTIONRECORD;

typedef struct {
	unsigned int key_up;
	unsigned int key_down;
	unsigned int mouse_up;
	unsigned int mouse_down;
	unsigned int mouse_move;
	unsigned int unload;
	unsigned int enter_frame;
	unsigned int load;
	unsigned int drag_over;
	unsigned int roll_out;
	unsigned int roll_over;
	unsigned int release_outside;
	unsigned int release;
	unsigned int press;
	unsigned int initialize;
	unsigned int data;
	unsigned int construct;
	unsigned int key_press;
	unsigned int drag_out;
} SWFCLIPEVENTFLAGS;

typedef struct {
	SWFCLIPEVENTFLAGS *event_flags;
	unsigned int action_record_size;
	unsigned char key_code;
	SWFACTIONRECORD actions;
} SWFCLIPACTIONRECORD;

typedef struct {
	SWFCLIPEVENTFLAGS *all_event_flags;
	SWFCLIPACTIONRECORD *clip_action_records;
} SWFCLIPACTIONS;

/* Tags Begin */
typedef struct {
	unsigned int len;
	unsigned char *data;
} SWFTagUnknown;

/* Display List Tags */
typedef struct {
	unsigned short character_id;
	unsigned short depth;
	SWFMATRIX *matrix;
	SWFCXFORM *color_transform;
} SWFTagPlaceObject;

typedef struct {
	/* flags */
	unsigned int has_clip_actions;
	unsigned int has_clip_depth;
	unsigned int has_name;
	unsigned int has_ratio;
	unsigned int has_color_transform;
	unsigned int has_matrix;
	unsigned int has_character;
	unsigned int move;

	unsigned short depth;
	unsigned short character_id;
	SWFMATRIX *matrix;
	SWFCXFORMWITHALPHA *color_transform;
	unsigned short ratio;
	char *name;
	unsigned short clip_depth;
	SWFCLIPACTIONS *clip_actions;
} SWFTagPlaceObject2;

/* Control Tags */
typedef struct {
	SWFRGB *background_color;
} SWFTagSetBackgroundColor;

/* Tags End */

typedef struct tSWFTag {
	struct tSWFTag *next;
	unsigned int original_tag_type;
	SWFTagTypes tag_type;
	void *tag_entity;
} SWFTag;

typedef struct {
	unsigned int is_compressed;
	unsigned int version;
	unsigned int file_length;
	SWFRect *frame_size;
	double frame_rate;
	unsigned int frame_count;
} SWFHeader;

typedef struct {
	SWFHeader *header;
	SWFTag *tags;
} SWFFile;

#endif 
