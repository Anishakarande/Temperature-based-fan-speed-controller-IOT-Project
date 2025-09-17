#define BLYNK_TEMPLATE_NAME "temperature based fan speed controller"
#define BLYNK_TEMPLATE_ID "TMPL328bkwt3J"   // Add this line with your actual Template ID
#define BLYNK_AUTH_TOKEN "EGHCNxyRho6l4t1B4tR5QAtFAtfpeiAf"

#include <DHT.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Blynk credentials
char auth[] = "EGHCNxyRho6l4t1B4tR5QAtFAtfpeiAf";
char ssid[] = "Madhura";
char pass[] = "madhubaby";

// DHT sensor settings
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Motor control pins
int motor1Pin1 = D6;
int motor1Pin2 = D7;
int enable1Pin = D2;

// Blynk virtual pins
#define VIRTUAL_PIN_FAN_SPEED V1
#define VIRTUAL_PIN_TEMPERATURE V2

void setup() {
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  Serial.begin(115200);
  dht.begin();
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();

  float temperature = dht.readTemperature();

  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  int fanSpeedPercentage = map(temperature, 30, 35, 0, 100);
  fanSpeedPercentage = constrain(fanSpeedPercentage, 0, 100);
  int dutyCycle = map(fanSpeedPercentage, 0, 100, 0, 255);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Fan Speed: ");
  Serial.print(fanSpeedPercentage);
  Serial.println("%");

  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  analogWrite(enable1Pin, dutyCycle);

  Blynk.virtualWrite(VIRTUAL_PIN_TEMPERATURE, temperature);
  Blynk.virtualWrite(VIRTUAL_PIN_FAN_SPEED, fanSpeedPercentage);

  delay(2000);

  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  analogWrite(enable1Pin, 0);

  delay(1000);
}
