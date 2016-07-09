#ifndef BLENDER_H
#define BLENDER_H

#include "global.h"
#include "actions.h"

#define BLENDER_MOVEMENT_DOWN 0
#define BLENDER_MOVEMENT_UP 1
#define BLENDER_MOVEMENT_IDLE 2

#define BLENDER_ON 1
#define BLENDER_OFF 0

#define PUMP_ON 1
#define PUMP_OFF 0

typedef struct{
  int position;
  char movement;  
  char blade;
  char water_pump;
  int actuator_up_address;
  int actuator_down_address;
  int blender_ssr_address;
  int blender_mech_address;
  int water_pump_address;
  int encoder_address; 
}blender_t;

void blender_init(blender_t*);
void blender_move(blender_t*, char);
void blender_on(blender_t*);
void blender_off(blender_t*);
void pump_on(blender_t*);
void pump_off(blender_t*);
void update_current_position(blender_t*);

char move_to_position(blender_t*, action_move_to_position_t*);
char wait(blender_t*, unsigned long, action_wait_t*);
char activate(blender_t*, action_activate_t*);
char agitate(blender_t*, action_agitate_t*);

#endif


