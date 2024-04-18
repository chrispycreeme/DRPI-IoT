#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include <Adafruit_BMP085.h>
#include <SoftwareSerial.h>

ArduinoLEDMatrix matrix;

Adafruit_BMP085 bmp;
SoftwareSerial BLEmod(13, 12);
int rainPin = A0;
int moistureSensorPin = A1;
int vibrationSensorPin = 7;

int noRain = 1000;
int lightRain = 850;
int mediumRain = 600;
int heavyRain = 400;

int rainStatus = 0;
int landslideError = 0;
int rainLevels[4] = { 0 };  // Array to store frequency of each rain level
unsigned long startTime;

const int timeThreshold = 10;

void setup() {
  pinMode(moistureSensorPin, INPUT);
  pinMode(vibrationSensorPin, INPUT);
  pinMode(rainPin, INPUT);
  Serial.begin(9600);
  BLEmod.begin(9600);
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
  matrix.begin();

  matrix.beginDraw();

  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(70);

  const char text[] = " Hi!  ^0^ ";
  matrix.textFont(Font_4x6);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(text);
  matrix.endText(SCROLL_LEFT);

  matrix.endDraw();
}


long vibration() {
  long measurement = pulseIn(vibrationSensorPin, HIGH);
  return measurement;
}

void loop() {


  float temperatureRecord = bmp.readTemperature();
  float pressureRecord = bmp.readPressure();

  unsigned long startMillis = millis();  // Record start time for loop iteration
  // Record start time for BMP085 sensor

  int moistureValue = analogRead(moistureSensorPin);
  int moisturePercentage = map(moistureValue, 0, 1023, 100, 0);
  tempPre();
  unsigned long moistureResponseTime = millis() - startMillis;  // Calculate response time for moisture sensor

  int sensorValue = analogRead(rainPin) + 20;  //+20 for calibration
  Serial.print("Rain Value ");
  Serial.println(sensorValue);
  unsigned long rainResponseTime = millis() - startMillis;  // Calculate response time for rain sensor

  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure();
  unsigned long bmpResponseTime = millis() - startMillis;
  //Soil Moisture and Vibration Calibration

  long measurement = vibration();
  Serial.print("Vibration: ");
  Serial.println(measurement);
  unsigned long vibrationResponseTime = millis() - startMillis;  // Calculate response time for vibration sensor

  Serial.print("Moisture Percentage: ");
  Serial.print(moisturePercentage);
  Serial.println("%");

  if (moisturePercentage >= 25 && measurement <= 400) {  //Level 1 Error
    Serial.println("Soil is wet and possible movement of soil detected, soil may be softened.");
    landslideError = 1;
  } else if ((moisturePercentage >= 50 && moisturePercentage < 70) && (measurement > 400)) {  // Level 2 Error
    Serial.println("Slight movements on soil detected, soil is too wet. Possible Landslide might occur.");
    landslideError = 2;
  } else if (moisturePercentage >= 70 && measurement > 700) {  // Level 3 Error
    Serial.println("Strong Movements on the Soil has been detected, Landslide might've occurred.");
    landslideError = 3;
  } else {
    landslideError = 0;  // No error detected
  }


  //Rain Sensor Classification

  if (millis() - startTime >= 10000) {
    reportMostFrequentRainLevel();
    startTime = millis();                       // Reset start time
    memset(rainLevels, 0, sizeof(rainLevels));  // Reset rain level counts
  }

  if (sensorValue >= noRain) {
    Serial.println("No Rain Detected");
    rainLevels[0]++;
    rainStatus = 1;
  } else if (sensorValue <= noRain && sensorValue >= lightRain) {
    Serial.println("Light Rain Detected");
    rainLevels[1]++;
    rainStatus = 2;
  } else if (sensorValue <= lightRain && sensorValue >= mediumRain) {
    Serial.println("Medium Rain Detected");
    rainLevels[2]++;
    rainStatus = 3;
  } else if (sensorValue <= mediumRain || sensorValue <= heavyRain) {
    Serial.println("Heavy Rain Detected");
    rainLevels[3]++;
    rainStatus = 4;
  }

  unsigned long responseTime = millis() - startMillis;
  Serial.print("Moisture Sensor Response Time: ");
  Serial.print(moistureResponseTime);
  Serial.println(" ms");

  Serial.print("Rain Sensor Response Time: ");
  Serial.print(rainResponseTime);
  Serial.println(" ms");

  Serial.print("Vibration Sensor Response Time: ");
  Serial.print(vibrationResponseTime);
  Serial.println(" ms");

  Serial.print("Barometer Sensor Respose Time: ");
  Serial.print(bmpResponseTime);
  Serial.println(" ms");

  Serial.print("Total Response Time: ");
  Serial.print(responseTime);
  Serial.println(" ms");

  BLEmod.print(rainStatus);
  BLEmod.print(",");
  BLEmod.print(landslideError);
  BLEmod.print(",");
  BLEmod.print(temperatureRecord);
  BLEmod.print(",");
  BLEmod.println(sensorValue);
  LCD();
  delay(2000);
}

void reportMostFrequentRainLevel() {
  int maxIndex = 0;
  int maxCount = rainLevels[0];

  // Find index of rain level with highest frequency
  for (int i = 1; i < 4; i++) {
    if (rainLevels[i] > maxCount) {
      maxIndex = i;
      maxCount = rainLevels[i];
    }
  }

  Serial.println("Most Frequent Rain Detected: ");
  // Report the most frequent rain level detected
  switch (maxIndex) {
    case 0:
      Serial.println("No Rain");
      break;
    case 1:
      Serial.println("Light Rain Detected");
      break;
    case 2:
      Serial.println("Medium Rain Detected");
      break;
    case 3:
      Serial.println("Heavy Rain Detected");
      break;
  }
}

void tempPre() {
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");
}

void LCD() {
  matrix.beginDraw();

  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(50);

  // add the text
  const char text[] = "    DRPI-IoT ";
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(text);
  matrix.endText(SCROLL_LEFT);

  matrix.endDraw();
}
