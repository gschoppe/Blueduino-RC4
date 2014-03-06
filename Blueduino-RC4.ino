#include <SoftwareSerial.h>
/*
                   -=Blueduino-RC4=-
  Bluetooth 4.0 LTE Remote Control for Zoom H4n Recorders
        -(c)2014 Greg Schoppe CC-Attribution-
        
  for Blueduino 8Mhz/3.3v or similar Arduino compatible
  board that supports 3.3v power and serial,
  and Bluetooth 4.0 LTE.
  
  NECESSARY CONNECTION:
    Connector: 2.5mm (3/32") TRRS Jackplug
    Pinout:
      Tip     Pin1/RX1
      Ring1   Pin0/TX0
      Ring2   GND
      Sleeve  VCC(3.3v)
  
  IMPORTANT NOTES:
  1) You MUST use a 3.3v Arduino variant and power it from
  the 3.3v Connection with the H4n.  You WILL destroy
  your H4n if you power the board with 5v or connect the
  3.3v line to an unregulated power source.
  
  2) This Sketch requires the use of the Arduino's Standard
  Serial Connection, making debugging difficult.  It could
  be modified to communicate with the RC4 using a second
  instance of SoftwareSerial on Pins 8 and 9, but might
  have trouble with concurrent reads blocking each other.
*/
#define BT_TX_PIN  = 12;
#define BT_RX_PIN  = 11;

// RC4 COMMANDS
// -HANDSHAKE (tx, rx, tx, rx)
byte RC4_HANDSHAKE[] = {0x00, 0x80, 0xA1, 0x80};
// -OUTGOING (2-byte Bitfields)
byte RC4_RELEASE[]   = {0x80, 0x00}; // 1000 0000 0000 0000 | Sent when any button is released
byte RC4_MIC[]       = {0x80, 0x01}; // 1000 0000 0000 0001 | Select Onboard or miniplug Mic as Input
byte RC4_LINE1[]     = {0x80, 0x02}; // 1000 0000 0000 0010 | Select Line1 XLR/TRS as Input
byte RC4_LINE2[]     = {0x80, 0x04}; // 1000 0000 0000 0100 | Select Line2 XLR/TRS as Input
byte RC4_VOLUP[]     = {0x80, 0x08}; // 1000 0000 0000 1000 | Increase Playback Volume
byte RC4_VOLDOWN[]   = {0x80, 0x10}; // 1000 0000 0001 0000 | Decrease Playback Volume
byte RC4_RECUP[]     = {0x80, 0x20}; // 1000 0000 0010 0000 | Increase Recording Levels
byte RC4_RECDOWN[]   = {0x80, 0x40}; // 1000 0000 0100 0000 | Decrease Recording Levels
// byte RC4_UNKNOWN1[]  = {0x80, 0x80}; // 1000 0000 1000 0000 | UNKNOWN (perhaps left empty to make first byte easily identifiable)
byte RC4_REC[]       = {0x81, 0x00}; // 1000 0001 0000 0000 | Arm for Recording
byte RC4_PLAY[]      = {0x82, 0x00}; // 1000 0010 0000 0000 | Trigger Play/Pause
byte RC4_STOP[]      = {0x84, 0x00}; // 1000 0100 0000 0000 | Stop Playback/Recording
byte RC4_NEXT[]      = {0x88, 0x00}; // 1000 1000 0000 0000 | Next Track
byte RC4_PREV[]      = {0x90, 0x00}; // 1001 0000 0000 0000 | Previous Track
// byte RC4_UNKNOWN2[]  = {0x80, 0x80}; // 1010 0000 0000 0000 | UNKNOWN (perhaps unimplemented)
// byte RC4_UNKNOWN3[]  = {0x80, 0x80}; // 1100 0000 0000 0000 | UNKNOWN (perhaps unimplemented)
// -INCOMING (1-byte bitfields)
byte RC4_LED_REC     = 0x01; // 0000 0001
byte RC4_LED_MIC_R   = 0x02; // 0000 0010
byte RC4_LED_LINE1_R = 0x04; // 0000 0100
byte RC4_LED_LINE2_R = 0x08; // 0000 1000
byte RC4_LED_MIC_G   = 0x10; // 0001 0000
byte RC4_LED_LINE1_G = 0x20; // 0010 0000
byte RC4_LED_LINE2_G = 0x40; // 0100 0000
byte RC4_LED_IGNORE  = 0x80; // 1000 0000
// 
bool RC4_initialize() {
    unsigned long timeout = millis() + 10000;
    if(Serial) Serial.end();
    Serial.begin(2400);
    while (!Serial) {
        if(millis() > timeout) return false;
    }
    byte reply = NULL;
    do {
        if(Serial.available()) {
            reply = Serial.read();
        } else {
            Serial.write(RC4_HANDSHAKE[0]);
            delay(200);
        }
        if(millis() > timeout) return false;
    } while(reply != RC4_HANDSHAKE[1]);
    Serial.write(RC4_HANDSHAKE[2]);
    reply = NULL;
    do {
        if(Serial.available()) {
            reply = Serial.read();
        }
        if(millis() > timeout) return false;
    } while(reply != RC4_HANDSHAKE[3]);
    return(true);
}
void RC4_send(byte button[]) {
    Serial.write(button[0]);
    Serial.write(button[1]);
    // hold and release the button
    delay(10);
    Serial.write(RELEASE[0]);
    Serial.write(RELEASE[1]);
}
byte RC4_getLEDbyte() {
    byte lastByte = RC4_LED_IGNORE;
    byte temp;
    // we'll likely build up a buffered back log.
    // Just trash it and grab the last meaningful
    // byte
    while(Serial.available()) {
        temp = Serial.read();
        if(temp != RC4_LED_IGNORE)
            lastByte = temp;
    }
    return lastByte;
}

SoftwareSerial btSerial(BT_RX_PIN, BT_TX_PIN);
String tmp;

void setup() {
    RC4_initialize();
    btSerial.begin(9600);
}

void loop() {
    if (btSerial.available()) {
        Serial.write(btSerial.read());
    }
    
}