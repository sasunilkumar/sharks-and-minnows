//Defining symbolic constants for color codes and motion directions
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

/* Function that averages out several hundred samples of light to
   determine the light level at a given point in time */
int16_t sample_light() {
    int16_t number_of_samples = 0;
    long sum = 0;
    while (number_of_samples < 300) {
        int16_t sample = get_ambientlight();    // obtain reading from light sensor
        if (sample != -1) {                     // -1 indicates bad sample, discard
            sum = sum + sample;
            number_of_samples = number_of_samples + 1;
        }
    }
    return sum / number_of_samples;             // average readings over many samples
}

// Function representing FSM of possible motion directions
void set_motion(int new_motion){
  if (new_motion == STOP) {
    set_motors(0,0);                      // Kilobot stands still
  } else if (new_motion == FORWARD) {
    spinup_motors();                      // Kilobot moves forward
    set_motors(kilo_straight_left, kilo_straight_right);
  } else if (new_motion == LEFT) {
    spinup_motors();
    set_motors(kilo_turn_left, 0);        // Kilobot turns left
  } else {
    spinup_motors();
    set_motors(0, kilo_turn_right);       // Kilobot turns right
  }
}

// Function dictating random walk behavior
void random_walk(int die) {
      if (die == 0) {
          set_motion(FORWARD);
      } else if (die == 1) {
          set_motion(LEFT);
      } else if (die == 2) {
          set_motion(RIGHT);
      } else if (die == 3) {                                  // Error handling, shouldn't reach unless for shark init
          set_color(GREEN);
          set_motion(STOP);
	  delay(2000);
      } else if (die == 4) {
          set_motion(STOP);
          set_color(WHITE);
	  delay(1000);
          set_color(GREEN);
	  delay(1000);
      }
      delay(100);
}
