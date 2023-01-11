#!/usr/bin/env python3
"""Script for parsing the output of the ``Dump Flash``
Play.Date program
"""
import os
import sys
import serial
import argparse

from pathlib import Path
from typing import NamedTuple

from alive_progress import alive_bar

class FlashInfo(NamedTuple):
    start: int
    end: int
    size: int
    num_reads: int

def parse_meta(data: bytes) -> FlashInfo:
    """Parses a message from the playdate to get the metadata
    about the flash dump.

    WARNING: assumes data from a ``read_until("dump_start:\r\n")``
    is the input.

    The information should be formatted like:

    ``0x%08x, 0x%08x, dump_start:\r\n``
    Where the first argument is the start address, and the second
    is the end address. Note that there could be any beginning characters,
    but the length of the message will always be the last 36 bytes.
    (len start + len end + dump_start: + newline)

    Parameters
    ----------

    data: bytes
        formatted message from the Dump Flash program
    """
    assert len(data) >= 37

    # get our start + end address, ignoree "dump_start:\n"
    start, end = data[-38:].decode("utf-8").strip().split(", ")[:2]

    start_address = int(start, 16)
    end_address = int(end, 16)
    flash_size = end_address - start_address

    print(f"Found Flash{{ start: {start}, end: {end}, size: {hex(flash_size)} }}")

    return FlashInfo(start_address,
                end_address,
                flash_size,
                flash_size // 4)

def decode_line(data: bytes) -> bytes:
    """Decodes the ascii encoded dump, stops after 100 repeated
    ``0xffffffff``.

    Format should follow:
    ``%08x\r\n``
    of the data values
    """
    data = data.decode("utf-8").strip()
    # print(data)

    # if there's data return it
    if len(data):
        return bytes.fromhex(data)
    return b""


def dump_firmware(s: serial.Serial, out: Path) -> None:
    """Catch dumped firmware and write it to ``out``
    """
    print(f"Catching dumped firmware from {s.name} to {str(out)}")

    # get the information describing the flash dump
    metadata = parse_meta(s.read_until(b"dump_start:\r\n"))

    # count the last n times ``0xffffffff`` was read from flash
    default_counter = 0

    # read each memory value dumped
    outfile = f"{hex(metadata.start)}_{hex(metadata.end)}_{str(out)}"
    with open(outfile, "wb") as f:
        with alive_bar(metadata.num_reads) as bar:
            for _ in range(metadata.num_reads):

                data = s.read_until(b"\r\n")
                value = decode_line(data)

                # check if value is ``0xffffffff``
                if value == b"0xffffffff":
                    if default_counter >= 100:
                        print("Detached end of modified flash, exiting...")
                        break

                    default_counter += 1
                else:
                    default_counter = 0

                # write data to file
                f.write(value)

                # increment progressbar
                bar()



if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument("device", help="serial device to connect to",
                   default="/dev/ttyACM0", type=Path)
    p.add_argument("--baud", help="baud rate to use", default=15200)
    p.add_argument("--outfile", help="file to dump flash to",
                   default="playdate.bin", type=Path)
    p.add_argument("-f", "--force", action="store_true",
                   default=False, help="overwrite outfile if it already exists")
    args = p.parse_args()

    args.device: Path
    if not args.device.exists():
        print(f"Serial Device: {args.device} does not exist!")
        sys.exit(1)

    args.outfile: Path
    if args.outfile.exists() and not args.force:
        print(f"Outfile: {args.outfile} exists, and --force was not specified")
        sys.exit(1)

    # connect to serial device
    s = serial.Serial(str(args.device), args.baud)

    try:
        # all dump logic is in here
        dump_firmware(s, args.outfile)
        print(f"Done dumping firmware to {args.outfile}")

    finally:
        # clean up serial when done / somethig happens
        if not s.closed:
            s.close()
