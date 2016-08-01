/***************************************************
  Machine                                <machine.c>
  Authored By: Steven McKeown

  The machine file controls the operation of a
  machine.

  A machine controls:
  - A blender
  - Input buttons

  NOTE: When implementing multiple machines, address
  changes will be needed, for now the addresses are:

  **** INPUTS ****
  1 - BLEND BUTTON
  2 - CLEAN BUTTON
  3 - STOP BUTTON
  4 - STEP BUTTON
  11 - LED READY
  12 - LED IN USE
  A1 - MOTOR POTENTIOMETER

  **** OUTPUTS ****
  5 - MOTOR FORWARD
  6 - MOTOR REVERSE
  7 - FORWARD ENABLED
  8 - REVERSE ENABLED
  9 - BLENDER
  10 - PUMP

  TODO: Implement stepping

  SM070716
***************************************************/
#include "machine.h"
#include "actions.h"

void machine_init(machine_t* machine_ptr) {
  machine_ptr->is_initialized = 0;
  machine_ptr->current_state = MACHINE_STATE_IDLE;
  // initialize the blender
  blender_init(&machine_ptr->blender);

  blend_actions_init();

  input_button_init(&machine_ptr->buttons[BLEND_BUTTON], 1);
  input_button_init(&machine_ptr->buttons[CLEAN_BUTTON], 2);
  input_button_init(&machine_ptr->buttons[STOP_BUTTON], 3); // NOT IMPLIMENTED
  input_button_init(&machine_ptr->buttons[STEP_BUTTON], 4); // NOT IMPLIMENTED
}

void machine_process(machine_t* machine_ptr) {
  int i;

  // ---------- BEGING INPUT BUTTON SECTION ----------
  for (i = 0; i < BUTTON_COUNT; i++) {
    input_button_read(&machine_ptr->buttons[i]);
  }

  if (machine_ptr->current_state == MACHINE_STATE_IDLE) {
    if (machine_ptr->buttons[BLEND_BUTTON].current_state) {
      LOG_PRINT(LOGGER_VERBOSE, "Blender button pushed, starting blending");
      machine_ptr->current_state = MACHINE_STATE_BLENDING;
    } else if (machine_ptr->buttons[CLEAN_BUTTON].current_state) {
      LOG_PRINT(LOGGER_VERBOSE, "Cleaning button pushed, starting cleaning");
      machine_ptr->current_state = MACHINE_STATE_CLEANING;
    }
  }

  if (machine_ptr->buttons[STOP_BUTTON].current_state) {
      LOG_PRINT(LOGGER_VERBOSE, "Stop button pushed, stopping machine");
    machine_ptr->current_state = MACHINE_STATE_IDLE;
  }

  LOG_PRINT(LOGGER_INFO, "Machine Current State: %d - Button States: B%d C%d S%d - Motor Position: %d", 
    machine_ptr->current_state,
    machine_ptr->buttons[BLEND_BUTTON].current_state,
    machine_ptr->buttons[CLEAN_BUTTON].current_state,
    machine_ptr->buttons[STOP_BUTTON].current_state,
    machine_ptr->blender.position);

  // ---------- END INPUT BUTTON SECTION ----------

  // change to mediator rather than switch
  switch (machine_ptr->current_state) {
    case MACHINE_STATE_IDLE:
      machine_ptr->current_step = 0;
      break;
    case MACHINE_STATE_BLENDING:
      if (machine_execute_action(machine_ptr, &blend_sequence.actions_ptr[machine_ptr->current_step])) {
        // we finished the last action, let's move to the next action.
        LOG_PRINT(LOGGER_VERBOSE, "Bending step %d completed, percent complete:%d", machine_ptr->current_step, (100*machine_ptr->current_step)/blend_sequence.total_actions);
        
        machine_ptr->current_step++;
        machine_ptr->last_step_time = millis();

        if (machine_ptr->current_step == blend_sequence.total_actions) {
          LOG_PRINT(LOGGER_VERBOSE, "Blending complete, stopping machine");
          machine_ptr->current_state = MACHINE_STATE_IDLE;
        }
      }
      break;
    case MACHINE_STATE_CLEANING:
      if (machine_execute_action(machine_ptr, &clean_sequence.actions_ptr[machine_ptr->current_step])) {
        // we finished the last action, let's move to the next action.
        machine_ptr->current_step++;
        machine_ptr->last_step_time = millis();

        if (machine_ptr->current_step == clean_sequence.total_actions) {
          machine_ptr->current_state = MACHINE_STATE_IDLE;
        }
      }
      break;
    case MACHINE_STATE_INITIALIZING:
      if (machine_execute_action(machine_ptr, &initializing_action)) {
        machine_ptr->current_state = MACHINE_STATE_IDLE;
        machine_ptr->is_initialized = 1;
      }
      break;
  }
}

char machine_execute_action(machine_t* machine_ptr, action_t* action) {
  switch (action->type) {
    case  ACTION_MTP:
      return move_to_position(&machine_ptr->blender, machine_ptr->last_step_time, &action->mtp);
      break;
    case ACTION_WAIT:
      return wait(&machine_ptr->blender, machine_ptr->last_step_time, &action->wait);
      break;
    case ACTION_ACTIVATE:
      return activate(&machine_ptr->blender, &action->activate);
      break;
    case ACTION_AGITATE:
      return agitate(&machine_ptr->blender, &action->agitate);
  }

  // error...
  LOG_PRINT(LOGGER_WARNING, "machine_execute_action - invalid action type: %d", action->type);
  return 0;
}

// function to check if the machine is in an unsafe state, and take action
char machine_check_safety_conditions(machine_t* machine_prt) {
  // TODO
  return 1;
}
