/*
 * trispg.c - additional tris pages
 *
 * Creation: 2014-12-21 shenweizhou
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <curses.h>
#include <ctype.h>
#include <ipxe/ansicol.h>
#include <ipxe/smbios.h>
#include <ipxe/parseopt.h>
#include <ipxe/version.h>
#include <ipxe/iobuf.h>
#include <ipxe/xfer.h>
#include <ipxe/open.h>
#include <ipxe/umalloc.h>
#include <ipxe/process.h>
#include <ipxe/tcpip.h>
#include <ipxe/netdevice.h>
#include <ipxe/uri.h>
#include <hci/ifmgmt_cmd.h>
#include <ipxe/sanboot.h>
#include <ipxe/init.h>
#include <ipxe/version.h>
#include <usr/autoboot.h>

#include <ipxe/pci_ids.h>

#include <tris/tris_error.h>
#include <tris/trispg.h>
#include <tris/tris_profile.h>
#include <tris/tris_usr.h>

//#define PCI_VENDOR_ID_NVIDIA			0x10de
//#define PCI_VENDOR_ID_ATI		0x1002

#undef ERRFILE
#define ERRFILE ERRFILE_open

#define ISCSI_URI_DOS_TMDT "iqn.2014-09.com.toshiba:tris-TMDT"
#define ISCSI_URI_DOS_HITACHI "iqn.2018-02.com.toshiba:tris-HITACHI"
char *iscsi_uri_dos;

/* for test */
static char _gateway_addr[128] = {0};
//static const char _gateway_addr[] = "http://172.17.11.36/GW_server/";
//static  char _gateway_addr[] = "http://10.0.2.2:9090/GW_server/";

/* put widget to center according to model name */
int CONSOLE_OFFSET_X = 0;
int CONSOLE_OFFSET_Y = 0;

const char string_ok[] = "OK";
const char string_ok2[] = "OK:";
const char string_err[] = "Error";
const char string_ongoing[] = "OnGoing";
const char string_linux[] = "Linux";
const char string_dos[] = "DOS";
const char string_bios[] = "bios";
const char string_fwtool[] = "fwtool";

/* ipxe status info */
struct ipxe_status_struct ipxe_status;

/* helper */
char *strltrim(char *str)
{
    while (isspace(*str))
    {
        str++;
    }

    return str;
}

/**
 * Print message centred on specified row
 *
 * @v row		Row
 * @v fmt		printf() format string
 * @v args		printf() argument list
 */
//static void vmsg ( unsigned int row, const char *fmt, va_list args ) {
//	char buf[COLS];
//	size_t len;
//
//	len = vsnprintf ( buf, sizeof ( buf ), fmt, args );
//	mvprintw ( row, ( ( COLS - len ) / 2 ), "%s", buf );
//}

/**
 * Print message centred on specified row
 *
 * @v row		Row
 * @v fmt		printf() format string
 * @v ..		printf() arguments
 */
//static void msg ( unsigned int row, const char *fmt, ... ) {
//	va_list args;
//
//	va_start ( args, fmt );
//	vmsg ( row, fmt, args );
//	va_end ( args );
//}

/**
 * Clear message on specified row
 *
 * @v row		Row
 */
static inline void clearmsg(unsigned int row)
{
    move(row, 0);
    clrtoeol();
}

/*
 * Print message from left to right
 */
static void vmsgln(unsigned int row, const char *fmt, va_list args)
{
    char buf[COLS];

    vsnprintf(buf, sizeof(buf), fmt, args);
    mvprintw(row, 0, "%s", buf);
}
    //static void msgln(unsigned int row, const char *fmt, ...) {
    //	va_list args;
    //
    //	va_start ( args, fmt );
    //	vmsgln ( row, fmt, args );
    //	va_end ( args );
    //}

#ifndef INFO_ROW
#define INFO_ROW (LINES - 1U)
#endif

void clearinfo()
{
    //	clearmsg(INFO_ROW);

    char buf[COLS];

    memset(buf, ' ', sizeof(buf));
    buf[0] = '\b';
    buf[COLS - 1] = 0;

    color_set(CPAIR_PXE, NULL);
    move(INFO_ROW, 1);
    wprintw(stdscr, buf);
    color_set(CPAIR_NORMAL, NULL);
}

static void vinfo(const char *fmt, va_list args)
{
    clearinfo();

    color_set(CPAIR_PXE, NULL);
    vmsgln(INFO_ROW, fmt, args);
    color_set(CPAIR_NORMAL, NULL);
}

void info(const char *fmt, ...)
{
    va_list va;

    va_start(va, fmt);
    vinfo(fmt, va);
    va_end(va);
}

/*
 * read bios information
 */
struct page_title_info_struct page_title_info;
char url_params[2048];

char *get_http_params(const char *name, ...)
{
    va_list va;
    char *key;
    size_t s, siz = sizeof(url_params);
    char *p = url_params;

    /* copy servlet name */
    s = strlen(name);
    memcpy(p, name, s);
    p += s, siz -= s + 1;
    *p++ = '?';

    /* loop all parameters */
    va_start(va, name);
    while ((key = va_arg(va, char *)))
    {
        /* need '&' ? */
        if (p[-1] != '?')
        {
            *p++ = '&';
            siz--;
        }

        /* encode the key */
        s = uri_encode_string(0, key, p, siz);
        p += s, siz -= s;

        /* add the "=" */
        *p++ = '=';
        siz--;

        /* encode the value */
        s = uri_encode_string(0, va_arg(va, char *), p, siz);
        p += s, siz -= s;
    };
    va_end(va);

    /* has done */
    *p = 0;

    return url_params;
}

static void SplitLongSn()
{
    char sn[9];

    if (strlen(page_title_info.sn) == 24)
    {
        /* 24 bits long SN type */
        char *pSN = &(page_title_info.sn[1]);
        strncpy(sn, pSN, 9);
        memset(page_title_info.sn, 0, sizeof(page_title_info.sn));
        strncpy(page_title_info.sn, sn, 9);
    }
}

static int read_setting_info(char *name, char *value)
{
    struct named_setting setting;
    char *q;
    int rc;

    /* parse setting name */
    if (!(rc = parse_existing_setting(name, &setting)))
    {
        /* Fetch formatted setting value */
        if ((rc = fetchf_setting_copy(setting.settings, &setting.setting,
                                      NULL, NULL, &q)) >= 0)
        {
            /* has got */
            strcpy(value, q); /* copy result */
            free(q);
            rc = 0;
        }
    }

    /* done */
    return rc;
}

#define SMBIOS_TYPE_MANUFACTURER SMBIOS_TYPE_SYSTEM_INFORMATION
#define SMBIOS_INSTANCE_MANUFACTURER 0
#define SMBIOS_INDEX_MANUFACTURER 1

#define SMBIOS_TYPE_MODEL SMBIOS_TYPE_SYSTEM_INFORMATION
#define SMBIOS_INSTANCE_MODEL 0
#define SMBIOS_INDEX_MODEL 2

#define SMBIOS_TYPE_SN SMBIOS_TYPE_SYSTEM_INFORMATION
#define SMBIOS_INSTANCE_SN 0
#define SMBIOS_INDEX_SN 4

#define SMBIOS_TYPE_PA_NO SMBIOS_TYPE_SYSTEM_INFORMATION
#define SMBIOS_INSTANCE_PA_NO 0
#define SMBIOS_INDEX_PA_NO 3

#define SMBIOS_TYPE_VERSION 0
#define SMBIOS_INSTANCE_VERSION 0
#define SMBIOS_INDEX_VERSION 2

static int read_smbios_info(unsigned int type, unsigned int instance,
                            int index, char *value)
{
    int ret;
    struct smbios_structure structure;

    if (!(ret = find_smbios_structure(type, instance, &structure)))
    {
        uint8_t buf[structure.header.len];
        if (!(ret = read_smbios_structure(&structure, buf, sizeof(buf))))
            ret = !read_smbios_string(&structure, index, value, COLS);
    }

    return ret;
}

static void init_title_info()
{
    /* clear all datas */
    memset(&page_title_info, 0, sizeof(page_title_info));

    /* product name */
#ifdef SMBIOS_PCBIOS
    strcpy(page_title_info.prod_name, "Dynabook Diagnostic System 1.3.0-PCBIOS (C) 2019 Dynabook Inc.");
#else
#ifdef SMBIOS_EFI
    strcpy(page_title_info.prod_name, "Dynabook Diagnostic System 1.3.0-UEFI (C) 2019 Dynabook Inc.");
#else
    strcpy(page_title_info.prod_name, "Dynabook Diagnostic System 1.3.0 (C) 2019 Dynabook Inc.");
#endif
#endif
    /* iPXE name */
    strcpy(page_title_info.ipxe_name, "iPXE ");
    if (read_setting_info("builtin/version:string", page_title_info.ipxe_name + strlen(page_title_info.ipxe_name)))
        page_title_info.ipxe_name[0] = 0;
    //cut ipxe name
    {
        char* quote = strchr(page_title_info.ipxe_name, '(');
        if (quote) {
            quote--;
            *quote = 0;
        }
    }

    /* product url */
    strcpy(page_title_info.prod_url, "(https://www.dynabook.com)");
    /* iPXE url */
    strcpy(page_title_info.ipxe_url, "(http://ipxe.org)");
}

static void read_title_info()
{
    /* get gateway address */
    if (read_setting_info("ip:ipv4", page_title_info.ip_addr))
        //  if (read_setting_info("ip:ipv4", "10.0.2.2"))
        page_title_info.ip_addr[0] = 0;
    if (read_setting_info("dhcp-server:ipv4", page_title_info.gateway_server))
        page_title_info.gateway_server[0] = 0;

    /* manufacturer */
    if (read_smbios_info(SMBIOS_TYPE_MANUFACTURER, SMBIOS_INSTANCE_MANUFACTURER,
                         SMBIOS_INDEX_MANUFACTURER, page_title_info.manufacturer))
    {
        page_title_info.manufacturer[0] = 0;
    }

// 	else {
// 		DBG("manufacturer >>>> %s\n",  page_title_info.manufacturer);
// 		DBG_PAUSE();
// 	}

        /* machine model */
#ifdef SMBIOS_TYPE_MODEL
    if (read_smbios_info(SMBIOS_TYPE_MODEL, SMBIOS_INSTANCE_MODEL,
                         SMBIOS_INDEX_MODEL, page_title_info.model))

#else
    if (read_setting_info("smbios/product:string", page_title_info.model))
#endif
        page_title_info.model[0] = 0;
    /* pa no */

    if (read_smbios_info(SMBIOS_TYPE_PA_NO, SMBIOS_INSTANCE_PA_NO,
                         SMBIOS_INDEX_PA_NO, page_title_info.pa_no))
        page_title_info.pa_no[0] = 0;

    if (read_smbios_info(11, 0,
                         1, page_title_info.oemString))
        page_title_info.oemString[0] = 0;
        /* sn */
#ifdef SMBIOS_TYPE_SN
    if (read_smbios_info(SMBIOS_TYPE_SN, SMBIOS_INSTANCE_SN,
                         SMBIOS_INDEX_SN, page_title_info.sn))
#else
    if (read_setting_info("smbios/serial:string", page_title_info.sn))
#endif
        page_title_info.sn[0] = 0;
    SplitLongSn();

    /* mac */
    if (read_setting_info("net0/mac:hex", page_title_info.mac))
        page_title_info.mac[0] = 0;

    /* HDDs */
    page_title_info.HDDs = 1;
    /* BIOS version */
    if (read_smbios_info(SMBIOS_TYPE_VERSION, SMBIOS_INSTANCE_VERSION,
                         SMBIOS_INDEX_VERSION, page_title_info.bios_version))
        page_title_info.bios_version[0] = 0;

    /* has done */
}

#if 0
/**
 * Print alert message
 *
 * @v fmt		printf() format string
 * @v args		printf() argument list
 */
static void valert ( const char *fmt, va_list args ) {
    clearmsg ( ALERT_ROW );
    color_set ( CPAIR_ALERT, NULL );
    vmsg ( ALERT_ROW, fmt, args );
    sleep ( 2 );
    color_set ( CPAIR_NORMAL, NULL );
    clearmsg ( ALERT_ROW );
}

/**
 * Print alert message
 *
 * @v fmt		printf() format string
 * @v ...		printf() arguments
 */
static void alert ( const char *fmt, ... ) {
    va_list args;

    va_start ( args, fmt );
    valert ( fmt, args );
    va_end ( args );
}
#endif

static void draw_msg_widgetv(const char *title, int rows, va_list va)
{
    int i, q;
    size_t siz, s;
    char *txt[LINES];    /* text buffer */
    char line[COLS + 1]; /* text line buffer */
    unsigned int row1, col1, row2, col2;

    /* get parameters & calc max length */
    siz = 0;
    for (i = 0; i < rows; i++)
    {
        txt[i] = va_arg(va, char *);
        s = strlen(txt[i]);
        if (s > siz)
            siz = s;
    }

    /* get message box position */
    row1 = (LINES - rows - 4) / 2;
    if (row1 <= TITLE_ROW_COUNT)
        row1 = TITLE_ROW_COUNT + 1;
    row2 = row1 + rows + 3;
    if (row2 >= LINES)
        row2 = LINES;
    col1 = (COLS - siz) / 2 - 3;
    if (col1 < 1)
        col1 = 1;
    col2 = col1 + 3 + siz + 2;
    if (col2 >= COLS)
        col2 = COLS;

    /* draw title line */
    /* 1st row */
    memset(line, TBLCH_LINE_HORZ, COLS);
    line[0] = TBLCH_LEFT_UPPER;
    memcpy(&line[1], title, strlen(title));
    q = col2 - col1 - 1;
    line[q++] = TBLCH_RIGHT_UPPER;
    line[q] = 0;
    move(row1 + CONSOLE_OFFSET_Y, col1 + CONSOLE_OFFSET_X);
    wprintw(stdscr, " ");
    color_set(CPAIR_ALERT, NULL);
    wprintw(stdscr, line);
    /* 2nd row */
    memset(line, ' ', COLS);
    line[0] = TBLCH_LINE_VERT;
    q = col2 - col1 - 1;
    line[q++] = TBLCH_LINE_VERT;
    line[q] = 0;
    move(row1 + 1 + CONSOLE_OFFSET_Y, col1 + CONSOLE_OFFSET_X);
    color_set(CPAIR_DEFAULT, NULL);
    wprintw(stdscr, " ");
    color_set(CPAIR_ALERT, NULL);
    wprintw(stdscr, line);
    /* 3rd row */
    for (i = 0; i < rows; i++)
    {
        memset(line, ' ', COLS);
        line[0] = TBLCH_LINE_VERT;
        memcpy(&line[2], txt[i], strlen(txt[i]));
        q = col2 - col1 - 1;
        line[q++] = TBLCH_LINE_VERT;
        line[q] = 0;
        move(row1 + 2 + i + CONSOLE_OFFSET_Y, col1 + CONSOLE_OFFSET_X);
        color_set(CPAIR_DEFAULT, NULL);
        wprintw(stdscr, " ");
        color_set(CPAIR_ALERT, NULL);
        wprintw(stdscr, line);
    }
    /* 4th row */
    memset(line, ' ', COLS);
    line[0] = TBLCH_LINE_VERT;
    q = col2 - col1 - 1;
    line[q++] = TBLCH_LINE_VERT;
    line[q] = 0;
    move(row1 + 2 + rows + CONSOLE_OFFSET_Y, col1 + CONSOLE_OFFSET_X);
    color_set(CPAIR_DEFAULT, NULL);
    wprintw(stdscr, " ");
    color_set(CPAIR_ALERT, NULL);
    wprintw(stdscr, line);
    /* 5th row */
    memset(line, TBLCH_LINE_HORZ, COLS);
    line[0] = TBLCH_LEFT_LOWER;
    q = col2 - col1 - 1;
    line[q++] = TBLCH_RIGHT_LOWER;
    line[q] = 0;
    move(row1 + 3 + rows + CONSOLE_OFFSET_Y, col1 + CONSOLE_OFFSET_X);
    color_set(CPAIR_DEFAULT, NULL);
    wprintw(stdscr, " ");
    color_set(CPAIR_ALERT, NULL);
    wprintw(stdscr, line);
    /* 6th row */
    memset(line, ' ', COLS);
    line[col2 - col1 - 1] = 0;
    move(row1 + 4 + rows + CONSOLE_OFFSET_Y, col1 + CONSOLE_OFFSET_X);
    color_set(CPAIR_DEFAULT, NULL);
    wprintw(stdscr, line);

    /* restore */
    color_set(CPAIR_NORMAL, NULL);
}

void draw_msg_widget(const char *title, int rows, ...)
{
    va_list va;

    va_start(va, rows);
    draw_msg_widgetv(title, rows, va);
    va_end(va);
}

void draw_reboot_widget(const char *error)
{
    draw_msg_widget("Error", 3,
                    error,
                    "You cannot proceed.",
                    "Press Ctrl+Alt+Del to reboot.");
}

void draw_title()
{
    int i;
    size_t len;

    /* clear title rows */
    for (i = 0; i < TITLE_ROW_COUNT; i++)
        clearmsg(i);

    /* 1st row */
    clearmsg(TITLE_ROW_VERSION);
    mvprintw(TITLE_ROW_VERSION, TITLE_LEFT_MARGIN, page_title_info.prod_name);
    len = strlen(page_title_info.ipxe_name);
    mvprintw(TITLE_ROW_VERSION, COLS - TITLE_RIGHT_MARGIN - len, page_title_info.ipxe_name);

    /* 2nd row */
    clearmsg(TITLE_ROW_URL);
    mvprintw(TITLE_ROW_URL, TITLE_LEFT_MARGIN, page_title_info.prod_url);
    len = strlen(page_title_info.ipxe_url);
    mvprintw(TITLE_ROW_URL, COLS - TITLE_RIGHT_MARGIN - len, page_title_info.ipxe_url);

    /* 3rd row */
    clearmsg(TITLE_ROW_MODEL);
    mvprintw(TITLE_ROW_MODEL, TITLE_LEFT_MARGIN, "Model: %s", page_title_info.model);
    len = strlen(page_title_info.pa_no);
    mvprintw(TITLE_ROW_MODEL, COLS - TITLE_RIGHT_MARGIN - len - 7, "PA No: %s", page_title_info.pa_no);

    /* 4th row */
    clearmsg(TITLE_ROW_SN);
    mvprintw(TITLE_ROW_SN, TITLE_LEFT_MARGIN, "S/N: %s", page_title_info.sn);
    len = strlen(page_title_info.mac);
    mvprintw(TITLE_ROW_SN, COLS - TITLE_RIGHT_MARGIN - len - 5, "MAC: %s", page_title_info.mac);

    /* 5th row */
    clearmsg(TITLE_ROW_BIOS);
    mvprintw(TITLE_ROW_BIOS, TITLE_LEFT_MARGIN, "BIOS HDDs: %d", page_title_info.HDDs);
    len = strlen(page_title_info.bios_version);
    mvprintw(TITLE_ROW_BIOS, COLS - TITLE_RIGHT_MARGIN - len - 14, "BIOS Version: %s", page_title_info.bios_version);

    /* redraw bios info after checking bios version*/
    if (strlen(newest_bios_version) > 0)
    {

        if (!strncmp(biosUpgradeable, "1", 1))
        {

            move(TITLE_ROW_BIOS, 0);
            clrtoeol();
            mvprintw(TITLE_ROW_BIOS, TITLE_LEFT_MARGIN, "BIOS HDDs: %d", page_title_info.HDDs);
            int len = strlen(page_title_info.bios_version);
            mvprintw(TITLE_ROW_BIOS, COLS - TITLE_RIGHT_MARGIN - len - 14, "BIOS Version:", "");
            color_set(CPAIR_STRIKING, NULL);
            mvprintw(TITLE_ROW_BIOS, COLS - TITLE_RIGHT_MARGIN - len, "%s", page_title_info.bios_version);
            DBG_PAUSE();
        }
    }
}

void clear_screen()
{
    unsigned int row = TITLE_ROW_COUNT;

    for (; row < LINES - 1; row++)
        clearmsg(row);
    clearinfo();
}

void wait_shutdown()
{
    for (;;)
    {
        getkey(TICKS_PER_SEC);
    }
}

/*
 * Gateway Request
 */
struct gateway_connection
{
    /** Data transfer interface */
    struct interface xfer;
    /** Data buffer */
    userptr_t buffer;
    /** Size of data buffer */
    size_t size;
    /** Starting offset of data buffer */
    size_t start;
    /** Data position */
    size_t offset;
    /** Block size */
    size_t blksize;
    /** Overall return status code */
    int rc;
};
enum
{
    GATEWAY_REQUEST_INIT_BUFSIZE = 4096,
    GATEWAY_REQUEST_BLKSIZE = 4096
};

/*
 * Check flow control window
 */
static size_t gateway_request_xfer_window(struct gateway_connection *gw_req)
{
    return gw_req->blksize;
}

/*
 * close gateway request connection
 */
static void gateway_request_xfer_close(struct gateway_connection *gw_req, int rc)
{
    intf_shutdown(&gw_req->xfer, rc);
    gw_req->rc = rc;
}

/*
 * Receiver new data
 */
static int gateway_request_xfer_deliver(struct gateway_connection *gw_req,
                                        struct io_buffer *iobuf, struct xfer_metadata *meta)
{
    size_t len = iob_len(iobuf);
    int rc = 0;

    /* Calculate new buffer position */
    if (meta->flags & XFER_FL_ABS_OFFSET)
        gw_req->offset = 0;
    gw_req->offset += meta->offset;

    /* copy data block to buffer */
    if (len == 0)
    {
        /* No data (pure seek); treat as success */
    }
    else if (gw_req->offset < gw_req->start)
    {
        DBG("Gateway_Request: buffer underrun at %zx (min %zx)",
            gw_req->offset, gw_req->start);
        rc = -ENOBUFS;
    }
    else if (gw_req->offset + len + 1 > gw_req->start + gw_req->size)
    {
        /* buffer overrun, re-allocate */
        userptr_t p = gw_req->buffer;
        size_t siz = gw_req->size;
        do
        {
            siz += GATEWAY_REQUEST_INIT_BUFSIZE;
        } while (gw_req->start + siz < gw_req->offset + len + 1);
        if (!(gw_req->buffer = urealloc(gw_req->buffer, siz)))
        {
            gw_req->buffer = p;
            rc = -ENOMEM;
        }
        else
        {
            gw_req->size = siz;
        }
    }

    /* copy data */
    if (!rc)
    {
        copy_to_user(gw_req->buffer, gw_req->offset - gw_req->start,
                     iobuf->data, len);

        /* Calculate new buffer position */
        gw_req->offset += len;

        *((char *)gw_req->buffer + gw_req->offset - gw_req->start) = 0;
    }
    else
    {
        /* terminate transfer on error */
        gateway_request_xfer_close(gw_req, rc);
    }

    free_iob(iobuf);
    return rc;
}

static struct interface_operation gateway_request_xfer_ops[] = {
    INTF_OP(xfer_deliver, struct gateway_connection *,
            gateway_request_xfer_deliver),
    INTF_OP(xfer_window, struct gateway_connection *,
            gateway_request_xfer_window),
    INTF_OP(intf_close, struct gateway_connection *,
            gateway_request_xfer_close),
};

static struct interface_descriptor gateway_request_xfer_desc =
    INTF_DESC(
        struct gateway_connection, xfer, gateway_request_xfer_ops);

static struct gateway_connection gateway_request_connection = {
    .xfer = INTF_INIT(gateway_request_xfer_desc),
};

static int gateway_request_init()
{
    /* save old buffer setting */
    size_t siz = gateway_request_connection.size;
    userptr_t buf = gateway_request_connection.buffer;

    /* reset gateway connection structure */
    memset(&gateway_request_connection, 0, sizeof(gateway_request_connection));
    intf_init(&gateway_request_connection.xfer, &gateway_request_xfer_desc, NULL);
    gateway_request_connection.blksize = GATEWAY_REQUEST_BLKSIZE;
    gateway_request_connection.rc = -EINPROGRESS;

    /* allocate user memory */
    if (!buf)
    {
        gateway_request_connection.buffer = umalloc(GATEWAY_REQUEST_INIT_BUFSIZE);
        gateway_request_connection.size = GATEWAY_REQUEST_INIT_BUFSIZE;
    }
    else
    {
        gateway_request_connection.buffer = buf;
        gateway_request_connection.size = siz;
    }

    return gateway_request_connection.buffer ? 0 : -ENOMEM;
}

#define REQUEST_TIMEOUT (70 * TICKS_PER_SEC) //30s

int gateway_request(const char *req,
                    char **buf, size_t *siz, void (*notify)())
{
    int rc;
    char uri_string[1024];
    unsigned long start = currticks();

#ifndef SMBIOS_EFI
    static unsigned int cnt = 0;
    static unsigned int flag = 0;
#endif

    /* initial gateway request */
    if (!(rc = gateway_request_init()))
    {
        /* construct URI string */
        snprintf(uri_string, sizeof(uri_string), "%s%s", _gateway_addr, req);

        /* open request */
        rc = xfer_open_uri_string(&gateway_request_connection.xfer, uri_string);
        if (!rc)
        {
            /* wait for response */
            while ((rc = gateway_request_connection.rc) == -EINPROGRESS)
            {
#ifndef SMBIOS_EFI
                cnt++;
                if ((cnt % 100) == 0)
                {
                    switch (flag % 4)
                    {
                    case 0:
                        mvprintw(LINES - 1U, COLS - 2U, "-");
                        break;
                    case 1:
                        mvprintw(LINES - 1U, COLS - 2U, "\\");
                        break;
                    case 2:
                        mvprintw(LINES - 1U, COLS - 2U, "|");
                        break;
                    case 3:
                        mvprintw(LINES - 1U, COLS - 2U, "/");
                        break;
                    }
                    flag++;
                }
#endif

                if (notify)
                    notify();
                step();
                if (currticks() - start > REQUEST_TIMEOUT)
                {
                    break;
                }
            };
        }
    }

    /* has done */
    if (buf)
        *buf = rc ? NULL : (char *)gateway_request_connection.buffer;
    if (siz)
        *siz = gateway_request_connection.offset - gateway_request_connection.start;
    return rc;
}

/*
 * tris_boot - boot tris test console image
 */
extern int imgselect_exec(int argc, char **argv);

extern int imgexec_exec(int argc, char **argv);

extern int imgfetch_exec(int argc, char **argv);

extern uint16_t g_video_card_type;

#ifdef SMBIOS_EFI
static void startUefiTMDT() {
    //prepare image
    char env[200];
    int rc;
    char *resp;
    size_t siz;

    rc = gateway_request(get_http_params("tmdtUefi/prepareInputImg",
                                         "pn", resolvedPn,
                                         "sn", resolvedSn,
                                         NULL),
                         &resp, &siz, NULL);
    if (rc) {
        draw_reboot_widget(ERROR_CONNECT_GW);
        wait_shutdown();
        return;
    }

    snprintf(env, sizeof(env), "set inputImage %s", resp);
    system(env);
    system("chain tftp://192.168.100.10/tmdt.ipxe");
    system("reboot");
}
static void startApmc() {
    //prepare image
    char env[200];
    int rc;
    char *resp;
    size_t siz;

    rc = gateway_request(get_http_params("Apmc/prepareInputImg",
                                         "pn", resolvedPn,
                                         "sn", resolvedSn,
                                         NULL),
                         &resp, &siz, NULL);
    if (rc) {
        draw_reboot_widget(ERROR_CONNECT_GW);
        wait_shutdown();
        return;
    }

    snprintf(env, sizeof(env), "set inputImage %s", resp);
    system(env);
    system("chain tftp://192.168.100.10/AutoPilot_Marker_Clear");
    system("reboot");
}
static void startUefiMemtest(char* uri, size_t size) {
    //prepare image
    int rc;
    char *resp;
    size_t siz;

    rc = gateway_request(get_http_params("memtestUefi/prepareInputImg",
                                         "pn", resolvedPn,
                                         "sn", resolvedSn,
                                         NULL),
                         &resp, &siz, NULL);
    if (rc) {
        draw_reboot_widget(ERROR_CONNECT_GW);
        wait_shutdown();
        return;
    }

    snprintf(uri, size, "%s", resp);
}
#endif

static char* targetToAddr(char* target) {
    int rc;
    char *resp;
    size_t siz;

    rc = gateway_request(get_http_params("iscsi/target",
                                         "pa", resolvedPn,
                                         "sn", resolvedSn,
                                         "target", target,
                                         NULL),
                         &resp, &siz, NULL);
    if (rc) {
        draw_reboot_widget(ERROR_CONNECT_GW);
        wait_shutdown();
        return NULL;
    }

    return resp;
}

static int image_boot()
{
    char uri[128];
    char iscsi_ip[64];
    char iscsi_node[128];
    char pn[60];
    char sn[60];
    char mac[128];
    struct profile_struct *profile;
    struct uri *root_paths[1];

    if (!auto_profile && current_profile_head == NULL)
    {
        return 0;
    }
    profile = auto_profile ? auto_profile : &current_profile_head->profiles[current_profile_head->current];

    if (profile == NULL)
    {
        DBG("profile is null!!!\n");
        DBG_PAUSE();
        return 0;
    }

    switch (profile->os)
    {
    case PROFILE_OS_LINUX:
    {
        DBG("LINUX OS SELECTED");
        DBG_PAUSE();
        char *argv[] = {uri, iscsi_ip, iscsi_node, pn, sn, mac, NULL};
        char *argv_v3[] = {uri, iscsi_ip, iscsi_node, pn, sn, mac, "i8042.nomux=1","psmouse.proto=bare", "psmouse.synaptics_intertouch=1", NULL};

        snprintf(pn, sizeof(pn), "pn=%s", resolvedPn);
        snprintf(sn, sizeof(sn), "sn=%s", resolvedSn);
        snprintf(mac, sizeof(mac), "BOOTIF=%s", page_title_info.mac);
        if (!strncmp(imageType, "0", 1))
        {
            snprintf(uri, sizeof(uri), "http://%s/download/bzImage", page_title_info.gateway_server);
        }
        else
        {
            snprintf(uri, sizeof(uri), "http://%s/download/bzImage.v%d", page_title_info.gateway_server, atoi(imageType) + 1);
        }

        snprintf(iscsi_ip, sizeof(iscsi_ip), "ISCSI_TARGET_IP=%s", page_title_info.gateway_server);

        switch (g_video_card_type)
        {
        case PCI_VENDOR_ID_ATI:
            DBG("video card - ATI");
            DBG_PAUSE();
            snprintf(iscsi_node, sizeof(iscsi_node),
                     "ISCSI_TARGET_NODE=iqn.2014-09.com.toshiba:tris-server-ati");
            //			system("set user-class ATI");
            break;
        case PCI_VENDOR_ID_NVIDIA:
            DBG("video card - NVIDIA");
            DBG_PAUSE();
            //			system("set user-class NVIDIA");

            snprintf(iscsi_node, sizeof(iscsi_node),
                     "ISCSI_TARGET_NODE=iqn.2014-09.com.toshiba:tris-server-nvidia");

            break;
        default:
            DBG("video card - others");
            DBG_PAUSE();
            snprintf(iscsi_node, sizeof(iscsi_node), "ISCSI_TARGET_NODE=iqn.2014-09.com.toshiba:tris-server");

            break;
        }

        if (!strncmp(imageType, "2", 1)) {
            imgselect_exec(sizeof(argv_v3) / sizeof(argv_v3[0]) - 1, argv_v3);
        } else {
            imgselect_exec(sizeof(argv) / sizeof(argv[0]) - 1, argv);
        }
        DBG("boot image downloaded: %s\n", uri);
        DBG("LINUX image: %s", iscsi_node);
        DBG_PAUSE();
        char *argv2[] = {NULL};
        return imgexec_exec(sizeof(argv2) / sizeof(argv2[0]) - 1, argv2);
    }
    case PROFILE_OS_LINUX_BASIC:
    {
        char *argv[] = {uri, iscsi_ip, iscsi_node, pn, sn, NULL};

        snprintf(pn, sizeof(pn), "pn=%s", resolvedPn);
        snprintf(sn, sizeof(sn), "sn=%s", resolvedSn);

        snprintf(uri, sizeof(uri), "http://%s/bzImage", page_title_info.gateway_server);
        snprintf(iscsi_ip, sizeof(iscsi_ip), "ISCSI_TARGET_IP=%s", page_title_info.gateway_server);
        snprintf(iscsi_node, sizeof(iscsi_node), "ISCSI_TARGET_NODE=iqn.2014-09.com.toshiba:tris-server");
        DBG("IMAGE_TYPE_LINUX_BASIC  %s is loaded", uri);
        imgselect_exec(sizeof(argv) / sizeof( argv[0]) - 1, argv);
        char *argv2[] = {NULL};
        return imgexec_exec(sizeof(argv2) / sizeof(argv2[0]) - 1, argv2);
    }
    case PROFILE_OS_LINUX_ATI:
    {
        char *argv[] = {uri, iscsi_ip, iscsi_node, pn, sn, NULL};

        snprintf(pn, sizeof(pn), "pn=%s", resolvedPn);
        snprintf(sn, sizeof(sn), "sn=%s", resolvedSn);
        snprintf(uri, sizeof(uri), "http://%s/bzImage", page_title_info.gateway_server);
        snprintf(iscsi_ip, sizeof(iscsi_ip), "ISCSI_TARGET_IP=%s", page_title_info.gateway_server);

        snprintf(iscsi_node, sizeof(iscsi_node), "ISCSI_TARGET_NODE=iqn.2014-09.com.toshiba:tris-server-ati");
        DBG("IMAGE_TYPE_LINUX_BASIC  %s is loaded", uri);
        imgselect_exec(sizeof(argv) / sizeof(argv[0]) - 1, argv);
        char *argv2[] = {NULL};
        return imgexec_exec(sizeof(argv2) / sizeof(argv2[0]) - 1, argv2);
    }
    case PROFILE_OS_LINUX_NVIDIA:
    {
        char *argv[] = {uri, iscsi_ip, iscsi_node, pn, sn, NULL};

        snprintf(pn, sizeof(pn), "pn=%s", resolvedPn);
        snprintf(sn, sizeof(sn), "sn=%s", resolvedSn);

        snprintf(uri, sizeof(uri), "http://%s/bzImage", page_title_info.gateway_server);
        snprintf(iscsi_ip, sizeof(iscsi_ip), "ISCSI_TARGET_IP=%s", page_title_info.gateway_server);

        snprintf(iscsi_node, sizeof(iscsi_node), "ISCSI_TARGET_NODE=iqn.2014-09.com.toshiba:tris-server-nvidia");
        DBG("IMAGE_TYPE_LINUX_BASIC  %s is loaded", uri);
        imgselect_exec(sizeof(argv) / sizeof(argv[0]) - 1, argv);
        char *argv2[] = {NULL};
        return imgexec_exec(sizeof(argv2) / sizeof(argv2[0]) - 1, argv2);
    }
    case PROFILE_OS_MEMTEST_SINGLE:
        system("chain tftp://192.168.100.10/memtestScript_single");
        system("reboot");
        break;
    case PROFILE_OS_MEMTEST_CONTINUE:
        system("chain tftp://192.168.100.10/memtestScript_continue");
        system("reboot");
        break;
    case PROFILE_OS_MEMTEST_4PASS:
    {
        system("chain tftp://192.168.100.10/memtestScript_4pass");
        system("reboot");
        break;
    }
    case PROFILE_OS_AUTOPILOT_MARKER_CLEAR:
    {
        //system("chain tftp://192.168.100.10/AutoPilot_Marker_Clear");
#ifdef SMBIOS_EFI
        startApmc();
#endif
        break;
    }
    case PROFILE_OS_UEFI_MEMTEST:
    {
#ifdef SMBIOS_EFI
        startUefiMemtest(uri, sizeof(uri));
#endif
        break;
    }
    case PROFILE_OS_DOS_TMDT:
    {
#ifndef SMBIOS_EFI
        snprintf(uri, sizeof(uri), "%s", targetToAddr(ISCSI_URI_DOS_TMDT));
        DBG("PROFILE_OS_DOS_TMDT  %s is loaded", uri);
#else
        startUefiTMDT();
#endif
        break;
    }
    case PROFILE_OS_DOS_HITACHI:
    {
        snprintf(uri, sizeof(uri), "%s", targetToAddr(ISCSI_URI_DOS_HITACHI));
        break;
    }
    case PROFILE_OS_DOS_TANDD:
    {
        strcpy(uri, iscsi_uri_dos);
        DBG("PROFILE_OS_DOS_TANDD  %s is loaded", uri);
        break;
    }
    case PROFILE_OS_DOS_BIOS:
    {
        strcpy(uri, iscsi_uri_dos);
        DBG("PROFILE_OS_DOS_BIOS  %s is loaded", uri);
        break;
    }
    default:
    {
        DBG("OS ERROR!!!!!!!");
        break;
    }
    }
    DBG_PAUSE();
    root_paths[0] = parse_uri(uri);
    return uriboot(NULL, root_paths, 1, san_default_drive(), NULL, 0);
}
    //#endif

#ifdef SMBIOS_EFI

#include <ipxe/efi/efi.h>

    /**
 * set screen to default size of 80x25 characters
 */
    /*
static int set_screen_size() {
    EFI_BOOT_SERVICES *bs = efi_systab->BootServices;
    union {
        EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *protocol;
        void *interface;
    }text_out;
    UINTN mode;
    UINTN rows, columns;
    UINTN selected_mode = 0;
    EFI_STATUS efirc;
    int rc;

    if ( ( efirc = bs->LocateProtocol(&efi_simple_text_output_protocol_guid,
                    NULL, &text_out.interface) ) != 0 ) {
        rc = -EEFI ( efirc );
        DBG ( "Could not open simple text output protocol\n" );
        goto err_open_protocol;
    }
    for ( mode = 0; mode < 16; mode++ ) {
        if ( ( efirc = text_out.protocol->QueryMode(text_out.protocol, mode, &columns, &rows) ) == 0 ) {
            DBG ( "Text mode %d is %d x %d\n", (unsigned int)mode, (unsigned int)columns, (unsigned int)rows );
            if (columns == 80 && rows == 25) {
                selected_mode = mode;
            }
        }
    }
    DBG_PAUSE();
    if ( ( efirc = text_out.protocol->SetMode(text_out.protocol, selected_mode) ) != 0 ) {
        rc = -EEFI ( efirc );
        DBG ( "Could not set screen resolution 80x25\n" );
        goto err_set_mode;
    }
    DBG ( "Sucessfully set screen resolution 80x25\n" );
    DBG_PAUSE();

    text_out.protocol->ClearScreen(text_out.protocol);

    rc = 0;

err_set_mode:
err_open_protocol:
    return rc;
}*/

#endif /* SMBIOS_EFI */

/*
 * tris_ui - tris step pages
 */
void tris_ui()
{
    /* initial gloabl info */
    memset(&ipxe_status, 0, sizeof(ipxe_status));

    newest_bios_version = malloc(5);
    memset(newest_bios_version, 0, sizeof(newest_bios_version));

    /* initial screen */
#ifdef SMBIOS_EFI
    //set_screen_size();
#endif /* SMBIOS_EFI */

    initscr();
    start_color();
    color_set(CPAIR_NORMAL, NULL);
    curs_set(0);

    /* fill last line */
    color_set(CPAIR_PXE, NULL);
    clearmsg(LINES - 1);
    color_set(CPAIR_NORMAL, NULL);

    /* initial devices */
    init_title_info();
    draw_title();

    /* clear screen */
    clear_screen();

    info("%s initialising devices...", product_short_name);
    startup();
    clearinfo();
    color_set(CPAIR_PXE, NULL);
    move(INFO_ROW, 1);
    wprintw(stdscr, "\b");
    if (ifconf_exec(0, NULL))
    {
        draw_reboot_widget(ERROR_INIT_NETWORK);
        wait_shutdown();
    }
    clearinfo();

    /* read title info */
    read_title_info();
    /* get gateway address */
    snprintf(_gateway_addr, sizeof(_gateway_addr),
             "http://%s/GW_server/", page_title_info.gateway_server);

    /* user-code & model-sn input */
    if (user_ui() == -1)
    {
        erase();
        return;
    }

    /* check bios*/
    check_bios();

    /* choose image type*/
    check_image();

    /* profile list */
    if (!auto_profile)
        profile_ui();

    /* load bootable image */
    info("Loading boot image...");

    /* end window */
    endwin();

    /* boot image */
    curs_set(0);
    image_boot();
}

/* EOF */
