/*
 * tris_profile.c - iPXE profile page
 *
 * Creation: 2014-12-21 shenweizhou
 */

/*
 * profile struct
 */
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <curses.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>
#include <ipxe/ansicol.h>
#include <ipxe/init.h>
#include <ipxe/version.h>
#include <usr/autoboot.h>
#include <ipxe/smbios.h>

#include <tris/tris_error.h>
#include <tris/trispg.h>
#include <tris/tris_usr.h>
#include <tris/tris_profile.h>

#define PROFILE_SEPERATOR_LINES 1
#define PROFILE_BOTTOM_LINES 2 /* for status bar */
#define PROFILE_AREA_FIRST (TITLE_ROW_COUNT + PROFILE_SEPERATOR_LINES + 1)
#define PROFILE_AREA_LINES (LINES - PROFILE_AREA_FIRST - PROFILE_BOTTOM_LINES)

static char str_profile_os_linux[] = "Linux";
static char str_profile_os_linux_basic[] = "Linux_Basic";
static char str_profile_os_linux_ati[] = "Linux_ATI";
static char str_profile_os_linux_nvidia[] = "Linux_NVIDIA";
static char str_profile_os_dos_tmdt[] = "TMDT";
static char str_profile_os_dos_hitachi[] = "HITACHI";
static char str_profile_os_dos_tandd[] = "TANDD";
static char str_profile_os_dos_bios[] = "BIOS";
static char str_profile_os_dos[] = "DOS";
static char str_profile_os_dos_linux[] = "DOS_LINUX";
static char str_profile_os_memtest_single[] = "Memtest_Single";
static char str_profile_os_memtest_4pass[] = "Memtest_4_Pass";
static char str_profile_os_memtest_continue[] = "Memtest_Continue";
static char str_profile_os_memtest_uefi[] = "MEMTEST_UEFI";
static char str_profile_os_autopilot_marker_clear[] = "AutoPilot";

struct profile_struct_head *current_profile_head = NULL;

// add by Gavin
struct profile_struct *auto_profile = NULL;

extern char *resolvedPn;
extern char *resolvedSn;

/*
 * close session
 */
#ifndef DEMO

static int profile_close_session(struct profile_struct *profile)
{
    int rc;
    char *resp;
    size_t siz;

    /* info */
    info("Close previous test session...");

    /* send "CloseSession */
    rc = gateway_request(
        get_http_params("CloseSession", "pa_no", resolvedPn,
                        "sn", resolvedSn, "mac", page_title_info.mac,
                        "session", ipxe_status.sessionId, "reason",
                        profile->name ? profile->name : profile->dispName, "from",
                        "iPXE",
                        NULL),
        &resp, &siz, NULL);
    clearinfo();
    if (!rc)
    {
        /* done */
    }
    else
    {
        draw_reboot_widget(ERROR_CONNECT_GW);
        wait_shutdown();
    }

    /* done */
    return PROFILE_SET_FINI;
}

/*
 * pre-defined profile list
 */
static struct profile_struct_head profile_close_prev; /* FWD */
static struct profile_struct profile_struct_close_prev_children[] = {{"Freeze", NULL, PROFILE_OS_ALL, &profile_close_prev, NULL,
                                                                      profile_close_session},
                                                                     {"Spontaneous halt", NULL, PROFILE_OS_ALL,
                                                                      &profile_close_prev, NULL, profile_close_session},
                                                                     {"Spontaneous reboot", NULL, PROFILE_OS_ALL, &profile_close_prev, NULL,
                                                                      profile_close_session},
                                                                     {"Cancelled by user", NULL, PROFILE_OS_ALL,
                                                                      &profile_close_prev, NULL, profile_close_session}};
static struct profile_struct_head profile_head_struct_close_prev_children = {
    sizeof(profile_struct_close_prev_children) / sizeof(profile_struct_close_prev_children[0]), 0, 0,
    profile_struct_close_prev_children};
static struct profile_struct profile_struct_close_prev[] = {{"Close previous session", NULL, PROFILE_OS_ALL, NULL,
                                                             &profile_head_struct_close_prev_children, NULL}};
static struct profile_struct_head profile_close_prev = {
    sizeof(profile_struct_close_prev) / sizeof(profile_struct_close_prev[0]), 0, 0,
    profile_struct_close_prev};
#endif

/*
 * reset profile current index
 */
static void reset_profile_index(struct profile_struct_head *head)
{
    int i;

    head->current = 0; /* reset it */
    head->first = 0;   /* reset first row */

    /* loop all profiles */
    for (i = 0; i < head->count; i++)
    {
        if (head->profiles[i].children)
            reset_profile_index(head->profiles[i].children);
    }
}

/*
 * normal profile process
 */
static int profile_process(struct profile_struct *profile)
{
    int rc;
    char *resp;
    size_t siz;
#ifndef SMBIOS_EFI
    char *mode = "csm";
#else
    char *mode = "uefi";
#endif

    if (profile->children)
    {
        /* has children */
        current_profile_head = profile->children;

        return PROFILE_SET_CONT;
    }
    else
    {
    /* send "SetProfile" request */
#ifndef DEMO
        rc = gateway_request(
            get_http_params("SetProfile", 
            "pa_no", resolvedPn,
            "sn", resolvedSn, 
            "mac", page_title_info.mac,
            "profile", profile->name, 
            "mode", mode,
            "from", "iPXE",
            NULL),
            &resp, &siz, NULL);

#else
        rc = 0;
#endif
        if (!rc)
        {
            // if the tandd profile is selected,the server must return an iscsi target
            // with the server made img
            //has got the result
            resp = strltrim(resp);
            if (!strnicmp(resp, string_ok2, strlen(string_ok2)))
            {
                char *flag = "0";
                strcpy(iscsi_uri_dos, resp + 5);
                flag = &resp[3];
                DBG("the flag is %s", flag);
            }

            if (!strnicmp(resp, string_ok, strlen(string_ok)))
            {
            }
            else
            {
                /* show error */
                draw_msg_widget("Error", 3,
                                ERROR_SET_PROFILE,
                                resp,
                                "");
                wait_shutdown();
            }

            /* has done */
        }
        else
        {
            draw_reboot_widget(ERROR_CONNECT_GW);
            wait_shutdown();
        }

        return PROFILE_SET_FINI;
    }
}

/*
 * profile struct
 */
struct profile_struct_item
{
    struct profile_struct_item *children;
    char *display_name;
    char *name;
};

/**
 * if session exists,set profile title to 'close previous session'/'Select a profile'
 */
static void change_profile_title(int flag)
{

    unsigned int row1, col1, col2;
    static char *_title;
    char buf[COLS + 1];
    if (flag == 0)
    {
        _title = "Close Previous Session";
    }
    else
    {
        _title = "Select a profile";
    }
    /* get box position */
    row1 = TITLE_ROW_COUNT + PROFILE_SEPERATOR_LINES;
    col1 = 1;

    col2 = COLS - 2;

    /* draw title line */
    move(row1 + CONSOLE_OFFSET_Y, col1 + CONSOLE_OFFSET_X);
    clrtoeol();
    memset(buf, TBLCH_LINE_HORZ, COLS);
    buf[0] = TBLCH_LEFT_UPPER; /* left-top coner */
    memcpy(&buf[2], _title, strlen(_title));
    buf[col2 - col1] = TBLCH_RIGHT_UPPER; /* right-top coner */
    buf[col2 - col1 + 1] = 0;
    wprintw(stdscr, buf);
}

/*
 * draw profile box
 */
static void draw_profile_box()
{
    unsigned int k;
    unsigned int row1, col1, row2, col2;
    char buf[COLS + 1];
    static const char _title[] = "Select a profile";
    static const char _bottom[] =
        "ESC or BACKSPACE=Steps back, ENTER=Selects, UP and DOWN=Moves Selection";

    /* get box position */
    row1 = TITLE_ROW_COUNT + PROFILE_SEPERATOR_LINES;
    col1 = 1;
    row2 = LINES - PROFILE_BOTTOM_LINES;
    col2 = COLS - 2;

    /* draw title line */
    move(row1 + CONSOLE_OFFSET_Y, col1 + CONSOLE_OFFSET_X);
    memset(buf, TBLCH_LINE_HORZ, COLS);
    buf[0] = TBLCH_LEFT_UPPER; /* left-top coner */
    memcpy(&buf[2], _title, strlen(_title));
    buf[col2 - col1] = TBLCH_RIGHT_UPPER; /* right-top coner */
    buf[col2 - col1 + 1] = 0;
    wprintw(stdscr, buf);

    /* draw border lines */
    memset(buf, ' ', sizeof(buf));
    buf[0] = TBLCH_LINE_VERT;
    buf[col2 - col1] = TBLCH_LINE_VERT;
    buf[col2 - col1 + 1] = 0;
    for (k = row1 + 1; k < row2; k++)
    {
        move(k + CONSOLE_OFFSET_Y, col1 + CONSOLE_OFFSET_X);
        wprintw(stdscr, buf);
    }

    /* draw bottom line */
    move(row2 + CONSOLE_OFFSET_Y, col1 + CONSOLE_OFFSET_X);
    memset(buf, TBLCH_LINE_HORZ, COLS);
    buf[0] = TBLCH_LEFT_LOWER; /* left-bottom coner */
    k = strlen(_bottom);
    memcpy(&buf[col2 - 2 - k], _bottom, k);
    buf[col2 - col1] = TBLCH_RIGHT_LOWER; /* right-bottom coner */
    buf[col2 - col1 + 1] = 0;
    wprintw(stdscr, buf);
}

/*
 * clear profile list screen
 */
static void clear_profile_list()
{
    unsigned int i;
    char buf[COLS + 1];

    /* prepare chars */
    memset(buf, ' ', sizeof(buf));
    buf[0] = TBLCH_LINE_VERT;
    buf[COLS - 3] = TBLCH_LINE_VERT;
    buf[COLS - 2] = 0;

    /* clear screen */
    for (i = PROFILE_AREA_FIRST; i < LINES - PROFILE_BOTTOM_LINES; i++)
    {
        move(i + CONSOLE_OFFSET_Y, 1 + CONSOLE_OFFSET_X);
        wprintw(stdscr, buf);
    }
}

/*
 * clear profile row
 */
static void clear_profile_row(struct profile_struct_head *profile, int row)
{
    char buf[COLS];

    memset(buf, ' ', sizeof(buf));
    buf[COLS - 4] = 0;
    move(PROFILE_AREA_FIRST + row - profile->first + CONSOLE_OFFSET_Y,
         2 + CONSOLE_OFFSET_X);
    wprintw(stdscr, buf);
}

/*
 * draw profile
 */
static void draw_profile_row(struct profile_struct_head *profile, int row)
{
    char text[COLS + 1];
    int k = row - profile->first;
    struct profile_struct *q = &profile->profiles[row];

    /* with children? */
    text[0] = q->children ? '>' : ' ';
    text[2] = text[1] = text[0];
    text[3] = ' ';

    /* draw the row */
    if (row == profile->current)
        color_set(CPAIR_ALERT, NULL);
    clear_profile_row(profile, row);
    move(PROFILE_AREA_FIRST + k + CONSOLE_OFFSET_Y, 2 + CONSOLE_OFFSET_X);

    strcpy(&text[4], q->dispName);
    wprintw(stdscr, text);

    /* reset color */
    if (row == profile->current)
        color_set(CPAIR_NORMAL, NULL);
}

/*
 * draw profile list
 */
static void draw_profile_list(struct profile_struct_head *profile)
{
    int i, j;

    /* calc rows */
    i = profile->count - PROFILE_AREA_LINES; /* max visible index */
    if (i >= 0 && i < profile->first)
        profile->first = i;
    else
        i = profile->first;
    j = profile->count;
    if (j - i > (int)(PROFILE_AREA_LINES))
        j = i + PROFILE_AREA_LINES;

    /* loop all rows */
    for (; i < j; i++)
    {
        //DBG("\nGavinDraw - dispname: %s\n",profile->profiles[i].dispName);
        //DBG_PAUSE();
        draw_profile_row(profile, i);
    }
}

/*
 * profile main loop
 */
static int profile_main_loop()
{
    int key, mv, k, t;
    struct profile_struct *q;
    int redraw = 1; /* need to redraw */

    /* user selection loop */
    for (;;)
    {
        /* redraw rows if necessary */
        if (redraw)
        {
            /* draw profile items */
            if (current_profile_head)
            {
                clear_profile_list();
                draw_profile_list(current_profile_head);
            }

            /* reset redraw */
            redraw = 0;
        }

        /* process keypress */
        key = getkey(0);
        if (current_profile_head)
        {
            mv = 0;
            switch (key)
            {
            case KEY_UP:
                mv = -1;
                break;
            case KEY_DOWN:
                mv = +1;
                break;
            case KEY_PPAGE:
                mv = -PROFILE_AREA_LINES + 1;
                redraw = 1;
                break;
            case KEY_NPAGE:
                mv = +PROFILE_AREA_LINES - 1;
                redraw = 1;
                break;
            case KEY_HOME:
                mv = -current_profile_head->current;
                redraw = 1;
                break;
            case KEY_END:
                mv = current_profile_head->count;
                redraw = 1;
                break;
            case ESC:
            case BACKSPACE:
                q =
                    &current_profile_head->profiles[current_profile_head->current];
                if (q->parent)
                {
                    current_profile_head = q->parent;
                    redraw = 1;
                }
                break;
            case CR:
            case LF:
                q =
                    &current_profile_head->profiles[current_profile_head->current];
                switch ((q->set ? q->set(q) : profile_process(q)))
                {
                case PROFILE_SET_CONT:
                    redraw = 1; /* need redraw */
                    break;
                case PROFILE_SET_FINI:
                    /* has done */
                    return 0;
                }
                break;
            }

            /* redraw? */
            if (mv)
            {
                /* get new select row index */
                t = current_profile_head->current;
                k = t + mv; /* new selected item */
                if (k < 0)
                    k = 0; /* set to first row */
                else if (k >= current_profile_head->count)
                    k = current_profile_head->count - 1; /* set to last row */
                current_profile_head->current = k;       /* set new selected item */

                /* make sure that new selected row is visible */
                if (k < current_profile_head->first)
                {
                    current_profile_head->first = k;
                    redraw = 1;
                }
                else if (k >= (int)(current_profile_head->first + PROFILE_AREA_LINES))
                {
                    current_profile_head->first = k - PROFILE_AREA_LINES + 1;
                    redraw = 1;
                }

                if (!redraw)
                {
                    /* clear old & set new */
                    draw_profile_row(current_profile_head, t);
                    draw_profile_row(current_profile_head, k);
                }
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}

/*
 * send "check" request
 */
#ifndef DEMO

static void send_check()
{
    int rc;
    char *resp, *p, *q;
    size_t siz;

    /* show info */
    info("Check previous test session status...");

    /* send request */
    for (;;)
    {
        rc = gateway_request(
            get_http_params("Check", "pa_no", resolvedPn, "sn",
                            resolvedSn, "mac", page_title_info.mac, "from",
                            "iPXE",
                            NULL),
            &resp, &siz, NULL);
        if (!rc)
        {
            /* has got result */
            p = strltrim(resp);
            if (!strnicmp(p, string_ok, strlen(string_ok)))
            {
                /* no session need to be closed */
                break; /* has done */
            }
            else if (!strncmp(p, string_ongoing, strlen(string_ongoing)))
            {
                /* get session id */
                p += strlen(string_ongoing);
                while (*p == ':')
                    p = strltrim(++p);
                if ((q = strpbrk(p, "\n\r")))
                    *q = 0;
                else
                    resp[siz] = 0;
                strcpy(ipxe_status.sessionId, p);
                // change profile title
                change_profile_title(0);

                /* set profile to predefined profile */
                reset_profile_index(
                    (current_profile_head = &profile_close_prev));

                /* show profile & let use to select */
                profile_main_loop();
                /* clear profile list */
                clear_profile_list();

                /* reset profile list */
                ipxe_status.sessionId[0] = 0;
                current_profile_head = NULL;
            }
        }
        else
        {
            draw_reboot_widget(ERROR_CONNECT_GW);
            wait_shutdown();
        }
    }

    /* clear info */
    clearinfo();
}

#else
static void send_check()
{
}
#endif

/*
 * create profile from response text
 */
static struct profile_struct_head *create_profile_list(char *text)
{
    enum
    {
        PROFILE_STRUCT_INC = 16
    };
    char *p, *p1;
    int k, deep = 0;
    int capacity = 0, cnt = 0;
    struct profile_struct q; /* temp profile */
    struct profile_struct *profiles = NULL;
    struct profile_struct_head *head = (struct profile_struct_head *)malloc(
        sizeof(struct profile_struct_head));
    struct profile_struct_head *parent = NULL, *current = head;

    /* initial head */
    memset(head, 0, sizeof(struct profile_struct_head));

    /* loop each line */
    for (;;)
    {
        /* get the line deep, character '\t' located at the beginning of a line */
        for (k = 0; *text == '\t'; k++, text++)
            ;

        p1 = strltrim(text);
        //		DBG("P1's value is : %s\n", p1);
        //		DBG_PAUSE();
        //		DBG("\n\n\n");

        /* get line */
        if ((p = strpbrk(p1, "\n\r")))
        {
            text = p + strspn(p, "\n\r");
            *p = 0; /* has got one line */

            /* unpack profile struct */
            /*Parse Display name*/
            if (!(p = strchr(p1, ',')) || !(q.dispName = (char *)malloc(p - p1 + 1))) /* memory leak */
                return NULL;
            memcpy(q.dispName, p1, p - p1);
            q.dispName[p - p1] = 0;
            //DBG("\nG111 - dispName  is: %s\n", q.dispName);
            //	DBG_PAUSE();

            /*Parse name*/
            p1 = strltrim(p + 1);

            if (!(p = strchr(p1, ',')) || (!(q.name = (char *)malloc(p - p1 + 1)))) /* memory leak */
                return NULL;
            memcpy(q.name, p1, p - p1);
            q.name[p - p1] = 0;

            /*os*/
            p1 = strltrim(p + 1);

            if ((p = strchr(p1, ',')))
                *p = 0;

            if (strnicmp(p1, str_profile_os_linux_basic,
                         strlen(str_profile_os_linux_basic)) == 0)
            {
                q.os = PROFILE_OS_LINUX_BASIC;
            }
            else if (strnicmp(p1, str_profile_os_linux_ati,
                              strlen(str_profile_os_linux_ati)) == 0)
            {
                q.os = PROFILE_OS_LINUX_ATI;
            }
            else if (strnicmp(p1, str_profile_os_linux_nvidia,
                              strlen(str_profile_os_linux_nvidia)) == 0)
            {
                q.os = PROFILE_OS_LINUX_NVIDIA;
            }
            else if (strnicmp(p1, str_profile_os_linux,
                              strlen(str_profile_os_linux)) == 0)
            {
                q.os = PROFILE_OS_LINUX;
            }
            else if (strnicmp(p1, str_profile_os_dos_tmdt,
                              strlen(str_profile_os_dos_tmdt)) == 0)
            {
                q.os = PROFILE_OS_DOS_TMDT;
            }
            else if (strnicmp(p1, str_profile_os_dos_hitachi,
                              strlen(str_profile_os_dos_hitachi)) == 0)
            {
                q.os = PROFILE_OS_DOS_HITACHI;
            }
            else if (strnicmp(p1, str_profile_os_dos_tandd,
                              strlen(str_profile_os_dos_tandd)) == 0)
            {
                q.os = PROFILE_OS_DOS_TANDD;
                //DBG("p1 = %s, os = DOS", p1);
                //DBG_PAUSE();
            }
            else if (strnicmp(p1, str_profile_os_dos_bios,
                              strlen(str_profile_os_dos_bios)) == 0)
            {
                q.os = PROFILE_OS_DOS_BIOS;
            }
            else if (strnicmp(p1, str_profile_os_dos_linux,
                              strlen(str_profile_os_dos_linux)) == 0)
            {
                q.os = PROFILE_OS_DOS_LINUX;
            }
            else if (strnicmp(p1, str_profile_os_dos,
                              strlen(str_profile_os_dos)) == 0)
            {
                q.os = PROFILE_OS_DOS;
            }
            else if (strnicmp(p1, str_profile_os_memtest_single,
                              strlen(str_profile_os_memtest_single)) == 0)
            {
                q.os = PROFILE_OS_MEMTEST_SINGLE;
            }
            else if (strnicmp(p1, str_profile_os_memtest_continue,
                              strlen(str_profile_os_memtest_continue)) == 0)
            {
                q.os = PROFILE_OS_MEMTEST_CONTINUE;
            }
            else if (strnicmp(p1, str_profile_os_memtest_4pass,
                              strlen(str_profile_os_memtest_4pass)) == 0)
            {
                q.os = PROFILE_OS_MEMTEST_4PASS;
            }
            else if (strnicmp(p1, str_profile_os_memtest_uefi,
                              strlen(str_profile_os_memtest_uefi)) == 0)
            {
                q.os = PROFILE_OS_UEFI_MEMTEST;
            }
            else if (strnicmp(p1, str_profile_os_autopilot_marker_clear,
                              strlen(str_profile_os_autopilot_marker_clear)) == 0)
            {
                q.os = PROFILE_OS_AUTOPILOT_MARKER_CLEAR;
            }
            else
            {
                return NULL;
            }

            //DBG("p1 = %s, os = %d, profile dispname = %s\n", p1, q.os, q.dispName);
            //DBG_PAUSE();

            q.parent = parent;
            q.children = NULL;
            q.set = NULL;

            //DBG("k=%d, and deep=%d\n", k, deep);
            //DBG_PAUSE();
            /* check deep */
            switch (k - deep)
            {

            case 1: /* child profile */
                /* create new head struct */
                //DBG("child profile, name is: %s\n", q.name);
                parent = current;
                current = (struct profile_struct_head *)malloc(
                    sizeof(struct profile_struct_head));
                if (!current)
                    return NULL;
                memset(current, 0, sizeof(struct profile_struct_head));

                /* link it */
                profiles[cnt - 1].children = current;
                q.parent = parent;
                parent->count = cnt;
                parent->profiles = profiles;

                /* reset profile structs */
                cnt = 1;
                capacity = PROFILE_STRUCT_INC;
                profiles = (struct profile_struct *)malloc(
                    sizeof(struct profile_struct) * capacity);
                memcpy(&profiles[0], &q, sizeof(struct profile_struct));

                /* do children */
                deep++;
                //DBG("Current deep=%d", deep);
                break;

            default:
                //DBG("default block\n");
                if (k < deep)
                {
                    /* back to parent profile */
                    do
                    {
                        /* save profiles */
                        current->count = cnt;
                        current->profiles = profiles;

                        /* reset parent & current */
                        current = current->profiles[0].parent;
                        parent = current->profiles[0].parent;

                        /* restore profiles cache */
                        cnt = current->count;
                        profiles = current->profiles;
                        capacity = PROFILE_STRUCT_INC;
                        while (capacity < cnt)
                            capacity += PROFILE_STRUCT_INC;

                        /* do parent */
                        deep--;
                    } while (k < deep);

                    /* push this profile */
                    q.parent = parent;

                    /* fall into sibling profile */
                }
                else
                {
                    /* invalid */
                    //DBG("K>=DEEP, k=%d, deep=%d\n", k, deep);
                    return NULL;
                }
            case 0: /* sibling profile */
                //DBG("sibling profile, NAME is %s\n", q.name);
                if (cnt >= capacity)
                { /* realloc */
                    capacity += PROFILE_STRUCT_INC;
                    //DBG("Profile capacity extended to %d\n", capacity);
                    profiles = (struct profile_struct *)realloc(profiles,
                                                                sizeof(struct profile_struct) * capacity);
                    if (!profiles)
                        return NULL;
                }
                memcpy(&profiles[cnt++], &q, sizeof(struct profile_struct));
                //DBG("Current profile count is %d\n", cnt);
                break;
                //				DBG_PAUSE();
            }
        }
        else
        {
            /* has done */
            break;
        }
    }

    /* save profiles */
    current->count = cnt;
    current->profiles = profiles;

    /* has done */
    return head;
}

/*
 * send "list profile" request
 */
#ifndef DEMO

static void send_list_profile()
{
    int rc;
    char *resp, *p;
    size_t siz;
    struct profile_struct_head *profile;

    /* show info */
    info("Get profile list from gateway...");

    /* send request */
    if (!(rc = gateway_request(
              get_http_params("ListProfile2", "pa_no", resolvedPn, "sn",
                              resolvedSn, "mac", page_title_info.mac, "bootMode",
#ifdef SMBIOS_EFI
                              "UEFI",
#else
                              "CSM",
#endif
                              "from", "iPXE",
                              NULL),
              &resp, &siz, NULL)))
    {
        /* has got result */
        p = strltrim(resp);

        if (!strnicmp(p, string_ok, strlen(string_ok)))
        {
            change_profile_title(1);
            /* has got profile list */
            do
            {
                if ((p = strpbrk(p, "\n\r")))
                {
                    p += strspn(p, "\n\r");
                    if ((profile = create_profile_list(p)))
                    {
                        /* set new profile */
                        current_profile_head = profile;
                        break;
                    }
                }

                draw_reboot_widget(ERROR_PARSE_PROFILE);
                wait_shutdown();
            } while (0);
        }
        else
        {
            draw_reboot_widget(ERROR_GET_PROFILE);
            wait_shutdown();
        }
    }
    else
    {
        /* show error message */
        draw_reboot_widget(ERROR_CONNECT_GW);
        wait_shutdown();
    }

    /* clear info */
    clearinfo();
}

#else
static void send_list_profile()
{
#ifdef SMBIOS_PCBIOS
    current_profile_head = create_profile_list(""
                                               "Enhanced Diagnostics,e_diagnostics,Linux\n"
                                               "Memtest86+ Single Pass,memtest86-single,Linux\n"
                                               "");
#endif
#ifdef SMBIOS_EFI
    current_profile_head = create_profile_list(""
                                               "Enhanced Diagnostics,e_diagnostics,Linux\n"
                                               "");
#endif
}
#endif

/*
 * profile screen
 */
int profile_ui()
{
    /* initial screen */
    draw_title();
    /* clear screen */
    clear_screen();
    /* draw profile box */
    draw_profile_box();
    /* initial profile list */
    clear_profile_list();

    /* send "check" */
    send_check();

    /* list profile */
    send_list_profile();

    /* major loop */
    return profile_main_loop();
}

/* EOF */
