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

int curr = 0, rand = 0, die = 4;
uint32_t last_changed = 0;
int16_t current_light = 0;

int16_t sample_light()
{
    // The ambient light sensor gives noisy readings. To mitigate this,
    // we take the average of 300 samples in quick succession.
    int16_t number_of_samples = 0;
    long sum = 0;

    while (number_of_samples < 300)
    {
        int16_t sample = get_ambientlight();
        // -1 indicates a failed sample, which should be discarded.
        if (sample != -1)
        {
            sum = sum + sample;
            number_of_samples = number_of_samples + 1;
        }
    }

    // Compute the average.
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
  set_motion(STOP);
  if (current_light < 1000) {
    set_color(CYAN);
  }
  delay(2000);
}

int16_t minnow(int16_t current) {
  if (kilo_ticks > last_changed + 32) {
      last_changed = kilo_ticks;
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
        delay(150);
  }
  return sample_light();
}

void loop() {
  while (current_light > 1000) {
    current_light = minnow(current_light);
  }
  current_light = 0;
        set_color(RED);
        rand = rand_soft();
        die = (rand % 3);
        if (die == 0) {
          set_motion(FORWARD);
        } else if (die == 1) {
          set_motion(LEFT);
        } else {
          set_motion(RIGHT);
        }
        delay(150);

}

int main() {
  kilo_init();
  kilo_start(setup, loop);
  return 0;
}
