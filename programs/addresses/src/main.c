#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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

/**
 * \brief used to hold the addresses to print to the screen
 */
typedef struct AddressBucket_t
{
    uint32_t api_base;
} AddressBucket_t;

// forward declaration
static inline void populateBucket(PlaydateAPI *pd, AddressBucket_t *bucket);
static void log_addresses(PlaydateAPI *pd, AddressBucket_t *bucket);
static int update(void *data);

static AddressBucket_t BUCKET = { 0 };
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

        // get the addresses for the bucket
        populateBucket(pd, &BUCKET);

        font = pd->graphics->loadFont(fontpath, &err);

        if (font == NULL)
        {
            pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);
        }

        PD_HANDLE = pd;
        pd->system->setUpdateCallback(update, &BUCKET);
    }

    return 0;
}

/**
 * \brief continuously logs the contents of the address bucket to the screen
 */
static int update(void *data)
{
    PlaydateAPI *pd = PD_HANDLE;
    AddressBucket_t * bucket = (AddressBucket_t *)data;
    static int done = 0;

    // only execute once
    if (done) return 0;

    pd->graphics->clear(kColorWhite);

    // log the addresses we grabbed
    log_addresses(pd, bucket);

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

static void log_addresses(PlaydateAPI *pd, AddressBucket_t *bucket)
{
    char * api_base;
    char * pd_address;
    char * heap_obj;
    char * heap_base;
    char * stack_obj;
    char * stack_base;
    char * code_ptr;

    // create the statements
    pd->system->formatString(&api_base, "API Base Address: %p", bucket->api_base);
    pd->system->formatString(&pd_address, "PD API Address: %p", pd);
    pd->system->formatString(&heap_base, "Heap Base Address: %p", ((uint32_t)api_base & 0xf0000000));
    pd->system->formatString(&heap_obj, "Heap Address: %p", api_base);
    pd->system->formatString(&stack_base, "Stack Base Address: %p", ((uint32_t)&api_base & 0xf0000000));
    pd->system->formatString(&stack_obj, "Stack Address: %p", &api_base);
    pd->system->formatString(&code_ptr, "&formatString: %p", pd->system->formatString);

    // write the statements
    write_line(pd, api_base);
    write_line(pd, pd_address);
    write_line(pd, heap_base);
    write_line(pd, heap_obj);
    write_line(pd, stack_base);
    write_line(pd, stack_obj);
    write_line(pd, code_ptr);

    // free the statements
    FREE(api_base);
    FREE(pd_address);
    FREE(heap_base);
    FREE(heap_obj);
    FREE(stack_base);
    FREE(stack_obj);
    FREE(code_ptr);
}

static inline void populateBucket(PlaydateAPI *pd, AddressBucket_t *bucket)
{
    bucket->api_base = (uint32_t)pd & 0xf0000000;
}