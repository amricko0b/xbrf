# xbrf
Yet another wireless gamepad adapter. It allows you to connect Xbox gamepad to your PC.

## Disclaimer
**Original idea (as well as code for Adrduino UNO) by Alex Martinez and dilandou - they rock!
Unfortunatelly, I couldn't find any articles or instructions by original authors (if there were any).** 

I just reworked code from [this post](https://www.instructables.com/Diy-Xbox-wireless-controller-adapter-for-Pc/) for ATTiny microcontroller.

**DON'T TRY THIS IF YOU HAVE NEVER WORKED WITH ELECTRIC CIRCUITS OR MICROCONTROLLERS!!!**

**Working with electricity without any expierience can damage your health or your devices (e.g. PC).**

## Prerequisites
1) A ready-2-use board with all components placed. PCB layout can be found [here](https://www.youtube.com/watch?v=IZtWN3-4vjI) in description
2) XBox 360 RF Shield (scavenged from useless console)
3) XBox gamepad

If you microcontroller hacker:
4) The AVR-GCC compiler and standard library for AVR microconrollers
5) An AVR programmer of your choice (usbASP, in my case)
6) The avrdude promgramming utility (and maybe some GUI for it)

And if you aren't:
4) Install [PlatfomIO](https://platformio.org/).
5) Create project for ATTiny13A and place source files at suitable directories.
6) Follow instructions on how to build and flash

## How to reproduce?
0) Make any wanted changes in source code at your own risk
1) Build firmware HEX file
2) Flash microcontroller
3) Install in on board
4) Connect USB plug

## Things to improve

While this piece of code was written in DIY manner, there are some stuff you can change
* Decrease or increase warm-up delay time at your convenience
* Use INT0 instead of PCINT0 for data tansmition (circuit schematics must be changed)
* Use another external interrupt for button (circuit schematics must be changed)
