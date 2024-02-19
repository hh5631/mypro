

/**
 * trispg.h - declare  shared variables and functions for trispg.c,  tris_profile.c, etc.
 */

/* special table characters */
#define TBLCH_LEFT_UPPER		'+'
#define TBLCH_LEFT_LOWER		'+'
#define TBLCH_RIGHT_UPPER		'+'
#define TBLCH_RIGHT_LOWER		'+'
#define TBLCH_LINE_HORZ		    '-'
#define TBLCH_LINE_VERT			'|'



#define TITLE_LEFT_MARGIN		1
#define TITLE_RIGHT_MARGIN		1

/* common string */
extern const char string_ok[];
extern const char string_ok2[];
extern const char string_err[];
extern const char string_ongoing[];
extern const char string_linux[];
extern const char string_dos[];
extern const char string_bios[];
extern const char string_fwtool[];

enum {
	TITLE_ROW_VERSION	 = 0,
	TITLE_ROW_URL,
	TITLE_ROW_MODEL,
	TITLE_ROW_SN,
	TITLE_ROW_BIOS,

	TITLE_ROW_COUNT
};

struct page_title_info_struct {
	char ip_addr[64];
	char gateway_server[64];

	char prod_name[64];
	char ipxe_name[64];

	char prod_url[64];
	char ipxe_url[64];

	char manufacturer[64];
        
        char oemString[64];

	char model[64];
	char pa_no[64];

	char sn[64];
	char mac[20];

	int HDDs;
	char bios_version[20];
};

 extern struct page_title_info_struct page_title_info;

 /* ipxe status info */
 struct ipxe_status_struct {
 	unsigned int supported;
 	char sessionId[128];
 };
 extern struct ipxe_status_struct ipxe_status;

 /* put widget to center according to model name */
 extern int CONSOLE_OFFSET_X;
 extern int CONSOLE_OFFSET_Y;

 extern char url_params[2048];

 void info(const char* fmt, ...);
 int gateway_request(const char* req,	char** buf, size_t *siz, void (*notify)()) ;
 char* get_http_params(const char* name, ...);


 void clearinfo();
 void draw_msg_widget(const char* title, int rows, ...);
 void draw_reboot_widget(const char* error);
 void wait_shutdown();
 void draw_title();
 void clear_screen();


 
 char* strltrim(char* str);
 
 extern char * iscsi_uri_dos;
 

 
 extern char * newest_bios_version;
 extern char * biosUpgradeable;
 extern  char * oemString;
 
 extern char * resolvedPn;
 extern char * resolvedSn;
 
 extern char * imageType;
