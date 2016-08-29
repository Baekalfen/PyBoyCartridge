# PyBoyCartridge

Abstract
========
Following the [PyBoy](https://github.com/baekalfen/pyboy) project, which covered emulation of the Nintendo Game Boy, this project continues the development from another perspective.
Emulation is the process of mimicking an existing system. To achieve a good or perfect emulation, one has to probe the existing system to find details, which might not have been documented. This could be an odd behavior, which has become part of the system, but not from the intention of the original design.
This project takes on the aspect of physically interacting with a Game Boy, to advance the development of the emulator. The project will first focus dumping of game cartridges, by connecting it to a microcontroller and sequentially ask for each byte it stores. Afterwards, the process gets turned around to connect a microcontroller to the Game Boy as if it was a cartridge. This will open the possibility of running test code on the Game Boy to pinpoint the undocumented behavior.


Status
======
All the code could use a bit of streamlining, but it works as described in the [report](https://github.com/Baekalfen/PyBoyCartridge/raw/master/Report.pdf).

The cartridge-dumper seems to be working flawlessly for MBC 1,2 and 3. I haven't made any illustration of how to connect all the wires, but the code should be fairly self-explanatory. If somebody requests it, I'll write up some details on how to get started -- and don't hold back, just create an issue :)

Emulating cartridges still needs work - see the [report](https://github.com/Baekalfen/PyBoyCartridge/raw/master/Report.pdf). The short explanation is, that I couldn't get the code to trigger on the interrupt-pin. But the code is ready-to-go, if somebody gets it to be triggered at the right time. Because of the very slim reaction time, I wrote a few lines of AVR-assembler, but it's fairly straight forward. You possibly could get the same optimization in C, but I didn't want the uncertainty.

