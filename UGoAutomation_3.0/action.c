#include "actions.h"
#include "blender.h"

void blend_actions_init() {
  int i, j;
    
  i = 0;

  // 1. Move the blender to above the cup
  blend_sequence.actions_ptr[i].type = ACTION_MTP;
  blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_CUP; // position
  blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;
  blend_sequence.actions_ptr[i].mtp.time_out = 5000;
  blend_sequence.actions_ptr[i++].mtp.new_position = MOTOR_SPEED_FULL;

  // 2. Turn blender on
  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.address = BLENDER_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = ON;

  // [3-6] Lower and wait, lower and wait, lower and wait
<<<<<<< HEAD
  for (j = 0; j < 4; i++) {
    blend_sequence.actions_ptr[i++].type = ACTION_MTP;
=======
  for (j = 0; j < 4; j++) {
    blend_sequence.actions_ptr[i].type = ACTION_MTP;
>>>>>>> origin/master
    blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_CUP + (5 * j) + 5; // position
    blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;
    blend_sequence.actions_ptr[i].mtp.time_out = 5000;
    blend_sequence.actions_ptr[i++].mtp.new_position = MOTOR_SPEED_HALF;

    blend_sequence.actions_ptr[i].type = ACTION_WAIT;
    blend_sequence.actions_ptr[i++].wait.time_to_wait = 1000; //ms
  }

<<<<<<< HEAD
   // [7-8] Lower and wait, Raise and wait
  for(j=0; j<2; j++)  {
    blend_sequence.actions_ptr[i++].type = ACTION_MTP;
    blend_sequence.actions_ptr[i].mtp.new_position = BOTTOM_OF_CUP; // position
    blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;
    blend_sequence.actions_ptr[i].mtp.new_position = MOTOR_SPEED_HALF;

    blend_sequence.actions_ptr[i++].type = ACTION_WAIT;
    blend_sequence.actions_ptr[i].wait.time_to_wait = 500; //ms

    blend_sequence.actions_ptr[i++].type = ACTION_MTP;
    blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_SMOOTHIE + 10; // position
    blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
    blend_sequence.actions_ptr[i].mtp.new_position = MOTOR_SPEED_HALF;

    blend_sequence.actions_ptr[i++].type = ACTION_WAIT;
    blend_sequence.actions_ptr[i].wait.time_to_wait = 500; //ms
  }

  // [9-16] Up and Down twice, not quite to top of cup
  for (j = 0; j < 8; i++) {
    blend_sequence.actions_ptr[i++].type = ACTION_MTP;
=======
  // [7-22] Up and Down twice, not quite to top of cup
  for (j = 0; j < 15; j++) {
    blend_sequence.actions_ptr[i].type = ACTION_MTP;
>>>>>>> origin/master
    blend_sequence.actions_ptr[i].mtp.new_position = BOTTOM_OF_CUP; // position
    blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;
    blend_sequence.actions_ptr[i].mtp.time_out = 5000;
    blend_sequence.actions_ptr[i++].mtp.new_position = MOTOR_SPEED_HALF;

    blend_sequence.actions_ptr[i].type = ACTION_WAIT;
    blend_sequence.actions_ptr[i++].wait.time_to_wait = 500; //ms
    
    blend_sequence.actions_ptr[i].type = ACTION_MTP;
    blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_CUP + (j < 2 ? 10 : 5); // position
    blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
    blend_sequence.actions_ptr[i].mtp.time_out = 5000;
    blend_sequence.actions_ptr[i++].mtp.new_position = MOTOR_SPEED_HALF;

    blend_sequence.actions_ptr[i].type = ACTION_WAIT;
    blend_sequence.actions_ptr[i++].wait.time_to_wait = 500; //ms
  }

<<<<<<< HEAD
  // 17. Move to top, stay in liquid
  blend_sequence.actions_ptr[i++].type = ACTION_MTP;
=======
  // 23. Move to top, stay in liquid
  blend_sequence.actions_ptr[i].type = ACTION_MTP;
>>>>>>> origin/master
  blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_CUP - 20; // position
  blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
  blend_sequence.actions_ptr[i].mtp.time_out = 5000;
  blend_sequence.actions_ptr[i++].mtp.new_position = MOTOR_SPEED_HALF;

<<<<<<< HEAD
  // 18. Turn blender off
  blend_sequence.actions_ptr[i++].type = ACTION_ACTIVATE;
=======
  // 24. Turn blender off
  blend_sequence.actions_ptr[i].type = ACTION_ACTIVATE;
>>>>>>> origin/master
  blend_sequence.actions_ptr[i].activate.address = BLENDER_ADDRESS;
  blend_sequence.actions_ptr[i++].activate.state = OFF;

<<<<<<< HEAD
  // 19. Return home
  blend_sequence.actions_ptr[i++].type = ACTION_MTP;
=======
  // 25. Return home
  blend_sequence.actions_ptr[i].type = ACTION_MTP;
>>>>>>> origin/master
  blend_sequence.actions_ptr[i].mtp.new_position = TOP_POSITION; // position
  blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_UP;
  blend_sequence.actions_ptr[i].mtp.time_out = 5000;
  blend_sequence.actions_ptr[i++].mtp.new_position = MOTOR_SPEED_HALF;

  blend_sequence.total_actions = i;
}

void initializing_action_init() {
  initializing_action.type = ACTION_MTP;
  initializing_action.mtp.new_position = TOP_POSITION;
  initializing_action.mtp.time_out = 25000;
  initializing_action.mtp.move_direction = BLENDER_MOVEMENT_UP;
  initializing_action.mtp.new_position = MOTOR_SPEED_FULL;


}
