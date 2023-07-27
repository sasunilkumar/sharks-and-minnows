#include "kilolib.h"
#include "common_functions.h"

/*
 * tagged: flag representing if minnow has finished conversion
 * new_message: flag representing if minnow has begun conversion
 * message_sent: flag representing shark message emission success
 * curr: current state of motion in finite state machine of possible motion directions
 * rand: random value generator
 * die: seed for random value generator
 * last_changed: time marker (compared with internal clock to count minutes passed since program start)
 * current_light: used to store result of calling sample_light function
*/
int new_message = 0, tagged = 0, message_sent = 0, curr = 0, rand = 0, die = 3;
uint32_t last_changed = 0;
int16_t current_light = 0;

// Function that listens for an incoming message and sets new_message flag on reception
message_t recv_message;
void message_rx(message_t *msg, distance_measurement_t *dist) {
  recv_message = *msg;
  if (recv_message.data[0] == 1) {
    new_message = 1;
  }
}

/* Functions continuously called once minnow converted to shark to
   emit a message, like a beacon */
message_t transmit_msg;
message_t *message_tx() {
  return &transmit_msg;
}
void message_tx_success() {
  message_sent = 1;
}

/* Function runs once upon program initialization
   takes measurement of light at beginning of execution */
void setup() {
  current_light = sample_light();
}

// Function called once converted to shark
int16_t shark() {
  transmit_msg.type = NORMAL;			// Set message parameters
  transmit_msg.data[0] = 1;
  transmit_msg.crc = message_crc(&transmit_msg);
  set_color(RED);				// Sharks are always red
  if (message_sent == 1) {
      message_sent = 0;				// Reset flag so message needs to send again
      rand = rand_soft();			// Creating random number generator
      random_walk(die);
      die = (rand % 3);				// Generate new random value
  }
  return sample_light();			// Return current light conditions to main loop
}

// Function initially called when minnow has not yet been converted
int16_t minnow() {
   if (new_message == 1) {	// Indicates transition behavior (turn purple, pause)
	tagged = 1;
	set_color(VIOLET);
	set_motion(STOP);
	delay(1500);		// 1.5 second conversion delay
	return sample_light();
    } else {
        set_color(BLUE); // Indicates random walk behavior with blue light for minnow
        rand = rand_soft();
	random_walk(die);
        die = (rand % 3);
    }
  return sample_light(); // Again return current light conditions to main loop
}

int16_t random_delayed() {
  last_changed = kilo_ticks;
  while (kilo_ticks < last_changed + 32) {
    if (tagged == 1) {
      shark();
    } else {
      minnow();
    }
  }
  return sample_light();
}

void loop() {
  if (current_light < 1020 && current_light > 1000) {
    set_motion(STOP);				// 1/2 second delay/movement blocking for either if in shadow
    set_color(GREEN);
    delay(500);
    current_light = random_delayed();
  } else {
    if (tagged == 1) {  // Shark, no delay as current_light is above threshold (nonshadow)
      current_light = shark();
    } else {
      current_light = minnow();			// Minnow, no delay as current_light is above threshold (nonshadow)
    }
  }
}

int main() {
  kilo_init();				// Begin program execution
  kilo_message_rx = message_rx;		// Begin listening loop
  kilo_message_tx = message_tx;		// Begin speaker loop
  kilo_message_tx_success = message_tx_success;	// Begin speaker validation loop
  kilo_start(setup, loop);		// Begin motion
  return 0;
}
