#ifndef COMET_EFFECT_H
#define COMET_EFFECT_H

#include <FastLED.h>

#define LED_PIN    12       // Pin connected to the Data line of WS2812B
#define NUM_LEDS    12       // Number of LEDs in the strip
#define BRIGHTNESS  200      // Brightness of LEDs
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

// Function prototypes
void setupCometEffect();
void cometEffect();
void lightUpOneAtATime(CRGB color, int delayTime);

void setupCometEffect() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void cometEffect() {
  int tailLength = 4;                // Length of the comet tail
  int cometPosition = 0;             // Initialize comet position at the start

  // Run the comet across the entire strip
  while (cometPosition < NUM_LEDS) {
    // Clear the LEDs before drawing the comet
    fadeToBlackBy(leds, NUM_LEDS, 80); // Slight fade to create trailing effect

    // Set the comet's head to a bright color (blue-white)
    leds[cometPosition] = CRGB(180, 240, 255);

    // Draw the comet tail with fading brightness
    for (int i = 1; i <= tailLength; i++) {
      int index = cometPosition - i;
      if (index < 0) index += NUM_LEDS;  // Wrap around the LED strip

      // Make the tail dimmer as it gets farther from the head
      leds[index] = CRGB(180 / (i + 1), 240 / (i + 1), 255 / (i + 1));
    }

    // Move the comet's position forward
    cometPosition++;

    FastLED.show();
    delay(150); // Adjust delay for comet speed
  }

  // Optional: Clear the LEDs after the comet has finished
  fadeToBlackBy(leds, NUM_LEDS, 255); // Complete fade out to turn off all LEDs
  FastLED.show();
}

// Function to light up one LED at a time in a forward direction
void lightUpOneAtATime(CRGB color, int delayTime) {
  // Light up each LED in the strip one at a time
  for (int i = 0; i < NUM_LEDS; i++) {
    // Clear all LEDs first
    FastLED.clear();

    // Light up the current LED
    leds[i] = color;

    // Show the changes
    FastLED.show();
    
    // Delay to control the speed of lighting up LEDs
    delay(delayTime);
  }
}

#endif // COMET_EFFECT_H