/*
  This example demonstrates how to use the SDFAT library for audio playback.
  Read time and program space are reduced by using SDFAT directly

  Requirements:
  The SDFAT library must be installed. See http://code.google.com/p/sdfatlib/
  The line #define SDFAT MUST be uncommented in pcmConfig.h

  "error: 'File' has not been declared" means you need to read the above text
*/

#include <SdFat.h> //"C:/Users/boema/Google Drive/nerd/dk7r soundboard/code/dk7r_soundboard/sdfat/src/SdFat.h"
SdFat sd;

#define SD_ChipSelectPin 4  //use digital pin 4 on arduino Uno, nano etc, or can use other pins
#include <TMRpcm.h> //"C:/Users/boema/Google Drive/nerd/dk7r soundboard/code/dk7r_soundboard/TMRpcm/TMRpcm.h"          //  also need to include this library...
#include <SPI.h>

volatile int gl_bounce_timer_count = 0;
volatile bool  button_press_start = false;




TMRpcm audio;   // create an object for use in this sketch

ISR(PCINT1_vect) {
    PCICR   = 0; // disable PCINT IRQs
    audio.play("8bpcm_zeuvm.wav");

    TCCR2B  = bit(CS22)|bit(CS21) |bit(CS20);   //prescaler = 1024 -> 1024/16Mhz = 64 us per tick. Enables the timer
   
    OCR2A   = 100;                              //64*100 us ~= 6.4 ms.

    char bla[30];
    //sprintf(bla, "Button data: 0x%02x", PINC);  // PINC is for reading, PORTC is for setting outputs
    sprintf(bla, "%05d", gl_bounce_timer_count);
    Serial.println(bla);


}

ISR(TIMER2_COMPA_vect){
    gl_bounce_timer_count += 1;

    // if we've had 6.4 ms * 50 = 320 ms, enable PCINT IRQs again
    if( gl_bounce_timer_count > 50 ) {
      PCIFR   = bit(PCIF1); // clear any PCINT flag that's still there.
      PCICR   = bit(PCIE1); // Enable PCINT IRQs
      gl_bounce_timer_count = 0;  // reset bounce timer
      TCCR2B   = 0;   // disable timer 2 clock
    }
}

void setup() {

  audio.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
  pinMode(10, OUTPUT); //Pin pairs: 9,10 Mega: 5-2,6-7,11-12,46-45

  Serial.begin(115200);
  Serial.println("Start");

  if (!sd.begin(SD_ChipSelectPin, SPI_FULL_SPEED)) {
    Serial.println("SD NOK");
    return;
  } else {
    Serial.println("SD OK");
  }

  
  
  audio.play("8bpcm_howtopay.wav");

  // START PIN CHANGE IRQ
  u8 buttonMask = bit(PCINT8) | bit(PCINT9) | bit(PCINT10) | bit(PCINT11);  //These are the pins that must generate IRQ
  PCICR   = bit(PCIE1); //enable pin change interrupts on PCINT8:14
  PCMSK1  = buttonMask; //But only on the pins we want.

  DDRC  = ~buttonMask;   //make certain portC pins an input (0 = input)
  PORTC = buttonMask;    //enable internal pullup
  // STOP PIN CHANGE IRQ

  // START TIMER SETUP
  // Een mechanische button stuitert op en neer, dit probeer ik hier mee weg te filteren. Zie https://en.wikipedia.org/wiki/Switch#Contact_bounce
  // basically zet ik de pin change interrupts uit voor x tijd (paar us-ms) zodra er eentje voor de eerste keer getriggerd wordt
  TCCR2B  = 0; // disable the timer2 clock 
  TCCR2A  = bit(WGM21);     // ctc timer, aka tel op tot timer = OCR2A en dan interrupt triggeren
  TIMSK2  = bit(OCIE2A);
  OCR2A   = 255;          //125 *6.4 ms wait after button press

  //STOP TIMER SETUP

  
  //char bar[30];
  //sprintf(bar, "buttonMask: 0x%02x", buttonMask);
  //Serial.println(bar);
  //Serial.print("ButtonMask: "); Serial.print(buttonMask,HEX); Serial.println("");
  //sei();
}


void loop() {

  if (!audio.isPlaying()) audio.disable();
  if (Serial.available()) {
    switch (Serial.read()) {
      case 'p':
        audio.play("8bpcm_sukakok.wav");
        Serial.println("playing p");
        //audio.disable();
        break;
      default:
        break;
    }
  }


}
