#ifndef _COMMOM_H
#define _COMMOM_H
#include <Arduino.h>
void callback(char *topic, byte *message, unsigned int length);
void reconnect();
void setup_wifi();
void bm_setup();
String get_BM_payload();
#endif //_COMMOM_H