#include <Adafruit_BMP085.h>
#include <SoftwareSerial.h>

Adafruit_BMP085 bmp;
SoftwareSerial BLEmod(12, 13);
int rainPin = A0;
int moistureSensorPin = A1;
int vibrationSensorPin = 7;

int noRain = 1000;
int lightRain = 850;
int mediumRain = 600;
int heavyRain = 400;

int rainLevels[4] = {0}; // Array to store frequency of each rain level
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
}

long vibration() {
  long measurement = pulseIn(vibrationSensorPin, HIGH);
  return measurement;
}

void loop() {
  int moistureValue = analogRead(moistureSensorPin);
  int moisturePercentage = map(moistureValue, 0, 1023, 100, 0);
  tempPre();
  int sensorValue = analogRead(rainPin) + 20;  //+20 for calibration
  Serial.print("Rain Value ");
  Serial.println(sensorValue);

  //Soil Moisture and Vibration Calibration

  long measurement = vibration();
  Serial.print("Vibration: ");
  Serial.println(measurement);

  Serial.print("Moisture Percentage: ");
  Serial.print(moisturePercentage);
  Serial.println("%");

  if (moisurePercentage >= 25 || measurement >= 400) {    //Level 1 Error
    Serial.println("Soil is wet and possible movent of soil detected, soil may be softened.");
  } else if ((moisturePercentage >= 50 && moisturePercentage <= 70) && measurement == 700) {
    Serial.println("Slight movements on soil detected, soil is too wet. Possible Landslide might occur.");
  } else if (moisturePercentage >= 70 && measurement >= 700) {
    Serial.println("Strong Movements on the Soil has been detected, Landslide might've occured.");
  }

  //Rain Sensor Classification

  if (millis() - startTime >= 10000) {
    reportMostFrequentRainLevel();
    startTime = millis(); // Reset start time
    memset(rainLevels, 0, sizeof(rainLevels)); // Reset rain level counts
  }

  if (sensorValue >= noRain) {
    Serial.println("No Rain Detected");
    rainLevels[0]++;
  } else if (sensorValue <= noRain && sensorValue >= lightRain) {
    Serial.println("Light Rain Detected");
    rainLevels[1]++;
  } else if (sensorValue <= lightRain && sensorValue >= mediumRain){
    Serial.println("Medium Rain Detected");
    rainLevels[2]++;
  } else if (sensorValue <= mediumRain || sensorValue <= HeavyRain){
    Serial.println("Heavy Rain Detected");
    rainLevels[3]++;
  }

  //Rain Sensor Most Frequent Classification (10 seconds)

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
