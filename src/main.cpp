#include <Arduino.h>
#include <Simpletimer.h>
#include <Adafruit_NeoPixel.h>
#include <ESP32Time.h>
#include <WiFi.h>

Simpletimer timer1{};
Simpletimer timer2{};

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN 21

// How many NeoPixels are attached to the Arduino?
#define LED_PER_SEGMENT 6
#define NUMPIXELS LED_PER_SEGMENT * 7

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


//ESP32Time rtc;
ESP32Time rtc(0);  // offset in seconds GMT+1



int number = 0;
int color = 0;
const uint32_t colors[10] =
{
  strip.Color(255, 0, 0),     //Red
  strip.Color(0, 255, 0),     //Green
  strip.Color(0, 0, 255),     //Blue
  strip.Color(255, 222, 30),  //GOld
  strip.Color(0, 255, 255),   //Cyan
  strip.Color(180, 0, 255),   //Purple
  strip.Color(255, 0, 20),    //Magewnta
  strip.Color(255, 40, 0),    //Orange
  strip.Color(0, 255, 0),     //Green
  strip.Color(242, 90, 255),  //Pink
};

void printTime();
void initWiFi();
void lightSegment(uint32_t color, uint segment, uint display);
void displayNumber(uint32_t color, int number, int display);

void setup() {
  Serial.begin(9600);
  initWiFi();
  //rtc.setTime(00, 46, 15, 1, 12, 2022);  // 17th Jan 2021 15:24:30
  /*---------set with NTP---------------*/
  configTime(-28800, 3600, "ca.pool.ntp.org");
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)){
    rtc.setTimeStruct(timeinfo); 
  }
  strip.begin();
  strip.setBrightness(25);
  strip.show();  // Initialize all pixels to 'off'
}

void loop() {

  if (timer1.timer(1000)) {
    printTime();
    // show the 0s place minute on the display and color it according to the 10s place
    int minute = rtc.getMinute();
    displayNumber(colors[minute / 10 % 10],minute % 10,0);
  }
}


void printTime() {
  //struct tm timeinfo = rtc.getTimeStruct();
  Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));  // (String) returns strTime with specified format
  //Serial.println(timeinfo.tm_hour);

  // formating options  http://www.cplusplus.com/reference/ctime/strftime/
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin("SSID", "password");
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void lightSegment(uint32_t color, uint segment, uint display)
{
  int offset = segment * LED_PER_SEGMENT;
  for (int i = offset; i < offset + LED_PER_SEGMENT; i++)
  {
    strip.setPixelColor(i, color);
  }
}

void displayNumber(uint32_t color, int number, int display)
{
  strip.clear();

  switch (number)
  {
  case 0:
    lightSegment(color,0, display);
    lightSegment(color,1, display);
    lightSegment(color,2, display);
    lightSegment(color,3, display);
    lightSegment(color,4, display);
    lightSegment(color,5, display);
    break;

  case 1:
    lightSegment(color,2, display);
    lightSegment(color,3, display);
    break;

  case 2:
    lightSegment(color,1, display);
    lightSegment(color,2, display);
    lightSegment(color,6, display);
    lightSegment(color,5, display);
    lightSegment(color,4, display);
    break;

  case 3:
    lightSegment(color,1, display);
    lightSegment(color,2, display);
    lightSegment(color,3, display);
    lightSegment(color,4, display);
    lightSegment(color,6, display);
    break;

  case 4:
    lightSegment(color,0, display);
    lightSegment(color,2, display);
    lightSegment(color,3, display);
    lightSegment(color,6, display);
    break;

  case 5:
    lightSegment(color,1, display);
    lightSegment(color,0, display);
    lightSegment(color,6, display);
    lightSegment(color,3, display);
    lightSegment(color,4, display);
    break;

  case 6:
    lightSegment(color,0, display);
    lightSegment(color,1, display);
    lightSegment(color,3, display);
    lightSegment(color,4, display);
    lightSegment(color,5, display);
    lightSegment(color,6, display);
    break;

  case 7:
    lightSegment(color,1, display);
    lightSegment(color,2, display);
    lightSegment(color,3, display);
    break;

  case 8:
    lightSegment(color,0, display);
    lightSegment(color,1, display);
    lightSegment(color,2, display);
    lightSegment(color,3, display);
    lightSegment(color,4, display);
    lightSegment(color,5, display);
    lightSegment(color,6, display);
    break;

  case 9:
    lightSegment(color,0, display);
    lightSegment(color,1, display);
    lightSegment(color,2, display);
    lightSegment(color,3, display);
    lightSegment(color,4, display);
    lightSegment(color,6, display);
    break;
  }
  strip.show();
}

