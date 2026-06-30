// Coal Mine Monitoring System using Arduino UNO
// Sensors: MQ2 Gas Sensor, LM35 Temperature Sensor, Flame Sensor
// Outputs: LCD, Buzzer, Bluetooth HC-05

#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
SoftwareSerial BT(2, 3); // RX, TX

const int gasSensor = A0;
const int tempSensor = A1;
const int flameSensor = 4;
const int buzzer = 6;

int gasThreshold = 300;
float tempThreshold = 45.0;

void setup() {
  pinMode(flameSensor, INPUT);
  pinMode(buzzer, OUTPUT);

  lcd.begin(16, 2);
  BT.begin(9600);
  Serial.begin(9600);

  lcd.print("Coal Mine Safety");
  delay(2000);
  lcd.clear();
}

void loop() {
  int gasValue = analogRead(gasSensor);
  int tempValue = analogRead(tempSensor);

  // Convert LM35 analog value to temperature
  float voltage = tempValue * (5.0 / 1023.0);
  float temperature = voltage * 100.0;

  int flameDetected = digitalRead(flameSensor);

  // Display values on LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperature);
  lcd.print(" C ");

  lcd.setCursor(0, 1);
  lcd.print("Gas:");
  lcd.print(gasValue);
  lcd.print("   ");

  // Send values via Bluetooth
  BT.print("Temperature: ");
  BT.println(temperature);

  BT.print("Gas Level: ");
  BT.println(gasValue);

  // Gas Alert
  if (gasValue > gasThreshold) {
    digitalWrite(buzzer, HIGH);
    lcd.clear();
    lcd.print("Gas Leak Alert!");
    BT.println("Warning: Gas Leak Detected!");
    delay(2000);
  }

  // Temperature Alert
  if (temperature > tempThreshold) {
    digitalWrite(buzzer, HIGH);
    lcd.clear();
    lcd.print("High Temp Alert!");
    BT.println("Warning: High Temperature!");
    delay(2000);
  }

  // Flame Alert
  if (flameDetected == LOW) {
    digitalWrite(buzzer, HIGH);
    lcd.clear();
    lcd.print("Fire Detected!");
    BT.println("Warning: Fire Detected!");
    delay(2000);
  }

  // Normal condition
  if (gasValue <= gasThreshold && temperature <= tempThreshold && flameDetected == HIGH) {
    digitalWrite(buzzer, LOW);
  }

  delay(1000);
}