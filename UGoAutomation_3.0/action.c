#include "actions.h"
#include "blender.h"

#define MAX_ACTIONS 150

void blend_actions_init(char reinit) {
  int i, j = 0;

  // allocate space for actions_ptr
  if (!reinit) {
    blend_sequence.actions_ptr = (action_t*) malloc(MAX_ACTIONS * sizeof(action_t));
  }

  // STARTING OF BLENDING SEQUENCE
  blend_sequence.actions_ptr[i].type = ACTION_WAIT_FOR;
  blend_sequence.actions_ptr[i].wait_for.type = WAIT_FOR_CUP_IN_PLACE; // position
  blend_sequence.actions_ptr[i].wait_for.value = 15;
  blend_sequence.actions_ptr[i++].wait_for.comparer = WAIT_FOR_LESS_THAN;

  blend_sequence.actions_ptr[i].type = ACTION_WAIT;
  blend_sequence.actions_ptr[i++].wait.time_to_wait = 2000; //ms

  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = LIQUID_FILLING_VALVE_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = ON;

  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = CLEANING_VALVE_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = OFF;

  // wait for valve to activate before turing pump on
  blend_sequence.actions_ptr[i].type = ACTION_WAIT;
  blend_sequence.actions_ptr[i++].wait.time_to_wait = 500; //ms
  
  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = PUMP_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = ON;

  blend_sequence.actions_ptr[i].type = ACTION_WAIT;
  blend_sequence.actions_ptr[i++].wait.time_to_wait = 2750; //ms
  
  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = PUMP_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = OFF;

  // 1. Move the blender to above the cup
  blend_sequence.actions_ptr[i].type = ACTION_MTP;
  blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_CUP + 20; // position
  blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;
  blend_sequence.actions_ptr[i].mtp.time_out = 5000;
  blend_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF;

  blend_sequence.actions_ptr[i].type = ACTION_WAIT;
  blend_sequence.actions_ptr[i++].wait.time_to_wait = 500; //ms

  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = BLENDER_SPEED_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = ON;
  
  blend_sequence.actions_ptr[i].type = ACTION_WAIT;
  blend_sequence.actions_ptr[i++].wait.time_to_wait = 100; //ms

  // 2. Turn blender on
  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = BLENDER_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = ON;

  blend_sequence.actions_ptr[i].type = ACTION_WAIT;
  blend_sequence.actions_ptr[i++].wait.time_to_wait = 100; //ms

  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = BLENDER_SPEED_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = OFF;

  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = LIQUID_FILLING_VALVE_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = ON;
  
  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = CLEANING_VALVE_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = OFF;

  for (j = 0; j < 6; j++) {
    blend_sequence.actions_ptr[i].type = ACTION_MTP;
    blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_SMOOTHIE + (20 * j) + 10; // position
    blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;
    blend_sequence.actions_ptr[i].mtp.time_out = 3000;
    blend_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF;

    blend_sequence.actions_ptr[i].type = ACTION_WAIT;
    blend_sequence.actions_ptr[i++].wait.time_to_wait = 250; //ms
    
    blend_sequence.actions_ptr[i].type = ACTION_MTP;
    blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_SMOOTHIE + (20 * j); // position
    blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
    blend_sequence.actions_ptr[i].mtp.time_out = 3000;
    blend_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF;

    blend_sequence.actions_ptr[i].type = ACTION_WAIT;
    blend_sequence.actions_ptr[i++].wait.time_to_wait = 250; //ms
  }
  
  for (j = 0; j < 11; j++) {
    blend_sequence.actions_ptr[i].type = ACTION_MTP;
    blend_sequence.actions_ptr[i].mtp.new_position = BOTTOM_OF_CUP; // position
    blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;
    blend_sequence.actions_ptr[i].mtp.time_out = 3000;
    blend_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_THIRD;

    blend_sequence.actions_ptr[i].type = ACTION_WAIT;
    blend_sequence.actions_ptr[i++].wait.time_to_wait = 1000; //ms
    
    blend_sequence.actions_ptr[i].type = ACTION_MTP;
    blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_SMOOTHIE + (j < 2 ? 25 : 15); // position
    blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
    blend_sequence.actions_ptr[i].mtp.time_out = 3000;
    blend_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF;

    blend_sequence.actions_ptr[i].type = ACTION_WAIT;
    blend_sequence.actions_ptr[i++].wait.time_to_wait = 500; //ms

    if (j == 3) {
        blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
        blend_sequence.actions_ptr[i].activate.address = BLENDER_SPEED_ADDRESS;
        blend_sequence.actions_ptr[i++].activate.state = ON;
    }
  }

  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = BLENDER_SPEED_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = OFF;

  // 23. Move to top, stay in liquid
  blend_sequence.actions_ptr[i].type = ACTION_MTP;
  blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_CUP; // position
  blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
  blend_sequence.actions_ptr[i].mtp.time_out = 5000;
  blend_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF;
    
  // 24. Turn blender off
  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = BLENDER_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = OFF;

  blend_sequence.actions_ptr[i].type = ACTION_WAIT;
  blend_sequence.actions_ptr[i++].wait.time_to_wait = 1500; //ms

//  NOT WORKING YET
//  blend_sequence.actions_ptr[i].type = ACTION_AGITATE;
//  blend_sequence.actions_ptr[i].agitate.rising_distance = 10;
//  blend_sequence.actions_ptr[i].agitate.lowering_distance = 15;
//  blend_sequence.actions_ptr[i].agitate.repeat_cycles = 5;
//  blend_sequence.actions_ptr[i++].agitate.start_direction = 0; // 0: LOWER; 1:RAISE

  // SHAKE OFF
  for (j = 0; j < 5; j++) {
    blend_sequence.actions_ptr[i].type = ACTION_MTP;
    blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_CUP - 20; // position
    blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
    blend_sequence.actions_ptr[i].mtp.time_out = 3000;
    blend_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_FULL;

    blend_sequence.actions_ptr[i].type = ACTION_WAIT;
    blend_sequence.actions_ptr[i++].wait.time_to_wait = 250; //ms
    
    blend_sequence.actions_ptr[i].type = ACTION_MTP;
    blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_CUP; // position
    blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;
    blend_sequence.actions_ptr[i].mtp.time_out = 3000;
    blend_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_FULL;

    blend_sequence.actions_ptr[i].type = ACTION_WAIT;
    blend_sequence.actions_ptr[i++].wait.time_to_wait = 250; //ms
  }
  
  // 25. Return home
  blend_sequence.actions_ptr[i].type = ACTION_MTP;
  blend_sequence.actions_ptr[i].mtp.new_position = TOP_POSITION; // position
  blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
  blend_sequence.actions_ptr[i].mtp.time_out = 10000;
  blend_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_FULL;

  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = LIQUID_FILLING_VALVE_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = OFF;

  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = CLEANING_VALVE_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = OFF;

  if (i > MAX_ACTIONS) {
    LOG_PRINT(LOGGER_ERROR, "Buffer overflow!!");
  } else {
    LOG_PRINT(LOGGER_ERROR, "total actions: %d", i);
  }
  
  blend_sequence.total_actions = i;
}

void clean_actions_init() {
  int i, j = 0;
  
  clean_sequence.actions_ptr = (action_t*) malloc(50 * sizeof(action_t));
  
  clean_sequence.actions_ptr[i].type = ACTION_WAIT_FOR;
  clean_sequence.actions_ptr[i].wait_for.type = WAIT_FOR_CUP_IN_PLACE; // position
  clean_sequence.actions_ptr[i].wait_for.value = 18;
  clean_sequence.actions_ptr[i++].wait_for.comparer = WAIT_FOR_GREATER_THAN;

  clean_sequence.actions_ptr[i].type = ACTION_WAIT;
  clean_sequence.actions_ptr[i++].wait.time_to_wait = 2000; //ms
  
  // 1. Move the blender to above the cup
  clean_sequence.actions_ptr[i].type = ACTION_MTP;
  clean_sequence.actions_ptr[i].mtp.new_position = CLEANING_LEVEL; // position
  clean_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;
  clean_sequence.actions_ptr[i].mtp.time_out = 5000;
  clean_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF;
  
  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = LIQUID_FILLING_VALVE_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = ON;

  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = CLEANING_VALVE_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = ON;

  // wait for valve to activate before turing pump on
  clean_sequence.actions_ptr[i].type = ACTION_WAIT;
  clean_sequence.actions_ptr[i++].wait.time_to_wait = 500; //ms
  
  // 2. Turn blender on
  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = BLENDER_SPEED_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = ON;
  
  // wait for valve to activate before turing pump on
  clean_sequence.actions_ptr[i].type = ACTION_WAIT;
  clean_sequence.actions_ptr[i++].wait.time_to_wait = 500; //ms
  
  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = BLENDER_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = ON;

  for (j = 0; j < 2; j++) {
    if ((j % 2) == 1) {
      clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
      clean_sequence.actions_ptr[i].activate.address = PUMP_ADDRESS;
      clean_sequence.actions_ptr[i++].activate.state = OFF;
    } else {
      clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
      clean_sequence.actions_ptr[i].activate.address = PUMP_ADDRESS;
      clean_sequence.actions_ptr[i++].activate.state = ON;
    }
    
    clean_sequence.actions_ptr[i].type = ACTION_MTP;
    clean_sequence.actions_ptr[i].mtp.new_position = BOTTOM_OF_CLEANING; // position
    clean_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;
    clean_sequence.actions_ptr[i].mtp.time_out = 5000;
    clean_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF;

    clean_sequence.actions_ptr[i].type = ACTION_WAIT;
    clean_sequence.actions_ptr[i++].wait.time_to_wait = 500; //ms

    clean_sequence.actions_ptr[i].type = ACTION_MTP;
    clean_sequence.actions_ptr[i].mtp.new_position = CLEANING_LEVEL + 75;
    clean_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
    clean_sequence.actions_ptr[i].mtp.time_out = 5000;
    clean_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF;

    clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
    clean_sequence.actions_ptr[i].activate.address = LIQUID_FILLING_VALVE_ADDRESS;
    clean_sequence.actions_ptr[i++].activate.state = ON;

    clean_sequence.actions_ptr[i].type = ACTION_MTP;
    clean_sequence.actions_ptr[i].mtp.new_position = CLEANING_LEVEL;
    clean_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
    clean_sequence.actions_ptr[i].mtp.time_out = 5000;
    clean_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF;

    clean_sequence.actions_ptr[i].type = ACTION_WAIT;
    clean_sequence.actions_ptr[i++].wait.time_to_wait = 500; //ms
    

  }
  
  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = BLENDER_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = OFF;
    
  clean_sequence.actions_ptr[i].type = ACTION_MTP;
  clean_sequence.actions_ptr[i].mtp.new_position = BOTTOM_OF_CLEANING; // position
  clean_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;
  clean_sequence.actions_ptr[i].mtp.time_out = 5000;
  clean_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF;
  
  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = CLEANING_VALVE_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = OFF;
  
  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = PUMP_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = ON;
  
  clean_sequence.actions_ptr[i].type = ACTION_WAIT;
  clean_sequence.actions_ptr[i++].wait.time_to_wait = 1000; //ms

  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = PUMP_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = OFF;
  
  clean_sequence.actions_ptr[i].type = ACTION_MTP;
  clean_sequence.actions_ptr[i].mtp.new_position = CLEANING_LEVEL - 10;
  clean_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
  clean_sequence.actions_ptr[i].mtp.time_out = 5000;
  clean_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_HALF;

  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = LIQUID_FILLING_VALVE_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = OFF;

  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = CLEANING_VALVE_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = ON;

  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = PUMP_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = ON;
    
  clean_sequence.actions_ptr[i].type = ACTION_WAIT;
  clean_sequence.actions_ptr[i++].wait.time_to_wait = 1000; //ms
  
  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = PUMP_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = OFF;
  
  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = LIQUID_FILLING_VALVE_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = ON;

  clean_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  clean_sequence.actions_ptr[i].activate.address = CLEANING_VALVE_ADDRESS;
  clean_sequence.actions_ptr[i++].activate.state = ON;
  
  clean_sequence.actions_ptr[i].type = ACTION_MTP;
  clean_sequence.actions_ptr[i].mtp.new_position = TOP_POSITION; // position
  clean_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
  clean_sequence.actions_ptr[i].mtp.time_out = 10000;
  clean_sequence.actions_ptr[i++].mtp.speed = MOTOR_SPEED_FULL; 

  if (i >= 50) {
    LOG_PRINT(LOGGER_ERROR, "Buffer overflow!!");
  }  
  clean_sequence.total_actions = i;
}

void initializing_action_init() {
  initializing_action.type = ACTION_MTP;
  initializing_action.mtp.new_position = TOP_POSITION;
  initializing_action.mtp.move_direction = BLENDER_MOVEMENT_UP;
  initializing_action.mtp.speed = MOTOR_SPEED_FULL;
  initializing_action.mtp.time_out = 5000;
}
