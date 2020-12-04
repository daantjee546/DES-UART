enum State_enum {IDLES, RECORD_SAMPLES, RECOVER_DATA, CHECK_PARITY};
uint8_t state = IDLES;

const byte RxPin = 2;
const byte TxPin = 3;
int count = 0;

byte messageBuffer[10] = {0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0};
byte sampleOne[10] = {0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0};
byte sampleTwo[10] = {0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0};
byte sampleThree[10] = {0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0};

int currentSample = 1;

void setup() {
  Serial.begin(9600);
  pinMode(TxPin, OUTPUT);
  pinMode(RxPin, INPUT);

  cli();//stop interrupts

  TCCR1A = 0;
  TCCR1B = B00001001;
  TCNT1  = 0;
  OCR1A = 554.55555655;
  //OCR1A = 52.33;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 = 0;

  sei();//allow interrupts
}

ISR(TIMER1_COMPA_vect) {
  if (count <= 9)
  {
    switch (currentSample)
    {
      case 1:
        sampleOne[count] = digitalRead(RxPin);
        currentSample++;
        break;

      case 2:
        sampleTwo[count] = digitalRead(RxPin);
        currentSample++;
        break;

      case 3:
        sampleThree[count] = digitalRead(RxPin);
        currentSample = 1;
        count++;
        break;

      default:
        break;
    }
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
      TIMSK1 |= (1 << OCIE1A); // interrupt is used for record samples
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

void RecoverData()
{
  Serial.println("DATA: ");
  for (int i = 0; i <= 9; i++)
  {
    Serial.print(sampleOne[i]);
    Serial.print(sampleTwo[i]);
    Serial.print(sampleThree[i]);
    Serial.println();
    Serial.println();
  }
  count = 0;
  state = IDLES;
}

void CheckParity()
{
  // not implemented
}
