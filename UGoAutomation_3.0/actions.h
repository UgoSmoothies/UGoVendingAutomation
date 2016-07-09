#ifndef ACTIONS_H
#define ACTIONS_H

// ALL OF THIS NEEDS TO BE DONE!!!

#include "global.h"

#define ACTION_MTP 0
#define ACTION_WAIT 1
#define ACTION_ACTIVATE 2
#define ACTION_AGITATE 3

typedef struct {
  /* The new position to move to */
  int new_position;
  /* The direction to move */
  char move_direction;
} action_move_to_position_t;

typedef struct {
  /* How long to wait */
  int time_to_wait;
} action_wait_t;

typedef struct {
  /* What to change */
  char item;
  /* On or off */
  char state; // 0:OFF; 1:ON;
} action_activate_t;

typedef struct {
  /* how many units to raise */
  char rising_distance;
  /* how many units to lower */
  char lowering_distance;
  /* how many times to repeat */
  char repeat_cycles;
  /* start by raising or lowering */
  char start_direction; // 0: LOWER; 1:RAISE
  /* internal variable to track number of times repeated */
  char current_cycle;
  /* insternal variable first or second step */
  char current_step;
  /* insternal variable to track where we started */
  char start_position;
  /* insternal variable to see if we have started */
  char is_running;
} action_agitate_t;

typedef struct {
  char type;
  union {
    action_move_to_position_t mtp;
    action_wait_t wait;
    action_activate_t activate;
    action_agitate_t agitate;
  };
} action_t;

typedef struct {
  action_t* actions_ptr;
  int total_actions;
} sequence_t;

sequence_t blend_sequence;
sequence_t clean_sequence;

action_t initializing_action;

void blend_actions_init();
void initializing_action_init();

#endif
