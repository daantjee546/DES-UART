enum State_enum {IDLES, RECORD_SAMPLES, CHECK_SAMPLES, CHECK_PARITYS, RECOVER_DATA};
uint8_t state = IDLES;

const byte RxPin = 2;
const byte TxPin = 3;

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

      break;

    case CHECK_SAMPLES:
      Serial.println("CHECK_SAMPLES");
      delay(1000);
      break;

    case CHECK_PARITYS:

      break;

    case RECOVER_DATA:

      break;
  }
}

void StartBitFound()
{
  state = CHECK_SAMPLES;
}
