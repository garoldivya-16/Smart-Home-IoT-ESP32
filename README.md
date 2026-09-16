# 🏠 Smart Home IoT Automation System

An IoT-based Smart Home Automation System developed using **ESP32, Blynk Cloud, and multiple sensors**. The system monitors temperature, humidity, light intensity, motion, and distance, while providing automatic and remote control of home appliances.

##  Project Overview

The Smart Home IoT system combines sensors, ESP32, Wi-Fi, and Blynk Cloud to create a simple and interactive home automation solution.

The ESP32 collects data from different sensors and processes it according to predefined automation rules. Sensor data is displayed on the Blynk dashboard, and users can also control devices remotely.

### System Architecture

```text
                    SMART HOME IoT
                           │
                         ESP32
                           │
          ┌────────────────┼────────────────┐
          │                │                │
        DHT11             LDR              PIR
          │                │                │
   Temperature &       Light Level      Motion
      Humidity
          │                │                │
          └────────────────┼────────────────┘
                           │
                      HC-SR04
                           │
                       Distance
                           │
                         Wi-Fi
                           │
                     Blynk Cloud
                           │
                  Blynk Dashboard
                           │
             ┌─────────────┼─────────────┐
             │             │             │
           Light          Fan           Door
            LED           LED           Servo
             │             │             │
             └─────────────┼─────────────┘
                           │
                         Buzzer
```

---

##  Features

*  Temperature monitoring using DHT11
*  Humidity monitoring using DHT11
*  Light intensity measurement using LDR
*  Motion detection using PIR
*  Distance measurement using HC-SR04
*  Automatic light control
*  Automatic fan control
*  Automatic door control using servo
*  Buzzer alert for nearby person detection
*  Remote control using Blynk dashboard
*  Real-time sensor data monitoring
*  Automatic and manual control logic
*  Wi-Fi-based IoT communication

---

##  Hardware Components

| Component     |    Quantity | Purpose                  |
| ------------- | ----------: | ------------------------ |
| ESP32 DevKit  |           1 | Main controller          |
| DHT11         |           1 | Temperature and humidity |
| LDR Module    |           1 | Light intensity          |
| PIR Sensor    |           1 | Motion detection         |
| HC-SR04       |           1 | Distance detection       |
| Servo Motor   |           1 | Door control             |
| LED           |           2 | Light and fan indication |
| Buzzer        |           1 | Alert indication         |
| 220Ω Resistor |           2 | LED current limiting     |
| Breadboard    |           1 | Circuit connection       |
| Jumper Wires  | As required | Connections              |

---

##  Pin Connections

| Component | Pin     | ESP32                |
| --------- | ------- | -------------------- |
| DHT11     | VCC     | 3.3V                 |
| DHT11     | GND     | GND                  |
| DHT11     | SIG     | GPIO 26              |
| LDR       | VCC     | 3.3V                 |
| LDR       | GND     | GND                  |
| LDR       | A0      | GPIO 34              |
| PIR       | +       | 3.3V                 |
| PIR       | D       | GPIO 27              |
| PIR       | −       | GND                  |
| HC-SR04   | VCC     | VIN                  |
| HC-SR04   | GND     | GND                  |
| HC-SR04   | TRIG    | GPIO 5               |
| HC-SR04   | ECHO    | GPIO 18              |
| Servo     | V+      | VIN                  |
| Servo     | GND     | GND                  |
| Servo     | Signal  | GPIO 19              |
| Light LED | Anode   | GPIO 23 through 220Ω |
| Light LED | Cathode | GND                  |
| Fan LED   | Anode   | GPIO 22 through 220Ω |
| Fan LED   | Cathode | GND                  |
| Buzzer    | +       | GPIO 21              |
| Buzzer    | −       | GND                  |

---

##  Software Used

* **Programming Language:** C/C++
* **Framework:** Arduino
* **Microcontroller:** ESP32
* **IoT Platform:** Blynk Cloud
* **Communication:** Wi-Fi
* **Simulation Platform:** Velxio

---

##  Blynk Virtual Pins

| Virtual Pin | Parameter       | Function             |
| ----------- | --------------- | -------------------- |
| V0          | Temperature     | Displays temperature |
| V1          | Humidity        | Displays humidity    |
| V2          | Motion          | Displays PIR status  |
| V3          | Light Intensity | Displays LDR value   |
| V4          | Light Control   | Controls light LED   |
| V5          | Fan Control     | Controls fan LED     |
| V6          | Door Control    | Controls servo door  |

---

##  Automation Logic

###  Automatic Light

The system checks both motion and light intensity.

```text
PIR detects motion
        +
Room is dark
        ↓
Light LED ON
```

The current simulation uses an LDR threshold of approximately **1500**.

---

###  Automatic Fan

The DHT11 measures the temperature.

```text
Temperature > 30°C
        ↓
Fan LED ON
```

If the temperature is 30°C or below:

```text
Fan LED OFF
```

---

###  Automatic Door

The HC-SR04 measures the distance of an object/person.

```text
Distance < 20 cm
        ↓
Servo → 90°
        ↓
Door OPEN
```

Otherwise:

```text
Servo → 0°
        ↓
Door CLOSED
```

---

###  Buzzer

The buzzer is activated when a person/object is detected near the door.

```text
Distance < 20 cm
        ↓
Buzzer ON
```

---

##  Remote Control

The Blynk dashboard provides remote control of:

* Light
* Fan
* Door

The dashboard also displays:

* Temperature
* Humidity
* Motion status
* Light intensity

Manual control from Blynk has priority over the automatic control logic.

---

##  Sensor Data

The ESP32 periodically sends sensor information to Blynk Cloud.

Example:

```text
Temperature: 25 C
Humidity: 50 %
Light Level: 138
Motion: NO MOTION
Distance: 9 cm
Light: OFF
Fan: OFF
Buzzer: ON
```

---

##  Working Principle

```text
Sensors
   ↓
ESP32
   ↓
Read Sensor Data
   ↓
Process Conditions
   ↓
Automatic Control
   ↓
Actuators

        AND

ESP32
   ↓
Wi-Fi
   ↓
Blynk Cloud
   ↓
Blynk Dashboard
   ↓
Remote Control
   ↓
ESP32
```

---

##  Simulation

The project was developed and tested using the **Velxio online electronics simulator**.

The simulation demonstrates:

* ESP32 operation
* Sensor readings
* Automatic control
* Servo control
* Buzzer operation
* Wi-Fi connectivity
* Blynk Cloud communication
* Remote dashboard control

Screenshots can be found in the `screenshots` folder.

---

##  Security Note

Do not upload private credentials to GitHub.

The following values should be replaced before publishing:

```cpp
#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"
```

Real Wi-Fi passwords and Blynk authentication tokens should never be stored in a public repository.

---

##  Future Scope

* Real AC appliance control using properly rated relays
* Mobile notifications
* Energy monitoring
* Voice control
* Smart door lock
* Gas and smoke detection
* Fire detection
* Camera integration
* AI-based automation
* Home security alerts
* MQTT integration
* Mobile application improvements

---

##  Real-World Safety

This project is currently a low-voltage prototype/simulation.

For a real home installation, ESP32 GPIO pins must **not** be connected directly to 230V AC appliances.

A real implementation should use:

* Properly rated relay/contactor modules
* Electrical protection
* Proper insulation
* Certified power supplies
* Suitable enclosures
* Correct grounding
* Appropriate wiring

High-voltage installation should be performed by a qualified electrician.

---

##  Learning Outcomes

Through this project, the following concepts were practiced:

* ESP32 programming
* C/C++ programming
* Sensor interfacing
* GPIO control
* Analog-to-digital conversion
* PWM/LEDC servo control
* Wi-Fi communication
* IoT architecture
* Blynk Cloud
* Virtual pins
* Automation logic
* Embedded systems simulation

---

##  Project Type

**IoT | Embedded Systems | Smart Home | ESP32 | Blynk | Automation**

---

##  Project Highlights

* Low-cost IoT-based smart home prototype
* Multiple sensors integrated with a single ESP32
* Real-time cloud monitoring
* Automatic and remote control
* Simulated before real-world implementation
* Modular design that can be extended with additional sensors and appliances
