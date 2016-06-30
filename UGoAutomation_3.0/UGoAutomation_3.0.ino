extern "C" {
  #include "blender.h"
}

#define ACTION_MTP 0
#define ACTION_WAIT 1
#define ACTION_ACTIVATE 2

typedef struct {
  int new_position;
  char move_direction;
} action_MoveToPosition;

typedef struct {
  int time_to_wait;
} action_Wait;

typedef struct {
  char type;
  union {
    action_MoveToPosition mtp;
    action_Wait wait;
  };
} action_t;

char execute_action(action_t);

action_t blend_actions[100];
blender_t blender;

typedef struct {
  char current_step;
  unsigned long last_step_time;
} machine_t;

machine_t machine;

void setup() {
  init_blender(&blender);
  
  blend_actions[0].type = ACTION_MTP;
  blend_actions[0].mtp.new_position = 250; // position
  blend_actions[0].mtp.move_direction = 0;
  
  blend_actions[1].type = ACTION_WAIT;
  blend_actions[1].wait.time_to_wait = 200; //ms
}

void loop() {
  // put your main code here, to run repeatedly:

  
  if (execute_action(blend_actions[machine.current_step])) {
    // we finished the last action, let's move to the next action.
    machine.current_step++;
    machine.last_step_time = millis();
  }
}

char execute_action(action_t action)
{
 switch(action.type) {
  case  ACTION_MTP:
    return move_to_position(&blender, action.mtp.new_position, action.mtp.move_direction);
  break;
  case ACTION_WAIT:
    return wait(&blender, machine.last_step_time, action.wait.time_to_wait);
  break;
 }
}


