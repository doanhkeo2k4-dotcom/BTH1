#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_AHTX0.h>

#define TFT_CS   5
#define TFT_DC   2
#define TFT_RST  4

#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  64
#define OLED_RESET     -1
#define OLED_ADDRESS   0x3C

Adafruit_ST7735  tft(TFT_CS, TFT_DC, TFT_RST);
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_AHTX0   aht;


void setup() {

  Serial.begin(115200);

  Wire.begin(21, 22);

  tft.initR(INITR_144GREENTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("OLED ERROR!");
    while (1);
  }

  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);

  if (!aht.begin()) {

    tft.setTextColor(ST77XX_RED);
    tft.setTextSize(2);
    tft.setCursor(10, 50);
    tft.print("AHT ERROR");

    oled.setCursor(0, 0);
    oled.print("AHT ERROR");
    oled.display();

    while (1);
  }

  tft.drawRect(0, 0, 128, 128, ST77XX_BLUE);

  tft.setCursor(25, 5);
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(2);
  tft.print("SENSOR");

  tft.drawLine(0, 28, 128, 28, ST77XX_BLUE);
}


void loop() {

  sensors_event_t hum, temp;
  aht.getEvent(&hum, &temp);

  float t = temp.temperature;
  float h = hum.relative_humidity;

  tft.fillRect(0, 30, 128, 98, ST77XX_BLACK);

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_ORANGE);
  tft.setCursor(5, 35);
  tft.print("Temperature");

  tft.setTextSize(3);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(5, 50);
  tft.print(t, 1);
  tft.print("C");

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_GREEN);
  tft.setCursor(5, 85);
  tft.print("Humidity");

  tft.setTextSize(3);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(5, 100);
  tft.print((int)h);
  tft.print("%");


  oled.clearDisplay();

  oled.drawRect(0, 0, 128, 64, SSD1306_WHITE);

  oled.setTextSize(1);
  oled.setCursor(40, 2);
  oled.print("MONITOR");

  oled.drawLine(0, 12, 128, 12, SSD1306_WHITE);

  oled.setCursor(5, 18);
  oled.print("TEMP");

  oled.setTextSize(2);
  oled.setCursor(5, 32);
  oled.print(t, 1);
  oled.cp437(true);
  oled.write(248);
  oled.print("C");

  oled.setTextSize(1);
  oled.setCursor(70, 18);
  oled.print("HUMI");

  oled.setTextSize(2);
  oled.setCursor(70, 32);
  oled.print((int)h);
  oled.print("%");

  oled.display();


  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print(" C  |  Humi: ");
  Serial.print(h);
  Serial.println(" %");


  delay(2000);
}
