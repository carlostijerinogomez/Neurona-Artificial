#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


float w[3] = {26.821363, -20.791611, 14.432850};
float b    = -7.745699;

const int BOTON_CENSAR = 2;
const int LED_ROJO     = 8;
const int LED_VERDE    = 9;


float sigmoid(float z) {
  return 1.0 / (1.0 + exp(-z));
}

void setup() {
  Serial.begin(9600);
  pinMode(BOTON_CENSAR, INPUT_PULLUP);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  digitalWrite(LED_ROJO, LOW);
  digitalWrite(LED_VERDE, LOW);

  if (!tcs.begin()) {
    Serial.println("Error: No se detecta el sensor TCS34725. Revisa A4 (SDA) y A5 (SCL).");
    while (1);
  }

  Serial.println("Sistema listo. Presiona el boton para empezar a censar.");
}

void censarYClasificar() {
  uint16_t r_raw, g_raw, b_raw, c_raw;
  tcs.getRawData(&r_raw, &g_raw, &b_raw, &c_raw);

  if (c_raw == 0) {
    Serial.println("Error: Canal Clear = 0 (sin luz reflejada).");
    return;
  }

  float x[3];
  x[0] = (float)r_raw / c_raw; // R
  x[1] = (float)g_raw / c_raw; // G
  x[2] = (float)b_raw / c_raw; // B

  float z = x[0] * w[0] + x[1] * w[1] + x[2] * w[2] + b;
  float y = sigmoid(z);

  Serial.print("R: "); Serial.print(x[0], 4);
  Serial.print(" | G: "); Serial.print(x[1], 4);
  Serial.print(" | B: "); Serial.print(x[2], 4);
  Serial.print(" | Salida y = "); Serial.print(y, 4);

  if (y >= 0.5) {
    Serial.println("  --> JITOMATE (1)");
    digitalWrite(LED_ROJO, HIGH);
    digitalWrite(LED_VERDE, LOW);
  } else {
    Serial.println("  --> LIMON (0)");
    digitalWrite(LED_ROJO, LOW);
    digitalWrite(LED_VERDE, HIGH);
  }
}

void loop() {
  if (digitalRead(BOTON_CENSAR) == LOW) {
    delay(50);
    if (digitalRead(BOTON_CENSAR) == LOW) {
      censarYClasificar();

      while (digitalRead(BOTON_CENSAR) == LOW) {
        delay(10);
      }
      delay(50);
    }
  }
}