#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int LM35_PIN = A0;     
const float VREF = 5.0;      
const int N_SAMPLES = 10;     

float leerTemperaturaC() {
  long suma = 0;
  for (int i = 0; i < N_SAMPLES; i++) {
    suma += analogRead(LM35_PIN);
    delay(2);
  }
  float lectura = suma / (float)N_SAMPLES;
  float voltaje = lectura * (VREF / 1023.0); 
  float tempC = voltaje * 100.0;             // 10 mV/°C => *100
  return tempC;                              
}

void setup() {
  Serial.begin(9600);
  delay(250); 
  display.begin(i2c_Address, true); 
  display.setContrast(255);
  display.clearDisplay();
  display.display();
}

void loop() {
  float tC = leerTemperaturaC();
  Serial.print("Temp (C): ");
  Serial.println(tC, 1);

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(8, 0);
  display.println("LM35 Temperature (C)");

  display.setTextSize(3);

  if (tC >= 30.0) {
    display.fillRect(0, 22, SCREEN_WIDTH, 36, SH110X_WHITE);
    display.setTextColor(SH110X_BLACK, SH110X_WHITE); 
  } else {
    display.setTextColor(SH110X_WHITE); 
  }

  char buf[10];
  dtostrf(tC, 5, 1, buf); 
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);
  int x = (SCREEN_WIDTH - w) / 2;
  display.setCursor(x, 30);
  display.print(buf);

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(8, 60 - 8);

  display.display();

  delay(400); 
}
