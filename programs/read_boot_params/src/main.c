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

// forward declaration
static void log_addresses(PlaydateAPI *pd);
static int update(void *data);

/**
 * Globals
 */
// local handle to use for accessing the play.date API
static PlaydateAPI *PD_HANDLE = (PlaydateAPI *)NULL;
const char *fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont *font = NULL;

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
        pd->system->setUpdateCallback(update, pd);
    }

    return 0;
}

/**
 * \brief continuously logs the contents of the address bucket to the screen
 */
static int update(void *data)
{
    PlaydateAPI *pd = (PlaydateAPI *)data;
    static int done = 0;

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

/***************************************/
// globals for accessing cpu registers //
/***************************************/
#define __IO volatile
typedef struct
{
    __IO uint32_t ACR;     /*!< FLASH access control register,     Address offset: 0x00 */
    __IO uint32_t KEYR;    /*!< FLASH key register,                Address offset: 0x04 */
    __IO uint32_t OPTKEYR; /*!< FLASH option key register,         Address offset: 0x08 */
    __IO uint32_t SR;      /*!< FLASH status register,             Address offset: 0x0C */
    __IO uint32_t CR;      /*!< FLASH control register,            Address offset: 0x10 */
    __IO uint32_t OPTCR;   /*!< FLASH option control register ,    Address offset: 0x14 */
    __IO uint32_t OPTCR1;  /*!< FLASH option control register 1 ,  Address offset: 0x18 */
} FLASH_TypeDef;
#define PERIPH_BASE 0x40000000UL /*!< Base address of : AHB/ABP Peripherals                                                   */
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000UL)
#define FLASH_R_BASE (AHB1PERIPH_BASE + 0x3C00UL)
#define FLASHSIZE_BASE 0x1FF0F442UL /*!< FLASH Size register base address */
#define FLASH ((FLASH_TypeDef *)FLASH_R_BASE)

#define WRITE_FSTRING(_pd, _fmt, ...)                       \
    do                                                      \
    {                                                       \
        char *tmp;                                          \
        _pd->system->formatString(&tmp, _fmt, __VA_ARGS__); \
        write_line(_pd, tmp);                               \
        FREE(tmp);                                          \
    } while (0)

static void log_addresses(PlaydateAPI *pd)
{
    WRITE_FSTRING(pd, "FLASH BASE PTR: %p\n", FLASH);
    // WRITE_FSTRING(pd, "Flash Access Control Register: 0x%08x\n", FLASH->ACR);
    // WRITE_FSTRING(pd, "FLash Option Key Register: 0x%08x\n", FLASH->OPTKEYR);
    // WRITE_FSTRING(pd, "Flash Status Register: 0x%08x\n", FLASH->SR);
    // WRITE_FSTRING(pd, "Flash Control Register: 0x%08x\n", FLASH->CR);
    // WRITE_FSTRING(pd, "Flash Option Control Register: 0x%08x\n", FLASH->OPTCR);
    // WRITE_FSTRING(pd, "Flash Option Control Register 1: 0x%08x\n", FLASH->OPTCR1);
}
