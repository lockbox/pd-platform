# Building

NOTE:
> Building binaries using their device / arm SDK requires some dependencies. The only things i needed to install were: apt install arm-none-eabi-binutils, and snap install cmake since the apt version is wildly out of date.

Assuming all the dependencies are installed and everything is setup correctly you should be able
to build the SDK's `Life` example and it should *just work*, note that you need to override the
defult toolchain file for cmake, see the below commands for more.

We're going to run the build commands, then dissect what's going on under the hood from all
the automagic stuffs we're given from Panic Inc. ( the automagic stuff is great btw <3 ).

## Running the Build

Without any of the build output here are the types of commands needed to build an actual project, nothing more than your standard cmake build.
```bash
# run from within the `Life` project directory
$ mkdir build && cd build
# run the initial cmake build setup step, and override the toolchain
$ PLAYDATE_SDK_PATH=~/d/o/play.date/sdk-1.12.3 cmake .. -DCMAKE_TOOLCHAIN_FILE=~/d/o/play.date/sdk-1.12.3/C_API/buildsupport/arm.cmake
# build it, not sure if you need to provide the sdk path but i did anyways
$ PLAYDATE_SDK_PATH=~/d/o/play.date/sdk-1.12.3 make Life_DEVICE
```

Again, except with output
```bash
# prep the build
$ pwd
[..]/sdk-1.12.3/C_API/Examples/Life
$ mkdir build && cd build
$ PLAYDATE_SDK_PATH=~/d/o/play.date/sdk-1.12.3 cmake .. -DCMAKE_TOOLCHAIN_FILE=~/d/o/play.date/sdk-1.12.3/C_API/buildsupport/arm.cmake
-- arm.cmake loaded
-- arm.cmake loaded
-- The C compiler identification is GNU 8.3.1
-- The ASM compiler identification is GNU
-- Found assembler: /usr/bin/arm-none-eabi-gcc
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/arm-none-eabi-gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- SDK Path: /home/lockbox/d/o/play.date/sdk-1.12.3
-- Configuring done
-- Generating done
-- Build files have been written to: /home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/build

# do the build
$ PLAYDATE_SDK_PATH=~/d/o/play.date/sdk-1.12.3 make Life_DEVICE
/snap/cmake/1210/bin/cmake -S/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life -B/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/build --check-build-system CMakeFiles/Makefile.cmake 0
make  -f CMakeFiles/Makefile2 Life_DEVICE
make[1]: Entering directory '/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/build'
/snap/cmake/1210/bin/cmake -S/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life -B/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/build --check-build-system CMakeFiles/Makefile.cmake 0
/snap/cmake/1210/bin/cmake -E cmake_progress_start /home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/build/CMakeFiles 3
make  -f CMakeFiles/Makefile2 CMakeFiles/Life_DEVICE.dir/all
make[2]: Entering directory '/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/build'
make  -f CMakeFiles/Life_DEVICE.dir/build.make CMakeFiles/Life_DEVICE.dir/depend
make[3]: Entering directory '/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/build'
cd /home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/build && /snap/cmake/1210/bin/cmake -E cmake_depends "Unix Makefiles" /home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life /home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life /home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/build /home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/build /home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/build/CMakeFiles/Life_DEVICE.dir/DependInfo.cmake --color=
make[3]: Leaving directory '/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/build'
make  -f CMakeFiles/Life_DEVICE.dir/build.make CMakeFiles/Life_DEVICE.dir/build
make[3]: Entering directory '/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/build'
[ 33%] Building C object CMakeFiles/Life_DEVICE.dir/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c.obj
/usr/bin/arm-none-eabi-gcc -DTARGET_EXTENSION=1 -DTARGET_PLAYDATE=1 -I/home/lockbox/d/o/play.date/sdk-1.12.3/C_API -Wall -Wno-unknown-pragmas -Wdouble-promotion -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -D__FPU_USED=1 -falign-functions=16 -fomit-frame-pointer -gdwarf-2 -fverbose-asm -ffunction-sections -fdata-sections -std=gnu11 -MD -MT CMakeFiles/Life_DEVICE.dir/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c.obj -MF CMakeFiles/Life_DEVICE.dir/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c.obj.d -o CMakeFiles/Life_DEVICE.dir/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c.obj -c /home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c
[ 66%] Building C object CMakeFiles/Life_DEVICE.dir/main.c.obj
/usr/bin/arm-none-eabi-gcc -DTARGET_EXTENSION=1 -DTARGET_PLAYDATE=1 -I/home/lockbox/d/o/play.date/sdk-1.12.3/C_API -Wall -Wno-unknown-pragmas -Wdouble-promotion -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -D__FPU_USED=1 -falign-functions=16 -fomit-frame-pointer -gdwarf-2 -fverbose-asm -ffunction-sections -fdata-sections -std=gnu11 -MD -MT CMakeFiles/Life_DEVICE.dir/main.c.obj -MF CMakeFiles/Life_DEVICE.dir/main.c.obj.d -o CMakeFiles/Life_DEVICE.dir/main.c.obj -c /home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/main.c
[100%] Linking C executable Life_DEVICE.elf
/snap/cmake/1210/bin/cmake -E cmake_link_script CMakeFiles/Life_DEVICE.dir/link.txt --verbose=1
/usr/bin/arm-none-eabi-gcc -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16 -D__FPU_USED=1 -T/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/link_map.ld -Wl,-Map=game.map,--cref,--gc-sections,--no-warn-mismatch "CMakeFiles/Life_DEVICE.dir/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c.obj" CMakeFiles/Life_DEVICE.dir/main.c.obj -o Life_DEVICE.elf
/usr/bin/arm-none-eabi-objcopy -Obinary Life_DEVICE.elf /home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/Source/pdex.bin
cd /home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life && /home/lockbox/d/o/play.date/sdk-1.12.3/bin/pdc -sdkpath /home/lockbox/d/o/play.date/sdk-1.12.3 Source Life.pdx
make[3]: Leaving directory '/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/build'
[100%] Built target Life_DEVICE
make[2]: Leaving directory '/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/build'
/snap/cmake/1210/bin/cmake -E cmake_progress_start /home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/build/CMakeFiles 0
make[1]: Leaving directory '/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/build'
```
# Examining the internal `CMake` files
\<todo - not really super important, understanding them just makes things less automagic\>

# Build Steps
## Build `setup.c` + `main.c`
This step is only one (ridiculous) gcc invocation, note that for each step (`setup.c` and `main.c`,
and any other compilation unit), the flags are the same unless manually changed:

```bash
# prettified
/usr/bin/arm-none-eabi-gcc \
    -DTARGET_EXTENSION=1 \
    -DTARGET_PLAYDATE=1 \
    -I/home/lockbox/d/o/play.date/sdk-1.12.3/C_API \
    -Wall \
    -Wno-unknown-pragmas \
    -Wdouble-promotion \
    -mthumb \
    -mcpu=cortex-m7 \
    -mfloat-abi=hard \
    -mfpu=fpv5-sp-d16 \
    -D__FPU_USED=1 \
    -falign-functions=16 \
    -fomit-frame-pointer \
    -gdwarf-2 \
    -fverbose-asm \
    -ffunction-sections \
    -fdata-sections \
    -std=gnu11 \
    -MD \
    -MT CMakeFiles/Life_DEVICE.dir/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c.obj \
    -MF CMakeFiles/Life_DEVICE.dir/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c.obj.d \
    -o CMakeFiles/Life_DEVICE.dir/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c.obj \
    -c /home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c
```

### Deciphering the `gcc` options they used

references:
- https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html
- https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html
- gcc manpage
- [processor manual](https://www.st.com/resource/en/reference_manual/dm00124865-stm32f75xxx-and-stm32f74xxx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
- https://stackoverflow.com/questions/4274804/query-on-ffunction-section-fdata-sections-options-of-gcc
    - I'm curious what decisions drove them to do this, their results is some jank
    form of PIC with a bunch of extra data they don't need?

| Option | What it's doing |
|--|--|
| -DTARGET_EXTENSION=1 | Custom define for some of the CMake build system |
| -DTARGET_PLAYDATE=1 | Custom define for some of the CMake build system |
| -I/home/lockbox/d/o/play.date/sdk-1.12.3/C_API | Add the `C_API` to the include path |
| -Wall | Warn against all the common stuff |
| -Wno-unknown-pragmas | manually disable `unknown-pragmas` since `-Wall` enables it |
| -Wdouble-promotion | enable `double-promotion` warning |
| -mthumb | emit thumb code |
| -mcpu=cortex-m7 | select cpu |
| -mfloat-abi=hard | enable hard float support |
| -mfpu=fpv5-sp-d16 | select the target FPU |
| -D__FPU_USED=1 | Define intrinsic variable for FPU support |
| -falign-functions=16 | Align functions to 16 bytes, (as per cpu docs) |
| -fomit-frame-pointer | Only use frame pointers when necessary (generally non-small functions) |
| -gdwarf-2 | use dwarf2 debug info |
| -fverbose-asm | add extra "readability" syntax to generated asm, shouldnt be used? gross. |
| -ffunction-sections | Place each function in it's own section |
| -fdata-sections |  Place each data item in it's own section |
| -std=gnu11 | Set compiler standard |
| -MD -MT CMakeFiles/Life_DEVICE.dir/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c.obj | preprocessor arg |
| -MF CMakeFiles/Life_DEVICE.dir/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c.obj.d | preprocessor arg |
| -o CMakeFiles/Life_DEVICE.dir/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c.obj | output object file |
| -c /home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c | input file to compile |


# Link Step

Now the fun part - the link step.

```bash
/usr/bin/arm-none-eabi-gcc \
    -mthumb \
    -mcpu=cortex-m7 \
    -mfloat-abi=hard \
    -mfpu=fpv5-sp-d16 \
    -D__FPU_USED=1 \
    -T/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/link_map.ld \
        -Wl,-Map=game.map,--cref,--gc-sections,--no-warn-mismatch \
    "CMakeFiles/Life_DEVICE.dir/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c.obj" \
    CMakeFiles/Life_DEVICE.dir/main.c.obj \
    -o Life_DEVICE.elf
```

Many of the options are similar to the above build commands, the new flags however, are 
parameters to pass to the linker.

References:
- https://sourceware.org/binutils/docs/ld.html

| Linker Options | Effect |
|--|--|
| -Tlink_map.ld | use `link_map.ld` as the linker script |
| -Map=game.map | output map to file `game.map` |
| --cref | print symbol cross reference information to the map file |
| --gc-sections | link-tme section garbage collection |
| --no-warn-mismatch | ignore errors from linking mismatched objects |

Quote from `ld` documentation:
> 3.6.4.4 Input Section and Garbage Collection
When link-time garbage collection is in use (‘--gc-sections’), it is often useful to mark sections that should not be eliminated. This is accomplished by surrounding an input section’s wildcard entry with KEEP(), as in KEEP(\*(.init)) or KEEP(SORT_BY_NAME(\*)(.ctors)).

### What this is doing
The output symbol / section map for `Life_DEVICE.elf` is getting dumped to `game.map`, so now
we can retroactively figure out what things are at x offset into memory etc.

## Parsing `link_map.ld`

Custom Linker script:
```
MEMORY
{
  EXTRAM (rwx): ORIGIN = 0x60000000, LENGTH = 8*1024K
}

GROUP(libgcc.a libc.a libm.a)

SECTIONS
{
	.capi :
	{
		KEEP(*(.capi_handler))
		KEEP(*(.bss_start))
		KEEP(*(.bss_end))
	} > EXTRAM

	.text :
	{
		*(.text)

		KEEP(*(.init))
		KEEP(*(.fini))

		/* .ctors */
		*crtbegin.o(.ctors)
		*crtbegin?.o(.ctors)
		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
		*(SORT(.ctors.*))
		*(.ctors)

		/* .dtors */
 		*crtbegin.o(.dtors)
 		*crtbegin?.o(.dtors)
 		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
 		*(SORT(.dtors.*))
 		*(.dtors)

		*(.rodata*)

		KEEP(*(.eh_frame*))

	} > EXTRAM

	.data :
	{
		__etext = .;

		__data_start__ = .;
		*(vtable)
		*(.data*)

		. = ALIGN(4);
		/* preinit data */
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP(*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);

		. = ALIGN(4);
		/* init data */
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);

		. = ALIGN(4);
		/* finit data */
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		PROVIDE_HIDDEN (__fini_array_end = .);

		. = ALIGN(4);
		/* All data end */
		__data_end__ = .;

	} > EXTRAM

	.bss :
	{
		. = ALIGN(4);
		__bss_start__ = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end__ = .;

	} > EXTRAM

  /DISCARD/ :
  {
		*(.ARM.exidx)
  }

}
```

The important parts of this linker script are that its making a `rwx` (Read Write Execute)
section at addresss `0x60000000` and of size 8M. It then maps all the code onto it, and
all data as well. Starting at offset `0x0` the sections, in order will be:

- `.capi`
- `.text`
- `.data`
- `.bss`

## Parsing `game.map`
Important output from `game.map`:

Eventually you get to the part of `game.map` that is going to detail all the sections,
symbols, addresses, sizes, and source compilation units. This is what is going to get
generated into the elf, and is *almost* what is run on the device.
```
.capi           0x0000000060000000        0xc
 *(.capi_handler)
 .capi_handler  0x0000000060000000        0x4 CMakeFiles/Life_DEVICE.dir/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c.obj
                0x0000000060000000                PD_eventHandler
 *(.bss_start)
 .bss_start     0x0000000060000004        0x4 CMakeFiles/Life_DEVICE.dir/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c.obj
                0x0000000060000004                _bss_start
 *(.bss_end)
 .bss_end       0x0000000060000008        0x4 CMakeFiles/Life_DEVICE.dir/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c.obj
                0x0000000060000008                _bss_end

.text           0x000000006000000c      0x19c
 *(.text)
 .text          0x000000006000000c       0x88 /usr/lib/gcc/arm-none-eabi/8.3.1/thumb/v7e-m+fp/hard/crtbegin.o
 .text          0x0000000060000094        0xc /usr/lib/gcc/arm-none-eabi/8.3.1/../../../arm-none-eabi/lib/thumb/v7e-m+fp/hard/libc.a(lib_a-atexit.o)
                0x0000000060000094                atexit
 .text          0x00000000600000a0       0x34 /usr/lib/gcc/arm-none-eabi/8.3.1/../../../arm-none-eabi/lib/thumb/v7e-m+fp/hard/libc.a(lib_a-fini.o)
                0x00000000600000a0                __libc_fini_array
 .text          0x00000000600000d4       0x4c /usr/lib/gcc/arm-none-eabi/8.3.1/../../../arm-none-eabi/lib/thumb/v7e-m+fp/hard/libc.a(lib_a-rand.o)
                0x00000000600000d4                srand
                0x00000000600000e4                rand
 .text          0x0000000060000120       0x68 /usr/lib/gcc/arm-none-eabi/8.3.1/../../../arm-none-eabi/lib/thumb/v7e-m+fp/hard/libc.a(lib_a-__atexit.o)
                0x0000000060000120                __register_exitproc
 *(.init)
 .init          0x0000000060000188        0x4 /usr/lib/gcc/arm-none-eabi/8.3.1/thumb/v7e-m+fp/hard/crti.o
                0x0000000060000188                _init
 .init          0x000000006000018c        0x8 /usr/lib/gcc/arm-none-eabi/8.3.1/thumb/v7e-m+fp/hard/crtn.o
 *(.fini)
 .fini          0x0000000060000194        0x4 /usr/lib/gcc/arm-none-eabi/8.3.1/thumb/v7e-m+fp/hard/crti.o
                0x0000000060000194                _fini
 .fini          0x0000000060000198        0x8 /usr/lib/gcc/arm-none-eabi/8.3.1/thumb/v7e-m+fp/hard/crtn.o
 *crtbegin.o(.ctors)
 *crtbegin?.o(.ctors)
 *(EXCLUDE_FILE(*crtend.o *crtend?.o) .ctors)
 *(SORT_BY_NAME(.ctors.*))
 *(.ctors)
 *crtbegin.o(.dtors)
 *crtbegin?.o(.dtors)
 *(EXCLUDE_FILE(*crtend.o *crtend?.o) .dtors)
 *(SORT_BY_NAME(.dtors.*))
 *(.dtors)
 *(.rodata*)
 .rodata        0x00000000600001a0        0x4 /usr/lib/gcc/arm-none-eabi/8.3.1/../../../arm-none-eabi/lib/thumb/v7e-m+fp/hard/libc.a(lib_a-impure.o)
                0x00000000600001a0                _global_impure_ptr
 *(.eh_frame*)
 .eh_frame      0x00000000600001a4        0x0 /usr/lib/gcc/arm-none-eabi/8.3.1/thumb/v7e-m+fp/hard/crtbegin.o
 .eh_frame      0x00000000600001a4        0x4 /usr/lib/gcc/arm-none-eabi/8.3.1/thumb/v7e-m+fp/hard/crtend.o

.glue_7         0x00000000600001a8        0x0
 .glue_7        0x00000000600001a8        0x0 linker stubs

.glue_7t        0x00000000600001a8        0x0
 .glue_7t       0x00000000600001a8        0x0 linker stubs

.vfp11_veneer   0x00000000600001a8        0x0
 .vfp11_veneer  0x00000000600001a8        0x0 linker stubs

.v4_bx          0x00000000600001a8        0x0
 .v4_bx         0x00000000600001a8        0x0 linker stubs

.iplt           0x00000000600001a8        0x0
 .iplt          0x00000000600001a8        0x0 /usr/lib/gcc/arm-none-eabi/8.3.1/thumb/v7e-m+fp/hard/crtbegin.o

.text.eventHandlerShim
                0x00000000600001b0       0x3c
 .text.eventHandlerShim
                0x00000000600001b0       0x3c CMakeFiles/Life_DEVICE.dir/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c.obj
                0x00000000600001b0                eventHandlerShim

.text.ison      0x00000000600001f0       0x44
 .text.ison     0x00000000600001f0       0x44 CMakeFiles/Life_DEVICE.dir/main.c.obj

.text.val       0x0000000060000240       0x48
 .text.val      0x0000000060000240       0x48 CMakeFiles/Life_DEVICE.dir/main.c.obj

.text.rowsum    0x0000000060000290       0x98
 .text.rowsum   0x0000000060000290       0x98 CMakeFiles/Life_DEVICE.dir/main.c.obj

.text.middlerowsum
                0x0000000060000330       0x74
 .text.middlerowsum
                0x0000000060000330       0x74 CMakeFiles/Life_DEVICE.dir/main.c.obj

.text.doRow     0x00000000600003b0       0xb0
 .text.doRow    0x00000000600003b0       0xb0 CMakeFiles/Life_DEVICE.dir/main.c.obj

.text.randomize
                0x0000000060000460       0x64
 .text.randomize
                0x0000000060000460       0x64 CMakeFiles/Life_DEVICE.dir/main.c.obj

.text.update    0x00000000600004d0       0xdc
 .text.update   0x00000000600004d0       0xdc CMakeFiles/Life_DEVICE.dir/main.c.obj

.text.eventHandler
                0x00000000600005b0       0x54
 .text.eventHandler
                0x00000000600005b0       0x54 CMakeFiles/Life_DEVICE.dir/main.c.obj
                0x00000000600005b0                eventHandler

.text.startup   0x0000000060000604       0x14
 .text.startup  0x0000000060000604       0x14 /usr/lib/gcc/arm-none-eabi/8.3.1/../../../arm-none-eabi/lib/thumb/v7e-m+fp/hard/libc.a(lib_a-__call_atexit.o)

.rel.dyn        0x0000000060000618        0x0
 .rel.iplt      0x0000000060000618        0x0 /usr/lib/gcc/arm-none-eabi/8.3.1/thumb/v7e-m+fp/hard/crtbegin.o

.data           0x0000000060000618      0x43c
                0x0000000060000618                __etext = .
                0x0000000060000618                __data_start__ = .
 *(vtable)
 *(.data*)
 .data          0x0000000060000618      0x430 /usr/lib/gcc/arm-none-eabi/8.3.1/../../../arm-none-eabi/lib/thumb/v7e-m+fp/hard/libc.a(lib_a-impure.o)
                0x0000000060000618                _impure_ptr
                0x0000000060000a48                . = ALIGN (0x4)
                0x0000000060000a48                PROVIDE (__preinit_array_start = .)
 *(.preinit_array)
                0x0000000060000a48                PROVIDE (__preinit_array_end = .)
                0x0000000060000a48                . = ALIGN (0x4)
                0x0000000060000a48                PROVIDE (__init_array_start = .)
 *(SORT_BY_NAME(.init_array.*))
 .init_array.00000
                0x0000000060000a48        0x4 /usr/lib/gcc/arm-none-eabi/8.3.1/../../../arm-none-eabi/lib/thumb/v7e-m+fp/hard/libc.a(lib_a-__call_atexit.o)
 *(.init_array)
 .init_array    0x0000000060000a4c        0x4 /usr/lib/gcc/arm-none-eabi/8.3.1/thumb/v7e-m+fp/hard/crtbegin.o
                0x0000000060000a50                PROVIDE (__init_array_end = .)
                0x0000000060000a50                . = ALIGN (0x4)
                0x0000000060000a50                PROVIDE (__fini_array_start = .)
 *(SORT_BY_NAME(.fini_array.*))
 *(.fini_array)
 .fini_array    0x0000000060000a50        0x4 /usr/lib/gcc/arm-none-eabi/8.3.1/thumb/v7e-m+fp/hard/crtbegin.o
                0x0000000060000a54                PROVIDE (__fini_array_end = .)
                0x0000000060000a54                . = ALIGN (0x4)
                0x0000000060000a54                __data_end__ = .

.tm_clone_table
                0x0000000060000a54        0x0
 .tm_clone_table
                0x0000000060000a54        0x0 /usr/lib/gcc/arm-none-eabi/8.3.1/thumb/v7e-m+fp/hard/crtbegin.o
 .tm_clone_table
                0x0000000060000a54        0x0 /usr/lib/gcc/arm-none-eabi/8.3.1/thumb/v7e-m+fp/hard/crtend.o

.igot.plt       0x0000000060000a54        0x0
 .igot.plt      0x0000000060000a54        0x0 /usr/lib/gcc/arm-none-eabi/8.3.1/thumb/v7e-m+fp/hard/crtbegin.o

.bss            0x0000000060000a54       0x24
                0x0000000060000a54                . = ALIGN (0x4)
                0x0000000060000a54                __bss_start__ = .
 *(.bss*)
 .bss           0x0000000060000a54       0x1c /usr/lib/gcc/arm-none-eabi/8.3.1/thumb/v7e-m+fp/hard/crtbegin.o
 .bss.pdrealloc
                0x0000000060000a70        0x4 CMakeFiles/Life_DEVICE.dir/home/lockbox/d/o/play.date/sdk-1.12.3/C_API/buildsupport/setup.c.obj
 .bss.pd        0x0000000060000a74        0x4 CMakeFiles/Life_DEVICE.dir/main.c.obj
 *(COMMON)
                0x0000000060000a78                . = ALIGN (0x4)
                0x0000000060000a78                __bss_end__ = .

```

### Tying back to the build steps
A lot of the contents in the generated map are due to the `gcc` build options that were passed in.

- `-ffunction-sections`
    - section symbols with names like `.text.evenHandlerShim` appear because this
    switch gives each function it's own section
    - notice how symbols like `atexit` and `__libc_fini_array` etc. (functions from NOT
    the play.date build system) only belong to the section `.text`? They don't have this
    build switch enabled.
- `-fdata-sections`
    - section symbols with names like `.bss.pdrealloc` and `.bss.pd` occur because of
    this switch (see above). 

Take note of the `.capi` section, and how there are ONLY three members (there will always
only be three due to `link_map.ld` above). This is the magic that creates a simple API
for all games to run hosted in their runtime.

# Packing Step

The packaging step is pretty simple, and uses a bunch of information from the link step.

```bash
# copy the section contents from `Life_DEVICE.elf` into a binary blob
/usr/bin/arm-none-eabi-objcopy \
    -Obinary \
    Life_DEVICE.elf \
    /home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life/Source/pdex.bin

# Package the binary blob in a .pdx archive
cd /home/lockbox/d/o/play.date/sdk-1.12.3/C_API/Examples/Life && \
/home/lockbox/d/o/play.date/sdk-1.12.3/bin/pdc \
    -sdkpath /home/lockbox/d/o/play.date/sdk-1.12.3 \
    Source \
    Life.pdx
```

All `objcopy` is going to do in this step is take the section contents from the elf and
dump them into a blob, this is effectively doing a memory map dump to disk. And because
in the link step it mashed all the sections next to each other there is one contiguous
blob of all our code + literal pools + data etc. No relocations necessary :D

In effect we get the information from `game.map` actualized into a `.bin`. The contents of
our `.bin` file are repsented exactly as defined in `game.map`, the only difference being
thatthe beginning of the `.bin` is (duh) going to be offset `0x0`, but will be relocated to
`0x60000000` on the actual device.

The `pdc` invocation when using C only projects basically just copy + pastes as far as I can tell.