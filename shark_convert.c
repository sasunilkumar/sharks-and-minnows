#include "kilolib.h"
#define RED RGB(1,0,0)
#define YELLOW RGB(1,1,0)
#define BLUE RGB(0,0,1)
#define GREEN RGB(0,1,0)
#define OFF RGB(0,0,0)
#define WHITE RGB(1,1,1)
#define STOP 0
#define FORWARD 1
#define LEFT 2
#define RIGHT 3

int message_sent = 0, new_message = 0, die = 4, rand = 0, curr;
message_t transmit_msg;
message_t *message_tx() {
  return &transmit_msg;
}
void message_tx_success() {
  message_sent = 1;
}

void set_motion(int new_motion){
  if (curr != new_motion) {
        curr = new_motion;
        if (curr == STOP) {
          set_motors(0,0);
        } else if (curr == FORWARD) {
          spinup_motors();
          set_motors(kilo_straight_left, kilo_straight_right);
        } else if (curr == LEFT) {
          spinup_motors();
          set_motors(kilo_turn_left, 0);
        } else {
          spinup_motors();
          set_motors(0, kilo_turn_right);
        }
  }
}

void setup() {
  transmit_msg.type = NORMAL;
  transmit_msg.data[0] = 1;
  transmit_msg.crc = message_crc(&transmit_msg);
}

void loop() {
  if (message_sent == 1) {
      message_sent = 0;
      set_color(RED);
      rand = rand_soft();
      if (die == 0) {
          set_motion(FORWARD);
      } else if (die == 1) {
          set_motion(LEFT);
      } else if (die == 2) {
          set_motion(RIGHT);
      } else {
	  set_color(WHITE);
	  set_motion(STOP);
	  delay(2500);
      }
      die = (rand % 3);
  }
}

int main() {
  kilo_init();
  kilo_message_tx = message_tx;
  kilo_message_tx_success = message_tx_success;
  kilo_start(setup, loop);

  return 0;
}
