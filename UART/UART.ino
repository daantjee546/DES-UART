enum State_enum {IDLES, START_BIT, REVERSE_DATA, PARITY_BIT, STOPT_BIT, SENDING, STOP};
uint8_t state = IDLES;

const int BaudRate = 9600;
const byte RxPin = 0;
const byte TxPin = 1;

byte message[8] = {0b0, 0b1, 0b0, 0b0, 0b0, 0b0, 0b1, 0b0}; // B

byte bufferbits[10];

void setup() {
  Serial.begin(9600);
  pinMode(TxPin, OUTPUT);
  pinMode(RxPin, INPUT);
  Serial.println("START....");
}

void loop() {
  switch (state)
  {
    case IDLES:
      state = START_BIT;
      break;

    case START_BIT:

      bufferbits[0] = 0b0;


      state = REVERSE_DATA;
      break;

    case REVERSE_DATA:

      for (int i = 1; i <= 8; i++)
      {
        bufferbits[i] = message[i - 1];
      }

      state = STOPT_BIT;
      break;

    case PARITY_BIT:

      break;

    case STOPT_BIT:

      bufferbits[9] = 0b1;

      state = SENDING;
      break;

    case SENDING:

      //      for (int i = 0; i <= 9; i++)
      //      {
      //        Serial.println(bufferbits[i]);
      //      }

      SendData(bufferbits);

      state = STOP;

      break;

    case STOP:

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

void SendData(byte input[])
{
  for (int i = 0; i <= 9; i++)
  {
    digitalWrite(TxPin, input[i]);
    Serial.println(input[i]);
    delay(1000);
  }
  Serial.println("END....");
  Serial.println();
}
