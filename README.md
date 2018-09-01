# wipi - a wireless multiprotocol thing

This is just a toy I was building to play around with various ESP8266
bits.

I've been developing this on an ESP8266 wemos D1 type modue.

# Protocols

All of the protocols are selected by connecting to port 23, entering
the letter that indicates the protocol you wish to use, and then
proceeding to do whatever protocol-specific junk you want to get done.

## `s` serial

Simple TTL serial protocol.  Wire TX and RX to your device, and you're
good to go.  The protocol is parameterized with a baud rate, so you
send the desired baud rate in base 10 after s and you should start
having stuff flowing back and forth.

e.g.:

    $ nc 192.168.1.13 23
    # Hello.  Please select a protocol.
    s9600
    # found serialprotocol, initializing...
    # Baud rate set to 9600
    $GNRMC,,V,,,,,,,,,,N*4D
    $GNVTG,,,,,,,,,N*2E
    $GNGGA,,,,,,0,00,99.99,,,,,,*56
    $GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*2E

## `a` analog

The analog protocol just spits out an integer value representing the
current analog reading from `A0` on the ESP8266.  Wire up a sensor as
you normally would and start reading values from it.

## `1` 1wire

The 1wire support is currently high level and narrowly focused.
Haven't figured out what I want to do yet, but it's a sort of repl
with a few subcommands:

* `l` - list devices
* `s` - select a device (by hex addr)
* `t` - get a temp reading for a given device.

e.g.:

    $ nc 192.168.1.13 23
    # Hello.  Please select a protocol.
    1
    # found 1wire protocol, initializing...
    l
    28FF7A84641603D0
    found 1
    s28FF7A84641603D0
    t
    26.44

## `g` GPIO

GPIO has a few subcommands:

* `0`-`8` - select `D0` - `D8` to operate on
* `o` - set the currently selected pin to `OUTPUT`
* `i` - set the currently selected pin to `INPUT`
* `p` - set the currently selected pin to `INPUT_PULLUP`
* `+` - set the currently selected pin high
* `-` - set teh currently selected pin low

## `w` WS2812

WS2812 controls WS28212 LEDs.  Do your normal wiring, connect, tell
the controller how many LEDs you've got, then have at it.

Subcommands:

* `l` - select a LED to operate upon
* `r` - set the red value of the current LED to the next decimal value (0-255)
* `g` - set the green value of the current LED to the next decimal value (0-255)
* `b` - set the blue value of the current LED to the next decimal value (0-255)
* `s` - show / refresh the strip's colors.


e.g. for a strip with four LEDs where you want to set the second one to pink:

    $ nc 192.168.1.13 23
    # Hello.  Please select a protocol.
    w4
    # found ws2812 protocol, initializing...
    l1r255g32b32s

## `h` DHT humidity/temp sensors

Connect and specify a pin the DHT is connected to.  Any input
refreshes the readings.

e.g.:

    $ nc 192.168.1.13 23
    # Hello.  Please select a protocol.
    h5
    # found dht22 protocol, initializing...
    26.70% 27.90C
    
    26.20% 27.90C
