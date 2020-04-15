enum State_enum {IDLES, START_BIT, REVERSE_DATA, PARITY_BIT, STOPT_BIT, SENDING, STOP};
uint8_t state = IDLES;

const int BaudRate = 9600;
const byte RxPin = 2;
const byte TxPin = 3;

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
      break;

    case START_BIT:

      bufferbits[0] = 0b0;


      state = REVERSE_DATA;
      break;

    case REVERSE_DATA:
      for (int i = 0; i <= 8; i++)
      {
        message[i] = (bufer[0] >> i) & 0b1; //first byte in bufer only
      }
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
      //            for (int i = 0; i <= 9; i++)
      //            {
      //              Serial.println(bufferbits[i]);
      //            }

      SendData(bufferbits);

      byte bufer[10] = {0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0};
      state = IDLES;

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
}
