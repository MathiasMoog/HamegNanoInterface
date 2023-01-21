# Serial Interface for HAMEG HM205-3 Analog/Digital Oscilloscope

I still use my old HAMEG HM205-3 scope. It has a socket for an digital interface, but the interface itself was missing when I got the scope. Here is my solution, a serial interface based on an Arduino Nano.

## Electical Connections

The Arduino Nano is mounted on a small double layer matrix board together with the 2 x 13 female header for the  connection to the scope.

There is no 8 bit parallel port available at the Arduino Nano with the ATMEGA 328 chip. I splitted it in two 4 bit reads on Port B (lower nibble) and port D (upper nibble). Details in the table below.

### Connection table between Scope and Nano
  
    Signal  Port  Arduino HM 205  pinMode  Hameg notation   Connection   Remarks
    D0      PB0   8       3       Input                     Direct
    D1      PB1   9       4       Input                     Direct
    D2      PB2   10      5       Input                     Direct
    D3      PB3   11      6       Input                     Direct
    D4      PD4   4       7       Input                     Direct
    D5      PD5   5       8       Input                     Direct
    D6      PD6   6       9       Input                     Direct
    D7      PD7   7       10      Input                     Direct
  
    CHSEL   PC5   A5      17      In/Output                 330 Ohm       1
    CLR     PC0   A4      18      Output    RESAZ           330 Ohm
    CLK     PC3   A3      19      Output    CLAC            330 Ohm
    SQR     PC2   A2      20      Output                    330 Ohm       1
    TE      PC1   A1      22      Input     LSING           330 Ohm       1
    Reset   PC0   A0      23      Output    CLRSING         330 Ohm       1
  
    GND           GND     1,25,26  
    5 V           5 V     11                                Diode, anode hameg -> kathode Arduino   
	
	Remark 1: These signals are not used at the moment. If I find some time I will extend it.
  
    I soldered resistors for all connections to port C since some of the signals might be input or output.
   
    The Arduino Nano is powered by the USB port, or from the scope via the diode if there is no USB connection.

## 3D printed housing 

Design with OpenScad in HamegNanoCover.scad.

## Arduino Sketch

Use the Arduin IDE to compile and upload the sketch. If you like

## Serial Protocoll

The Nano accepts the following commands:
- a  Ascii output mode, human readable output
- b  Binary output mode, faster for M2M communication
- s  status, printe status of all signals at port C, see below
- r1  readout, one channel, 2048 Points
- r2  readout, one channel, 2 x 2048 Points

In ascii mode, on decimal number per line, in binary mode block with 2048 or 4096 raw bytes.

You might use my Octave Code for communication, see my OctaveLab repository at GitHub, https://github.com/MathiasMoog/OctaveLab


## Literature

- Hameg 205-3 manual
- Hendrik Klassssen, Die Symbiose, Atari ST als Controller für Hameg-Oszilloskope, Elrad 10/91, S. 41 ff.

