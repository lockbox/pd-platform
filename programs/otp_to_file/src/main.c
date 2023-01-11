#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define TARGET_EXTENSION 1UL

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
#define OTP_END OTP_START + (16 * 64) // 16 OTP regions size 64

#define WRITE_FSTRING(_pd, _fmt, ...)                       \
    do                                                      \
    {                                                       \
        char *tmp;                                          \
        _pd->system->formatString(&tmp, _fmt, __VA_ARGS__); \
        write_line(_pd, tmp);                               \
        FREE(tmp);                                          \
    } while (0)

static void write_kmem_to_file(PlaydateAPI *pd, uint32_t base, uint32_t size, const char *fname)
{
    SDFile *file = pd->file->open(fname, kFileWrite);

    // make sure we can actually write to a file
    if (file == NULL)
    {
        WRITE_FSTRING(pd, "%s was unable to be opened because:\n\t%s", fname, pd->file->geterr());
        return;   
    }

    // tell the kernel to write to the file
    if ((-1 == pd->file->write(file, (void*)base, size)) == 1)
    {
        WRITE_FSTRING(pd, "ERROR when writing to file:\n\t%s", pd->file->geterr());
        return;
    }

    // close the file
    pd->file->close(file);
}


static void dump_otp(PlaydateAPI *pd)
{
    WRITE_FSTRING(pd, "Attempting to read memory from %p to %p", OTP_START, OTP_END);
    write_kmem_to_file(pd, OTP_START, OTP_END - OTP_START, "otp_mem.bin");
}
