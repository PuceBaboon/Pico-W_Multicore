# Pico-W_Multicore
This is a demonstration of the use of the Arduino-Pico (Raspberry Pi Pico Arduino core) super-simple multicore implementation.

To use both cores on the RP2040 processor, you only need to know two function calls:-
+ setup1()
+ loop1()

Yup, if the Arduino-Pico core comes across setup1() function call, it uses the code within that function to initialize core-1 of the processor.

When it finds loop1() it uses the code there as the application code specifically for core-1.

As usual, the normal setup() and loop() function calls are used by core-0, which is the default behaviour for an Arduino application and effectively limits your Pico to being a single core processor.
