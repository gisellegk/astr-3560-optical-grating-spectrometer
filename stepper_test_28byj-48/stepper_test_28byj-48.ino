#define NUM_STATES 8

int states[NUM_STATES][4] = { {0,1,1,1},
                              {0,0,1,1},
                              {1,0,1,1}, 
                              {1,0,0,1},
                              {1,1,0,1},
                              {1,1,0,0},
                              {1,1,1,0},
                              {0,1,1,0} };


                     
int currentState = 0; // 0-3, index of state array
int dir = 1; // 1 = fwd 3 = backwards
int numSteps = 0;

String user_input_str;

void setup() {
  // put your setup code here, to run once:
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  Serial.begin(9600);
  Serial.print("Begin motor control");
  Serial.println();
}

void loop() {
  Serial.println("Direction?");
  Serial.println("1=FWD 7=BWD\n");
  while(!Serial.available()) {}
  user_input_str = Serial.readString();
  dir = user_input_str.toInt();
  if(dir != 1 && dir != NUM_STATES-1) return;
  
  Serial.println("Num steps?");
  Serial.println("Enter an integer");
  while(!Serial.available()) {}
  user_input_str = Serial.readString();
  numSteps = user_input_str.toInt();

  while(numSteps > 0) {
    Serial.println(numSteps);
    stepMotor();
    numSteps--;

  }
  Serial.println("Done.");
  digitalWrite(4, 0);
  digitalWrite(5, 0);
  digitalWrite(6, 0);
  digitalWrite(7, 0);
  

}

void stepMotor(){
  digitalWrite(4, states[currentState][0]);
  digitalWrite(5, states[currentState][1]);
  digitalWrite(6, states[currentState][2]);
  digitalWrite(7, states[currentState][3]);
  delay(2);
  currentState+=dir;
  currentState = currentState % NUM_STATES;
}
