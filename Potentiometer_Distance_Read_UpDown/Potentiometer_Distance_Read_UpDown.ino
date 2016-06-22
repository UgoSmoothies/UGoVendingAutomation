int Read[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  // Array to hold Actuator Portentiometer Reads
int ValRef = 0;  // holder value for use during quicksort
const int Potentiometer = A1;    // Actuator Potentiometer Pin


const int pinButtonBlend = 2; 
const int pinButtonClean = 3;
const int ActuatorHot = 7; 
const int ActuatorNeutral = 8; 

void setup() {

  pinMode(Potentiometer, INPUT);
  pinMode(pinButtonBlend, INPUT);
  pinMode(pinButtonClean, INPUT);
  pinMode(ActuatorHot, OUTPUT);
  pinMode(ActuatorNeutral, OUTPUT);

  digitalWrite(ActuatorHot, HIGH);
  digitalWrite(ActuatorNeutral, HIGH);
  // put your setup code here, to run once:
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  Serial.println();
  /*
  Serial.print("Origonal List: ");
  for(int i = 0; i < 15; i++)
  {
    Serial.print(Read[i]);
    Serial.print(", ");
  }
  
  QuickSort (); 
  Serial.println();
  Serial.println("Sorted List: ");
  for(int i = 0; i < 15; i++)
  {
    Serial.print(Read[i]);
    Serial.print(", ");
  }
  */
  
  
}

int MeasureDistance()  
// Measure distance of actuator - takes 15 reads to compensate for inaccuracy
{
  for (int i = 0; i < 15; i++)
  {
    Read [i] = analogRead(Potentiometer);
    delay(1); 
  }
  QuickSort();  // sort entries 
  return Read[7];
  // send middle read value which is supposed to be the intended distance
}

void QuickSort ()      
// quick sort algorithm to arrange an array of length 15 in ascending order
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

void loop() {
  // put your main code here, to run repeatedly:
 Serial.println();
 Serial.print("Read: ");
 Serial.print(MeasureDistance());

 digitalWrite(ActuatorHot, HIGH);
 digitalWrite(ActuatorNeutral, HIGH);

 if (digitalRead(pinButtonBlend) == LOW)
 {
     digitalWrite(ActuatorHot, LOW);
 }
 
  if (digitalRead(pinButtonClean) == LOW)
 {
     digitalWrite(ActuatorNeutral, LOW);
 }
 
 delay(15);
}



