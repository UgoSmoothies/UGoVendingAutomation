#ifndef BLENDER_H
#define BLENDER_H

typedef struct{
  int postiion;
  char movement;  
  char blade;
  char water_pump;
  int actuator_up_address;
  int actuator_down_address;
  int blender_ssr_address;
  int blender_mech_address;
  int water_pump_address; 
}blender_t;

void init_blender(blender_t*);
void move_down(blender_t*);
void move_up(blender_t*);
void blender_on(blender_t*);
void blender_off(blender_t*);
void pump_on(blender_t*);
void pump_off(blender_t*);

char move_to_position(blender_t*, int, char);
char wait(blender_t*, unsigned long, int);
char activate(blender_t*, char);


#endif


