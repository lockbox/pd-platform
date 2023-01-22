#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define TARGET_EXTENSION 1

#include "pd_api.h"

#define FREE(_ptr)                                   \
    do                                               \
    {                                                \
        if (_ptr)                                    \
        {                                            \
            if (PD_HANDLE)                           \
            {                                        \
                PD_HANDLE->system->realloc(_ptr, 0); \
                _ptr = NULL;                         \
            }                                        \
        }                                            \
    } while (0)

const char *fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont *font = NULL;

// local handle to use for accessing the play.date API
static PlaydateAPI *PD_HANDLE = (PlaydateAPI *)NULL;

// forward declaration
static void log_addresses(PlaydateAPI *pd);
static int update(void *data);

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

        PD_HANDLE = pd;
        pd->system->setUpdateCallback(update, PD_HANDLE);
    }

    return 0;
}

/**
 * \brief continuously logs the contents of the address bucket to the screen
 */
static int update(void *data)
{
    PlaydateAPI *pd = PD_HANDLE;
    static int done = 0;
    (void)data;

    // only execute once
    if (done)
        return 0;

    pd->graphics->clear(kColorWhite);

    // log the addresses we grabbed
    log_addresses(pd);

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

#define WRITE_FSTRING(_pd, _fmt, ...)                       \
    do                                                      \
    {                                                       \
        char *tmp;                                          \
        _pd->system->formatString(&tmp, _fmt, __VA_ARGS__); \
        write_line(_pd, tmp);                               \
        FREE(tmp);                                          \
    } while (0)

#define WRITE_FCONSOLE(_pd, _fmt, ...)                \
    do                                                \
    {                                                 \
        _pd->system->logToConsole(_fmt, __VA_ARGS__); \
    } while (0)

/**
 * \brief macro to dump pointer tables
 */
#define LOG_TABLE(__pd, __name, __sz, __ptr)                                                \
    do                                                                                      \
    {                                                                                       \
        WRITE_FCONSOLE(__pd,                                                                \
                       "{\"name\": \"%s\", \"address\": \"%p\", \"size\": %d, \"data\": [", \
                       __name,                                                              \
                       (void *)__ptr,                                                       \
                       __sz);                                                               \
        for (int i = 0; i < __sz / 4; i++)                                                  \
        {                                                                                   \
            WRITE_FCONSOLE(__pd, "\"%08x\",", *(((uint32_t *)__ptr) + i));                  \
        }                                                                                   \
        WRITE_FCONSOLE(__pd, "%s", "]}");                                                   \
    } while (0)

/**
 * \brief dumps a jsonl of symbols to the console
 */
static void log_addresses(PlaydateAPI *pd)
{
    LOG_TABLE(pd, "pd->system", sizeof(struct playdate_sys), pd->system);
    LOG_TABLE(pd, "pd->file", sizeof(struct playdate_file), pd->file);
    LOG_TABLE(pd, "pd->graphics", sizeof(struct playdate_graphics), pd->graphics);
    LOG_TABLE(pd, "pd->sprite", sizeof(struct playdate_sprite), pd->sprite);
    LOG_TABLE(pd, "pd->display", sizeof(struct playdate_display), pd->display);
    LOG_TABLE(pd, "pd->sound", sizeof(struct playdate_sound), pd->sound);
    LOG_TABLE(pd, "pd->lua", sizeof(struct playdate_lua), pd->lua);
    LOG_TABLE(pd, "pd->json", sizeof(struct playdate_json), pd->json);
    LOG_TABLE(pd, "pd->scoreboards", sizeof(struct playdate_scoreboards), pd->scoreboards);
    WRITE_FSTRING(pd, "Wrote tables to console", "");
}
