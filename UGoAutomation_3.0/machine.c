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

char step_request;

void machine_init(machine_t* machine_ptr) {
  machine_ptr->is_initialized = 0;
  machine_ptr->current_state = MACHINE_STATE_IDLE;

  // MAGIC NUMBERS FOR NOW, DEFINE AFTER......
  machine_ptr->cup_detect_sensor = new_ping_c_wrapper_init(12,11);
  
  // initialize the blender
  blender_init(&machine_ptr->blender);

  input_button_init(&machine_ptr->buttons[BLEND_BUTTON], 41);
  input_button_init(&machine_ptr->buttons[CLEAN_BUTTON], 39);
  input_button_init(&machine_ptr->buttons[STOP_BUTTON], 37);
  input_button_init(&machine_ptr->buttons[STEP_BUTTON], 0); // NOT IMPLIMENTED
  
  input_button_init(&machine_ptr->buttons[MOVE_UP], 47);
  input_button_init(&machine_ptr->buttons[MOVE_DOWN], 45);
  input_button_init(&machine_ptr->buttons[INITIALIZE], 43);
  step_request = 0;
}

void machine_process(machine_t* machine_ptr) {
  int i;
  update_current_position(&machine_ptr->blender);
  machine_ptr->cup_detect_reading = new_ping_c_wrapper_sonar_ping(machine_ptr->cup_detect_sensor);

  // ---------- BEGING INPUT BUTTON SECTION ----------
  for (i = 0; i < BUTTON_COUNT; i++) {
    input_button_read(&machine_ptr->buttons[i]);
  }

  if (machine_ptr->current_state == MACHINE_STATE_IDLE) {
    if (machine_ptr->buttons[BLEND_BUTTON].current_state) {
      LOG_PRINT(LOGGER_VERBOSE, "Blender button pushed, starting blending, total actions: %d", blend_sequence.total_actions );
      machine_ptr->current_state = MACHINE_STATE_BLENDING;
    } else if (machine_ptr->buttons[CLEAN_BUTTON].current_state) {
      LOG_PRINT(LOGGER_VERBOSE, "Cleaning button pushed, starting cleaning");
      machine_ptr->current_state = MACHINE_STATE_CLEANING;
    }
  }
  
  if (machine_ptr->buttons[INITIALIZE].current_state) {
      LOG_PRINT(LOGGER_VERBOSE, "Initializing");
      machine_ptr->current_state = MACHINE_STATE_INITIALIZING;
  }
  
  if (machine_ptr->buttons[STOP_BUTTON].current_state && machine_ptr->current_state != MACHINE_STATE_IDLE) {
    LOG_PRINT(LOGGER_VERBOSE, "Stop button pushed, stopping machine");
    machine_stop(machine_ptr);
    machine_ptr->current_state = MACHINE_STATE_IDLE;
  }

//  if (machine_ptr->buttons[STEP_BUTTON].current_state) {
//    if (machine_ptr->current_state != MACHINE_STATE_STEPPING) {
//      machine_ptr->current_state = MACHINE_STATE_STEPPING;
//    }
//    step_request = 1;
//  }

  LOG_PRINT(LOGGER_DEBUG , "Machine Current State: %d - Button States: B%d - C%d - S%d - MU%d - MD%d - I%d - MP%d - CS%d", 
    machine_ptr->current_state,
    machine_ptr->buttons[BLEND_BUTTON].current_state,
    machine_ptr->buttons[CLEAN_BUTTON].current_state,
    machine_ptr->buttons[STOP_BUTTON].current_state,
    machine_ptr->buttons[MOVE_UP].current_state,
    machine_ptr->buttons[MOVE_DOWN].current_state,
    machine_ptr->buttons[INITIALIZE].current_state,
    machine_ptr->blender.position,
    machine_ptr->cup_detect_reading);

  // ---------- END INPUT BUTTON SECTION ----------

  // change to mediator rather than switch
  switch (machine_ptr->current_state) {
    case MACHINE_STATE_IDLE:    
      if (machine_ptr->buttons[MOVE_UP].current_state) {
        LOG_PRINT(LOGGER_VERBOSE, "MOVING UP, current position:%d, speed:%d", machine_ptr->blender.position, MOTOR_SPEED_HALF);
        blender_move(&machine_ptr->blender, BLENDER_MOVEMENT_UP, MOTOR_SPEED_HALF);
      } else if (machine_ptr->buttons[MOVE_DOWN].current_state) {
        LOG_PRINT(LOGGER_VERBOSE, "MOVING DOWN, current position:%d, speed:%d", machine_ptr->blender.position, MOTOR_SPEED_HALF);
        blender_move(&machine_ptr->blender, BLENDER_MOVEMENT_DOWN, MOTOR_SPEED_HALF);
      } else {
        //LOG_PRINT(LOGGER_VERBOSE, "IDLE");
        machine_stop(machine_ptr);
        blender_move(&machine_ptr->blender, BLENDER_MOVEMENT_IDLE, 0);
      }
    
      machine_ptr->current_step = 0;
      machine_ptr->last_step_time = millis();
      machine_ptr->last_jam_check_position = millis();
      break;
    case MACHINE_STATE_BLENDING:
      if (machine_execute_action(machine_ptr, &blend_sequence.actions_ptr[machine_ptr->current_step])) {
        // we finished the last action, let's move to the next action.
        LOG_PRINT(LOGGER_VERBOSE, "Bending step %d completed, percent complete:%d", machine_ptr->current_step, (100*machine_ptr->current_step+1)/blend_sequence.total_actions);
        if (blend_sequence.actions_ptr[machine_ptr->current_step].type == ACTION_MTP) {
          LOG_PRINT(LOGGER_VERBOSE, "current position:%d, desired position:%d, direction:%d", machine_ptr->blender.position, blend_sequence.actions_ptr[machine_ptr->current_step].mtp.new_position, blend_sequence.actions_ptr[machine_ptr->current_step].mtp.move_direction);
        } else if (blend_sequence.actions_ptr[machine_ptr->current_step].type == ACTION_ACTIVATE) {
          LOG_PRINT(LOGGER_VERBOSE, "toggling output:%d, desired state:%d", blend_sequence.actions_ptr[machine_ptr->current_step].activate.address, blend_sequence.actions_ptr[machine_ptr->current_step].activate.state);
        }
        machine_ptr->current_step++;
        machine_ptr->last_step_time = millis();
        machine_ptr->last_jam_check_position = millis();

        if (machine_ptr->current_step == blend_sequence.total_actions) {
          LOG_PRINT(LOGGER_VERBOSE, "Blending complete, stopping machine");
          machine_ptr->current_state = MACHINE_STATE_IDLE;
        }
      } else {
        // we need to check if we are actually moving properly
        if (blend_sequence.actions_ptr[machine_ptr->current_step].type == ACTION_MTP) {
          // we we are supposed to be moving, let's validate that we are actually moving
          if (machine_ptr->last_jam_check_time + 100 > millis()) {
            switch (blend_sequence.actions_ptr[machine_ptr->current_step].mtp.move_direction) {
              int where_should_we_be = 0;
              // change to ABS calc instead of switch
              case BLENDER_MOVEMENT_UP:
                  where_should_we_be = machine_ptr->last_jam_check_position - 5;
                  if (where_should_we_be < machine_ptr->blender.position) {
                    // JAMMED
                    LOG_PRINT(LOGGER_VERBOSE, "Jammed moving up");
                  }
                break;
              case BLENDER_MOVEMENT_DOWN:
                  where_should_we_be = machine_ptr->last_jam_check_position + 5;
                  if (where_should_we_be > machine_ptr->blender.position) {
                    // JAMMED
                    LOG_PRINT(LOGGER_VERBOSE, "Jammed moving down");
                  }
                break;
            }
            machine_ptr->last_jam_check_position = machine_ptr->blender.position;
            machine_ptr->last_jam_check_time = millis();
          }
        }
      }
      break;
    case MACHINE_STATE_CLEANING:
      if (machine_execute_action(machine_ptr, &clean_sequence.actions_ptr[machine_ptr->current_step])) {
        LOG_PRINT(LOGGER_VERBOSE, "Cleaning step %d completed, percent complete:%d", machine_ptr->current_step, (100*machine_ptr->current_step+1)/clean_sequence.total_actions);
        if (clean_sequence.actions_ptr[machine_ptr->current_step].type == ACTION_MTP) {
          LOG_PRINT(LOGGER_VERBOSE, "current position:%d, desired position:%d, direction:%d", machine_ptr->blender.position, clean_sequence.actions_ptr[machine_ptr->current_step].mtp.new_position, clean_sequence.actions_ptr[machine_ptr->current_step].mtp.move_direction);
        } else if (blend_sequence.actions_ptr[machine_ptr->current_step].type == ACTION_ACTIVATE) {
          LOG_PRINT(LOGGER_VERBOSE, "toggling output:%d, desired state:%d", blend_sequence.actions_ptr[machine_ptr->current_step].activate.address, blend_sequence.actions_ptr[machine_ptr->current_step].activate.state);
        }
        // we finished the last action, let's move to the next action.
        machine_ptr->current_step++;
        machine_ptr->last_step_time = millis();

        if (machine_ptr->current_step == clean_sequence.total_actions) {
          machine_ptr->current_state = MACHINE_STATE_IDLE;
        }
      }
      break;
    case MACHINE_STATE_STEPPING:
      if (step_request) {
        if (machine_execute_action(machine_ptr, &blend_sequence.actions_ptr[machine_ptr->current_step])) {
          // we finished the last action, let's move to the next action.
          LOG_PRINT(LOGGER_VERBOSE, "Bending step %d completed, percent complete:%d", machine_ptr->current_step, (100*machine_ptr->current_step)/blend_sequence.total_actions);
          if (blend_sequence.actions_ptr[machine_ptr->current_step].type == ACTION_MTP) {
            LOG_PRINT(LOGGER_VERBOSE, "current position:%d, desired position:%d, direction:%d", machine_ptr->blender.position, blend_sequence.actions_ptr[machine_ptr->current_step].mtp.new_position, blend_sequence.actions_ptr[machine_ptr->current_step].mtp.move_direction);
          }
          machine_ptr->current_step++;
          machine_ptr->last_step_time = millis();
  
          if (machine_ptr->current_step == blend_sequence.total_actions) {
            LOG_PRINT(LOGGER_VERBOSE, "Blending complete, stopping machine");
            machine_ptr->current_state = MACHINE_STATE_IDLE;
          }

          step_request = 0;
        }  
      }
      break;
    case MACHINE_STATE_INITIALIZING:
      if (machine_execute_action(machine_ptr, &initializing_action)) {
        machine_ptr->current_state = MACHINE_STATE_IDLE;
        machine_ptr->is_initialized = 1;
        LOG_PRINT(LOGGER_VERBOSE, "Machine Initialized");
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
      break;
    case ACTION_WAIT_FOR:
      return machine_wait_for(machine_ptr, &action->wait_for);
      break;
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

void machine_stop(machine_t* machine_prt) {
  // turn off blender and pump!!
  digitalWrite(PUMP_ADDRESS, HIGH);
  digitalWrite(BLENDER_ADDRESS, LOW);
}

char machine_wait_for(machine_t* machine_ptr, action_wait_for_t* wait_for) {
  switch (wait_for->type) {
    case WAIT_FOR_CUP_IN_PLACE:
      switch (wait_for->comparer) {
        case WAIT_FOR_LESS_THAN:
          if (machine_ptr->cup_detect_reading < wait_for->value) {
            return true;
          }
          break;
        case WAIT_FOR_GREATER_THAN:
          if (machine_ptr->cup_detect_reading > wait_for->value) {
            return true;
          }
          break;
        case WAIT_FOR_EQUALS:
          if (machine_ptr->cup_detect_reading == wait_for->value) {
            return true;
          }
          break;
      }
      break;
  }

  return false;
}

