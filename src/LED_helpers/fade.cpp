/* Fade.cpp helper functions to soft on or soft off LEDs. For esp8266 */
#include "fade.h"
#include <LightStateService.h>
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int logistic( double x, uint8_t maxval) {
    double maxval_as_percentage = maxval / 100.0;
    double u = -K*(x - MIDPOINT);
    return (int) floor(mapfloat(maxval_as_percentage/(1+exp(u)), 0, maxval_as_percentage, 0, MAX_BRIGHTNESS));
}

void soft_off(LightState* light_state, int time_to_off) {
  if (light_state->ledOn == LED_ON) {
    int time_delta = time_to_off / NUM_TIMESTEPS;
    int total_time = 0;
    unsigned long previous_millis = 0;
    unsigned long value = 0;
    while (total_time < time_to_off) {
      unsigned long current_millis = millis();
      
      if (current_millis - previous_millis >= time_delta) {
        previous_millis = current_millis;
        value += logistic(total_time, light_state->maxBrightness);
        total_time += time_delta;
      //   analogWrite(LED_BUILTIN, value);
        analogWrite(LED_PIN, value);
      }
    }
    // light_state->ledOn = LED_OFF;
    // light_state->ledBrightness = MAX_BRIGHTNESS - value; // on off is inverted for esp8266
  }
}

void soft_on(LightState* light_state, int time_to_on) {
  if (light_state->ledOn == LED_OFF) {
    int time_delta = time_to_on / NUM_TIMESTEPS;
    int total_time = 0;
    unsigned long previous_millis = 0;
    unsigned long value = MAX_BRIGHTNESS;
    while (total_time < time_to_on) {
      unsigned long current_millis = millis();
      
      if (current_millis - previous_millis >= time_delta) {
        previous_millis = current_millis;
        value -= logistic(total_time, light_state->ledBrightness);
        total_time += time_delta;
      //   analogWrite(LED_BUILTIN, value);
        analogWrite(LED_PIN, value);
      }
    }
    // light_state->ledOn = LED_ON;
    // light_state->ledBrightness = MAX_BRIGHTNESS - value; // on off is inverted for esp8266
  }
}