#define EnA 10   // white
#define EnB 5   // yellow
#define In4 6   // green
#define In3 7   // blue
#define In2 13  // purple
#define In1 12  // grey

volatile char serialInput;
volatile int delayTime;


void setup() {
  pinMode(EnA, OUTPUT);
  pinMode(EnB, OUTPUT);
  pinMode(In4, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In1, OUTPUT);

  
  Serial.begin(9600);
}

void loop() {
  analogWrite(EnA, 2);
  analogWrite(EnB, 2);

  digitalWrite(In1, HIGH);
  digitalWrite(In3, HIGH);
  digitalWrite(In2, LOW);
  digitalWrite(In4, LOW);
}