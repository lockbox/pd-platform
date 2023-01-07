# Game Console

We know that new games get delivered over Wi-Fi, and that
there is an ESP8266 [onboard (pg. 2)](https://fccid.io/2AR29-PDU01/Internal-Photos/Internal-Photos-4801627.pdf), but there is also no interface to communicate
with it via the play.date SDK, so we'll have to do a little digging and try to
look at the underlying operating system.

To do that we're going to need to write some code to figure out where the
OS is stored, where the heap is, where code objects are, which flash banks are being used, whch hardware is being used etc.

We do know that the [ESP8266 is connected via UART](https://github.com/jaames/playdate-reverse-engineering/blob/main/usb/usb.md#esp), and we also know that
there is a [peripheral accelerometer](https://sdk.play.date/1.12.3/Inside%20Playdate%20with%20C.html#_accelerometer)
onboard as well (and soon a [stereo dock](https://play.date/stereo-dock/), though
they redid the FCC filing for it in [December 2022](https://fccid.io/2AR29-PDDY01) so we probably have to wait a bit).

- [Valid Memory](./ValidMemory.md)
- [Connected Peripherals](./peripherals.md)
- [Game Console overview]()
