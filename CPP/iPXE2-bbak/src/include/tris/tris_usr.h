

/**
 * tris_usr.h - shared  declaration for  trispg.c, tris_profile.c etc.
 */

/* machine supported features */
enum sku_supported {
	SKU_SUPPORT_INDEX_LINUX	 = 0,
# define SKU_SUPPORT_LINUX		(1<<SKU_SUPPORT_INDEX_LINUX)
	SKU_SUPPORT_INDEX_BIOS,
# define SKU_SUPPORT_BIOS		(1<<SKU_SUPPORT_INDEX_BIOS)
	SKU_SUPPORT_INDEX_FW,
# define SKU_SUPPORT_FW			(1<<SKU_SUPPORT_INDEX_FW)

	SKU_SUPPORT_INDEX_COUNT
# define SKU_SUPPORT_ALL		(SKU_SUPPORT_INDEX_COUNT-1)
};


/* add by Gavin */
#define GEDB_YES	 1
#define GEDB_NO      0
#define GEDB_ERROR		-1
#define GEDB_AVAILABLE


int user_ui();
void check_bios();
void check_image(); 

extern void ShowBox5();

