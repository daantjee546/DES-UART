enum State_enum {IDLES, START_BIT, REVERSE_DATA, PARITY_BIT, STOPT_BIT, SENDING};
uint8_t state = IDLES;

const byte RxPin = 3;
const byte TxPin = 2;

byte bufer[10] = {0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0};
byte message[8];
byte bufferbits[10];

bool received = false;

int count = 0;

void setup() {
  Serial.begin(9600);
  pinMode(TxPin, OUTPUT);
  pinMode(RxPin, INPUT);

  cli();//stop interrupts

  TCCR1A = 0;
  TCCR1B = B00001001;
  TCNT1  = 0;
  OCR1A = 1665.66666667;
  //OCR1A = 159;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 = 0;

  sei();//allow interrupts
}

ISR(TIMER1_COMPA_vect) {
  if (count <= 9)
  {
    digitalWrite(TxPin, bufferbits[count]);
    Serial.println(bufferbits[count]);
    count++;
  }
  else
  {
    TIMSK1 = 0;
    ResetBuffer();
  }
}

void loop() {
  switch (state)
  {
    case IDLES:
      count = 0;
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
      TIMSK1 |= (1 << OCIE1A); // interrupt is used for sending
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
  TIMSK1 = 0;
  byte bufer[10] = {0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0};
  delay(12500);
  state = IDLES;
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
