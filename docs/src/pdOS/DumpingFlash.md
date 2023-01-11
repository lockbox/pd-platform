# Dumping Flash

From [`Valid Memory`](../console/ValidMemory.md) we know that flash starts
at `0x08000000` and goes until `0x080FFFFF`. So lets write up a quick C stub
to dump it over serial, and a python script to pickup the dump.

See pd-usb for details but the high level is that you just need to connect
with baud rate `115200` to the play.date, the ID's (just in case you want custom
udev rules for mitm serial or something similar, or its not under `/dev/ttyACM0`) are:

- Vendor ID: `0x1331`
- Product ID: `0x5740`

The general algorithm is very simple:

>Note:
>
>Flash has a default value of ``0xffffffff``, so I used shortcut logic to
>wait until 100 were read in a row, no need to waste time, though the baud
>rate is 115200 so at least it moves pretty quick in this case. Many times
>you need to do this it is prohibitively slow to read + dump flash though.

```c
static void dump_flash(PlaydateAPI *pd)
{
    uint32_t addr;
    uint32_t value = 0;
    uint32_t default_value = 0; // number of consecutive ``0xffffffff``

    // pattern so python script to pickup the dump knows we're printing
    println("0x%08x, 0x%08x, dump_start:\r\n", FLASH_START, FLASH_END);

    // iterate over each word in flash, and dump it
    for (addr = FLASH_START; addr <= FLASH_END; addr += 0x4)
    {
        value = *((uint32_t *)addr);

        // check if ``0xffffffff``
        if (value == 0xffffffff)
        {
            default_value++;
            
            // if there are 100 ``0xffffffff`` in a row then quit,
            // assume that we hit the end of the flash memory used
            if (default_value >= 100)
                break;
        }
        else
        {
            default_value = 0;
        }

        println("0x%08x: 0x%08x\r\n", addr, value);
    }
}
```

### A couple speed bumps along the way:
>So I've been using the docs as given in the [awesome playdate]() repo,
>which started crashing things a bunch when reading from flash `0x08000000`
>until `0x08050000` which then let me dump until the end of flash, so I
>went back through the FCC filing photos to see the actual MCU onboard.
>
>The actual MCU is [STM23f746ie](https://www.st.com/en/microcontrollers-microprocessors/stm32f746ie.html), which really doesn't change
>anything wrt what docs to use. So for the moment I'm still stumped why it's
>crashing attempting to read flash below `0x08050000`.
>
>Then I found [this](https://github.com/STMicroelectronics/STM32CubeF7/blob/master/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c#L914),


Step 1: get the typedef for the System Flash Registers:
```c
// github STM32CubeF7/Drivers/CMSIS/Device/ST/STM32F7xx/Include/stm32f7xx.h
typedef struct
{
  __IO uint32_t ACR;      /*!< FLASH access control register,     Address offset: 0x00 */
  __IO uint32_t KEYR;     /*!< FLASH key register,                Address offset: 0x04 */
  __IO uint32_t OPTKEYR;  /*!< FLASH option key register,         Address offset: 0x08 */
  __IO uint32_t SR;       /*!< FLASH status register,             Address offset: 0x0C */
  __IO uint32_t CR;       /*!< FLASH control register,            Address offset: 0x10 */
  __IO uint32_t OPTCR;    /*!< FLASH option control register ,    Address offset: 0x14 */
  __IO uint32_t OPTCR1;   /*!< FLASH option control register 1 ,  Address offset: 0x18 */
} FLASH_TypeDef;
```

Well so we're still crashing, heck. So they probably are using the Memory Protection Unit.
At least we could dump user flash. Time to try to dump some more stuff and move on.

See if we can dump OTP memory, though it might also be protected.
It starts at `0x1FF0F000` and is (16 * 64) bytes (16 banks):

```c
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
```

F.

Still crashing, tho duh we're still not privileged.

Time to go about this a different way - what if their kernel doesn't do usermode pointer validation?

Lets try again:

```c
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
```

Still crashing, though after looking back through the re notes I noticed

1) they do use OTP (cool) and
2) once upon a time they [didn't validate the pointers](https://github.com/jaames/playdate-reverse-engineering/blob/main/usb/usb.md#unlock) (or something to that effect)

Upon further attempts I'm assuming that any memory accessed by the kernel for
the process is limited to the program stack / heap as the following did not work.

- start of flash `0x08000000`
- known kernel addresses (from forcing crashes) `0x0803xxxx`
- known user space addresses (from pd api) `0x0805xxxx`
- system registers
- OTP memory

So after perusing the forums a [bit](https://devforum.play.date/t/file-open-crashes-on-device-c-api/9841/11) it's using (as i actually assumed
right for once) [FreeeRTOS](https://www.freertos.org/). And now it looks like
the new play.date versions are using (working) MPU builds, rude.

Oh well, got user mode dumps, time to RE a bit.

![skeletor.gif](https://media.tenor.com/jdFD8PpUK64AAAAC/skeletor-running-away.gif)