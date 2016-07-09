#ifndef __MEDIATOR_H
#define __MEDIATOR_H

#include "global.h"

#define MEDIATOR_AUTO_CYCLE_START 0
#define MEDIATOR_STOP_REQUEST 1

typedef void (* ACTION_PTR)(char*);

void mediator_init();
void mediator_register(int event_id, ACTION_PTR action);
void mediator_unregister(int event_id, ACTION_PTR action);
void mediator_send_message(int event_id, char* message);

#endif
