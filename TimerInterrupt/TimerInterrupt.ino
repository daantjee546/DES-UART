const int ledPin = 13;
volatile byte state = LOW;
boolean toggle1 = 0;
int count = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  cli();//stop interrupts

  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624; // = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts
}

void loop() {
  //digitalWrite(ledPin, state);
//  cli();//stop interrupts
//  delay(2500);
//  sei();//allow interrupts
delay(5000);
TIMSK1 = 0;
}


ISR(TIMER1_COMPA_vect) { //timer1 interrupt 1Hz toggles pin 13 (LED)
  //generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  count++;
    if (toggle1) {
      digitalWrite(ledPin, HIGH);
      toggle1 = 0;
//      Serial.println("ON");
    }
    else {
      digitalWrite(ledPin, LOW);
      toggle1 = 1;
//      Serial.println("OFF");
    }
    Serial.println(count);
}
