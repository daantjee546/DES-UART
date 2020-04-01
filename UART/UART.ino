enum State_enum {IDLES, START_BIT, REVERSE_DATA, PARITY_BIT, STOPT_BIT, SENDING};
uint8_t state = IDLES;

const int BaudRate = 9600;
const byte RxPin = 2;
const byte TxPin = 3;

byte message = 0B1111000;

void setup() {
  Serial.begin(9600);
  pinMode(TxPin, OUTPUT);
  pinMode(RxPin, INPUT);
}

void loop() {
  switch (state)
  {
    case IDLES:

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

      break;
  }
}

void AddStartBit(byte data)
{
  
}

void AddStopBit()
{
  
}

void AddParityBit()
{
  
}

void ReverseData()
{
  
}

void SendData(byte input)
{
  // startbit
  // data
  // stopbit
}
