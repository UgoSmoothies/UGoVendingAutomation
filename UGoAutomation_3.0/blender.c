#include "blender.h"

void blender_init(blender_t* blender){
  blender->position = 0;
  blender->movement = BLENDER_MOVEMENT_IDLE;  
  blender->blade = BLENDER_OFF;
  blender->water_pump = PUMP_OFF;
  blender->actuator_up_address = 8;
  blender->actuator_down_address = 7;
  blender->blender_ssr_address = 31;
  blender->blender_mech_address = 12;
  blender->water_pump_address = 6; 
  blender->encoder_address = A1;

  pinMode(blender->actuator_up_address, OUTPUT);
  pinMode(blender->actuator_down_address, OUTPUT);
  pinMode(blender->blender_ssr_address, OUTPUT);
  pinMode(blender->blender_mech_address, OUTPUT);
  pinMode(blender->water_pump_address, OUTPUT);
}

void blender_move(blender_t* blender, char direction){
  blender->movement = direction;
  
  switch (direction) {
    case BLENDER_MOVEMENT_DOWN:
      digitalWrite(blender->actuator_down_address, LOW);   
      digitalWrite(blender->actuator_up_address, HIGH);
    break;
    case BLENDER_MOVEMENT_UP:
      digitalWrite(blender->actuator_up_address, LOW);
      digitalWrite(blender->actuator_down_address, HIGH);
    break;
    case BLENDER_MOVEMENT_IDLE:
      digitalWrite(blender->actuator_up_address, HIGH);
      digitalWrite(blender->actuator_down_address, HIGH);
    break;
  }
}

void blender_on(blender_t* blender){
   digitalWrite(blender->blender_ssr_address, HIGH);
   delay(20);
   digitalWrite(blender->blender_mech_address, LOW);
   
   blender->blade = BLENDER_ON;
}
void blender_off(blender_t* blender){
   digitalWrite(blender->blender_ssr_address, LOW);
   delay(20);
   digitalWrite(blender->blender_mech_address, HIGH);
   
   blender->blade = BLENDER_OFF;
}

void pump_on(blender_t* blender){
   digitalWrite(blender->water_pump_address, LOW);
   blender->water_pump = PUMP_ON;
}

void pump_off(blender_t* blender){
   digitalWrite(blender->water_pump_address, HIGH);
   blender->water_pump = PUMP_OFF;
}

void update_current_position(blender_t* blender) {
  blender->position = analogRead(blender->encoder_address);
}



char move_to_position(blender_t* blender, action_move_to_position_t* action_move_to_position) {
  update_current_position(blender);

  // make sure we are actually moving
  if (blender->movement != action_move_to_position->move_direction) {
    blender_move(blender, action_move_to_position->move_direction);
  }
  
  switch (action_move_to_position->move_direction) {
    case BLENDER_MOVEMENT_DOWN:
      if(blender->position >= action_move_to_position->new_position) {
        // destination reached
        blender_move(blender, BLENDER_MOVEMENT_IDLE);
        return true;
      } else {
        // destination not reached
        return false;
      }
    break;
    case BLENDER_MOVEMENT_UP:
      if(blender->position <= action_move_to_position->new_position) {
        // destination reached
        blender_move(blender, BLENDER_MOVEMENT_IDLE);
        return true;
      } else {
        // destination not reached
        return false;
      }
    break;
    default:
      // error
        blender_move(blender, BLENDER_MOVEMENT_IDLE);
      return true; 
    break;
  }
}

char wait(blender_t* blender, unsigned long start_wait_time, action_wait_t* action_wait) {
  return ((start_wait_time + action_wait->time_to_wait) < millis());
}

char activate(blender_t* blender, action_activate_t* action_activate) {
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

  if (move_to_position(blender_ptr, action_move_to_position_ptr)) {
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
