//program for controlling AY-3-8910 Programmable Sound Generator
HardwareTimer timer(2); // defining timer 2. Channel1 on PA0

#define BDIR PB7 
#define BC1 PB4
#define BC2 PB6

#define DA0 PB15
#define DA1 PA8
#define DA2 PA9
#define DA3 PA10
#define DA4 PA11
#define DA5 PA12
#define DA6 PA15
#define DA7 PB3

#define resetPSGpin PB14

#define led PB11

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin led as an output.
  pinMode(led, OUTPUT);

  //initialize 1 - 2mhz clock for PSG
  clock_setup();

  //initialize bus control pins and set to INACTIVE mode
  pinMode(BC2, OUTPUT);
  pinMode(BDIR, OUTPUT);
  pinMode(BC1, OUTPUT);
  digitalWrite(BDIR, LOW);
  digitalWrite(BC1, LOW);
  digitalWrite(BC2, HIGH);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second

  //Example 
  //write_reg(6);
  //write_reg_value(8); //8 would be 00001000

}
void bus_control(int function){
  //pull BC2 pin high(+5v)
  //control BDIR, BC1 pins to specify bus functions
  //four functions:
  //INACTIVE
  //READ from PSG
  //WRITE to PSG
  //LATCH address
  switch(function){
    case 0: //inactive
      digitalWrite(BDIR, LOW);
      digitalWrite(BC1, LOW);
      break;
    case 1: //read
      //set DA pins to input mode
      pinMode(DA0, INPUT);
      pinMode(DA1, INPUT);
      pinMode(DA2, INPUT);
      pinMode(DA3, INPUT);
      pinMode(DA4, INPUT);
      pinMode(DA5, INPUT);
      pinMode(DA6, INPUT);
      pinMode(DA7, INPUT);
      //set bus control pins to READ state
      digitalWrite(BDIR, LOW);
      digitalWrite(BC1, HIGH);
      
      break;
    case 2: //write
      //set DA pins to output mode
      pinMode(DA0, OUTPUT);
      pinMode(DA1, OUTPUT);
      pinMode(DA2, OUTPUT);
      pinMode(DA3, OUTPUT);
      pinMode(DA4, OUTPUT);
      pinMode(DA5, OUTPUT);
      pinMode(DA6, OUTPUT);
      pinMode(DA7, OUTPUT);
      //set bus control pins to WRITE state
      digitalWrite(BDIR, HIGH);
      digitalWrite(BC1, LOW);
      
      break;
    case 3: //latch
      //set DA pins to output mode
      pinMode(DA0, OUTPUT);
      pinMode(DA1, OUTPUT);
      pinMode(DA2, OUTPUT);
      pinMode(DA3, OUTPUT);
      pinMode(DA4, OUTPUT);
      pinMode(DA5, OUTPUT);
      pinMode(DA6, OUTPUT);
      pinMode(DA7, OUTPUT);
      //set bus control pins to LATCH state
      digitalWrite(BDIR, HIGH);
      digitalWrite(BC1, HIGH);

      break;
    default: 
      break;
  }

}
void write_reg(int reg){
  //select one of the 16 registers to be latched 
  //cases will set pins DA0 to DA3 to the correct binary value representing register number
  //before switch set pins DA4 to DA7 all low
  digitalWrite(DA4, LOW);
  digitalWrite(DA5, LOW);
  digitalWrite(DA6, LOW);
  digitalWrite(DA7, LOW);

  switch(reg){

    //Tone Generator Control 
    case 0:
      //Fine Tune Channel A
      digitalWrite(DA0, LOW);
      digitalWrite(DA1, LOW);
      digitalWrite(DA2, LOW);
      digitalWrite(DA3, LOW);
      break;
    case 1:
      //Coarse Tune Channel A
      digitalWrite(DA0, HIGH);
      digitalWrite(DA1, LOW);
      digitalWrite(DA2, LOW);
      digitalWrite(DA3, LOW);
      break;
    case 2:
      //Fine Tune Channel B
      digitalWrite(DA0, LOW);
      digitalWrite(DA1, HIGH);
      digitalWrite(DA2, LOW);
      digitalWrite(DA3, LOW);
      break;
    case 3:
      //Coarse Tune Channel B
      digitalWrite(DA0, HIGH);
      digitalWrite(DA1, HIGH);
      digitalWrite(DA2, LOW);
      digitalWrite(DA3, LOW);
      break;
    case 4:
      //Fine Tune Channel C
      digitalWrite(DA0, LOW);
      digitalWrite(DA1, LOW);
      digitalWrite(DA2, HIGH);
      digitalWrite(DA3, LOW);
      break;
    case 5:
      //Coarse Tune Channel C
      digitalWrite(DA0, HIGH);
      digitalWrite(DA1, LOW);
      digitalWrite(DA2, HIGH);
      digitalWrite(DA3, LOW);
      break;
    
    //Noise Generator Control
    case 6:
      //Noise Period
      digitalWrite(DA0, LOW);
      digitalWrite(DA1, HIGH);
      digitalWrite(DA2, HIGH);
      digitalWrite(DA3, LOW);
      break;
    
    //Mixer Control-IO Enable
    case 7:
      digitalWrite(DA0, HIGH);
      digitalWrite(DA1, HIGH);
      digitalWrite(DA2, HIGH);
      digitalWrite(DA3, LOW);
      break;
    
    //Amplitude Control
    case 10:
      //Channel A
      digitalWrite(DA0, LOW);
      digitalWrite(DA1, HIGH);
      digitalWrite(DA2, LOW);
      digitalWrite(DA3, HIGH);
      break;
    case 11:
      //Channel B
      digitalWrite(DA0, HIGH);
      digitalWrite(DA1, HIGH);
      digitalWrite(DA2, LOW);
      digitalWrite(DA3, HIGH);
      break;
    case 12:
      //Channel C
      digitalWrite(DA0, LOW);
      digitalWrite(DA1, LOW);
      digitalWrite(DA2, HIGH);
      digitalWrite(DA3, HIGH);
      break;

    //Envelope Generator Control
    case 13:
      //Evelope Fine Tune Control
      digitalWrite(DA0, HIGH);
      digitalWrite(DA1, LOW);
      digitalWrite(DA2, HIGH);
      digitalWrite(DA3, HIGH);
      break;
    case 14:
      //Evelope Coarse Control
      digitalWrite(DA0, LOW);
      digitalWrite(DA1, HIGH);
      digitalWrite(DA2, HIGH);
      digitalWrite(DA3, HIGH);
      break;
    case 15:
      //Evelope Cycle/Shape Control
      digitalWrite(DA0, HIGH);
      digitalWrite(DA1, HIGH);
      digitalWrite(DA2, HIGH);
      digitalWrite(DA3, HIGH);
      break;
    default:
      printf("Specify a register");
      break;
  }
  //use bus_control function to latch the specified register address to PSG
  bus_control(3);
}
void write_reg_value(uint8_t value){
  //Input 8 bit value to add to register

  //uint8_t can be 0 to 255
  //use bitRead() in a for look to read all 8 bytes of value
  //if bit < 1 
    //set DA pin(i) to low 
  for(int i = 0; i < 8; i++){
    int bitValue = bitRead(value, i);
    if (bitValue < 1){
      int pins[] = { DA0, DA1 , DA2, DA3, DA4, DA5, DA6, DA7};
      digitalWrite(pins[i],LOW);
    }
  }
 

  //use bus_control function to write data to currently addressed register 
  bus_control(2);
}
uint8_t read_reg_value(){
  //read current value from currently latched register
  //set pins DA0 to DA7 to input 
  //digitalRead all pins and store this in value

  bus_control(1);

  uint8_t regValue;

  for(int i = 0; i < 8; i++){
      regValue = regValue + digitalRead(i);
  }
  return regValue;
}
void clock_setup(){
  //clock needs to be 1 mhz to 2 mhz with 50% duty cycle 
  timer.pause();
  timer.setPeriod(1);
  timer.setChannel1Mode(TIMER_OUTPUT_COMPARE);
  timer.setCompare(TIMER_CH1, 1);
  timer.refresh();
  timer.resume();
  
}
void resetPSG(){
  //set reset pih LOW to set all registers to zero
}
