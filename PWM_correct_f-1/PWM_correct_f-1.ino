#define TM_BITS 254


String inString = "";    // string to hold input
unsigned long g = 0;


void setup() {
  pinMode (9, OUTPUT);    // Timer 1 "B" output: OC2B
  pinMode (10, OUTPUT);   // Timer 1 "A" output: OC2A

  // Set OC1A on Compare Match when up-counting.
  // Clear OC1B on Compare Match when up-counting.
  TCCR1A =  bit (COM1B1) | bit (COM1A1) | bit (COM1A0);       
  TCCR1B = bit (WGM13) | bit (CS10);         // phase correct PWM, prescaler of 1
  ICR1H = 0;
  ICR1L = TM_BITS;
  OCR1A = 125;           
  OCR1B = 121;
  
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }
  Serial.println("Input PWM frequency:");
  Serial.println();

}

void set_freq(unsigned long freq){
  unsigned long top = round(8000000/freq); //where 8000000 = f_clk / 2
  unsigned long temp = 0;
  if (top > TM_BITS){
     top = TM_BITS; // avoid overrange
  }
  if (top < 10){
    top = 10;  // avoid overrange
    }
      ICR1L = top; 
      temp = round(top/2);
      OCR1A = temp;
      OCR1B = temp - 1;
      temp = 0;
  
  }

void loop() {
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      inString += (char)inChar;
    }
    if (inChar == '\n') {
      Serial.println(inString.toInt());
      g = inString.toInt();
      set_freq(g);
      inString = "";
    }
  }
}
