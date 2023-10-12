#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char *ssid     = "";
const char *password = "";

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Nedela", "Ponedelnik", "Vtornik", "Sreda", "Cetvrtok", "Petok", "Sabota"};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){
  WiFi.begin(ssid, password);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.clearDisplay();
  testdrawtriangle();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(3,3);
  display.println("Manev");
  display.display();
  delay(1000);
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);    
  display.print("Povrzuvanje.");
  display.display();
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    display.print('.');
    display.display();
  }
  
  timeClient.begin();
  delay(1000);
  timeClient.update();
}

void loop() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(daysOfTheWeek[timeClient.getDay()]);
  display.print(F(" "));
  display.print(String(timeClient.getHours()));
  display.print(F(":"));
  display.print(String(timeClient.getMinutes()));
  display.print(F(":"));
  display.println(String(timeClient.getSeconds()));
  display.println(' ');
  display.println("Developed by:");
  display.println("  Dushko Manev");
  display.display();
  delay(1000);
}

void testdrawtriangle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(1000);
}
