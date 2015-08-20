//  *****************************************************************************************************************
//  *                                                                                                               *
//  *                                         SpikenzieLabs.com                                                     *
//  *                                                                                                               *
//  *                                           Drum Kit - Kit                                                      *
//  *                                                                                                               *
//  *                                                                                                               *
//  *****************************************************************************************************************
//
//  BY: MARK DEMERS Copywrite 20009
//  April. 2009
//  VERSION: 1.b
//
//  DESCRIPTION:
//  Arduino analog input used to sense piezo drum hits then sent serialy to processing.
//  
//  Required - Hardware:
//  1. Drum kit - kit (From SpikenzieLabs.com)
//  2. Arduino
//
//  Required - Software:
//  1. Serial MIDI converter
//  2. Garage Band, Ableton Live etc ...
//
// LEGAL:
// This code is provided as is. No guaranties or warranties are given in any form. It is your responsibilty to 
// determine this codes suitability for your application.




//*******************************************************************************************************************
// User settable variables			
//*******************************************************************************************************************

unsigned char PadNote[5] = {42,33,35,50,31};         // MIDI notes from 0 to 127 (Mid C = 60)

int PadCutOff[5] = {100,600,600,100,100};           // Minimum Analog value to cause a drum hit

int MaxPlayTime[5] = {90,90,90,90,90};               // Cycles before a 2nd hit is allowed

#define  midichannel	0;                              // MIDI channel from 0 to 15 (+1 in "real world")

boolean VelocityFlag  = true;                           // Velocity ON (true) or OFF (false)





//*******************************************************************************************************************
// Internal Use Variables			
//*******************************************************************************************************************

boolean activePad[5] = {0,0,0,0,0};                   // Array of flags of pad currently playing
int PinPlayTime[5] = {0,0,0,0,0};                     // Counter since pad started to play

unsigned char status;

int pin = 0;     
int hitavg = 0;

//*******************************************************************************************************************
// Setup			
//*******************************************************************************************************************

void setup() 
{
  Serial.begin(115200);                                  // connect to the serial port 115200
}

//*******************************************************************************************************************
// Main Program			
//*******************************************************************************************************************

void loop() 
{
  for(int pin=0; pin < 5; pin++)
  {
    hitavg = analogRead(pin);                              // read the input pin

    if((hitavg > PadCutOff[pin]))
    {
      if((activePad[pin] == false))
      {
        if(VelocityFlag == true)
        {
          hitavg = 127 / ((1023 - PadCutOff[pin]) / (hitavg - PadCutOff[pin]));    // With full range (Too sensitive ?)
          //hitavg = (hitavg / 8) -1 ;                                                 // Upper range
        }
        else
        {
          hitavg = 127;
        }

        MIDI_TX(144,PadNote[pin],hitavg); 
        PinPlayTime[pin] = 0;
        activePad[pin] = true;
      }
      else
      {
        PinPlayTime[pin] = PinPlayTime[pin] + 1;
      }
    }
    else if((activePad[pin] == true))
    {
      PinPlayTime[pin] = PinPlayTime[pin] + 1;
      
      if(PinPlayTime[pin] > MaxPlayTime[pin])
      {
        activePad[pin] = false;
        MIDI_TX(128,PadNote[pin],127); 
      }
    }
  } 
}


//*******************************************************************************************************************
// Transmit MIDI Message			
//*******************************************************************************************************************
void MIDI_TX(unsigned char MESSAGE, unsigned char PITCH, unsigned char VELOCITY) 
{
  status = MESSAGE;
  Serial.write(status);
  Serial.write(PITCH);
  Serial.write(VELOCITY);
}



