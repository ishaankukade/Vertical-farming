/*
 * Smart Farm Controller (ESP32 + FreeRTOS)
 *
 * Reads soil moisture, water level and light (LDR) sensors,
 * automatically runs a water pump when soil is dry and
 * turns on a grow LED when it is too dark.
 */

#define MOISTURE_SENSOR_PIN 34
#define WATER_LEVEL_PIN     35
#define LDR_PIN             32
#define PUMP_RELAY_PIN      26
#define LED_PIN             27   // LED pin
#define MOISTURE_THRESHOLD  20
#define LDR_THRESHOLD       40   // Below this % = too dark

struct FarmData {
  int  moisture;
  int  waterLevel;
  int  ldr;
  bool pumpOn;
  bool ledOn;   // track LED state
};
FarmData farmData = { 0, 0, 0, false, false };
SemaphoreHandle_t farmMutex;

void TaskReadMoisture(void *pvParameters) {
  for (;;) {
    int raw      = analogRead(MOISTURE_SENSOR_PIN);
    int moisture = map(raw, 4095, 0, 0, 100);
    if (xSemaphoreTake(farmMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
      farmData.moisture = moisture;
      Serial.printf("Moisture Level: %d %%\n", farmData.moisture);
      xSemaphoreGive(farmMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void TaskReadWaterLevel(void *pvParameters) {
  for (;;) {
    int raw        = analogRead(WATER_LEVEL_PIN);
    int waterLevel = map(raw, 0, 4095, 0, 100);
    if (xSemaphoreTake(farmMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
      farmData.waterLevel = waterLevel;
      Serial.printf("Water Level: %d %%\n", farmData.waterLevel);
      xSemaphoreGive(farmMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void TaskReadLDR(void *pvParameters) {
  for (;;) {
    int raw = analogRead(LDR_PIN);
    int ldr = map(raw, 0, 4095, 0, 100);
    if (xSemaphoreTake(farmMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
      farmData.ldr = ldr;
      Serial.printf("Light Level: %d %%\n", farmData.ldr);
      xSemaphoreGive(farmMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void TaskPump(void *pvParameters) {
  for (;;) {
    int currentMoisture = 0;
    if (xSemaphoreTake(farmMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
      currentMoisture = farmData.moisture;
      xSemaphoreGive(farmMutex);
    }
    if (currentMoisture < MOISTURE_THRESHOLD) {
      digitalWrite(PUMP_RELAY_PIN, LOW);
      if (xSemaphoreTake(farmMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        farmData.pumpOn = true;
        Serial.println("Pump: ON (moisture too low)");
        xSemaphoreGive(farmMutex);
      }
      vTaskDelay(pdMS_TO_TICKS(5000));
      digitalWrite(PUMP_RELAY_PIN, HIGH);
      if (xSemaphoreTake(farmMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        farmData.pumpOn = false;
        Serial.println("Pump: OFF");
        xSemaphoreGive(farmMutex);
      }
    } else {
      Serial.println("Pump: IDLE (moisture OK)");
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void TaskLED(void *pvParameters) {
  for (;;) {
    int currentLDR = 0;
    if (xSemaphoreTake(farmMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
      currentLDR = farmData.ldr;
      xSemaphoreGive(farmMutex);
    }
    if (currentLDR < LDR_THRESHOLD) {
      digitalWrite(LED_PIN, HIGH);
      if (xSemaphoreTake(farmMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        farmData.ledOn = true;
        Serial.println("LED: ON (too dark)");
        xSemaphoreGive(farmMutex);
      }
    } else {
      digitalWrite(LED_PIN, LOW);
      if (xSemaphoreTake(farmMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        farmData.ledOn = false;
        Serial.println("LED: OFF (light OK)");
        xSemaphoreGive(farmMutex);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  digitalWrite(PUMP_RELAY_PIN, HIGH);   // relay is active-LOW: HIGH = pump off
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);           // LED off by default
  farmMutex = xSemaphoreCreateMutex();
  xTaskCreate(TaskReadMoisture,   "ReadMoisture",   4096, NULL, 1, NULL);
  xTaskCreate(TaskReadWaterLevel, "ReadWaterLevel", 4096, NULL, 1, NULL);
  xTaskCreate(TaskReadLDR,        "ReadLDR",        4096, NULL, 1, NULL);
  xTaskCreate(TaskPump,           "Pump",           4096, NULL, 1, NULL);
  xTaskCreate(TaskLED,            "LED",            4096, NULL, 1, NULL);
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(10000));
}
