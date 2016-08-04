
#include "input_button.h"

void input_button_init(input_button_t* input_button, char address) {
  input_button->address = address;
  input_button->current_state = 0;
  input_button->last_state = 0;
  input_button->last_debounce_time = millis();
  pinMode(input_button->address, INPUT_PULLUP);
}

void input_button_read(input_button_t* input) {
  int reading = digitalRead(input->address);

  if (reading != input->last_state) {
    input->last_debounce_time = millis();
    LOG_PRINT(LOGGER_VERBOSE, "CHANGED STATE");
  }

  if ((millis() - input->last_debounce_time) > DEBOUNCING_TIME) {
    input->current_state = reading;
  }
}
