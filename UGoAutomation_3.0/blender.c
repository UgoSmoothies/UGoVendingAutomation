#include "blender.h"

void blender_init(blender_t* blender){
  blender->position = 0;
  blender->movement = BLENDER_MOVEMENT_IDLE;  
  blender->blade = BLENDER_OFF;
  blender->water_pump = PUMP_OFF;
  blender->actuator_up_address = 5;
  blender->actuator_down_address = 6;
  blender->blender_ssr_address = BLENDER_ADDRESS;
  blender->water_pump_address = PUMP_ADDRESS; 
  blender->encoder_address = A1;
  blender->actuator_up_enabled_address = 7;
  blender->actuator_down_enabled_address = 8;

  pinMode(blender->actuator_up_address, OUTPUT);
  pinMode(blender->actuator_down_address, OUTPUT);
  pinMode(blender->blender_ssr_address, OUTPUT);
  pinMode(blender->water_pump_address, OUTPUT);
  pinMode(blender->actuator_up_enabled_address, OUTPUT);
  pinMode(blender->actuator_down_enabled_address, OUTPUT);

  // for now we are going to activate motor enabled. later on we
  // will check safety and activated as needed
  digitalWrite(blender->actuator_up_enabled_address, ON);
  digitalWrite(blender->actuator_down_enabled_address, ON);
}

void blender_move(blender_t* blender, char direction, char speed){
  blender->movement = direction;
  
  switch (direction) {
    case BLENDER_MOVEMENT_DOWN:
      analogWrite(blender->actuator_down_address, 0);   
      analogWrite(blender->actuator_up_address, speed);
    break;
    case BLENDER_MOVEMENT_UP:
      digitalWrite(blender->actuator_up_address, 0);
      digitalWrite(blender->actuator_down_address, speed);
    break;
    case BLENDER_MOVEMENT_IDLE:
      digitalWrite(blender->actuator_up_address, 0);
      digitalWrite(blender->actuator_down_address, 0);
    break;
  }
}

void update_current_position(blender_t* blender) {
  blender->position = analogRead(blender->encoder_address);
}

char move_to_position(blender_t* blender, unsigned long start_time, action_move_to_position_t* action_move_to_position) {
  update_current_position(blender);

  // make sure we are actually moving
  if (blender->movement != action_move_to_position->move_direction) {
    LOG_PRINT(LOGGER_VERBOSE, "Activating the motor to move %s", action_move_to_position->move_direction == BLENDER_MOVEMENT_DOWN ? "down" : "up");
    blender_move(blender, action_move_to_position->move_direction, action_move_to_position->speed);
  }

  // add a timeout in case it gets jammed
  if (start_time + action_move_to_position->time_out < millis()) {
    LOG_PRINT(LOGGER_VERBOSE, "Movement timeout");
    return true;
  }
  
  switch (action_move_to_position->move_direction) {
    case BLENDER_MOVEMENT_DOWN:
      if(blender->position >= action_move_to_position->new_position) {
        // destination reached
        blender_move(blender, BLENDER_MOVEMENT_IDLE, 0);
        return true;
      } else {
        // destination not reached
        return false;
      }
    break;
    case BLENDER_MOVEMENT_UP:
      if(blender->position <= action_move_to_position->new_position) {
        // destination reached
        blender_move(blender, BLENDER_MOVEMENT_IDLE, 0);
        return true;
      } else {
        // destination not reached
        return false;
      }
    break;
    default:
      // error
        blender_move(blender, BLENDER_MOVEMENT_IDLE, 0);
      return true; 
    break;
  }
}

char wait(blender_t* blender, unsigned long start_wait_time, action_wait_t* action_wait) {
  return ((start_wait_time + action_wait->time_to_wait) < millis());
}

char activate(blender_t* blender, action_activate_t* action_activate) {
    digitalWrite(action_activate->address, action_activate->state);
    return 1;
}

char agitate(blender_t* blender_ptr, action_agitate_t* action_agitate) {
  // agitate is a series of move_to_positions, this is where we set it up
  action_move_to_position_t* action_move_to_position_ptr;

  if(!action_agitate->is_running) {
    // first entry point
    update_current_position(blender_ptr);
    
    action_agitate->is_running = 1;
    action_agitate->start_position = blender_ptr->position;
  }
  
  if ((!action_agitate->current_step && action_agitate->start_direction) || (action_agitate->current_step && !action_agitate->start_direction)) {
    // LOWER
    action_move_to_position_ptr->new_position = action_agitate->start_position + action_agitate->lowering_distance ;
    action_move_to_position_ptr->move_direction = BLENDER_MOVEMENT_DOWN;
  } else {
    // RAISE
    action_move_to_position_ptr->new_position = action_agitate->start_position - action_agitate->rising_distance ;
    action_move_to_position_ptr->move_direction = BLENDER_MOVEMENT_UP;
  }

  if (move_to_position(blender_ptr, millis(), action_move_to_position_ptr)) {
    // we are in the right position
    action_agitate->start_position = blender_ptr->position;
    
    if (action_agitate->current_step) {
      action_agitate->current_cycle++;
    }
    
    action_agitate->current_step = !action_agitate->current_step;
  }
  
  // if we have ran the required repetitions return true
  if (action_agitate->current_cycle == action_agitate->repeat_cycles) {
    action_agitate->is_running = 0;
    action_agitate->current_cycle = 0;
    action_agitate->current_step = 0;
    return 1;
  }
}
