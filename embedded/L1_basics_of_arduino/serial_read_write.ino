#define LED 13

volatile char serialInput;
volatile int delayTime;


void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  delayTime = 1000;
}

void loop() {
  while(Serial.available())
  {
    serialInput = Serial.read();
    switch(serialInput)
    {
      case 's' : Serial.println("szybciej");
                  delayTime -= 250;
                  break;
      case 'w' : Serial.println("wolniej"); 
                  delayTime += 250;
                  break;
      default  : Serial.println("?");
    }
  }
  doTheBlinking(3);   // blokujÄce!
  Serial.println(".");

}


void doTheBlinking(int times) {
  for(int i=0; i<times; i++) {
    digitalWrite(LED, HIGH);
    delay(delayTime);
    digitalWrite(LED, LOW);    
    delay(delayTime);
  }  
}