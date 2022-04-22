// Timer: https://microcontrollerslab.com/arduino-timer-interrupts-tutorial/

#define TIMER1_COMP 10000
#define NUM_DATA_PTS 400
#define STEP_RESOLUTION 4096
#define NUM_MOTOR_STATES 8

// motor states
int motor_states[NUM_MOTOR_STATES][4] = { {0,1,1,1},
                              {0,0,1,1},
                              {1,0,1,1}, 
                              {1,0,0,1},
                              {1,1,0,1},
                              {1,1,0,0},
                              {1,1,1,0},
                              {0,1,1,0} };
// state machine state
String state = "idle";
String input;
int numSamples = 0;
int sampleStop = 1;

int dir = 1; // FWD = 1 BWD = 7
int currentMotorState = 0; // index of state array 

int analogPin = A0;
int val = 0;
float voltage = 0;

int data[NUM_DATA_PTS];
int ddd = 0;

void setup() {
  Serial.begin(9600);
  analogReference(EXTERNAL); // use external AREF pin
  pinMode(analogPin, INPUT);
  configureSampleTimer();
}

void loop() {
  if(state == "idle") {
    Serial.println("Type an integer to begin a conversion of that length");
    while(!Serial.available()) {}
    input = Serial.readString();
    sampleStop = input.toInt();
    state = "start";
  } else if (state == "start") {
    // Start timers
    ddd = 0;
    startDataLog();
    state = "wait";
  } else if ( state == "wait") {
    // waiting for conversion to complete
  } else if (state == "stop"){
    stopDataLog();
    for(int i = 0; i < NUM_DATA_PTS; i++){
      Serial.print(numSamples); // sample ID 
      Serial.print(",");
      Serial.print(i*5); // timestamp. unit = ms
      Serial.print(",");
      voltage = data[i] * (4.0 / 1023.0);
      Serial.println(voltage,6); 
    }
    if(numSamples >= sampleStop) {
      state = "idle";
      numSamples = 0;      
    } else {
      state = "turn";
      numSamples++;
    }

  } else if (state == "turn") {
    Serial.println("Turning Motor...");
    for(int i = 0; i < STEP_RESOLUTION; i++){
      stepMotor();
    }
  } else {
    state = "idle";
  }
  
}

ISR(TIMER1_COMPA_vect){
  val = analogRead(analogPin); // read input pin
//  voltage = val * (4.0 / 1023.0); // convert to voltage
  data[ddd] = val; // voltage;
  ddd++;
  if(ddd >= NUM_DATA_PTS) state = "stop";
 // Serial.println(voltage,6);
  OCR1A = TIMER1_COMP;
}

// This timer controls the sample rate that the data is taken at
void configureSampleTimer(){
  cli(); // stop interrupts
  // reset timer control registers
  TCCR1A = 0;
  TCCR1B = 0; 
  
  // set compare register
  OCR1A = TIMER1_COMP; // (16000000/(8*200) for 200 samples/sec
  TCCR1B = (1<<WGM12) | (1<<CS11);   // set prescaler to 8 (CS11) set operating mode to OCR1A (WGM12)
  sei();
}

void startDataLog(){
  cli();
  TCNT1 = 0; // initialize counter value to 0
  TIMSK1 = (1<<OCIE1A);  // enable output compare match 

  sei(); // enable interrupts
}

void stopDataLog(){
  cli();
  TIMSK1 &= ~(1<<OCIE1A);  // enable output compare match 
  sei(); // enable interrupts
  
}

/******************
 *  STEPPER MOTOR * 
 *******************/

 void setupMotor(){
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
 }
void stepMotor(){
  digitalWrite(4, motor_states[currentMotorState][0]);
  digitalWrite(5, motor_states[currentMotorState][1]);
  digitalWrite(6, motor_states[currentMotorState][2]);
  digitalWrite(7, motor_states[currentMotorState][3]);
  delay(5);
  currentMotorState+=dir;
  currentMotorState = currentMotorState % NUM_MOTOR_STATES;
}
