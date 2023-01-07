#!/usr/bin/env python3
"""Dumps the sections from the built playdate ELF.

Optionally outputs the offsets from the ``pdex.bin``
for each section / function.

Linker Script in question:

.. code::

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

"""
import json
import elftools
import argparse

from pathlib import Path
from typing import NamedTuple
from typing import List
from dataclasses import dataclass

from elftools.elf.elffile import ELFFile

class PlayDateFunction(NamedTuple):
    name: str
    address: int
    bin_offset: int

@dataclass
class PlayDateSection:
    """Holds data about the sections in the generated code destined
    for the target play.date
    """

    address: int
    size: int
    name: int
    bin_offset: int
    functions: List[PlayDateFunction]


def sections_from_elf(file: ELFFile):
    """Dumps desired metadata from elf file according to packaged
    linker scripts
    """
    sections: List[PlayDateSection] = []
    entry_point = file.header.e_entry
    print(f"entry_point: {hex(entry_point)}")

    for sec in file.iter_sections():
        # skip 0 size sections
        if sec.data_size == 0:
            continue

        # skip sections with address 0
        address = sec.header["sh_addr"]
        if address == 0:
            continue

        print(f"name: {sec.name}, size: {hex(sec.data_size)}, address: {hex(address)}")

if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument("input", help="input file", type=Path)
    p.add_argument("--json", help="output json or not", default=False, action="store_true")
    args = p.parse_args()

    with open(args.input, "rb") as f:
        elf_data = ELFFile(f)

        sections = sections_from_elf(elf_data)