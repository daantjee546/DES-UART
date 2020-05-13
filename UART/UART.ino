enum State_enum {IDLES, START_BIT, REVERSE_DATA, PARITY_BIT, STOPT_BIT, SENDING};
uint8_t state = IDLES;

// ESP32
const int BaudRate = 9600;
const byte RxPin = 15; //15
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

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624 / 1667; // = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  //  TIMSK1 |= (1 << OCIE1A);
  TIMSK1 = 0;

  sei();//allow interrupts
}

ISR(TIMER1_COMPA_vect) { //timer1 interrupt 1Hz toggles pin 13 (LED)
  Serial.println("INTERRUPT");
  Serial.println("");
  if (count <= 9)
  {
    digitalWrite(TxPin, bufferbits[count]);
    Serial.println(bufferbits[count]);
    count++;
  }
  else
  {
    ResetBuffer();
  }
}

void loop() {
  switch (state)
  {
    case IDLES:
      //      cli();//stop interrupts
      Serial.println("IDLES");
      CheckBuffer();
      break;

    case START_BIT:
      Serial.println("START_BIT");
      AddStartBit();
      break;

    case REVERSE_DATA:
      Serial.println("REVERSE_DATA");
      ReverseData();
      break;

    case PARITY_BIT:
      AddParityBit();
      break;

    case STOPT_BIT:
      Serial.println("STOPT_BIT");
      AddStopBit();
      break;

    case SENDING:

      TIMSK1 |= (1 << OCIE1A);
      Serial.println("SENDING");
      SendData(bufferbits);
      //      ResetBuffer();
      //      TIMSK1 = 0;
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
  state = IDLES;
}

void SendData(byte input[])
{
  //  sei();//allow interrupts

  //  for (int i = 0; i <= 9; i++)
  //  {
  //    digitalWrite(TxPin, input[i]);
  //    Serial.println(input[i]);
  //    delay(1000);
  //  }
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
