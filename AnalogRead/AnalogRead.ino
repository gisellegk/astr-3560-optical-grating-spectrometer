int analogPin = A0;
int val = 0;
float voltage = 0;
void setup() {
  Serial.begin(9600);
  analogReference(EXTERNAL); // use external AREF pin
  pinMode(analogPin, INPUT);
}

void loop() {
  val = analogRead(analogPin); // read input pin
  voltage = val * (4.0 / 1023.0); // convert to voltage
  Serial.println(voltage,6);
  delay(100);
}
