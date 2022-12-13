# Pico-W_Multicore

This is a demonstration of the use of the Arduino-Pico (Raspberry Pi Pico Arduino core) super-simple multicore implementation.


### What's the secret?

To use both cores on the RP2040 processor, you only need to know two function calls:-
+ `setup1()`
+ `loop1()`

Yup, if the Arduino-Pico core comes across `setup1()` function call, it uses the code within that function to initialize core-1 of the processor.

When it finds `loop1()` it uses the code there as the application code specifically for core-1.

As usual, the normal `setup()` and `loop()` function calls are used by core-0, which is the default behaviour for an Arduino application and effectively limits your Pico to being a single core processor.


### What do I need?

You should have access to some version of the RP2040 hardware (the Arduino-Pico core supports lots of different third-party boards, as well as the mainstream Pico and Pico-W boards).

You can use this new functionality if you are using the Arduino-Pico (Raspberry Pi Pico Arduino core).  I happen to be using it from PlatformIO, but you can just as easily use it directly from the Arduino IDE.

### Where do I find the Arduino-Pico core?

The [Arduino-Pico core respository is available on GitHub](https://github.com/earlephilhower/arduino-pico).

There is also a ton of documentation available, [including this installation guide](https://arduino-pico.readthedocs.io/en/latest/install.html#).


### Notes on the demo.


#### How do I use it?

First of all, you need to add your access-point SSID and password at the top of the src/multicore_demo.ino file.

Using whatever system you prefer (PlatformIO, Arduino IDE, etc), compile the code and load it to you Pico/Pico-W board.
+ The board will normally reset and run the new firmware on completion of the upload.
+ Once the board starts to initialize, the on-board LED will come on for 5-seconds.
  - This delay is to allow you time to connect your terminal program (screen, putty, etc) via USB.
+ At the end of the 5-second delay, the demo will begin to display start-up messages from core-0.
```
         === Multicore Demo ===

Core-0: On-Line.

Connecting to AccessPoint1

IP number assigned by DHCP is 192.168.1.10
Core-1: On-Line.

 ==>> Push BOOTSEL button to trigger WiFi scan.
```
  - Note that core-1 doesn't come on-line at the same time as core-0.  We use a delay at the start of `setup1()` to ensure that the WiFi can initialize, first.
+ The initial start-up messages end with the instruction to push the BOOTSEL button.

The BOOTSEL button is actually monitored by core-1.   If you push and release BOOTSEL, the core-1 handler will load a fifo to signal core-0 that it should begin a WiFi network scan.

Core-0 also loads a fifo to notify core-1 that it should run a simple fade-up/fade-down sequence on the on-board LED.  So, while core-0 is running the WiFi scan, core-1 is concurrently running the LED fader.

```
Core-1: Registered button press...

Core-0: Initiating WiFi scan...

Core-1: Received a start-fader command...

Beginning scan at 1630368
Found 3 networks

                            SSID   ENC     BSSID         CH RSSI
                  Buffalo-A-6928  AUTO 18:C2:FB:18:D1:16  7  -97
                    AccessPoint1  WPA2 30:5A:A3:C7:82:10  6  -54
                 Free_Range_Eggs  NONE 68:FF:B7:6A:69:AB  1  -88
---
```
At this point everything goes back to waiting for further BOOTSEL events.


#### Pico vs Pico-W hardware

I have to admit that I don't have a non-WiFi Pico model, so I don't know how well (or badly) one will react to the request for a WiFi scan.  If it all falls over in a heap, then you can probably just completely remove the WiFi.h include and replace the WiFi scan with a simple message from core-0 instead.

You will notice that the src directory has a pwm.ino file, in addition to the multicore_demo.ino.  This was prompted by the fact that the on-board LED on the Pico-W is not attached to an RP2040 GPIO pin, but to one of the wireless module's pins.  This means that it doesn't have PWM functionality and the normal `analogWrite()` call cannot be used.  The (crude) code in pwm.ino is just a very simple up/down fader using nothing more than on/off writes to the pin with some very short delays between them.  The calls to `delay()` are, of course, blocking, but as this is running solely on core-1, it does not impact the WiFi scan process, which runs concurrently on core-0.
If nothing else, this does provide a real-life genuine example of the multitasking capability of the RP2040 (and yes, I know it's still a really crappy fader!).
