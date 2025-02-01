#include <Wire.h>
#include "rgb_lcd.h"

#define anInput     A0  // MQ135 sensor analog pin
#define co2Zero     55  // Calibration value for CO2
#define buzzerPin   8   // Buzzer connected to pin 8
#define redLEDPin   3   // Red LED connected to pin 3 for high CO2
#define greenLEDPin 4   // Green LED connected to pin 4 for low CO2
#define threshold   1000 // Threshold for CO2 level to activate buzzer
#define highThreshold 1000 // High CO2 level threshold for red LED

rgb_lcd lcd;

void setup() {
  pinMode(anInput, INPUT);
  pinMode(buzzerPin, OUTPUT);  // Set buzzer pin as output
  pinMode(redLEDPin, OUTPUT);  // Set red LED pin as output
  pinMode(greenLEDPin, OUTPUT);  // Set green LED pin as output
  Serial.begin(9600);
  
  lcd.begin(16, 2);  // Initialize the LCD (16x2)
  lcd.setRGB(0, 0, 255);  // Set background color to blue
  lcd.print("CO2 Monitor");
  delay(2000);
  lcd.clear();
}

void loop() {
  int co2now[10];
  int co2raw = 0;
  int co2ppm = 0;
  int zzz = 0;

  for (int x = 0; x < 10; x++) {
    co2now[x] = analogRead(anInput);
    delay(200);
  }

  for (int x = 0; x < 10; x++) {
    zzz += co2now[x];
  }

  co2raw = zzz / 10;
  co2ppm = co2raw - co2Zero;

  Serial.print("AirQuality=");
  Serial.print(co2ppm);
  Serial.println(" PPM");

  // Display CO2 reading on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);  // First line
  lcd.print("CO2: ");
  lcd.print(co2ppm);
  lcd.print(" PPM");

  lcd.setCursor(0, 1);  // Second line
  lcd.print("CO2 Detected");

  // Activate buzzer if CO2 level exceeds threshold
  if (co2ppm > threshold) {
    digitalWrite(buzzerPin, HIGH);  // Turn on buzzer
  } else {
    digitalWrite(buzzerPin, LOW);   // Turn off buzzer
  }

  // Control LED lights based on CO2 levels
  if (co2ppm > highThreshold) {
    digitalWrite(redLEDPin, HIGH);  // Turn on red LED for high CO2
    digitalWrite(greenLEDPin, LOW); // Turn off green LED
  } else if (co2ppm < threshold) {
    digitalWrite(greenLEDPin, HIGH);  // Turn on green LED for low CO2
    digitalWrite(redLEDPin, LOW);     // Turn off red LED
  } else {
    // If CO2 level is between the threshold (400 PPM) and highThreshold (700 PPM)
    digitalWrite(redLEDPin, LOW);    // Ensure red LED is off
    digitalWrite(greenLEDPin, LOW);  // Ensure green LED is off
  }
}
