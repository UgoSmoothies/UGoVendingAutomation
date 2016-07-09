#include "actions.h"
#include "blender.h"

#define MAX_ACTIONS 100

// WE MIGHT NEED TO INTRODUCE SPEED
void blend_actions_init() {
  int i, j;

  // allocate space for actions_ptr
  blend_sequence.actions_ptr = (action_t*) malloc(MAX_ACTIONS * sizeof(action_t));
    
  for (i = 0; i < MAX_ACTIONS; i++) {
  }

  // 1. Move the blender to above the cup
  blend_sequence.actions_ptr[i++].type = ACTION_MTP;
  blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_CUP; // position
  blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;

  // 2. Turn blender on
  blend_sequence.actions_ptr[i++].type = ACTION_ACTIVATE;
  blend_sequence.actions_ptr[i].activate.item = 0;
  blend_sequence.actions_ptr[i].activate.state = 0;

  // [3-8] Lower and wait, lower and wait, lower and wait
  for (j = 0; j < 3; i++) {
    blend_sequence.actions_ptr[i++].type = ACTION_MTP;
    blend_sequence.actions_ptr[i].mtp.new_position = TOP_OF_CUP + (5 + j); // position
    blend_sequence.actions_ptr[i].mtp.move_direction = BLENDER_MOVEMENT_DOWN;

    blend_sequence.actions_ptr[i++].type = ACTION_WAIT;
    blend_sequence.actions_ptr[i].wait.time_to_wait = 1000; //ms
  }

  // 3. Move into cup

  // 4. Blend for 1 second

  blend_sequence.total_actions = i;
}

void initializing_action_init() {
  initializing_action.type = ACTION_MTP;
  initializing_action.mtp.new_position = TOP_POSITION;
  initializing_action.mtp.move_direction = BLENDER_MOVEMENT_UP;

}
