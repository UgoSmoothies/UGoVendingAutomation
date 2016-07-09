#ifndef MACHINE_H
#define MACHINE_H

#include "global.h"
#include "actions.h"
#include "blender.h"
#include "input_button.h"

#define BUTTON_COUNT 4
#define BLEND_BUTTON 0
#define CLEAN_BUTTON 1
#define STOP_BUTTON 2
#define STEP_BUTTON 3

typedef struct {
  char id;
  char is_initialized;
  char current_state;
  char cuurent_cycle_type;
  char current_step;
  blender_t blender;
  unsigned long last_step_time;
  input_button_t buttons[BUTTON_COUNT];
} machine_t;

void machine_init(machine_t*);
void machine_process(machine_t*);
void machine_change_state(machine_t*);

char machine_execute_action(machine_t*, action_t);

char machine_check_safety_conditions(machine_t*);
#endif

