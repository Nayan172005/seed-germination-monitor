#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// LCD Setup 
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DHT Setup 
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Pin Definitions
const int moisturePin = A0;
const int ldrPin = A1;

// Calibration Values
const int dryValue = 1023;
const int wetValue = 550;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Seed Germination");
  lcd.setCursor(0, 1);
  lcd.print("  Monitoring  ");
  delay(2000);
  dht.begin();

  Serial.println("----------------------------------------");
  Serial.println("       SEED GERMINATION MONITORING       ");
  Serial.println("----------------------------------------");
}

void loop() {
  // Sensor Readings 
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int moistureRaw = analogRead(moisturePin);
  int ldrValue = analogRead(ldrPin);
  
  // Conversions 
  int moisturePercent = map(moistureRaw, dryValue, wetValue, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);
  int lightIntensity = map(ldrValue, 10, 1005, 0, 100);
  lightIntensity = constrain(lightIntensity, 0, 100);

  // Your Original Probability Calculation 
  float probability = calculateProbability(temperature, humidity, moisturePercent, lightIntensity);

  // LCD Display 
  updateLCD(temperature, humidity, moisturePercent, lightIntensity, probability);
  
  // Enhanced Serial Monitor Output (NEW ALERTS)
  printSerialDataWithAlerts(temperature, humidity, moisturePercent, lightIntensity, probability);
  
  delay(5000);
}

// Probability logic
float calculateProbability(float temp, float hum, int moisture, int light) {
  // Temperature factor (optimal 20-25°C)
  float tempFactor = 0;
  if (temp >= 20 && temp <= 25) {
    tempFactor = 0.4;
  } else if (temp >= 15 && temp < 20) {
    tempFactor = 0.3 * (temp - 15) / 5;
  } else if (temp > 25 && temp <= 30) {
    tempFactor = 0.4 - (0.1 * (temp - 25) / 5);
  }
  
  // Moisture factor (optimal 40-70%)
  float moistureFactor = 0;
  if (moisture >= 40 && moisture <= 70) {
    moistureFactor = 0.3;
  } else if (moisture >= 20 && moisture < 40) {
    moistureFactor = 0.3 * (moisture - 20) / 20;
  } else if (moisture > 70 && moisture <= 90) {
    moistureFactor = 0.3 - (0.1 * (moisture - 70) / 20);
  }
  
  // Light factor (optimal 50-80%)
  float lightFactor = 0;
  if (light >= 50 && light <= 80) {
    lightFactor = 0.3;
  } else if (light >= 20 && light < 50) {
    lightFactor = 0.3 * (light - 20) / 30;
  } else if (light > 80 && light <= 100) {
    lightFactor = 0.3 - (0.1 * (light - 80) / 20);
  }
  
  return (tempFactor + moistureFactor + lightFactor) * 100;
}

// Your original LCD function
void updateLCD(float temp, float hum, int moisture, int light, float prob) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp, 1);
  lcd.print("C H:");
  lcd.print(hum, 0);
  lcd.print("%");
  
  lcd.setCursor(0, 1);
  lcd.print("M:");
  lcd.print(moisture);
  lcd.print("% L:");
  lcd.print(light);
  lcd.print("%");
  
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Germ Prob:");
  lcd.setCursor(0, 1);
  lcd.print(prob, 1);
  lcd.print("% ");
  if (prob > 70) lcd.print("(Good)");
  else if (prob > 40) lcd.print("(Fair)");
  else lcd.print("(Poor)");
}

void printSerialDataWithAlerts(float temp, float hum, int moisture, int light, float prob) {
  Serial.println("----------------------------------------");
  
  // Temperature Alert
  Serial.print("Temperature: ");
  Serial.print(temp, 1);
  Serial.print("°C\t");
  if (temp < 15) Serial.println("[ALERT: TOO COLD FOR GERMINATION]");
  else if (temp > 30) Serial.println("[ALERT: TOO HOT FOR GERMINATION]");
  else if (temp >= 20 && temp <= 25) Serial.println("[OPTIMAL TEMPERATURE]");
  else Serial.println("[Suboptimal Temperature]");

  // Humidity Alert
  Serial.print("Humidity: ");
  Serial.print(hum, 0);
  Serial.print("%\t\t");
  if (hum < 30) Serial.println("[ALERT: TOO DRY]");
  else if (hum > 80) Serial.println("[ALERT: TOO HUMID]");
  else if (hum >= 40 && hum <= 70) Serial.println("[OPTIMAL HUMIDITY]");
  else Serial.println("[Suboptimal Humidity]");

  // Soil Moisture Alert
  Serial.print("Soil Moisture: ");
  Serial.print(moisture);
  Serial.print("%\t");
  if (moisture < 20) Serial.println("[ALERT: CRITICALLY DRY]");
  else if (moisture > 80) Serial.println("[ALERT: WATERLOGGED]");
  else if (moisture >= 40 && moisture <= 70) Serial.println("[OPTIMAL MOISTURE]");
  else Serial.println("[Suboptimal Moisture]");

  // Light Alert
  Serial.print("Light Intensity: ");
  Serial.print(light);
  Serial.print("%\t");
  if (light < 20) Serial.println("[ALERT: TOO DARK]");
  else if (light > 90) Serial.println("[ALERT: TOO BRIGHT]");
  else if (light >= 50 && light <= 80) Serial.println("[OPTIMAL LIGHT]");
  else Serial.println("[Suboptimal Light]");

  // Probability (using your original classification)
  Serial.print("Germination Probability: ");
  Serial.print(prob, 1);
  Serial.print("%\t");
  if (prob > 70) Serial.println("[HIGH CHANCE]");
  else if (prob > 40) Serial.println("[MODERATE CHANCE]");
  else Serial.println("[LOW CHANCE]");

  Serial.println("----------------------------------------");
  Serial.println();

    // For the Serial Plotter (formatted for graphing):
  Serial.print("TEMP:"); Serial.print(temp); 
  Serial.print(",HUM:"); Serial.print(hum);
  Serial.print(",MOIST:"); Serial.print(moisture);
  Serial.print(",LIGHT:"); Serial.print(light);
  Serial.print(",PROB:"); Serial.println(prob); 
}
