# Blueduino-RC4
*Bluetooth 4.0 LTE Remote Control for Zoom H4n Recorders*
*(c)2014 Greg Schoppe CC-Attribution*
        
for Blueduino 8Mhz/3.3v or similar Arduino compatible
board that supports 3.3v power and serial,
and Bluetooth 4.0 LTE.
  
### NECESSARY CONNECTION:
    Connector: 2.5mm (3/32") TRRS Jackplug
    Pinout:
      Tip     Pin1/RX1
      Ring1   Pin0/TX0
      Ring2   GND
      Sleeve  VCC(3.3v)

### IMPORTANT NOTES:
1. You **MUST** use a 3.3v Arduino variant and power it from the 3.3v connection with the H4n.  You **WILL** destroy your H4n if you power the board with 5v or connect the 3.3v line to an unregulated power source.
2. This Sketch requires the use of the Arduino's Standard Serial Connection, making debugging difficult.  It could be modified to communicate with the RC4 using a second instance of SoftwareSerial on Pins 8 and 9, but might have trouble with concurrent reads blocking each other.