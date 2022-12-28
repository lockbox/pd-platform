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
