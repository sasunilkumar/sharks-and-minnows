#include "kilolib.h"
#define RED RGB(1,0,0)
#define YELLOW RGB(1,1,0)
#define BLUE RGB(0,0,1)
#define CYAN RGB(0,1,1)
#define GREEN RGB(0,1,0)
#define OFF RGB(0,0,0)
#define WHITE RGB(1,1,1)
#define VIOLET RGB(1,0,1)
#define STOP 0
#define FORWARD 1
#define LEFT 2
#define RIGHT 3

int message_sent = 0, new_message = 0, curr = 0, rand = 0, die = 4;
uint32_t last_changed = 0;
int16_t current_light = 0;

message_t recv_message;
void message_rx(message_t *msg, distance_measurement_t *dist) {
  recv_message = *msg;
  if (recv_message.data[0] == 1) {
    new_message = 1;
  }
}

message_t transmit_msg;
message_t *message_tx() {
  return &transmit_msg;
}
void message_tx_success() {
  message_sent = 1;
}

int16_t sample_light() {
    int16_t number_of_samples = 0;
    long sum = 0;
    while (number_of_samples < 300) {
        int16_t sample = get_ambientlight();
        if (sample != -1) {
            sum = sum + sample;
            number_of_samples = number_of_samples + 1;
        }
    }
    return sum / number_of_samples;
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
  current_light = sample_light();
}

int16_t shark() {
  transmit_msg.type = NORMAL;
  transmit_msg.data[0] = 1;
  transmit_msg.crc = message_crc(&transmit_msg);
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
  return sample_light();
}

int16_t minnow() {
   if (new_message == 1) {
        set_color(VIOLET);
        set_motion(STOP);
	delay(1000);
	shark();
    } else {
        set_color(BLUE);
        rand = rand_soft();
        die = (rand % 3);
        if (die == 0) {
          set_motion(FORWARD);
        } else if (die == 1) {
          set_motion(LEFT);
        } else {
          set_motion(RIGHT);
        }
        delay(100);
      }
  return sample_light();
}

void loop() {
  if (new_message == 1 && current_light > 1000) {
    current_light = shark();
  } else if ((new_message == 0) && (current_light > 1000)) {
    current_light = minnow();
  } else {
    delay(500);
    if (new_message == 1) {
	current_light = shark();
    } else {
	current_light = minnow();
    }
  }
}

int main() {
  kilo_init();
  kilo_message_rx = message_rx;
  kilo_message_tx = message_tx;
  kilo_message_tx_success = message_tx_success;
  kilo_start(setup, loop);
  return 0;
}
