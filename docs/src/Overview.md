# Overview

This aims to delve into the actual play.date console platform itself, including
(but not limited to) the game build process, the onboard operating system, and
the peripherals on board the console, the things with no existing documentation.

## Previous Work

Most of the past work has been focused on the API, the overal structure of the
packaged game files, and the soundtracks, not too much has been focused on
platform introspection itself. At a high level a lot of the work is already laid
out for us a la [play.date reversing](https://github.com/jaames/playdate-reverse-engineering),
but the things it ignores - the underlying runtime, the api for the games
themselves, and how the play.date *is* the play.date. That still needs to be
uncovered.

Also i want a web browser on it lol.

## Is any of this even allowed?

Yeah basically.

The big "don't"'s of the policy are:

```
You will not:
    - Modify, disassemble, or decompile any part of the SDK;
    - Distribute or transfer the SDK to others (other than the incorporation of distributable elements of the SDK in Your Developed Programs in accordance with the terms of this Agreement);
    - Modify, adapt, alter, translate, or incorporate into or with other software or create a derivative work of any part of the SDK, except as permitted herein, without express written permission from Panic;
    - Use the SDK to develop applications for other platforms or to develop another SDK, without express written permission from Panic.
```

The other fun one not in the initial bullets:

```
"[You will not] Use the SDK to create, develop, or use any program, software, or service which (1) contains any viruses, Trojan horses, worms, or other computer programming routines that are intended to damage, detrimentally interfere with, surreptitiously intercept, or expropriate any system, data, or personal information; (2) when used in the manner in which it is intended, violates any applicable law, statute, ordinance, or regulation (including without limitation the laws and regulations governing export control, unfair competition, anti-discrimination, false advertising, or data privacy); (3) infringes the intellectual property rights of third parties; or (4) interferes with the operability of other Panic or third-party programs or software; "
```


### Addressing the `don't`'s

    - No changes are being made to the SDK, theres nothing of interest in the SDK to disassemble or decompile (the only things we could would be `pdutils` and `pdc`).
    - The SDK is not being redistributed so othing to worry about
    - The SDK is not being modified
    - The SDK is not being used to develop for another platform or "develop another SDK"
    - not doing any of that illegal stuff
