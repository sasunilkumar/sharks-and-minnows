#include "kilolib.h"
#include "common_functions.h"

/*
 * message_sent: flag representing shark message emission success
 * die: seed for random value generator
 * rand: random value generator
 * current_light: used to store result of calling sample_light function
*/
int message_sent = 0, die = 4, rand = 0;
int16_t current_light = 0;

// Functions continuously called to emit a message, like a beacon
message_t transmit_msg;
message_t *message_tx() {
  return &transmit_msg;
}
void message_tx_success() {
  message_sent = 1;
}

/* Function runs once upon program initialization
   * takes measurement of light and sets message parameters
   * at beginning of program execution */
void setup() {
  transmit_msg.type = NORMAL;
  transmit_msg.data[0] = 1;
  transmit_msg.crc = message_crc(&transmit_msg);
  current_light = sample_light();
}

// Function dictating behavior of shark
int16_t shark() {
  if (message_sent == 1) {
      message_sent = 0;
      set_color(RED);
      rand = rand_soft();
      random_walk(die);
      die = (rand % 3);
  }
  return sample_light();
}

void loop() {
  if (current_light < 1020 && current_light > 1000) {	// Continuously check if light level above threshold
    set_motion(STOP);
    set_color(WHITE);
    delay(250);			// If below, block for 1/4 second
    shark();
  }
  current_light = shark();	// Take new light level reading
}

int main() {
  kilo_init();                          // Begin program execution
  kilo_message_tx = message_tx;         // Begin speaker loop
  kilo_message_tx_success = message_tx_success; // Begin speaker validation loop
  kilo_start(setup, loop);              // Begin motion
  return 0;
}
