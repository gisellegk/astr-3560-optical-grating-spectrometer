int analogPin = A0;
int val = 0;
float voltage = 0;
void setup() {
  Serial.begin(9600);
  analogReference(EXTERNAL); // use external AREF pin
  pinMode(analogPin, INPUT);
  cli(); // stop interrupts
  // reset timer control registers
  TCCR1A = 0;
  TCCR1B = 0; 
  TCNT1 = 0; // initialize counter value to 0
  // set compare register
  OCR1A = 10000; // (16000000/(8*200) for 200 samples/sec
  TCCR1B = (1<<WGM12) | (1<<CS11);   // set prescaler to 8 (CS11) set operating mode to OCR1A (WGM12)
  TIMSK1 = (1<<OCIE1A);  // enable output compare match 
  sei(); // enable interrupts
}

void loop() {
  //
}

ISR(TIMER1_COMPA_vect){
  val = analogRead(analogPin); // read input pin
  voltage = val * (4.0 / 1023.0); // convert to voltage
  Serial.println(voltage,6);
}
