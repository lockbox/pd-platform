# Binary Structure

TLDR; the compiled binary looks like this (Note that address `0x0` is actually `0x60000000` on hardware):

![layout.svg](./layout.png)

What this means is that:
- the first word in the binary ust be a pointer to our "event handler"
- the second word points to the start of our program globals
- the third word points to the end of program globals