# Searching for FreeRTOS code

So starting off, we know a couple things, we have a memory dump, and we know that the
processor is the `STM32F746IE`, which has internal flash range from `0x08000000` to
`0x080FFFFF` inclusive. We are able to dump flash starting at `0x08050000`, and we know
that the MPU is enabled. We are (currently) trying to figure out where the `FreeRTOS`
code is stored in the flash dump.

Let's see where some default FreeRTOS builds for ARM Cortex M7 put their code.
In the `FreeRTOS` source tree, the demo projects are in the path `demo/<platform-info>`,
so we'll see if we can find a couple `MPU` + `Cortex M7` projects and see if
they have attached linker scripts.

Bingo.
 - [CORTEX_MPU_M7_NUCLEO_H743ZI2_GCC_IAR_Keil](https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/CORTEX_MPU_M7_NUCLEO_H743ZI2_GCC_IAR_Keil/Projects/GCC/STM32H743ZITX_FLASH.ld)

 - [CORTEX_M7_STM32F7_STM32756G-EVAL_IAR_Keil](https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/CORTEX_M7_STM32F7_STM32756G-EVAL_IAR_Keil/System_IAR/stm32f756xx_flash.icf)

What we can guesstimate off of these (and the fact that we can read half the flash space)
is that the protected "kernel" code is in the first half, and that user mode is in the
second half of flash. Looking further at the linker scripts (the first one linked mainly),
we can see that they shove the syscalls at the beginning of the user flash region like so:

```
  /* The program code and other data into "FLASH" Rom type memory */
  .text :
  {
    /* Place the FreeRTOS System Calls first in the unprivileged region. */
    . = ALIGN(4);
    __syscalls_flash_start__ = .;
    *(freertos_system_calls)
    __syscalls_flash_end__ = .;
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH
```
So we can probably just make a sample project for ourselves and then use the generated
`.map` listing files to see which syscalls are included and if any of the code looks similar.

Except, I'm lazy and I don't want to do it by hand. So we're going to attempt to make binary
ninja signatures with [sigkit](https://github.com/Vector35/sigkit/) and go hunting for syscalls.

First though, we're going to do some basic recon:

From the [MPU Syscalls](https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/bb6071e1df3168a64dc2ce79de8aa91b7995ba23/include/mpu_prototypes.h) page:

>When the MPU is used the standard (non MPU) API functions are mapped to
>equivalents that start "MPU_", the prototypes for which are defined in this
>header files.  This will cause the application code to call the MPU_ version
>which wraps the non-MPU version with privilege promoting then demoting code,
>so the kernel code always runs will full privileges.

This sounds super jank lol, in an ideal secure world the user thread should not be getting promoted ever.
Here's to minimal os security woop woooppp

### grep == win

First I'm going to see if there are any [SVC](https://developer.arm.com/documentation/dui0646/c/The-Cortex-M7-Instruction-Set/Miscellaneous-instructions/SVC)
invocations so we know what we're dealing with. To do that (instead of a cool way that is better like
a python script in binary ninja so we know what functions own the `svc` calls), we're going to
disassemble all of userspace bin, then `grep` for all the syscall instructions,
then trim down to the unique syscalls, and then count the lines.

```bash
arm-none-eabi-objdump -D -b binary -marm -Mforce-thumb --adjust-vma=0x08050000 0x8050000_0x80fffff_playdate.bin | grep svc | sed -e"s/;//p" | awk '{$1=$1};1' | cut -d ' ' -f 4 | sort | uniq | wc -l
$ 66
```

So our kernel surface area is 66 syscalls, by default FreeRTOS ships with a couple depending on the platform, and has a ton pre-implemented (but not assigned to numbers afaics) so theres a lot of things going on here, cool. I'm assuming that
the pre-implemented syscalls are going to be lower numbers and any custom ones are descending starting from `0xff` (255).

Reminder of our goals here - we know that [only season games](https://devforum.play.date/t/network-access-like-whitewater-wipeout-leader-boards/5598/2)
can use internet, and we can safely assume that there is some
functionality on board to do that - the only question is going to
be if that is exposed to games in userspace or if it is hidden
behind a syscall or completely handled in kernel space.

And we need find a way to automatically signature a bunch of
functions so we don't have to waste a bunch of time doing boring
RE. So we're going to build an example FreeRTOS project with
MPU enabled and all of the syscalls, then build signatures from
that and attempt to locate any code in our userspace dump.


# Building FreeRTOS
TODO

[Example `FreeRTOSConfig.h`](https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/CORTEX_MPU_M7_NUCLEO_H743ZI2_GCC_IAR_Keil/Config/FreeRTOSConfig.h)