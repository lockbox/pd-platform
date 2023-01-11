#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "pd_api.h"

#define FREE(_ptr)                                   \
    do                                               \
    {                                                \
        if (_ptr != NULL)                            \
        {                                            \
            if (PD_HANDLE)                           \
            {                                        \
                PD_HANDLE->system->realloc(_ptr, 0); \
                *(int *)_ptr = 0;                    \
            }                                        \
        }                                            \
    } while (0)

// globals
const char *fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont *font = NULL;
static const PlaydateAPI *PD_HANDLE;
static const void (*println)(const char *format, ...);

// forward declaration
static int update(void *data);
static void write_line(PlaydateAPI *pd, const char *str);
static void dump_otp(PlaydateAPI *pd);

/**
 * \brief Sets up the font, finds the addresses for the bucket,
 * and sets the callback to draw them to the screen.
 */
int eventHandler(PlaydateAPI *pd, PDSystemEvent event, uint32_t arg)
{
    (void)arg;

    if (event == kEventInit)
    {
        const char *err;

        font = pd->graphics->loadFont(fontpath, &err);

        if (font == NULL)
        {
            pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);
        }

        // setup globals
        PD_HANDLE = pd;                     // api handle
        println = pd->system->logToConsole; // abbreviate logToConsole invocation

        pd->system->setUpdateCallback(update, pd);
    }

    return 0;
}

/**
 * \brief continuously logs the contents of the address bucket to the screen
 */
static int update(void *data)
{
    PlaydateAPI *pd = data;
    static int done = 0;
    char buffer[] = "Dumping OTP...";

    // only execute once
    if (done)
        return 0;

    // reset screen
    pd->graphics->clear(kColorWhite);

    // write message
    write_line(pd, buffer);

    // dump flash
    dump_otp(pd);

    done = 1;
    return 1;
}

/**
 * \brief holds current metadata of the output text, used by \fn write_line.
 */
typedef struct TextMetadata_t
{
    uint16_t current_y;
    uint16_t height;
} TextMetadata_t;

#define SCREEN_WIDTH LCD_COLUMNS
#define SCREEN_HEIGHT LCD_ROWS
static TextMetadata_t TEXT_META = {0, 20};

/**
 * \brief writes a line to the screen, if debug also the console
 */
static void write_line(PlaydateAPI *pd, const char *str)
{
    // reset the printer if we hit the bottom of the screen
    if (TEXT_META.current_y >= SCREEN_HEIGHT)
    {
        pd->graphics->clear(kColorWhite);
        TEXT_META.current_y = 0;
    }

    // set the font to use
    pd->graphics->setFont(font);

    // do the actual writing
    pd->graphics->drawText(str, strlen(str), kASCIIEncoding, 0, TEXT_META.current_y);

    // move the the next line ("\n")
    TEXT_META.current_y += TEXT_META.height;
}

#define OTP_START 0x1FF0F000UL
#define OTP_END OTP_START + (16 * 64)

static void dump_otp(PlaydateAPI *pd)
{
    uint32_t addr;
    uint32_t value = 0;

    println("0x%08x, 0x%08x, dump_start:\r\n", OTP_START, OTP_END);

    for (addr = OTP_START; addr <= OTP_END; addr += 0x4)
    {
        value = *((uint32_t *)addr);

        // print value
        println("%08x\r\n", value);
    }
}
