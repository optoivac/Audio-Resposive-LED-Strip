#include <Adafruit_NeoPixel.h>

#define PIN 8  // Pin where the LED strip is connected
#define NUM_LEDS 60  // Number of LEDs in the strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setStripColor(uint32_t color);
uint32_t calculateColor(int amplitude);

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();  // Initialize all pixels to 'off'
  Serial.println("Setup complete");
}

void loop() {
  if (Serial.available() > 0) {
    int amplitude = Serial.parseInt();
    Serial.print("Received amplitude: ");  // Debug print
    Serial.println(amplitude);

    uint32_t color = calculateColor(amplitude);
    setStripColor(color);
  }
}

uint32_t calculateColor(int amplitude) {
  uint8_t baseValue = 3;
  uint8_t maxAmplitude = 100;  // Adjust based on expected amplitude range
  float factor = (float)amplitude / maxAmplitude;  // Normalize amplitude to a factor between 0 and 1

  // Calculate RGB values based on amplitude
  uint8_t red = constrain(baseValue + (255 - baseValue) * factor, 0, 255);
  uint8_t green = constrain(baseValue + (255 - baseValue) * (1 - factor), 0, 255);
  uint8_t blue = baseValue;  // Keep blue as the base value

  // Adjust brightness based on amplitude
  uint8_t brightness = constrain(baseValue + (255 - baseValue) * factor, 0, 255);

  // Apply brightness to RGB values
  red = (red * brightness) / 255;
  green = (green * brightness) / 255;
  blue = (blue * brightness) / 255;

  Serial.print("Calculated color - Red: ");  // Debug print
  Serial.print(red);
  Serial.print(", Green: ");
  Serial.print(green);
  Serial.print(", Blue: ");
  Serial.print(blue);
  Serial.print(", Brightness: ");
  Serial.println(brightness);

  return strip.Color(red, green, blue);
}

void setStripColor(uint32_t color) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
