enum State_enum {IDLES, RECORD_SAMPLES, SAMPLES_TO_BITS, RECOVER_DATA, CHECK_PARITY};
uint8_t state = IDLES;

const byte RxPin = 2;
const byte TxPin = 3;

byte messageBuffer[10] = {0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0, 0b0};

void setup() {
  Serial.begin(9600);
  pinMode(TxPin, OUTPUT);
  pinMode(RxPin, INPUT);
}

void loop() {
  switch (state)
  {
    case IDLES:
      attachInterrupt(digitalPinToInterrupt(RxPin), StartBitFound, FALLING);
      break;

    case RECORD_SAMPLES:
      RecordData();
      break;

    case SAMPLES_TO_BITS:
      SampleToBits();
      break;

    case CHECK_PARITY:
      CheckParity();
      break;

    case RECOVER_DATA:
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
  for (int i = 0; i <= 9; i++)
  {
    messageBuffer[i] = digitalRead(RxPin);
    delay(1000);
  }
  state = RECOVER_DATA;
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
