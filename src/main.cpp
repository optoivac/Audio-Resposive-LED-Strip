#include <Adafruit_NeoPixel.h>

#define PIN 8  // Pin where the LED strip is connected
#define NUM_LEDS 60  // Number of LEDs in the strip
#define SMOOTHING 4  // Reduced number of values to average for more responsiveness

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

int amplitudeValues[SMOOTHING];  // Array to store amplitude values for smoothing
int amplitudeIndex = 0;
int totalAmplitude = 0;

// Function prototypes
void setStripColor(uint32_t color);
uint32_t calculateColor(int amplitude);

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();  // Initialize all pixels to 'off'
  Serial.println("Setup complete");

  // Initialize smoothing array
  for (int i = 0; i < SMOOTHING; i++) {
    amplitudeValues[i] = 0;
  }
}

void loop() {
  if (Serial.available() > 0) {
    int amplitude = Serial.parseInt();
    Serial.print("Received amplitude: ");  // Debug print
    Serial.println(amplitude);

    // Update smoothing array
    totalAmplitude -= amplitudeValues[amplitudeIndex];
    amplitudeValues[amplitudeIndex] = amplitude;
    totalAmplitude += amplitude;
    amplitudeIndex = (amplitudeIndex + 1) % SMOOTHING;

    // Calculate the average amplitude
    int averageAmplitude = totalAmplitude / SMOOTHING;

    uint32_t color = calculateColor(averageAmplitude);
    setStripColor(color);
  }
}

uint32_t calculateColor(int amplitude) {
  // Enhanced color mapping for better visualization
  uint8_t red, green, blue;
  
  // Map the amplitude range appropriately
  amplitude = map(amplitude, 0, 1024, 0, 255);
  
  if (amplitude < 85) {
    red = amplitude * 3;
    green = 255 - amplitude * 3;
    blue = 0;
  } else if (amplitude < 170) {
    red = 255 - (amplitude - 85) * 3;
    green = 0;
    blue = (amplitude - 85) * 3;
  } else {
    red = 0;
    green = (amplitude - 170) * 3;
    blue = 255 - (amplitude - 170) * 3;
  }

  return strip.Color(red, green, blue);
}

void setStripColor(uint32_t color) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
