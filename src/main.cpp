#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <Arduino.h>
#include <time.h>
#include "EEPROM.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_NeoPixel.h>
#include <vector>
#include "Gen_op.h"

using std::vector;
#define PIN 4


const char *ssid = "Bleibmalocka-LAN";
const char *password = "21744762451772810918";

const char *NTP_SERVER = "de.pool.ntp.org";
const char *TZ_INFO = "CET-1CEST-3,M3.5.0/02:00:00,M10.5.0/03:00:00"; // enter your time zone (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)



tm oldTimeinfo;
tm timeinfo;
time_t now;
long unsigned lastNTPtime;
unsigned long lastEntryTime;

Adafruit_NeoPixel strip(110, PIN, NEO_RGB + NEO_KHZ800);
int minute;
int hour;
Gen_op Outputs(strip);


uint8_t Encode_PIN_DT = 17 ;
uint8_t Clock_PIN_CLK = 16; 
uint8_t Color_Select_SW = 26; 
int old_Color=0;
RTC_DATA_ATTR volatile uint8_t Color_Sel;
static long alteZeit;




uint8_t counter = 0; 
int oldCounter = 0;
int aState;
int aLastState;  



void Color_Select()
{
  if(millis() > alteZeit){
    Color_Sel=(Color_Sel+1)%9;
    alteZeit = millis() + 100;
  }

}

bool getNTPtime(int sec)
{

  {
    uint32_t start = millis();
    do
    {
      time(&now);
      localtime_r(&now, &timeinfo);
    //  Serial.print(".");
      delay(10);
    } while (((millis() - start) <= (1000 * sec)) && (timeinfo.tm_year < (2016 - 1900)));
    if (timeinfo.tm_year <= (2016 - 1900))
      return false; // the NTP call was not successful
    //Serial.print("now ");
    //Serial.println(now);
    char time_output[30];
    strftime(time_output, 30, "%a  %d-%m-%y %T", localtime(&now));
    //Serial.println(time_output);
    //Serial.println();
  }
  return true;
}

void showTime(tm localTime)
{
  Serial.print(localTime.tm_mday);
  Serial.print('/');
  Serial.print('/');
  Serial.print(localTime.tm_mon + 1);
  Serial.print('/');
  Serial.print(localTime.tm_year - 100);
  Serial.print('-');
  Serial.print(localTime.tm_hour);
  Serial.print(':');
  Serial.print(localTime.tm_min);
  Serial.print(':');
  Serial.print(localTime.tm_sec);
  Serial.print(" Day of Week ");
  if (localTime.tm_wday == 0)
    Serial.println(7);
  else
    Serial.println(localTime.tm_wday);
}

void wakeup()
{
esp_sleep_wakeup_cause_t reaseon;

}

void setBrightness(){

  int a = millis();
  while(millis()< a+1000 && Color_Sel == old_Color)
  {
  aState = digitalRead(Encode_PIN_DT); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState != aLastState){   
     alteZeit = millis() + 500;  
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(Clock_PIN_CLK) != aState) { 
      if (counter <245)
       counter = counter +10;
     } else {
    if(counter >10)
       counter = counter- 10;
     }
     Serial.print("Position: ");
     Serial.println(counter);
   } 
   aLastState = aState; // Updates the previous state of the outputA with the current state
   if (counter != oldCounter) 
   {
    oldCounter = counter;

    Outputs.set_param(hour, minute, Color_Sel, counter);
   }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\nNTP Time Test\n");
  WiFi.begin(ssid, password);

  int counterw = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    if (++counterw > 100) ESP.restart();
    Serial.print ( "." );
  }
  Serial.println("\n\nWiFi connected\n\n");

  configTime(0, 0, NTP_SERVER);
  // See https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv for Timezone codes for your region
  setenv("TZ", TZ_INFO, 1);

  if (getNTPtime(10)) {  // wait up to 10sec to sync
  } else {
    Serial.println("Time not set");
    ESP.restart();
  }
  //showTime(timeinfo);
  lastNTPtime = time(&now);
  lastEntryTime = millis();

  pinMode(Color_Select_SW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Color_Select_SW), Color_Select, RISING);  

  alteZeit = millis();

esp_sleep_enable_timer_wakeup(14400000000);
//esp_sleep_enable_gpio_wakeup()
  pinMode (Encode_PIN_DT,INPUT);
  pinMode (Clock_PIN_CLK,INPUT);

  aLastState = digitalRead(Encode_PIN_DT);
}

void loop()
{
  getNTPtime(10);
  //showTime(timeinfo);
    
  if(timeinfo.tm_min != oldTimeinfo.tm_min || Color_Sel != old_Color ){
    Outputs.set_param(timeinfo.tm_hour, timeinfo.tm_min , Color_Sel, counter);
    oldTimeinfo = timeinfo;
    old_Color = Color_Sel;
  }

  setBrightness();
}




