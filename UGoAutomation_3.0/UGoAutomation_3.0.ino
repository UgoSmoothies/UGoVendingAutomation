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

unsigned long start_time;

void setup() {
  int i;
#ifdef USB_COMMUNICATION
  // set up the serial port for communucation
  Serial.begin(9600);
#endif

  // initialize the patterns
  blend_actions_init();
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

  LOG_PRINT(LOGGER_INFO, "Setup complete");

  start_time = millis();
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
}

void auto_cycle_start(char* args) {
  LOG_PRINT(LOGGER_INFO, "Starting Auto Cycle");
  machines[0].current_state = MACHINE_STATE_BLENDING;
}
