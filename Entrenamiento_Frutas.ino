#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// Configuración de la fruta actual: 1 = Jitomate, 0 = Limón
const int ETIQUETA_FRUTA = 1   ; 


bool leyendoSensor = false; 

void setup() {
  Serial.begin(9600);
  if (!tcs.begin()) {
    Serial.println("Error: No se detecta el sensor TCS34725.");
    while (1);
  }
  
  
  Serial.println("SENSOR LISTO.");
  Serial.println("Envía '1' o 's' para INICIAR la captura.");
  Serial.println("Envía '0' o 'p' para DETENER la captura.");
  
}

void loop() {
  if (Serial.available() > 0) {
    char comando = Serial.read();

    if (comando == '1' || comando == 's' || comando == 'S') {
      leyendoSensor = true;
      Serial.println("\n--- CAPTURA INICIADA ---");
    } 
    else if (comando == '0' || comando == 'p' || comando == 'P') {
      leyendoSensor = false;
      Serial.println("\n--- CAPTURA DETENIDA ---\n");
    }
  }

  if (leyendoSensor) {
    uint16_t r_raw, g_raw, b_raw, c_raw;
    tcs.getRawData(&r_raw, &g_raw, &b_raw, &c_raw);

    if (c_raw > 0) {
      float r = (float)r_raw / c_raw;
      float g = (float)g_raw / c_raw;
      float b = (float)b_raw / c_raw;

      Serial.print(r, 4); Serial.print(" ");
      Serial.print(g, 4); Serial.print(" ");
      Serial.print(b, 4); Serial.print(" ");
      Serial.println(ETIQUETA_FRUTA);
    }

    delay(500);
  }
}