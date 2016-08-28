#include "actions.h"
#include "blender.h"

#define MAX_ACTIONS 100

void blend_actions_init() {
  int i, j = 0;

  // allocate space for actions_ptr
  blend_sequence.actions_ptr = (action_t*) malloc(MAX_ACTIONS * sizeof(action_t));
    
  // 1. Move the blender to above the cup
  blend_sequence.actions_ptr[i].type = ACTION_MTP;
  blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_CUP; // position
  blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;
  blend_sequence.actions_ptr[i].mtp.time_out = 5000;
  blend_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF;

  // 2. Turn blender on
  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = BLENDER_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = ON;

  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = LIQUID_FILLING_VALVE_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = ON;
  
  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = CLEANING_VALVE_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = OFF;

  // [3-6] Lower and wait, lower and wait, lower and wait
  for (j = 0; j < 4; j++) {
    blend_sequence.actions_ptr[i].type = ACTION_MTP;
    blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_SMOOTHIE + (15 * j) + 5; // position
    blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;
    blend_sequence.actions_ptr[i].mtp.time_out = 5000;
    blend_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF;

    blend_sequence.actions_ptr[i].type = ACTION_WAIT;
    blend_sequence.actions_ptr[i++].wait.time_to_wait = 1000; //ms

    if (j == 3) {
      blend_sequence.actions_ptr[i].type = ACTION_WAIT;
      blend_sequence.actions_ptr[i++].wait.time_to_wait = 2000; //ms
    }
  }

  // [7-22] Up and Down twice, not quite to top of cup
  for (j = 0; j < 15; j++) {
    blend_sequence.actions_ptr[i].type = ACTION_MTP;
    blend_sequence.actions_ptr[i].mtp.new_position = BOTTOM_OF_CUP; // position
    blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;
    blend_sequence.actions_ptr[i].mtp.time_out = 5000;
    blend_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF;

    blend_sequence.actions_ptr[i].type = ACTION_WAIT;
    blend_sequence.actions_ptr[i++].wait.time_to_wait = 200; //ms
    
    blend_sequence.actions_ptr[i].type = ACTION_MTP;
    blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_SMOOTHIE + (j < 2 ? 10 : 5); // position
    blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
    blend_sequence.actions_ptr[i].mtp.time_out = 5000;
    blend_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF;

    blend_sequence.actions_ptr[i].type = ACTION_WAIT;
    blend_sequence.actions_ptr[i++].wait.time_to_wait = 200; //ms
  }

  // 23. Move to top, stay in liquid
  blend_sequence.actions_ptr[i].type = ACTION_MTP;
  blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_CUP - 20; // position
  blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
  blend_sequence.actions_ptr[i].mtp.time_out = 5000;
  blend_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF;

  // 24. Turn blender off
  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = BLENDER_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = OFF;

  // 25. Return home
  blend_sequence.actions_ptr[i].type = ACTION_MTP;
  blend_sequence.actions_ptr[i].mtp.new_position = TOP_POSITION; // position
  blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
  blend_sequence.actions_ptr[i].mtp.time_out = 10000;
  blend_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_FULL;

  blend_sequence.total_actions = i;
}

void clean_actions_init() {
  int i, j = 0;
  
  clean_sequence.actions_ptr = (action_t*) malloc(MAX_ACTIONS * sizeof(action_t));
  
  // 1. Move the blender to above the cup
  clean_sequence.actions_ptr[i].type = ACTION_MTP;
  clean_sequence.actions_ptr[i].mtp.new_position = CLEANING_LEVEL; // position
  clean_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;
  clean_sequence.actions_ptr[i].mtp.time_out = 5000;
  clean_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF;
  
  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = LIQUID_FILLING_VALVE_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = OFF;

  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = CLEANING_VALVE_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = ON;

  // wait for valve to activate before turing pump on
  clean_sequence.actions_ptr[i].type = ACTION_WAIT;
  clean_sequence.actions_ptr[i++].wait.time_to_wait = 500; //ms
  
  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = PUMP_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = ON;
  
  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = BLENDER_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = ON;

  for (j = 0; j < 5; j++) {
    clean_sequence.actions_ptr[i].type = ACTION_MTP;
    clean_sequence.actions_ptr[i].mtp.new_position = BOTTOM_OF_CLEANING; // position
    clean_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;
    clean_sequence.actions_ptr[i].mtp.time_out = 5000;
    clean_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF /2;

    clean_sequence.actions_ptr[i].type = ACTION_WAIT;
    clean_sequence.actions_ptr[i++].wait.time_to_wait = 200; //ms
    
    clean_sequence.actions_ptr[i].type = ACTION_MTP;
    clean_sequence.actions_ptr[i].mtp.new_position = CLEANING_LEVEL;
    clean_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
    clean_sequence.actions_ptr[i].mtp.time_out = 5000;
    clean_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF /2;

    clean_sequence.actions_ptr[i].type = ACTION_WAIT;
    clean_sequence.actions_ptr[i++].wait.time_to_wait = 200; //ms
  }

  clean_sequence.actions_ptr[i].type = ACTION_WAIT;
  clean_sequence.actions_ptr[i++].wait.time_to_wait = 2000; //ms
  
  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = PUMP_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = OFF;
  
  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = BLENDER_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = OFF;
  
  clean_sequence.actions_ptr[i].type = ACTION_MTP;
  clean_sequence.actions_ptr[i].mtp.new_position = TOP_POSITION; // position
  clean_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
  clean_sequence.actions_ptr[i].mtp.time_out = 10000;
  clean_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_FULL; 
  
  clean_sequence.total_actions = i;
}

void initializing_action_init() {
  initializing_action.type = ACTION_MTP;
  initializing_action.mtp.new_position = TOP_POSITION;
  initializing_action.mtp.move_direction = BLENDER_MOVEMENT_UP;
  initializing_action.mtp.speed = MOTOR_SPEED_FULL;
  initializing_action.mtp.time_out = 5000;


}
