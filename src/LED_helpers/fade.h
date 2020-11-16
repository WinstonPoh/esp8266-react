/* Soft on and soft off for LED*/
#ifndef Fade_h
#define Fade_h

#include <LightStateService.h>


// for logistic sigmoid
#define K 0.01
// mid point of 500 ms
#define MIDPOINT 500
#define MAXVAL 1
#define NUM_TIMESTEPS 100

// max brightness for analogWrite, not to be confused with LightState's max brightness
#define MAX_BRIGHTNESS 1024

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
int logistic( double x, uint8_t maxval);
void soft_off(LightState* light_state, int time_to_off);
void soft_on(LightState* light_state, int time_to_on);


#endif  // end Fade_h
