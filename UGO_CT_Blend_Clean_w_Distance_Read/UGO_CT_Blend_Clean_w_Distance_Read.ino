#include <stdlib.h>
#include <math.h>

// **HIGH -> means off for normal relays
// **LOW -> means on for normal relays
// ** HIGH -> means on for Solid State relays
// ** LOW -> means off for Solid State relays 
// ** Assumming "hot" -> actuator goes down
// ** Assuming "neutral" -> actuator goes up to orginal position 

//Distance Calibration Measurments
int TopPosition = 145;//Old:125
int TopOfCup = 310; //Old:280 300
int TopOfSmoothie= 340; //Old:310
int BottomOfCup = 415; //Old:390 //410
int BottomOfCleaning = 405;//Old:420
int CleaningLevel = 358; //Old:350


int Read[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  // Array to hold Actuator Portentiometer Reads
int ValRef = 0;  // holder value for use during BubbleSort

const int Potentiometer = A1;    // Actuator Potentiometer Pin

const int pinButtonBlend = 2; 
const int pinButtonClean = 3;
const int pinWaterPump = 6;
const int ActuatorHot = 7; 
const int ActuatorNeutral = 8; 
const int pinLED_READY = 9;
const int pinLED_IN_USE = 10;
const int pinBlender = 12; //normal relay
const int pinBlenderSSR = 31; // solid state relay 

const int START= 14;

int ValButtonClean = LOW;

int ValButtonBlend = LOW;


void setup() {
  //Serial.begin(9600);
  pinMode(Potentiometer, INPUT);
  pinMode(pinButtonBlend, INPUT);
  pinMode(pinButtonClean, INPUT);
  
  pinMode(ActuatorHot, OUTPUT);
  digitalWrite(ActuatorHot, HIGH);
  
  pinMode(ActuatorNeutral, OUTPUT);

  Unjam();
  if (MeasureDistance() > TopPosition){     // is blender at top?
    digitalWrite(ActuatorNeutral, LOW);
    
  }
  else
  {
    digitalWrite(ActuatorNeutral, HIGH);
  }
  
  pinMode(pinBlenderSSR, OUTPUT);
  pinMode(pinBlender, OUTPUT);
  TurnBlenderOff ();
  
  pinMode(pinLED_READY, OUTPUT);
  digitalWrite(pinLED_READY, LOW);
  
  pinMode(pinLED_IN_USE, OUTPUT);
  digitalWrite(pinLED_IN_USE, LOW);
  
  //Redundant 
  pinMode(pinButtonBlend, INPUT);
  pinMode(pinButtonClean, INPUT);
  //
  
  pinMode(pinWaterPump, OUTPUT);
  digitalWrite(pinWaterPump, HIGH);


  
  while (MeasureDistance() > TopPosition){
    delay(1);   //move to top position
  }
  digitalWrite(ActuatorNeutral, HIGH);
  digitalWrite(pinLED_READY, HIGH);   // status ready

}

void TurnBlenderOn () {

  digitalWrite(pinBlenderSSR, HIGH);
  delay(20);
  digitalWrite(pinBlender, LOW);
   
}

void TurnBlenderOff () {

  digitalWrite(pinBlenderSSR, LOW);
  delay(50);
  digitalWrite(pinBlender, HIGH);

}

int MeasureDistance()  
// Measure distance of actuator - takes 15 reads to compensate for inaccuracy
{
  for (int i = 0; i < 15; i++)
  {
    Read [i] = analogRead(Potentiometer);
    delay(1); 
  }
  BubbleSort();  // sort entries 
  return Read[7];
  // send middle read value which is supposed to be the intended distance
}

void BubbleSort ()      
// Bubble sort algorithm to arrange an array of length 15 in ascending order
{
  for (int i = 0; i < 14; i++)
  {
    for (int j = i+1 ; j>0; j--)
    if (Read[j] < Read[j-1])      // element out of order
    {
      ValRef = Read[j];  // tempory hold value j
      Read[j] = Read[j-1];  // replace j with higher entry
      Read[j-1] = ValRef;  // replace other entry
    }
  }
}

void CustomPulses(int Time, int cycles, int Delay, int offset)
//blending sequence for custome Time, number of cycles, delay between pulses, and offset between going down and up 
  {
    digitalWrite(ActuatorNeutral, HIGH);  
    digitalWrite(ActuatorHot, HIGH);  
    for(int i = 0; i < cycles; i++)
    {
      digitalWrite(ActuatorHot, LOW);       //plunge down
      delay (Time + offset);
      digitalWrite(ActuatorHot, HIGH);
  
      delay(Delay);
  
      digitalWrite(ActuatorNeutral, LOW);     //plunge up
      delay (Time - offset);
      digitalWrite(ActuatorNeutral, HIGH);
  
      delay(Delay);
      
    }
    digitalWrite(ActuatorNeutral, HIGH);  
    digitalWrite(ActuatorHot, HIGH);  
  }

//Jack Implementation : Not Being Used
void RaiseBlenderFromMiddleToTop () {

digitalWrite(ActuatorNeutral, LOW);

delay(100);

digitalWrite(ActuatorNeutral, HIGH);

}
//Jack Implementation : Not Being Used
void PlungeBlenderToBottomFromTop () {

digitalWrite(ActuatorHot, LOW);

delay(160);

digitalWrite(ActuatorHot, HIGH);

}
//

void Unjam(){
//unjam actuator if stuck
   digitalWrite(ActuatorNeutral, LOW);
   delay(50);
   digitalWrite(ActuatorHot, LOW);
   delay(25);
   digitalWrite(ActuatorNeutral, HIGH);
   delay(25);
   digitalWrite(ActuatorHot, HIGH);
   delay(50);
   digitalWrite(ActuatorNeutral, LOW);
   delay(25);
   digitalWrite(ActuatorHot, LOW);
   delay(25);
   digitalWrite(ActuatorNeutral, HIGH);
   delay(25);
   digitalWrite(ActuatorHot, HIGH);
}

//Not Being Used
void MediumPulses() {
  //make medium pulses for blender actuator (1.65 secs)
  digitalWrite(ActuatorNeutral, LOW);   // reverse actuator
  digitalWrite(ActuatorHot, HIGH);
  delay(20);
  digitalWrite(ActuatorNeutral, HIGH);   // actuator down
  delay(10); // pause to reduce impact
  digitalWrite(ActuatorHot, LOW);
  delay(5);
  digitalWrite(ActuatorHot, HIGH);
  delay(5); // pause to reduce impact
  digitalWrite(ActuatorNeutral, LOW);   // reverse actuator
  delay(20);
  digitalWrite(ActuatorHot, LOW);
  delay(50); // power both actuator leads to prevent jamming
  digitalWrite(ActuatorNeutral, HIGH);   // actuator down
  delay(10);
  digitalWrite(ActuatorNeutral, LOW);  // power both actuator leads to prevent jamming
  delay(50); // pause to reduce impact
  digitalWrite(ActuatorHot, HIGH); // reverse actuator
  delay(20);
  digitalWrite(ActuatorNeutral, HIGH);   // actuator down
  delay(10); // pause to reduce impact
  digitalWrite(ActuatorHot, LOW);
  delay(60);
  digitalWrite(ActuatorNeutral, LOW);   // reverse actuator
  delay(20); // pause to reduce impact
  digitalWrite(ActuatorHot, HIGH);
  delay(50);
  digitalWrite(ActuatorNeutral, HIGH);   // actuator down
  delay(5); // pause to reduce impact
  digitalWrite(ActuatorHot, LOW);
  delay(60);
  digitalWrite(ActuatorHot, HIGH);
  delay(5);
  digitalWrite(ActuatorNeutral, LOW);   // reverse actuator
  delay(100); // pause to reduce impact
  digitalWrite(ActuatorNeutral, HIGH);   // actuator down
  delay(5); // pause to reduce impact
  digitalWrite(ActuatorHot, LOW);
  delay(60);
  digitalWrite(ActuatorNeutral, HIGH);   // actuator down
  digitalWrite(ActuatorHot, HIGH);


}
//

//roughly 20s for one iteration of blend process 

void Blend() { 
    
    digitalWrite(ActuatorHot, LOW);
    
    while (MeasureDistance()  < TopOfCup - 100)  // check for top of cup
    {
      delay(1);
    }

     while (MeasureDistance()  < TopOfCup - 50)  // check for top of cup
    {
      digitalWrite(ActuatorHot, LOW);
      delay(15);
      digitalWrite(ActuatorHot, HIGH);
    }

    while (MeasureDistance()  < TopOfCup)  // check for top of cup
    {
      digitalWrite(ActuatorHot, LOW);
      delay(15);
      digitalWrite(ActuatorHot, HIGH);
      delay(15);
    }
    
    while (MeasureDistance()  < TopOfSmoothie- 25)  // check for top of cup
    {
      digitalWrite(ActuatorHot, LOW);
      delay(15);
      digitalWrite(ActuatorHot, HIGH);
      delay(30);
    }
    
    TurnBlenderOn ();
  
    //preblend to reduce splatter
    //Lower blender into smothie
    while (MeasureDistance()  < TopOfSmoothie + 5)  // check for top of cup
    {
      digitalWrite(ActuatorHot, LOW);
      delay(15);
      digitalWrite(ActuatorHot, HIGH);
      delay(45);
    }
    delay(1000);
    while (MeasureDistance()  < TopOfSmoothie + 10)  // check for top of cup
    {
      digitalWrite(ActuatorHot, LOW);
      delay(15);
      digitalWrite(ActuatorHot, HIGH);
      delay(45);
    }
    delay(1000);
    while (MeasureDistance()  < TopOfSmoothie + 15)  // check for top of cup
    {
      digitalWrite(ActuatorHot, LOW);
      delay(15);
      digitalWrite(ActuatorHot, HIGH);
      delay(60);
    }
    delay(1000);
   CustomPulses(25, 12, 120, 5);  //time:20 Delay:100
   CustomPulses(40, 5, 220, 10);  //time:40 Delay:200
   delay(1000);
   
   //Main Blend Sequence
   while(MeasureDistance()  < BottomOfCup -45){
   Unjam();
   CustomPulses(120, 4, 250, -15);  
   if (MeasureDistance()  < BottomOfCup){
      CustomPulses(25, 6, 0, 4);  //time:20 
      CustomPulses(55, 5, 250, 15);   //time:50 
    }
    delay(1500);
   }
   
   delay(1500);
   
   while(MeasureDistance()  < BottomOfCup -25){
   Unjam();
   CustomPulses(180, 5, 250, -20);  
   if (MeasureDistance()  < BottomOfCup){
      CustomPulses(20, 10, 0, 5);
      CustomPulses(50, 8, 250, 15);   
    }
    delay(2000);
   }
   
   delay(3000);
   
   while(MeasureDistance()  < BottomOfCup + 5){
   Unjam();
   CustomPulses(150, 5, 250, -25);  
   if (MeasureDistance()  < BottomOfCup){
      CustomPulses(10, 20, 0, 2);
      CustomPulses(50, 8, 250, 20);   
    }
    delay(1500);
   } 
   
   Unjam();
   
   digitalWrite(ActuatorNeutral, LOW);
   //bring blender to top of smoothie
   while(MeasureDistance()  > TopOfSmoothie)
   {
    delay (1);
   }
   digitalWrite(ActuatorNeutral, HIGH);
   delay(1000);
   //slow down
   while(MeasureDistance()  > TopOfSmoothie-20)
   {
    digitalWrite(ActuatorNeutral, LOW);
    delay (15);
    digitalWrite(ActuatorNeutral, HIGH);
   }    
   digitalWrite(ActuatorNeutral, HIGH);
   //final pump
    
    CustomPulses(150, 4, 150, 50);
    delay(2000);
    CustomPulses(150, 2, 150, 25);
    CustomPulses(50, 5, 150, 5);
   


   // final stir
   for( int i =0; i < 2; i++){
      Unjam();
      //Raise blender through cup
      while (MeasureDistance()  > TopOfSmoothie + 50)  // check for top of cup
      {
        digitalWrite(ActuatorNeutral, LOW);
        delay(15);
        digitalWrite(ActuatorNeutral, HIGH);
      }
      while (MeasureDistance()  > TopOfSmoothie + 25)  // check for top of cup
      {
        digitalWrite(ActuatorNeutral, LOW);
        delay(15);
        digitalWrite(ActuatorNeutral, HIGH);
        delay(15);
      }
      
      while (MeasureDistance()  > TopOfSmoothie + 10)  // check for top of cup
      {
        digitalWrite(ActuatorNeutral, LOW);
        delay(15);
        digitalWrite(ActuatorNeutral, HIGH);
        delay(30);
      }
      
      delay(1000);

      //Raise blender through cup
         while (MeasureDistance()  < BottomOfCup - 50)  // check for top of cup
      {
        digitalWrite(ActuatorHot, LOW);
        delay(15);
        digitalWrite(ActuatorHot, HIGH);
      }
  
      while (MeasureDistance()  < BottomOfCup - 25)  // check for top of cup
      {
        digitalWrite(ActuatorHot, LOW);
        delay(15);
        digitalWrite(ActuatorHot, HIGH);
        delay(15);
      }
      
      while (MeasureDistance()  < BottomOfCup)  // check for top of cup
      {
        digitalWrite(ActuatorHot, LOW);
        delay(15);
        digitalWrite(ActuatorHot, HIGH);
        delay(30);
      }
      delay(1000);
  }

   TurnBlenderOff ();

   Unjam();
   //Bring Blender out of smoothie 
   digitalWrite(ActuatorNeutral, LOW);
   while(MeasureDistance()  > TopOfSmoothie-10)
   {
    delay (1);
   }
   digitalWrite(ActuatorNeutral, HIGH);
   //slow down to half speed
   while(MeasureDistance()  > TopOfSmoothie)
   {
    digitalWrite(ActuatorNeutral, LOW);
    delay (15);
    digitalWrite(ActuatorNeutral, HIGH);
   }    

   //delay(2000);
    Unjam();

    // shake off fruit
    CustomPulses(40, 5 , 0, -15); 
    CustomPulses(20, 10 , 0, 0);
    
    //bring blender to top position
    digitalWrite(ActuatorNeutral, LOW);
    while (MeasureDistance()  > TopPosition){
    delay(1);
    }
    digitalWrite(ActuatorNeutral, HIGH);
    
    
}

void Clean() {       
   digitalWrite(ActuatorHot, LOW);
    
    while (MeasureDistance()  < TopOfCup)  // check for top of cup
    {
      delay(1);
    }
     while (MeasureDistance()  < BottomOfCleaning - 50)  // check position
    {
      digitalWrite(ActuatorHot, LOW);
      delay(15);
      digitalWrite(ActuatorHot, HIGH);
    }
    while (MeasureDistance()  < BottomOfCleaning + 27)  // check for bottom of cleaning cup // SA220616
    {
      digitalWrite(ActuatorHot, LOW);
      delay(15);
      digitalWrite(ActuatorHot, HIGH);
      delay(30);
    }
 
    
    TurnBlenderOn ();

    delay(1500); 
    
    digitalWrite(pinWaterPump, LOW);
    
    delay(500); //1000
    Unjam();
    while (MeasureDistance()  > CleaningLevel + 20)  // bring blades into cleaning position
    {
      digitalWrite(ActuatorNeutral, LOW);
      delay(1);  
    }
    digitalWrite(ActuatorNeutral, HIGH);

    CustomPulses(50, 3, 35, -15);
    delay(500);
    CustomPulses(100, 3, 50, -5);
    TurnBlenderOff ();
    CustomPulses(150, 1, 15, 0);
    digitalWrite(pinWaterPump, HIGH); 

    delay(200);

    digitalWrite(ActuatorNeutral, LOW);
    while (MeasureDistance()  > TopPosition){
    delay(1);
    }
    digitalWrite(ActuatorNeutral, HIGH);
}



void loop() {
  

 
 if (digitalRead(pinButtonClean) == LOW) {  
 delay(5);     
 if (digitalRead(pinButtonClean) == LOW) {
      digitalWrite(pinLED_READY, LOW);
      digitalWrite(pinLED_IN_USE, HIGH); 
  
      Clean();

      digitalWrite(pinLED_IN_USE, LOW);
      digitalWrite(pinLED_READY, HIGH);
  }  
  }

 if (digitalRead(pinButtonBlend) == LOW) {   
 delay(5);    
 if (digitalRead(pinButtonBlend) == LOW) {
 
      digitalWrite(pinLED_READY, LOW);
      digitalWrite(pinLED_IN_USE, HIGH);  
  
      Blend();
    
      digitalWrite(pinLED_IN_USE, LOW);
      digitalWrite(pinLED_READY, HIGH);
  }  
  }  

}
