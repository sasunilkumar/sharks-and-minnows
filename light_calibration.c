#include "kilolib.h"
#define RED RGB(1,0,0)
#define BLUE RGB(0,0,1)
#define GREEN RGB(0,1,0)
int16_t current_light = 0;

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

void setup() {
  current_light = sample_light();
}

void loop() {
  current_light = sample_light();
  if (current_light > 1000) {
    set_color(RED);
  } else if (current_light < 1000 && current_light > 800) {
    set_color(BLUE);
  } else {
    set_color(GREEN);
  }
}

int main() {
  kilo_init();
  kilo_start(setup, loop);
  return 0;
}
