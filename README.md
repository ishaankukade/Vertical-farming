# Smart Farm Controller (ESP32 + FreeRTOS)



The idea is to use an ESP to collect sensor data with FreeRTOS, analyze it, and send control signals to actuators using simple threshold logic. The data is also sent to a ThingSpeak channel over HTTP, and you can see a dashboard with all the relevant visualizations.

## Features
- Reads **soil moisture**, **water tank level** and **ambient light (LDR)** every 2 seconds
- **Auto-watering:** runs the pump for 5 s when moisture drops below 20 %
- **Auto grow-light:** turns the LED on when light drops below 40 %
- Thread-safe shared data using a FreeRTOS mutex
- Live readings over Serial Monitor (115200 baud)

## Hardware
| Component            | ESP32 Pin |
|----------------------|-----------|
| Soil moisture sensor | GPIO 34   |
| Water level sensor   | GPIO 35   |
| LDR (light sensor)   | GPIO 32   |
| Pump relay (active-LOW) | GPIO 26 |
| LED                  | GPIO 27   |

## Getting Started
1. Install the [Arduino IDE](https://www.arduino.cc/en/software) and add ESP32 board support.
2. Open `smart_farm/smart_farm.ino`.
3. Select your ESP32 board and port, then upload.
4. Open Serial Monitor at **115200 baud**.

## Configuration
Edit the thresholds at the top of the sketch:
```cpp
#define MOISTURE_THRESHOLD  20   // % below which pump runs
#define LDR_THRESHOLD       40   // % below which LED turns on
```
Calibrate the moisture mapping (`map(raw, 4095, 0, 0, 100)`) for your sensor if readings look off.

## Project Structure
```
smart-farm-esp32/
├── smart_farm/
│   └── smart_farm.ino
├── README.md
└── .gitignore
```

## License
MIT


## Features
- Reads **soil moisture**, **water tank level** and **ambient light (LDR)** every 2 seconds
- **Auto-watering:** runs the pump for 5 s when moisture drops below 20 %
- **Auto grow-light:** turns the LED on when light drops below 40 %
- Thread-safe shared data using a FreeRTOS mutex
- Live readings over Serial Monitor (115200 baud)

## Hardware
| Component            | ESP32 Pin |
|----------------------|-----------|
| Soil moisture sensor | GPIO 34   |
| Water level sensor   | GPIO 35   |
| LDR (light sensor)   | GPIO 32   |
| Pump relay (active-LOW) | GPIO 26 |
| LED                  | GPIO 27   |

## Getting Started
1. Install the [Arduino IDE](https://www.arduino.cc/en/software) and add ESP32 board support.
2. Open `smart_farm/smart_farm.ino`.
3. Select your ESP32 board and port, then upload.
4. Open Serial Monitor at **115200 baud**.

## Configuration
Edit the thresholds at the top of the sketch:
```cpp
#define MOISTURE_THRESHOLD  20   // % below which pump runs
#define LDR_THRESHOLD       40   // % below which LED turns on
```
Calibrate the moisture mapping (`map(raw, 4095, 0, 0, 100)`) for your sensor if readings look off.

## Project Structure
```
smart-farm-esp32/
├── smart_farm/
│   └── smart_farm.ino
├── README.md
└── .gitignore
```

## License
MIT
