/*
 * tris_usr.c - iPXE user code & sn verify page
 *
 * Creation: 2014-12-21 shenweizhou
 */

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>
#include <ipxe/ansicol.h>
#include <ipxe/smbios.h>
#include <ipxe/init.h>
#include <ipxe/version.h>
#include <usr/autoboot.h>

#include <tris/tris_error.h>
#include <tris/trispg.h>
#include <tris/tris_usr.h>
#include <tris/tris_profile.h>

char *newest_bios_version;
char *biosUpgradeable;
char *resolvedPn;
char *resolvedSn;
char *imageType;

void ShowBox5();

struct user_input_struct
{
    unsigned int user_code_idx;
    char user_code[64];
    unsigned int mach_sn_idx;
    char mach_sn[64];
};
static struct user_input_struct user_input_struct;

#define CHECK_KEYBOARD_STUCK_COUNT 3

static void check_keyboard()
{
    int k, row = (LINES + 1) / 2;

    if (row <= TITLE_ROW_COUNT)
        row = TITLE_ROW_COUNT + 1;

    /* draw key stuck tip */
    info("Check key is stuck...");

    /* check key stuck */
    k = getkey(TICKS_PER_SEC);
    clearinfo();

    if (k > 0)
    {
    /* report key is stuck */
#ifndef DEMO
        gateway_request(get_http_params("KeyboardIsStuck",
                                        "pa_no", page_title_info.pa_no,
                                        "sn", page_title_info.sn,
                                        "mac", page_title_info.mac,
                                        "prodName", page_title_info.model,
                                        "codeName", page_title_info.model,
                                        "from", "iPXE",
                                        NULL),
                        NULL, NULL, NULL);
#endif

        /* key stuck */
        draw_msg_widget("Error", 4,
                        ERROR_KEY_STUCK,
                        "User input on the unit is unreliable.",
                        "Remove the built-in keyboard, attach a USB keyboard",
                        "and reboot the unit.");

        wait_shutdown();
    }
}

static void get_inputs()
{
    enum
    {
        INPUT_STATE_USER,
        INPUT_STATE_SN
    };
    int row, col;
    int key, state = INPUT_STATE_USER;
    int r = 0, c = 0;
    static const char line_chars[] = "__________________________";

    /* get row index */
    row = (LINES - TITLE_ROW_COUNT - 10) / 2 + TITLE_ROW_COUNT;
    if (row <= TITLE_ROW_COUNT)
        row = TITLE_ROW_COUNT + 1;
    /* get col index */
    col = (COLS - sizeof(line_chars)) / 2;

    /* draw user code input */
    wmove(stdscr, row, col + 3);
    wprintw(stdscr, "User Code:");
    wmove(stdscr, row + 2, col);
    wprintw(stdscr, line_chars);

    /* draw serial number input */
    wmove(stdscr, row + 5, col + 3);
    wprintw(stdscr, "Unit Serial Number:");
    wmove(stdscr, row + 7, col);
    wprintw(stdscr, line_chars);

    /* initial user input struct */
    memset(&user_input_struct, 0, sizeof(user_input_struct));
    if (strlen(page_title_info.sn) != 1)
        strcpy(user_input_struct.mach_sn, page_title_info.sn);
    //DBG("page_sn length: %d\n",(int)strlen(page_title_info.sn));
    //DBG_PAUSE();
    user_input_struct.mach_sn_idx = strlen(user_input_struct.mach_sn);

    /* initial serial number */
    wmove(stdscr, row + 7, col);
    wprintw(stdscr, user_input_struct.mach_sn);

    /* set cursor */
    wmove(stdscr, row + 2, col);
    curs_set(1);

    /* get user inputs */
    for (;;)
    {
        switch ((key = getkey(0)))
        {
        case CR:
        case LF:
            if (state == INPUT_STATE_USER)
            {
                state = INPUT_STATE_SN;
                wmove(stdscr, row + 7, col + user_input_struct.mach_sn_idx);
            }
            else
            {
                /* has done */

                if (user_input_struct.user_code_idx == 0)
                {

                    ShowBox5();

                    int row, col;

                    static const char line_chars[] = "__________________________";

                    /* get row index */
                    row = (LINES - TITLE_ROW_COUNT - 10) / 2 + TITLE_ROW_COUNT;
                    if (row <= TITLE_ROW_COUNT)
                        row = TITLE_ROW_COUNT + 1;
                    /* get col index */
                    col = (COLS - sizeof(line_chars)) / 2;

                    /* draw user code input */
                    wmove(stdscr, row, col + 3);
                    wprintw(stdscr, "User Code:");
                    wmove(stdscr, row + 2, col);
                    wprintw(stdscr, line_chars);

                    /* draw serial number input */
                    wmove(stdscr, row + 5, col + 3);
                    wprintw(stdscr, "Unit Serial Number:");
                    wmove(stdscr, row + 7, col);
                    wprintw(stdscr, line_chars);

                    /* initial user input struct */
                    memset(&user_input_struct, 0, sizeof(user_input_struct));
                    if (strlen(page_title_info.sn) != 1)
                        strcpy(user_input_struct.mach_sn, page_title_info.sn);
                    //DBG("page_sn length: %d\n",(int)strlen(page_title_info.sn));
                    //DBG_PAUSE();
                    user_input_struct.mach_sn_idx = strlen(user_input_struct.mach_sn);

                    /* initial serial number */
                    wmove(stdscr, row + 7, col);
                    wprintw(stdscr, user_input_struct.mach_sn);

                    /* set cursor */
                    wmove(stdscr, row + 2, col);
                    state = INPUT_STATE_USER;
                    curs_set(1);
                }
                else
                {
                    user_input_struct.user_code[user_input_struct.user_code_idx] = 0;
                    user_input_struct.mach_sn[user_input_struct.mach_sn_idx] = 0;

                    /* hide cursor */
                    curs_set(0);
                    return;
                }
            }
            break;

        case KEY_UP:
            switch (state)
            {
            case INPUT_STATE_SN:
                state = INPUT_STATE_USER;
                wmove(stdscr, row + 2, col + user_input_struct.user_code_idx);
                break;
            case INPUT_STATE_USER:
            default:
                break;
            }
            break;

        case KEY_DOWN:
            switch (state)
            {
            case INPUT_STATE_USER:
                state = INPUT_STATE_SN;
                wmove(stdscr, row + 7, col + user_input_struct.mach_sn_idx);
                break;
            case INPUT_STATE_SN:
            default:
                break;
            }
            break;

        case BACKSPACE:
            switch (state)
            {
            case INPUT_STATE_USER:
                if (user_input_struct.user_code_idx)
                {
                    user_input_struct.user_code_idx--;
                    r = row + 2, c = col + user_input_struct.user_code_idx;
                    break;
                }
                else
                    continue;
            case INPUT_STATE_SN:
                if (user_input_struct.mach_sn_idx)
                {
                    user_input_struct.mach_sn_idx--;
                    r = row + 7, c = col + user_input_struct.mach_sn_idx;
                    break;
                }
                else
                    continue;
            }
            wmove(stdscr, r, c);
            wprintw(stdscr, "_");
            wmove(stdscr, r, c);
            break;

        default:
            /* accept ASCII code */
            if (key >= 32 && key <= 127)
            {
                switch (state)
                {
                case INPUT_STATE_USER:
                    if (user_input_struct.user_code_idx < sizeof(line_chars) - 2)
                    {
                        user_input_struct.user_code[user_input_struct.user_code_idx++] = key;
                        break;
                    }
                    else /* buffer full */
                        continue;
                case INPUT_STATE_SN:
                    if (user_input_struct.mach_sn_idx < sizeof(line_chars) - 2)
                    {
                        user_input_struct.mach_sn[user_input_struct.mach_sn_idx++] = key;
                        break;
                    }
                    else
                        continue;
                }
                wprintw(stdscr, "%c", key);
            }
        }
    }
}

// added by xyato
bool ShowBox6(char *msg)
{
    int key;
    char *btnBk = "     ";
    int btnSpace = 4;
    int msglen = strlen(msg);
    int infoWidth = COLS - 4;
    int infoHeight = msglen / (COLS - 2) + 7;

    int btnYesX = (infoWidth - (2 * strlen(btnBk) + btnSpace)) / 2;
    int btnNoX = btnYesX + strlen(btnBk) + btnSpace;
    int btnYesY;
    int btnNoY;

    bool bYes = FALSE;

    btnYesY = btnNoY = infoHeight - 2;

    WINDOW *infoWin = newwin(infoHeight, infoWidth, (LINES - infoHeight) / 2, (COLS - infoWidth) / 2);
    //wbkgdset(infoWin, COLOR_PAIR(CPAIR_URL) | A_COLOR);
    wcolour_set(infoWin, CPAIR_ALERT, infoWin);
    werase(infoWin);
    //wattron(infoWin, COLOR_PAIR(CPAIR_ALERT) | A_COLOR);
    box(infoWin, TBLCH_LINE_VERT | infoWin->attrs, TBLCH_LINE_HORZ | infoWin->attrs);

    // THIS IS A KENG
    mvwprintw(infoWin, 2, 2, msg);
    mvwprintw(infoWin, 3, 2, "Continue anyway?");

    wattron(infoWin, A_BOLD);

    wcolour_set(infoWin, CPAIR_NORMAL, infoWin);
    mvwprintw(infoWin, btnYesY, btnYesX, btnBk);
    mvwprintw(infoWin, btnNoY, btnNoX, btnBk);

    do
    {
        wcolour_set(infoWin, CPAIR_EDIT, infoWin);
        wattron(infoWin, A_BOLD);
        bYes ? mvwprintw(infoWin, btnYesY, btnYesX + 1, "YES") : mvwprintw(infoWin, btnNoY, btnNoX + 1, "NO");
        wattroff(infoWin, A_BOLD);

        wcolour_set(infoWin, CPAIR_NORMAL, infoWin);
        bYes ? mvwprintw(infoWin, btnNoY, btnNoX + 1, "NO") : mvwprintw(infoWin, btnYesY, btnYesX + 1, "YES");

        switch (key = getkey(0))
        {
        case KEY_LEFT:
            bYes = TRUE;
            break;
        case KEY_RIGHT:
            bYes = FALSE;
            break;
        }
    } while (key != KEY_ENTER);

    delwin(infoWin);
    return bYes;
}

// added by xyato
bool ShowBox7(char *msg)
{
    int key;
    char *btnBk = "     ";
    int btnSpace = 4;
    int msglen = strlen(msg);
    int infoWidth = COLS - 4;
    // 对 msg 按固定长度 60 字符拆分
    int count = (msglen + 59) / 60;
    int infoHeight = count + 6;

    int btnYesX = (infoWidth - (2 * strlen(btnBk) + btnSpace)) / 2;
    int btnNoX = btnYesX + strlen(btnBk) + btnSpace;
    int btnYesY;
    int btnNoY;

    bool bYes = FALSE;

    btnYesY = btnNoY = infoHeight - 2;

    WINDOW *infoWin = newwin(infoHeight, infoWidth, (LINES - infoHeight) / 2, (COLS - infoWidth) / 2);
    //wbkgdset(infoWin, COLOR_PAIR(CPAIR_URL) | A_COLOR);
    wcolour_set(infoWin, CPAIR_ALERT, infoWin);
    werase(infoWin);
    //wattron(infoWin, COLOR_PAIR(CPAIR_ALERT) | A_COLOR);
    box(infoWin, TBLCH_LINE_VERT | infoWin->attrs, TBLCH_LINE_HORZ | infoWin->attrs);

    // THIS IS A KENG
    char *tmp = (char *)calloc(1, 61);
    memset(tmp, '\0', 61);
    int j = 2;
    int i = 0;
    for (; i < count; i++, j++)
    {
        strncpy(tmp, msg + i * 60, 60);
        mvwprintw(infoWin, j, 2, tmp);
        memset(tmp, '\0', 61);
    }
    mvwprintw(infoWin, j + 1, 2, "Continue anyway?");

    wattron(infoWin, A_BOLD);

    wcolour_set(infoWin, CPAIR_NORMAL, infoWin);
    mvwprintw(infoWin, btnYesY, btnYesX, btnBk);
    mvwprintw(infoWin, btnNoY, btnNoX, btnBk);

    do
    {
        wcolour_set(infoWin, CPAIR_EDIT, infoWin);
        wattron(infoWin, A_BOLD);
        bYes ? mvwprintw(infoWin, btnYesY, btnYesX + 1, "YES") : mvwprintw(infoWin, btnNoY, btnNoX + 1, "NO");
        wattroff(infoWin, A_BOLD);

        wcolour_set(infoWin, CPAIR_NORMAL, infoWin);
        bYes ? mvwprintw(infoWin, btnNoY, btnNoX + 1, "NO") : mvwprintw(infoWin, btnYesY, btnYesX + 1, "YES");

        switch (key = getkey(0))
        {
        case KEY_LEFT:
            bYes = TRUE;
            break;
        case KEY_RIGHT:
            bYes = FALSE;
            break;
        }
    } while (key != KEY_ENTER);

    delwin(infoWin);
    return bYes;
}

//提醒框
void ShowBox5()
{
    int key;

    int infoWidth = COLS - 4;
    int infoHeight = 9;

    WINDOW *infoWin = newwin(infoHeight, infoWidth, (LINES - infoHeight) / 2, (COLS - infoWidth) / 2);
    //wbkgdset(infoWin, COLOR_PAIR(CPAIR_URL) | A_COLOR);
    wcolour_set(infoWin, CPAIR_ALERT, infoWin);
    werase(infoWin);
    //wattron(infoWin, COLOR_PAIR(CPAIR_ALERT) | A_COLOR);
    box(infoWin, TBLCH_LINE_VERT | infoWin->attrs, TBLCH_LINE_HORZ | infoWin->attrs);

    // THIS IS A KENG
    mvwprintw(infoWin, 2, 2, ERROR_EMPTY_USERNAME);

    mvwprintw(infoWin, 4, 2, "Press Enter to back!");

    curs_set(0);

    while ((key = getkey(0)) == KEY_ENTER)
    {

        clear_screen();
        return;
    }
}

#ifndef DEMO

static int send_aloha()
{
    int rc;
    char *resp;
    size_t siz;

    /* show info */
    info("Connecting to Gateway Server...");

    /* get request parameters */
    snprintf(url_params, sizeof(url_params), "Aloha?pa_no=%s&sn=%s&from=iPXE",
             page_title_info.pa_no, page_title_info.sn);

#ifndef SMBIOS_EFI
    char *mode = "csm";
#else
    char *mode = "uefi";
#endif

    /* send request */
    rc = gateway_request(get_http_params("Aloha",
                                         "pa_no", page_title_info.pa_no,
                                         "sn", page_title_info.sn,
                                         "mac", page_title_info.mac,
                                         "mode", mode,
                                         "from", "iPXE",
                                         NULL),
                         &resp, &siz, NULL);
    if (!rc)
    {
        clearinfo();

        /* has got result */
        resp = strltrim(resp);

        if (!strnicmp(resp, string_ok, strlen(string_ok)))
        {
            /* got OK#xx   xxx# is the prompt string*/
            char *prompt = (char *)calloc(1, 200);
            memset(prompt, '\0', 200);
            char *pos1 = strchr(resp, '#');
            char *pos2 = strchr(pos1 + 1, '#');

            strncpy(prompt, pos1 + 1, pos2 - pos1 - 1);
            if (strlen(prompt) > 0)
            {
                if (!ShowBox7(prompt))
                {

                    return -1;
                };
            }
            clear_screen();
            draw_title();
        }
        else
        {
            /* show error */
            draw_reboot_widget(ERROR_CONNECT_TRIS);
            wait_shutdown();
        }
    }
    else
    {
        clearinfo();
        draw_reboot_widget(ERROR_CONNECT_GW);
        wait_shutdown();
    }

    /* done, clear info */
    clearinfo();
    return 0;
}

#else
static int send_aloha()
{
    return 0;
}
#endif

void check_bios()
{

    int rc;
    char *resp;
    size_t siz;

 #ifndef SMBIOS_EFI
    char *mode = "csm";
#else
    char *mode = "uefi";
#endif

    /* show info */
    info("Checking Bios Version...");

    /* send request */
    rc = gateway_request(get_http_params("checkBios",
                                         "pa_no", resolvedPn,
                                         "sn", resolvedSn,
                                         "mac", page_title_info.mac,
                                         "curBios", page_title_info.bios_version,
                                         "mode", mode,
                                         "from", "iPXE",
                                         NULL),
                         &resp, &siz, NULL);
    if (!rc)
    {
        /* has got result */
        resp = strltrim(resp);
        if (!strnicmp(resp, string_ok2, strlen(string_ok2)))
        {

            /*extract the bios version & upgradeable info*/

            char *upgradeable = (char *)calloc(1, 2);
            char *biosVersion = (char *)calloc(1, 10);
            memset(upgradeable, '\0', 2);
            memset(biosVersion, '\0', 10);

            strncpy(upgradeable, resp + 3, 1);

            if (!strncmp(upgradeable, "1", 1) || !strncmp(upgradeable, "0", 1))
            {

                biosVersion[9] = '\0';
                strncpy(biosVersion, resp + 5, 5);

                newest_bios_version = biosVersion;
                biosUpgradeable = upgradeable;
            }
            DBG("newest bios version is: %s\n", newest_bios_version);
            DBG("upgradeable is: %s\n", biosUpgradeable);
            DBG_PAUSE();
        }
        else
        {
        }
    }
    else
    {
        clearinfo();
        draw_reboot_widget(ERROR_CONNECT_GW);
        wait_shutdown();
    }

    /* done, clear info */
    clearinfo();
}

void check_image()
{
    int rc;
    char *resp;
    size_t siz;

    /* send request */
    rc = gateway_request(get_http_params("getImageType",
                                         "sn", resolvedSn,
                                         NULL),
                         &resp, &siz, NULL);
    if (!rc)
    {
        /* has got result */
        resp = strltrim(resp);
        if (!strnicmp(resp, string_ok2, strlen(string_ok2)))
        {

            /*extract the bios version & upgradeable info*/

            char *imageUse = (char *)calloc(1, 2);
            memset(imageUse, '\0', 2);

            strncpy(imageUse, resp + 3, 1);
            imageType = imageUse;
            DBG("imageType is: %s\n", imageType);
            DBG_PAUSE();
        }
    }
    else
    {
        clearinfo();
        draw_reboot_widget(ERROR_CONNECT_GW);
        wait_shutdown();
    }
}

static bool matchStrIgnoreCase(const char* src, const char* dst) {
    return strlen(src) == strlen(dst) && strnicmp(src, dst, strlen(dst)) == 0;
}

static int user_main_loop()
{
    int rc;
    char *resp;

    size_t resp_siz;

#ifndef SMBIOS_EFI
    color_set(CPAIR_PXE, NULL);
    move(LINES - 1U, COLS - 2);
    wprintw(stdscr, " ");
    color_set(CPAIR_NORMAL, NULL);
#endif

    /* get tester inputs */
    get_inputs();

    /* show info */
    info("Registering...");

    /* send request to gateway */
#ifndef DEMO
    rc = gateway_request(get_http_params("RegisterFromIpxe",
                                         "pa_no", page_title_info.oemString,
                                         "sn", page_title_info.sn,
                                         "mac", page_title_info.mac,
                                         "tester", user_input_struct.user_code,
                                         "isn", user_input_struct.mach_sn,
                                         "prodName", page_title_info.model,
                                         "codeName", page_title_info.model,
                                         "from", "iPXE",
                                         NULL),
                         &resp, &resp_siz, NULL);
#else
    rc = 0;
    resp = "OK:linux,bios,fwtool!6.80";
#endif
    clearinfo();

    if (rc)
    {
        draw_reboot_widget(ERROR_CONNECT_GW);
        wait_shutdown();
        return -1;
    }

    resp = strltrim(resp);
    if (!strnicmp(resp, "Error:Connect TDS", strlen("Error:Connect TDS")))
    {
        draw_reboot_widget(ERROR_CONNECT_TRIS);
        wait_shutdown();
        return -1;
    }

    if (!strnicmp(resp, "Error:Invalid User Account", strlen("Error:Invalid User Account")))
    {
        draw_reboot_widget(ERROR_INVALID_ACCOUNT);
        wait_shutdown();
        return -1;
    }

    if (!strnicmp(resp, "Error:Invalid SN", strlen("Error:Invalid SN")))
    {
        draw_reboot_widget(ERROR_INVALID_SN);
        wait_shutdown();
        return -1;
    }

    if (!strnicmp(resp, "Error:Invalid PN", strlen("Error:Invalid PN")))
    {
        draw_reboot_widget(ERROR_INVALID_PN);
        wait_shutdown();
        return -1;
    }
    //response format: result!#xxx#xxx#xxx#!newest_bios_version
    char *flagPart = (char *)calloc(1, 20);
    char *gedbPn = (char *)calloc(1, 20);
    char *gedbSn = (char *)calloc(1, 20);
    memset(flagPart, '\0', 20);
    memset(gedbPn, '\0', 20);
    memset(resolvedSn, '\0', 20);

    char *pos1 = strchr(resp, '#');
    char *pos2 = strchr(pos1 + 1, '#');
    char *pos3 = strchr(pos2 + 1, '#');
    char *pos4 = strchr(pos3 + 1, '#');

    strncpy(flagPart, pos1 + 1, pos2 - pos1 - 1);
    strncpy(gedbPn, pos2 + 1, pos3 - pos2 - 1);
    strncpy(gedbSn, pos3 + 1, pos4 - pos3 - 1);

    resolvedPn = gedbPn;
    resolvedSn = gedbSn;

    DBG("flag part is: %s\n", flagPart);
    DBG("gedbPn part is: %s\n", gedbPn);
    DBG("resolvedSn is: %s\n", resolvedSn);

    bool isRegOk = !strnicmp(resp, string_ok, strlen(string_ok));
    bool isNoGedb = !strncmp(flagPart, "4", 1);
    bool isNoSling = !strncmp(flagPart, "5", 1);

    if (!isRegOk) 
    {
        if (isNoGedb) 
        {
            if (!ShowBox6(ERROR_NO_GEDB_OR_NOT_SUPPORT))
                return -1;
        } 
        else if (isNoSling) 
        {
            if (!ShowBox6(ERROR_NO_SLING_OR_NOT_SUPPORT))
                return -1;
        }
        else
        {
            if (!ShowBox6(ERROR_NOT_SUPPORT))
                return -1;
        }
    }
    else if (isNoGedb) 
    {
        if (!ShowBox6(ERROR_NO_GEDB))
            return -1;
    }
    else if (isNoSling)
    {
        if (!ShowBox6(ERROR_NO_SLING))
            return -1;
    }

    if (!strncmp(flagPart, "3", 1))
    {
        //Can not connect GEDB
        if (!ShowBox6(ERROR_CONNECT_GEDB))
        {
            return -1;
        }
    }
    
    //not TARGET machine
    if (!matchStrIgnoreCase(page_title_info.manufacturer, "TOSHIBA") 
    && !matchStrIgnoreCase(page_title_info.manufacturer, "Dynabook")
    && !matchStrIgnoreCase(page_title_info.manufacturer, "Dynabook Inc")
    && !matchStrIgnoreCase(page_title_info.manufacturer, "Dynabook Inc.")
    ){
        if (!ShowBox6(ERROR_NOT_TARGET))
        {
            return -1;
        }
    }

    if (isRegOk) 
    {
        /* has registered */
        if ((resp = strchr(resp, ':')))
        {
            do
            {
                resp = strltrim(resp + 1);
                if (!strnicmp(resp, string_linux, strlen(string_linux)))
                    ipxe_status.supported |= SKU_SUPPORT_LINUX;
                else if (!strnicmp(resp, string_bios, strlen(string_bios)))
                    ipxe_status.supported |= SKU_SUPPORT_BIOS;
                else if (!strnicmp(resp, string_fwtool, strlen(string_fwtool)))
                    ipxe_status.supported |= SKU_SUPPORT_FW;
            } while ((resp = strchr(resp, ',')));
        }
        else
        {
            /* default to support all features */
            ipxe_status.supported |= SKU_SUPPORT_ALL;
        }
    }
    return rc;
}

/*
 * initial screen, and input user-code & sn
 */
int user_ui()
{
    /* initial screen */
    draw_title();
    /* clear screen */
    clear_screen();
    /* send "aloha" */
    if (send_aloha() == -1)
    {
        return -1;
    }
    /* check pressed key(s) */
    check_keyboard();

    /* add by Gavin */
    if (auto_profile)
    {
        return 0;
    }

    /* user code & serial number */
    return user_main_loop();
}

/* EOF */
