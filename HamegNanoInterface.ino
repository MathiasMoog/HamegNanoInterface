/* Hameg HM 205-3 Interface with an Arduino Nano.
 *
 *  Arduino Nano Interface for my Hameg 205-3 Scope. 
 *
 *  See readme.md for further details
 *
 *  Copyright, 2023, Mathias Moog, Deutschland, CC-BY-NC-SA
 */

// -------------------------------------------------------------------------------
// Pin Mappings

// Data Pins, in right order
const byte Dpins[8] = { 8, 9, 10, 11, 4, 5, 6, 7 };
// Other Pins
#define CHSEL A5
#define CLR A4
#define CLK A3
#define SQR A2
#define TE A1
#define RESET A0

// -------------------------------------------------------------------------------
// Ascii or binary protocoll, here default settings, change with a = ascii and b = binary

boolean asciiOutput = true;

// -------------------------------------------------------------------------------
// Inline helper functions

// parallel read data pins, see table above
inline byte parallelRead() {
  return (PINB & 0x0f) | (PIND & 0xf0);
}

// set CLR
inline void clr(const boolean v) {
  digitalWrite(CLR, v);
}

// set CLK
inline void clk(const boolean v) {
  digitalWrite(CLK, v);
}

// Set SQR
inline void sqr(const boolean v) {
  digitalWrite(SQR, v);
}

// Set TE
inline boolean te() {
  return digitalRead(TE);
}

// disable all outputs, set to input
inline void disableOutput() {
  pinMode(CLR, INPUT);
  pinMode(CLK, INPUT);
  pinMode(SQR, INPUT);
  // Momentan noch nicht genutzt, erst mal als Input
  pinMode(CHSEL, INPUT);
  pinMode(RESET, INPUT);
  // Normale Inputs ebenfalls setzen
  pinMode(TE, INPUT);
}

// enable all outputs, set to output and to default value
inline void enableOutput() {
  pinMode(CLR, OUTPUT);
  clr(HIGH);
  pinMode(CLK, OUTPUT);
  clk(LOW);
  pinMode(SQR, OUTPUT);
  sqr(LOW);
}

// -------------------------------------------------------------------------------
// Setup

void setup() {
  Serial.begin(115200);
  if (asciiOutput) {
    Serial.println("HM205-3 Scope Interface");
    Serial.println("Activate 'STOR' befor reading and data.");
  }

  // Configure pins for parallel input
  for (byte i = 0; i < 8; i++) pinMode(Dpins[i], INPUT);
  // Configure remaining pins, Port C, see above, set to input per default
  disableOutput();
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// -------------------------------------------------------------------------------
// Loop

void loop() {
  // Wait for command
  if (Serial.available()) {
    char command = Serial.read();
    // command switch
    switch (command) {
      case 'a':  // Switch to ascii output
        asciiOutput = true;
        break;
      case 'b':  // Switch to binary output
        asciiOutput = false;
        break;
      case 'r':  // Read Data
        readData(Serial.parseInt());
        break;
      case 's':  // Status
        {
          const byte s = PINC;
          if (asciiOutput) {
            Serial.print("Status ");
            Serial.println(s, BIN);
            Serial.print(", CHSEL ");
            Serial.print(bitRead(s, 5));
            Serial.print(", CLR ");
            Serial.print(bitRead(s, 4));
            Serial.print(", CLK ");
            Serial.print(bitRead(s, 3));
            Serial.print(", SQR ");
            Serial.print(bitRead(s, 2));
            Serial.print(", TE ");
            Serial.print(bitRead(s, 1));
            Serial.print(", TE (digitalRead) ");
            Serial.print(te());
            Serial.print(", RESET ");
            Serial.print(bitRead(s, 0));
            Serial.println();
            Serial.print("Value ");
            Serial.println(parallelRead());
          } else {
            Serial.write(s);
          }
        }
        break;
    };  // End Switch
  }     // end if
}  // End loop

// -------------------------------------------------------------------------------
// Read Data

/* Read one or two channels
 *  HM 205-3 has 2048 bytes per channel.
 *  If only one channel is active and two channels should be read, the first channel ist repeated.
 */
void readData(byte channel) {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  if (asciiOutput) {
    Serial.print("Start Read, channel = ");
    Serial.println(channel);
  }
  const int n = (channel == 1) ? 2048 : 4096;  // Number of bytes to read
  enableOutput();
  delay(1);
  // Set SQR to HIGH level, want to read ...
  sqr(HIGH);
  delayMicroseconds(10);
  // Reset Counter
  clr(LOW);
  delayMicroseconds(10);
  clr(HIGH);
  delayMicroseconds(5);
  // Read Data and Output to Serial Port
  for (int i = 0; i < n; i++) {
    // todo check clock defintion and timing
    clk(HIGH);
    delayMicroseconds(1);
    const uint8_t d = parallelRead();
    if (asciiOutput) {
      Serial.println(d, DEC);
    } else {
      Serial.write(d);
    }
    clk(LOW);
    delayMicroseconds(1);
  }
  // Set SQR back to LOW
  sqr(LOW);
  delay(1);
  // disable all outputs, set to input
  disableOutput();
  digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
}  // End of readData
