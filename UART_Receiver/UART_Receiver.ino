enum State_enum {IDLES, RECORD_SAMPLES, SAMPLES_TO_BITS, RECOVER_DATA, CHECK_PARITY};
uint8_t state = IDLES;

// arduino uno
const byte RxPin = 2;
const byte TxPin = 3;
int count = 0;

byte messageBuffer[10] = {0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0};

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
  //  Serial.println("INTERRUPT");
  //  Serial.println("");
  if (count <= 9)
  {
    messageBuffer[count] = digitalRead(RxPin);
    count++;
  }
  else
  {
    state = RECOVER_DATA;
  }
}

void loop() {
  switch (state)
  {
    case IDLES:
      attachInterrupt(digitalPinToInterrupt(RxPin), StartBitFound, FALLING);
      break;

    case RECORD_SAMPLES:
      TIMSK1 |= (1 << OCIE1A);
      RecordData();
      break;

    case SAMPLES_TO_BITS:
      SampleToBits();
      break;

    case CHECK_PARITY:
      CheckParity();
      break;

    case RECOVER_DATA:
      TIMSK1 = 0;
      RecoverData();
      break;
  }
}

void StartBitFound()
{
  state = RECORD_SAMPLES;
}

void RecordData()
{
//  for (int i = 0; i <= 9; i++)
//  {
//    messageBuffer[i] = digitalRead(RxPin);
//    delay(1000);
//  }
//  state = RECOVER_DATA;
}

void RecoverData()
{
  Serial.println("DATA: ");
  for (int i = 1; i <= 8; i++)
  {
    Serial.print(messageBuffer[i]);
    Serial.println();
  }
  state = IDLES;
}

void SampleToBits()
{
  // not implemented
}

void CheckParity()
{
  // not implemented
}
