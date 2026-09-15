#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Smart Home"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// =====================================================
// Wi-Fi
// =====================================================

char ssid[] = "Velxio-GUEST";
char pass[] = "";

// =====================================================
// DHT11
// =====================================================

#define DHTPIN 26
#define DHTTYPE DHT11

// =====================================================
// Sensors
// =====================================================

#define LDR_PIN 34
#define PIR_PIN 27

#define TRIG_PIN 5
#define ECHO_PIN 18

// =====================================================
// Outputs
// =====================================================

#define LIGHT_LED 23
#define FAN_LED 22
#define BUZZER_PIN 21

// =====================================================
// Servo
// =====================================================

#define SERVO_PIN 19

#define SERVO_FREQ 50
#define SERVO_RESOLUTION 16

#define SERVO_MIN_DUTY 1638
#define SERVO_MAX_DUTY 8192

// =====================================================
// Objects
// =====================================================

DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

// =====================================================
// Manual Control Flags
// =====================================================

bool lightManual = false;
bool fanManual = false;
bool doorManual = false;

bool manualLightState = false;
bool manualFanState = false;
bool manualDoorState = false;

// =====================================================
// Servo Function
// =====================================================

void setServoAngle(int angle)
{
  angle = constrain(angle, 0, 180);

  uint32_t duty = map(
    angle,
    0,
    180,
    SERVO_MIN_DUTY,
    SERVO_MAX_DUTY
  );

  ledcWrite(SERVO_PIN, duty);
}

// =====================================================
// Read Sensors and Control System
// =====================================================

void sendSensorData()
{
  // ---------------------------------------------------
  // Read DHT11
  // ---------------------------------------------------

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // ---------------------------------------------------
  // Read LDR and PIR
  // ---------------------------------------------------

  int lightLevel = analogRead(LDR_PIN);
  int motion = digitalRead(PIR_PIN);

  // ---------------------------------------------------
  // Read HC-SR04
  // ---------------------------------------------------

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  int distance = 0;

  if (duration > 0)
  {
    distance = duration * 0.034 / 2;
  }

  // ---------------------------------------------------
  // Automatic Light
  // ---------------------------------------------------

  bool dark = lightLevel < 1500;

  if (!lightManual)
  {
    if (motion == HIGH && dark)
    {
      digitalWrite(LIGHT_LED, HIGH);
    }
    else
    {
      digitalWrite(LIGHT_LED, LOW);
    }
  }

  // ---------------------------------------------------
  // Automatic Fan
  // ---------------------------------------------------

  if (!fanManual)
  {
    if (!isnan(temperature) && temperature > 30)
    {
      digitalWrite(FAN_LED, HIGH);
    }
    else
    {
      digitalWrite(FAN_LED, LOW);
    }
  }

  // ---------------------------------------------------
  // Automatic Door
  // ---------------------------------------------------

  if (!doorManual)
  {
    if (distance > 0 && distance < 20)
    {
      setServoAngle(90);
    }
    else
    {
      setServoAngle(0);
    }
  }

  // ---------------------------------------------------
  // Buzzer
  // ---------------------------------------------------

  if (distance > 0 && distance < 20)
  {
    digitalWrite(BUZZER_PIN, HIGH);
  }
  else
  {
    digitalWrite(BUZZER_PIN, LOW);
  }

  // ===================================================
  // Serial Monitor
  // ===================================================

  Serial.println();
  Serial.println("================================");

  if (!isnan(temperature))
  {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
  }
  else
  {
    Serial.println("Temperature: ERROR");
  }

  if (!isnan(humidity))
  {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }
  else
  {
    Serial.println("Humidity: ERROR");
  }

  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  Serial.print("Motion: ");

  if (motion == HIGH)
  {
    Serial.println("MOTION DETECTED");
  }
  else
  {
    Serial.println("NO MOTION");
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("Light: ");

  if (digitalRead(LIGHT_LED) == HIGH)
  {
    Serial.println("ON");
  }
  else
  {
    Serial.println("OFF");
  }

  Serial.print("Fan: ");

  if (digitalRead(FAN_LED) == HIGH)
  {
    Serial.println("ON");
  }
  else
  {
    Serial.println("OFF");
  }

  Serial.print("Door: ");

  if (doorManual)
  {
    if (manualDoorState)
    {
      Serial.println("OPEN - MANUAL");
    }
    else
    {
      Serial.println("CLOSED - MANUAL");
    }
  }
  else
  {
    if (distance > 0 && distance < 20)
    {
      Serial.println("OPEN - AUTOMATIC");
    }
    else
    {
      Serial.println("CLOSED - AUTOMATIC");
    }
  }

  Serial.print("Buzzer: ");

  if (digitalRead(BUZZER_PIN) == HIGH)
  {
    Serial.println("ON");
  }
  else
  {
    Serial.println("OFF");
  }

  Serial.println("================================");

  // ===================================================
  // Send Data to Blynk
  // ===================================================

  if (!isnan(temperature))
  {
    Blynk.virtualWrite(V0, temperature);
  }

  if (!isnan(humidity))
  {
    Blynk.virtualWrite(V1, humidity);
  }

  Blynk.virtualWrite(V2, motion);
  Blynk.virtualWrite(V3, lightLevel);
}

// =====================================================
// Blynk Light Control - V4
// =====================================================

BLYNK_WRITE(V4)
{
  int value = param.asInt();

  lightManual = true;
  manualLightState = value;

  if (value == 1)
  {
    digitalWrite(LIGHT_LED, HIGH);
    Serial.println("Blynk Light Control: ON");
  }
  else
  {
    digitalWrite(LIGHT_LED, LOW);
    Serial.println("Blynk Light Control: OFF");
  }
}

// =====================================================
// Blynk Fan Control - V5
// =====================================================

BLYNK_WRITE(V5)
{
  int value = param.asInt();

  fanManual = true;
  manualFanState = value;

  if (value == 1)
  {
    digitalWrite(FAN_LED, HIGH);
    Serial.println("Blynk Fan Control: ON");
  }
  else
  {
    digitalWrite(FAN_LED, LOW);
    Serial.println("Blynk Fan Control: OFF");
  }
}

// =====================================================
// Blynk Door Control - V6
// =====================================================

BLYNK_WRITE(V6)
{
  int value = param.asInt();

  doorManual = true;
  manualDoorState = value;

  if (value == 1)
  {
    setServoAngle(90);
    Serial.println("Blynk Door Control: OPEN");
  }
  else
  {
    setServoAngle(0);
    Serial.println("Blynk Door Control: CLOSED");
  }
}

// =====================================================
// Setup
// =====================================================

void setup()
{
  Serial.begin(115200);

  // ---------------------------------------------------
  // Sensor Setup
  // ---------------------------------------------------

  dht.begin();

  pinMode(PIR_PIN, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // ---------------------------------------------------
  // Output Setup
  // ---------------------------------------------------

  pinMode(LIGHT_LED, OUTPUT);
  pinMode(FAN_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LIGHT_LED, LOW);
  digitalWrite(FAN_LED, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // ---------------------------------------------------
  // Servo Setup
  // ---------------------------------------------------

  ledcAttach(
    SERVO_PIN,
    SERVO_FREQ,
    SERVO_RESOLUTION
  );

  setServoAngle(0);

  // ---------------------------------------------------
  // Start Message
  // ---------------------------------------------------

  Serial.println();
  Serial.println("================================");
  Serial.println("       SMART HOME IoT");
  Serial.println("================================");

  Serial.println("Starting Blynk...");

  // ---------------------------------------------------
  // Connect to Blynk
  // ---------------------------------------------------

  Blynk.begin(
    BLYNK_AUTH_TOKEN,
    ssid,
    pass
  );

  Serial.println("Blynk connected!");

  // ---------------------------------------------------
  // Sensor update interval
  // ---------------------------------------------------

  timer.setInterval(
    2000L,
    sendSensorData
  );
}

// =====================================================
// Main Loop
// =====================================================

void loop()
{
  Blynk.run();
  timer.run();
}