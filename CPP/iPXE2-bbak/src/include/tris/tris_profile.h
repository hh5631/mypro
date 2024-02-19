

/**
 * tris_profiile.h - shared  declaration for  trispg.c, tris_profile.c etc.
 */

enum profile_os_type {
	PROFILE_OS_ALL = 0,
	PROFILE_OS_LINUX,
	PROFILE_OS_LINUX_BASIC,
	PROFILE_OS_LINUX_ATI,
	PROFILE_OS_LINUX_NVIDIA,
	PROFILE_OS_DOS,
	PROFILE_OS_DOS_TMDT,
	PROFILE_OS_DOS_TANDD,
	PROFILE_OS_DOS_BIOS,
	PROFILE_OS_DOS_LINUX, //Profile to run accross multiple OS
	PROFILE_OS_MEMTEST_SINGLE,
    PROFILE_OS_MEMTEST_CONTINUE,
	PROFILE_OS_MEMTEST_4PASS,
	PROFILE_OS_DOS_HITACHI,
	PROFILE_OS_UEFI_MEMTEST,
	PROFILE_OS_COUNT,
	PROFILE_OS_AUTOPILOT_MARKER_CLEAR
};
enum profile_set_result {
	PROFILE_SET_CONT = 0,					/* new profile list or update profile list */
	PROFILE_SET_FINI,						/* has done */
};
struct profile_struct_head;		/* FWD */
struct profile_struct {
	char* dispName;						/* profile display name */
	char* name;							/* profile name */
	unsigned os;							/* support OS type */
	struct profile_struct_head *parent;	/* parent */
	struct profile_struct_head *children;	/* childrens */

	/* operations */
	int (*set)(struct profile_struct*);
};

struct profile_struct_head {
	int count;								/* count of profile */
	int current;							/* current selected index */
	int first;								/* first visible row */
	struct profile_struct *profiles;	/* profiles */
};

extern struct profile_struct_head *current_profile_head;
extern struct profile_struct *auto_profile;

int profile_ui() ;
