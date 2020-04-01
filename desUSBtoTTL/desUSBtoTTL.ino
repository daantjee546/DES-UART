#include <SoftwareSerial.h>

SoftwareSerial ReceivingSerial(2, 3); // RX, TX

enum State_enum {IDLES, START_BIT, REVERSE_DATA, PARITY_BIT, STOPT_BIT, SENDING};

uint8_t state = IDLES;

void setup() {
  // Open serial communications and wait for port to open:
//  Serial.begin(115200);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for Native USB only
//  }
//
//  Serial.println("Serial is connected");

  //Serial.begin(9600, SERIAL_8N1);
  Serial.begin(9600);
}

void loop() {
  switch (state)
  {
    case IDLES:
//      if (Serial.available())
//      {
        state = SENDING;
//      }
      break;

    case START_BIT:

      break;

    case REVERSE_DATA:

      break;

    case PARITY_BIT:

      break;

    case STOPT_BIT:

      break;

    case SENDING:
    Serial.print("B");
      //Serial.write('A'); // Will transmit the value 65
      break;
  }
}
