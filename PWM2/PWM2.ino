#define TM_BITS 254
#define DUTY_SCALER 2.54

String inString = "";    // string to hold input
unsigned long g = 0;
char mode; //if mode == T => setup frequency 
           //if mode == S => setup duty
bool in_mode = false;

void setup() {
  pinMode (9, OUTPUT);    // Timer 1 "B" output: OC1B
  pinMode (10, OUTPUT);   // Timer 1 "A" output: OC1A
  pinMode (3, OUTPUT);      // Timer 2 "B" output: OC2B
  
//configure 2nd timer at FASTPWM mode (output is pin 3)
// Set OC2B on Compare Match when up-counting.
  TCCR2A = bit(COM2B1)|bit(WGM21)|bit(WGM20);
  TCCR2B = bit(CS20); //prescaler of 1
  OCR2B = 150;  // ini lvl of pwm (1-254)




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


void set_duty(int duty){ // 0..100
  duty = round(duty*DUTY_SCALER); 
  if (duty > TM_BITS){
    duty = 254;
  }
  if (duty < 1){
    duty = 1;
    }
  OCR2B = duty; 
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
    else if (inChar != '\n')
    {
      mode = inChar;
      in_mode = true;
      if (mode == 'T'){
          Serial.print("Input frequency: ");
          in_mode = false;
        }
        if (mode == 'S'){
          Serial.print("Input duty: ");
        }
    }
    
    if ((inChar == '\n')&&(!in_mode)) 
    {
      Serial.print("Current frequency: ");
      Serial.print(inString.toInt());
      Serial.println(" Hz");
      g = inString.toInt();
      set_freq(g);
      inString = "";
    }
    if ((inChar == '\n')&&(in_mode))
    {
      Serial.print("Current duty: ");
      Serial.print(inString.toInt());
      Serial.println(" %");
      g = inString.toInt();
      set_duty(g);
      inString = "";
    }
  }
}
