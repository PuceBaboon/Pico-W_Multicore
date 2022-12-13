# Pico-W_Multicore

This is a demonstration of the use of the Arduino-Pico (Raspberry Pi Pico Arduino core) super-simple multicore implementation.

### What's the secret?

To use both cores on the RP2040 processor, you only need to know two function calls:-
+ setup1()
+ loop1()

Yup, if the Arduino-Pico core comes across setup1() function call, it uses the code within that function to initialize core-1 of the processor.

When it finds loop1() it uses the code there as the application code specifically for core-1.

As usual, the normal setup() and loop() function calls are used by core-0, which is the default behaviour for an Arduino application and effectively limits your Pico to being a single core processor.

### What do I need?

You should have access to some version of the RP2040 hardware (the Arduino-Pico core supports lots of different third-party boards, as well as the mainstream Pico and Pico-W boards).

You can use this new functionality if you are using the Arduino-Pico (Raspberry Pi Pico Arduino core).  I happen to be using it from PlatformIO, but you can just as easily use it directly from the Arduino IDE.

### Where do I find the Arduino-Pico core?

The [Arduino-Pico core respository is available on GitHub](https://github.com/earlephilhower/arduino-pico).

There is also a ton of documentation available, [including this installation guide](https://arduino-pico.readthedocs.io/en/latest/install.html#).
