#ifndef USB_COMMUNICATION_H
#define USB_COMMUNICATION_H

#include "global.h"

#define START_OF_MESSAGE 0x557E
#define END_OF_MESSAGE 0x557F

#define MSG_AUTO_CYCLE            0x0001
#define MSG_MACHINE_ERROR         0x0002
#define MSG_MACHINE_STATE         0x0003`
#define MSG_GET_FIRMWARE_VERSION  0x0004
#define MSG_GET_SENSOR_STATE      0x0005
#define MSG_GET_ACTUATOR_STATE    0x0006
#define MSG_SANITIZE_BLENDER      0x0007
#define MSG_LOG                   0x0008

#define MAX_HMI_PAYLOAD_SIZE 200

/* CRC calculation macros */
#define CRC_INIT 0xFFFF
#define CRC(crcval,newchar) crcval = (crcval >> 8) ^ \
                                     crc_table[(crcval ^ newchar) & 0x00ff]

typedef struct __attribute__((__packed__, aligned(1))) {
    char level;
    char message[200];
} log_message_t;

typedef struct  __attribute__((__packed__, aligned(1))) {
  char protien;
  char liquid;
} auto_cycle_t;

typedef struct  __attribute__((__packed__, aligned(1))) {
  short major;
  short minor;
  short revision;
} firmware_t;

typedef struct  __attribute__((__packed__, aligned(1))) {
  short start_of_frame;
  short len;
  char src;
  char dest;
  short message_id;
  union {
    char payload[MAX_HMI_PAYLOAD_SIZE];
    auto_cycle_t auto_cycle;
    firmware_t firmware;
    log_message_t log_message;
  };
} hmi_message_t;

int usb_communication_process();
void usb_communication_send_message(hmi_message_t, unsigned int);
void usb_communication_create_default_message(short, hmi_message_t*);
void usb_communication_parse_message(short, char*);

#ifdef __cplusplus 
extern "C" {
#endif
void c_send_message(hmi_message_t msg, unsigned int size);
#ifdef __cplusplus 
}
#endif

#endif
