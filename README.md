# Poppy
## Small eFidget, binary counter
At its core, Poppy is just a binary counter. The four LEDs count in binary, wrapping every 15 numbers. Each Cherry MX switch increases the master value by a binary value, with switch 1 increasing the value by 1, switch 2 by 2, 3 by 4, and 4 by 8. The rotary encoder can also increase or decrease the master value, and pressing it down resets the master value to 0.

I also added a repeat with initial delay function, where holding a switch will repeat inputs every 100 milliseconds, with an initial delay of 300 milliseconds.


![PCB Front screenshot](https://hc-cdn.hel1.your-objectstorage.com/s/v3/ea767daa5c2a288a4a60b11b75582d17a64d592c_screenshot_2025-07-04_203537.png)
