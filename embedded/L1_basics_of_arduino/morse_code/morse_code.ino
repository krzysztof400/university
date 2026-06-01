#define LED 13

volatile char serialInput;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  Serial.println("Enter a letter (a-z) to blink Morse code:");
}

void loop() {
  if (Serial.available() > 0) {
    serialInput = Serial.read();

    switch(serialInput) {
      case 'a': emit_morse("sl");   break; // .-
      case 'b': emit_morse("lsss"); break; // -...
      case 'c': emit_morse("lsls"); break; // -.-.
      case 'd': emit_morse("lss");  break; // -..
      case 'e': emit_morse("s");    break; // .
      case 'f': emit_morse("ssls"); break; // ..-.
      case 'g': emit_morse("lls");  break; // --.
      case 'h': emit_morse("ssss"); break; // ....
      case 'i': emit_morse("ss");   break; // ..
      case 'j': emit_morse("slll"); break; // .---
      case 'k': emit_morse("lsl");  break; // -.-
      case 'l': emit_morse("slss"); break; // .-..
      case 'm': emit_morse("ll");   break; // --
      case 'n': emit_morse("ls");   break; // -.
      case 'o': emit_morse("lll");  break; // ---
      case 'p': emit_morse("slls"); break; // .--.
      case 'q': emit_morse("llsl"); break; // --.-
      case 'r': emit_morse("sls");  break; // .-.
      case 's': emit_morse("sss");  break; // ...
      case 't': emit_morse("l");    break; // -
      case 'u': emit_morse("ssl");  break; // ..-
      case 'v': emit_morse("sssl"); break; // ...-
      case 'w': emit_morse("sll");  break; // .--
      case 'x': emit_morse("lssl"); break; // -..-
      case 'y': emit_morse("lsll"); break; // -.--
      case 'z': emit_morse("llss"); break; // --..
      case ' ': delay(1000);        break; // Gap for space
      case '\n':
      case '\r':                    break; // Ignore newlines
      default: Serial.println("?"); break;
    }
  }
}

void emit_morse(String morse_code) {
  for(int i = 0; i < morse_code.length(); i++) {
    char c = morse_code[i];

    if (c == 's') {
      digitalWrite(LED, HIGH);
      delay(250);
      digitalWrite(LED, LOW);
    }
    else if (c == 'l') {
      digitalWrite(LED, HIGH);
      delay(750);
      digitalWrite(LED, LOW);
    }
    delay(250); 
  }
  delay(500); 
}