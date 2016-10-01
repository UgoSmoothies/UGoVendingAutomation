//kaikk, 10.01
/*
 * TODO: 
 * LEDs not implelemted
 * 
 */
#include "global.h"

#ifdef __cplusplus 
extern "C" {
#endif
  #include "actions.h"
  #include "machine.h"
#ifdef __cplusplus 
}
#endif

#ifdef USB_COMMUNICATION
#include "usb_comm.h"
#endif

machine_t machines[NUMBER_OF_MACHINES];

void auto_cycle_start(char*);
void clean_cycle_start(char*);
void initialize(char*);
void stop_machine(char*);
void machine_reblend(char*);

hmi_message_t heartbeat_msg;

long last_heartbeat;

void setup() {
  int i;
#ifdef USB_COMMUNICATION
  // set up the serial port for communucation
  Serial.begin(115200);
#endif

  // initialize the patterns
  blend_actions_init(0);
  clean_actions_init();
  initializing_action_init();
 
  // initialize the machine
  for (i = 0; i < NUMBER_OF_MACHINES; i++) {
    machine_init(&machines[i]);
  }
  
  // initialize the mediator
  mediator_init();

  // initialize the logger
  logger_init();

  // For the time being, explicitly initialize the machine
  //machines[0].current_state = MACHINE_STATE_INITIALIZING;
  
  mediator_register(MEDIATOR_AUTO_CYCLE_START, auto_cycle_start);
  mediator_register(MEDIATOR_CLEAN_CYCLE_START, clean_cycle_start);
  mediator_register(MEDIATOR_INITIALIZE, initialize);
  mediator_register(MEDIATOR_STOP_REQUEST, stop_machine);
  mediator_register(MEDIATOR_REBLEND, machine_reblend);
  mediator_register(MEDIATOR_JOG_TOP,machine_jog_top);
  mediator_register(MEDIATOR_JOG_BOTTOM, machine_bottom);
  mediator_register(MEDIATOR_MOVE_UP, machine_move_up);
  mediator_register(MEDIATOR_MOVE_DOWN, machine_move_down);

  heartbeat_msg.message_id = MSG_HEARTBEAT;

  LOG_PRINT(LOGGER_INFO, "Setup complete");
  
  last_heartbeat = millis();
  machines[0].last_step_time = millis();
}

void loop() {
  int i = 0;
#ifdef USB_COMMUNICATION
  // check if there are any messages to process
  usb_communication_process();
#endif

  // initialize the machine
  for (i = 0; i < NUMBER_OF_MACHINES; i++) {
    machine_check_safety_conditions(&machines[i]);
    machine_process(&machines[i]);
  } 

  // send out heartbeat message
  if (millis() > last_heartbeat + 1000) {
    last_heartbeat = millis(); 
    usb_communication_send_message(heartbeat_msg, 0);
  }
}

// TODO: seriously, we need to add validation to this, otherwise
// we can change from blending to cleaning without stopping.....
void auto_cycle_start(char* args) {
  LOG_PRINT(LOGGER_INFO, "Starting auto cycle");
  machines[0].current_state = MACHINE_STATE_BLENDING;
}

void clean_cycle_start(char*) {
  LOG_PRINT(LOGGER_INFO, "Starting clean cycle");
  machines[0].current_state = MACHINE_STATE_CLEANING;
}

void initialize(char*){
  LOG_PRINT(LOGGER_INFO, "Initializing");
  machines[0].current_state = MACHINE_STATE_INITIALIZING;
}

void stop_machine(char* args) {
  LOG_PRINT(LOGGER_INFO, "Stopping machine");
  machines[0].current_state = MACHINE_STATE_IDLE;
}

void machine_reblend(char* message){
  LOG_PRINT(LOGGER_VERBOSE, "Starting reblending");
  blend_sequence.actions_ptr[5].activate.state = OFF;
  machines[0].current_state = MACHINE_STATE_BLENDING;
}


void machine_jog_top(char* message){
  LOG_PRINT(LOGGER_VERBOSE, "Jogging top");
  digitalWrite(PUMP_ADDRESS, 1);
  digitalWrite(LIQUID_FILLING_VALVE_ADDRESS, 1);
}

void machine_jog_bottom(char* message){
  LOG_PRINT(LOGGER_VERBOSE, "Jogging bottom");
  digitalWrite(PUMP_ADDRESS, 1);
  digitalWrite(CLEANING_VALVE_ADDRESS, 1);
}

void machine_move_up(char* message){
  LOG_PRINT(LOGGER_VERBOSE, "Moving up");
  analogWrite(blender->actuator_up_address, 0);
  analogWrite(blender->actuator_down_address, speed);
}

void machine_move_down(char* message){
  LOG_PRINT(LOGGER_VERBOSE, "Moving down");
  analogWrite(blender->actuator_down_address, 0);   
  analogWrite(blender->actuator_up_address, speed);
}