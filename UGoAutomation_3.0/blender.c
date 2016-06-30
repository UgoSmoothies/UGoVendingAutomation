#include "blender.h"
#include "Arduino.h"



#define BLENDER_MOVEMENT_DOWN 0
#define BLENDER_MOVEMENT_UP 1
#define BLENDER_MOVEMENT_IDLE 2
#define BLENDER_ON 1
#define BLENDER_OFF 0
#define PUMP_ON 1
#define PUMP_OFF 0

void init_blender(blender_t* blender){
  blender->postiion = 0;
  blender->movement = BLENDER_MOVEMENT_IDLE;  
  blender->blade = BLENDER_OFF;
  blender->water_pump = PUMP_OFF;
  blender->actuator_up_address = 8;
  blender->actuator_down_address = 7;
  blender->blender_ssr_address = 31;
  blender->blender_mech_address = 12;
  blender->water_pump_address = 6; 
}

void move_down(blender_t* blender){
  digitalWrite(blender->actuator_down_address, LOW);
  blender->movement = BLENDER_MOVEMENT_DOWN;
}

void move_up(blender_t* blender){
   digitalWrite(blender->actuator_up_address, LOW);
   blender->movement = BLENDER_MOVEMENT_UP;
}
void blender_on(blender_t* blender){
   digitalWrite(blender->blender_ssr_address, HIGH);
   delay(20);
   digitalWrite(blender->blender_mech_address, LOW);
   
   blender->blade = BLENDER_ON;
}
void blender_off(blender_t* blender){
   digitalWrite(blender->blender_ssr_address, LOW);
   delay(20);
   digitalWrite(blender->blender_mech_address, HIGH);
   
   blender->blade = BLENDER_OFF;
}
void pump_on(blender_t* blender){
   digitalWrite(blender->water_pump_address, LOW);
   blender->water_pump = PUMP_ON;
}

void pump_off(blender_t* blender){
   digitalWrite(blender->water_pump_address, HIGH);
   blender->water_pump = PUMP_OFF;
}


char wait(blender_t* blender, unsigned long start_wait_time, int time_to_wait) {
  return ((start_wait_time + time_to_wait) < millis());
}

