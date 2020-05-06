enum State_enum {IDLES, START_BIT, REVERSE_DATA, PARITY_BIT, STOPT_BIT, SENDING};
uint8_t state = IDLES;

// ESP32
const int BaudRate = 9600;
const byte RxPin = 15;
const byte TxPin = 2;

byte bufer[10] = {0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0};
byte message[8];
byte bufferbits[10];

bool received = false;

void setup() {
  Serial.begin(9600);
  pinMode(TxPin, OUTPUT);
  pinMode(RxPin, INPUT);
}

void loop() {
  switch (state)
  {
    case IDLES:
      CheckBuffer();
      break;

    case START_BIT:
      AddStartBit();
      break;

    case REVERSE_DATA:
      ReverseData();
      break;

    case PARITY_BIT:
      AddParityBit();
      break;

    case STOPT_BIT:
      AddStopBit();
      break;

    case SENDING:
      SendData(bufferbits);
      ResetBuffer();
      break;
  }
}

void AddStartBit()
{
  bufferbits[0] = 0b0;

  state = REVERSE_DATA;
}

void AddStopBit()
{
  bufferbits[9] = 0b1;

  state = SENDING;
}

void AddParityBit()
{
  // not implemented
}

void ReverseData()
{
  for (int i = 0; i <= 8; i++)
  {
    message[i] = (bufer[0] >> i) & 0b1; //first byte in bufer only
  }
  for (int i = 1; i <= 8; i++)
  {
    bufferbits[i] = message[i - 1];
  }

  state = STOPT_BIT;
}

void ResetBuffer()
{
  byte bufer[10] = {0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0};
  state = IDLES;
}

void SendData(byte input[])
{
  for (int i = 0; i <= 9; i++)
  {
    digitalWrite(TxPin, input[i]);
    Serial.println(input[i]);
    delay(1000);
  }
}

void CheckBuffer()
{
  digitalWrite(TxPin, HIGH);
  for ( int i = 0; Serial.available() > 0; i++)
  {
    byte incomingByte = Serial.read();
    if (incomingByte != 10) {
      bufer[i] = incomingByte;
      received = true;
      delay(100);
    }
  }
  if (received)
  {
    state = START_BIT;
    received = false;
  }
}
