/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/student/Desktop/IoT/ArjunLab17/src/ArjunLab17.ino"
#include "MQTT.h"

void setup();
void loop();
#line 3 "/Users/student/Desktop/IoT/ArjunLab17/src/ArjunLab17.ino"
#define NORTH 0
#define EAST 1
#define NE 2
#define EN 3

void callback(char *topic, byte *payload, unsigned int length);
MQTT client("lab.theWCL.com", 1883, callback);

struct State
{
  int out[6];
  unsigned long wait;
  int nextState[2];
};

typedef struct State State_t;

State_t fsm[4] = {
    {{0, 0, 1, 1, 0, 0}, 100, {0, 2}},
    {{1, 0, 0, 0, 0, 1}, 100, {3, 1}},
    {{0, 1, 0, 1, 0, 0}, 5000, {1, 1}},
    {{1, 0, 0, 0, 1, 0}, 5000, {0, 0}}};

int cState = 0;
bool directionFlow = false;

SYSTEM_THREAD(ENABLED);

void setup()
{
  client.subscribe("FSM/EpicTopic1");
}
void loop()
{
  if (client.isConnected())
  {
    client.loop();
  }
  else
  {
    client.connect(System.deviceID());
    client.subscribe("FSM/EpicTopic1");
  }
  for (int i = 0; i < 6; i++)
  {
    int sendOut = fsm[cState].out[i];
    String trueFalse = String(sendOut);
    String numLed = String(i + 1);
    String newTop = "FSM/LED" + numLed;
    client.publish(newTop, trueFalse);
  }
  delay(fsm[cState].wait);
  cState = fsm[cState].nextState[directionFlow];
}
void callback(char *topic, byte *payload, unsigned int length)
{
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  if (String(p).equals("true"))
  {
    directionFlow = true;
    client.publish("FSM/CoolTopic1", "EastWait");
    client.publish("FSM/LED1", "true");
  }
  else if (String(p).equals("false"))
  {
    directionFlow = false;
    client.publish("FSM/CoolTopic1", "NorthWait");
    client.publish("FSM/LED1", "false");
  }
  else
  {
    client.publish("FSM/CoolTopic1", "Other thing");
  }
}