#include "kilolib.h"
#include "common_functions.h"
int16_t current_light = 0;

void setup() {
  current_light = sample_light();
}

void loop() {
  current_light = sample_light();
  if (current_light < 1030 && current_light > 1020) {
    set_color(RED);
  } else if (current_light < 1020 && current_light > 1010) {
    set_color(GREEN);
  } else if (current_light < 1000 && current_light > 0) {
    set_color(BLUE);
  } else {
    set_color(OFF);
  }
}

int main() {
  kilo_init();
  kilo_start(setup, loop);
  return 0;
}
