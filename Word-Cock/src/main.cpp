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

using std::vector;

//LED Stripes
#define PIN 4
#define PIN2 5 //16

Adafruit_NeoPixel strip(110, PIN, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel strip2(4, PIN2, NEO_RGB + NEO_KHZ800);

//WLAN
const char *ssid = "WLAN-HW12"; //"Bleibmalocka-LAN";//"FRITZ!Box WLAN 3270"; //Bleibmalocka-LAN             "FRITZ!Box 6591 Cable IZ";   WLAN-HW12
const char *password = "$Koller_3st!";// "21744762451772810918";//"6741829495683578"; //21744762451772810918    "09638417849357229831"          $Koller_3st!

//Uhrzeit
const char *NTP_SERVER = "de.pool.ntp.org";
const char *TZ_INFO = "CET-1CEST-3,M3.5.0/02:00:00,M10.5.0/03:00:00"; // enter your time zone (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)

tm oldTimeinfo;
tm timeinfo;
time_t now;
long unsigned lastNTPtime;
unsigned long lastEntryTime;

//Rotary Encoder
uint8_t Encode_PIN_DT = 17;
uint8_t Clock_PIN_CLK = 16; //5
uint8_t Color_Select_SW = 26;

static long alteZeit;

int old_Color = 0; //Farbe
volatile int Color_Sel;
RTC_DATA_ATTR int Color_Sel_Save = 0;

int aState;
int aLastState;
static long rotary_time;

RTC_DATA_ATTR int brightness = 50; //Helligkeit
int old_Brightness;

namespace O_ar
{
  /**                 H      A      L       B **/
  vector<int> halb = {65, 64, 63, 62};

  /**            E    S       I        S       T **/
  vector<int> intr = {109, 108, 106, 105, 104};
  /**            F    Ü       N        F       **/
  vector<int> min_5 = {102, 101, 100, 99};
  /**             Z     E      H         N **/
  vector<int> min_10 = {88, 89, 90, 91};
  /**            V   I   E   R   T   E   L**/
  vector<int> min_15 = {83, 82, 81, 80, 79, 78, 77};

  vector<vector<int>> mins = {min_5, min_10, min_15};

  /**            N    A     C   H**/
  vector<int> nach = {68, 69, 70, 71};
  /**         V   O   R**/
  vector<int> vor = {72, 73, 74};
  vector<vector<int>> vor_nach = {vor, nach};
  /**E   I   N   S**/
  vector<int> hours_1 = {46, 47, 48, 49};
  /**E   I   N   **/
  vector<int> hours_1_ = {46, 47, 48};
  /**Z    W   E   I**/
  vector<int> hours_2 = {44, 45, 46, 47};

  /**D   R   E   I**/
  vector<int> hours_3 = {42, 41, 40, 39};
  /**V  I     E   R**/
  vector<int> hours_4 = {29, 30, 31, 32};
  /**F  Ü   N   F**/
  vector<int> hours_5 = {36, 35, 34, 33};
  /**S  E  C   H  S **/
  vector<int> hours_6 = {1, 2, 3, 4, 5};
  /**S  I E  B  E N**/
  vector<int> hours_7 = {49, 50, 51, 52, 53, 54};
  /**A  C H  T**/
  vector<int> hours_8 = {20, 19, 18, 17};
  /**N  E U N**/
  vector<int> hours_9 = {25, 26, 27, 28};
  /**Z  E H N**/
  vector<int> hours_10 = {16, 15, 14, 13};
  /** E L  F**/
  vector<int> hours_11 = {22, 23, 24};
  /**Z  W Ö L F**/
  vector<int> hours_12 = {60, 59, 58, 57, 56};
  /**U  H R**/
  vector<vector<int>> hours = {hours_1, hours_2, hours_3, hours_4, hours_5, hours_6, hours_7, hours_8, hours_9, hours_10, hours_11, hours_12};
  vector<int> o_Uhr = {8, 9, 10};

  vector<int> sleep = {88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                       87, 86, 66, 67,
                       44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
                       31, 32, 33, 34,
                       0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21};
  
  vector<int> wakeup = {108,109,                     100,99,
                        88, 89,                      97,98,
                        87,86,                       78,77,
                        66,67,                       75,76,
                        65,64,                       56,55,
                        44,45,                       53,54,
                        43,42,      39, 38, 37,      34,33,
                        22,23,      26, 27, 28,      31,32,
                        21,20, 19,18,         14,13, 12,11,       
                        0, 1,  2, 3,          7 , 8, 9, 10
                        };
  vector<int>dick = {           105,104,103,
                                92,  93, 94,
                                83,  82, 81, 
                                70,  71, 72, 
                                61,  60, 59, 
                                48 , 49, 50, 
                     42,41,     39, 38,  37,     35, 34,
                 22, 23,24, 25, 26,  27, 28, 29, 30, 31, 32,
                 21, 20,19, 18, 17,  16, 15, 14, 13, 12, 11,
                     1,  2,      4,   5,  6,     8,  9
  };
 
  /**
 *109   108 107 106 105 104 103 102 101 100 99
 * E    S   K   I   S   T   L   F   Ü   N   F  
 * 88   89  90  91  92  93  94  95  96  97  98
 * Z    E   H   N   Z   W   A  N    Z   I   G
 * 87   86  85  84  83  82  81  80  79  78  77  
 * D    R   E   I   V   I   E   R   T   E   L
 * 66   67  68  69  70  71  72  73  74  75  76
 * T    G   N   A   C   H   V   O   R   J   M
 * 65   64  63  62  61  60  59  58  57  56  55
 * H    A   L   B   X   Z   W   Ö   L   F   P
 * 44   45  46  47  48  49  50  51  52  53  54
 * Z    W   E   I   N   S   I   E   B   E   N
 * 43   42 41   40 39   38 37   36 35   34 33
 * K    D   R   E   I   R   H   F   Ü   N   F
 * 22   23  24  25  26  27  28  29  30  31  32
 * E    L   F   N   E   U   N   V   I   E   R
 * 21   20  19  18  17  16  15  14  13  12  11  
 * W    A   C   H   T   Z   E   H   N   R   S
 * 0    1   2  3    4   5   6   7   8   9   10
 * B    S   E   C   H   S   F   M   U   H   R
 **/

  vector<int> rot = {255, 0, 0};
  vector<int> gelb = {255, 255, 0};
  vector<int> blau = {0, 0, 255};
  vector<int> orange = {255, 102, 0};
  vector<int> dunkelgelb = {255, 153, 0};
  vector<int> helleresgelb = {255, 204, 0};
  vector<int> rosa = {255, 0, 255};
  vector<int> weiss = {255, 255, 255};
  vector<int> hellblau = {0, 255, 255};
  vector<int> sleap = {255, 0, 255};
  vector<vector<int>> Cola = {rot, gelb, blau, orange, dunkelgelb, helleresgelb, rosa, weiss, hellblau, sleap};
} // namespace O_ar

void sleep()
{
  long Faktor_hour = 3600000000;
  long Faktor_min =  60000000;

  Color_Sel_Save = Color_Sel;

  long sleeptime = 0;

  if (timeinfo.tm_hour >= 7)
  {
    sleeptime = ((24 - timeinfo.tm_hour) + 7) * Faktor_hour;
    sleeptime -= timeinfo.tm_min * Faktor_min;
  }
  else if (timeinfo.tm_hour < 7)
  {
    sleeptime = (7 - timeinfo.tm_hour) * Faktor_hour;
    sleeptime -= timeinfo.tm_min * Faktor_min;
  }

  strip.clear();
  strip2.clear();
  strip.show();
  strip2.show();
  Serial.println("Zeit zu schlafen!");

  esp_sleep_enable_timer_wakeup(sleeptime);

  esp_deep_sleep_start();
}
void wakeup()
{
  strip.clear();
    for (int i : O_ar::wakeup)
    {
      if(i<=94 && i>=92)
      {
      strip.setPixelColor(i, strip.Color(255, 0, 0));
      }else{
      strip.setPixelColor(i, strip.Color(0, 255, 0));
     }
    }
    strip.show();
  
}
void prepare_deepsleep()
{
  strip.clear();
  long switch_off = millis() + 3000;
  while (Color_Sel == 9 && switch_off > millis())
  {
    for (int i : O_ar::sleep)
    {
      strip.setPixelColor(i, strip.Color(255, 0, 255));
    }
    strip.show();
    delay(200);
  }

  if (Color_Sel == 9)
  {
    sleep();
  }
}

void Color_Select()
{
  if (millis() > alteZeit)
  {
    Color_Sel = (Color_Sel + 1) % 10;
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

vector<int> get_minute(int minute)
{
  if ((minute >= 5 && minute < 10) || (minute >= 25 && minute < 30) || (minute >= 35 && minute < 40) || (minute >= 55 && minute < 60)) //fünf
  {
    Serial.print("fünf ");
    return O_ar::min_5;
  }
  else if ((minute >= 10 && minute < 15) || (minute >= 20 && minute < 25) || (minute >= 40 && minute < 45) || (minute >= 50 && minute < 55)) //10 nach, 10 vor halb, 10 nach halb, 10 vor
  {
    Serial.print("zehn ");
    return O_ar::min_10;
  }
  else if ((minute >= 15 && minute < 20) || (minute >= 45 && minute < 50)) //viertel
  {
    Serial.print("viertel ");
    return O_ar::min_15;
  }

  return vector<int>(0, 0);
};

vector<int> get_vor_nach(int minute)
{
  /* vor is 0 nach is 1 in vector*/
  if ((minute >= 20 && minute < 30) || (minute >= 45 && minute < 60))
  {
    Serial.print("vor ");
    return O_ar::vor_nach.at(0);
  }
  else if ((minute >= 5 && minute < 20) || (minute >= 35 && minute < 45))
  {
    Serial.print("nach ");
    return O_ar::vor_nach.at(1);
  }
  else
  {
    return vector<int>(0, 0);
  }
};

vector<int> get_hour(int hour, int minute)
{
  int t_hour = hour;
  if (hour > 12)
  {
    t_hour = hour - 12;
  }

  //"Ein" Uhr
  if (t_hour == 1 && minute < 5)
  {
    return O_ar::hours_1_;
  }

  if (minute >= 20)
  {
    if(t_hour == 12){
      Serial.print(1);
      return O_ar::hours.at(0);
      
    } else {
      Serial.print(t_hour + 1);
      return O_ar::hours.at(t_hour); //11:20 10 vor halb 12  hour +1
    }
    
  } else {
    Serial.print(t_hour);
    return O_ar::hours.at(t_hour - 1);
  }
};

vector<int> get_halb(int minute)
{
  if (minute >= 20 && minute < 45)
  {
    Serial.print("halb ");
    return O_ar::halb;
  }
  return vector<int>(0);
}

vector<int> get_outro(int minute)
{
  if (minute >= 0 && minute < 5)
  {
    Serial.print("Uhr");
    return O_ar::o_Uhr;
  }
  return vector<int>(0, 0);
};

void GenerateOutputs(int hour, int minute, uint8_t g, uint8_t r, uint8_t b)
{
  strip.clear();
  strip2.clear();

  //Punkte aussen
  for (int i = 0; i < minute % 5; i++)
  {
    strip2.setPixelColor(i, strip.Color(r, g, b));
  }

  for (int i : O_ar::intr)
  {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  Serial.print("Es ist ");

  for (int i : get_minute(minute))
  {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }

  for (int i : get_vor_nach(minute))
  {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }

  for (int i : get_halb(minute))
  {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }

  for (int i : get_hour(hour, minute))
  {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }

  for (int i : get_outro(minute))
  {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }

  strip.show();
  strip2.show();
  Serial.println(".");
};

void setup()
{
  strip.begin();
  strip.clear();
  strip.setBrightness(brightness);
  strip.show(); // Initialize all pixels to 'off'

  strip2.begin();
  strip2.clear();
  strip2.setBrightness(brightness);
  strip2.show(); // Initialize all pixels to 'off'

  old_Brightness = brightness;

  Color_Sel = Color_Sel_Save;
  pinMode(Color_Select_SW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Color_Select_SW), Color_Select, RISING);

  pinMode(Encode_PIN_DT, INPUT);
  pinMode(Clock_PIN_CLK, INPUT);

  aLastState = digitalRead(Encode_PIN_DT);

  alteZeit = millis();

  //esp_sleep_enable_timer_wakeup(14400000000);
  //esp_sleep_enable_timer_wakeup(30000000);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_26, 0);

  if (Color_Sel == 9)
  {
    Color_Sel = 0;
  }
  wakeup();
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  int counterw = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    if (++counterw > 50)
    {
      //ESP.restart();
      esp_sleep_enable_timer_wakeup(1000);
      esp_deep_sleep_start();
    }

    Serial.print(".");
  }
  Serial.println("\n\nWLAN verbunden\n\n");

  configTime(0, 0, NTP_SERVER);
  // See https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv for Timezone codes for your region
  setenv("TZ", TZ_INFO, 1);

  if (getNTPtime(10))
  { // wait up to 10sec to sync
  }
  else
  {
    Serial.println("Time not set");
    //ESP.restart();
    esp_sleep_enable_timer_wakeup(1000);
    esp_deep_sleep_start();
  }
  //showTime(timeinfo);
  lastNTPtime = time(&now);
  lastEntryTime = millis();


}


void loop()
{
  if (Color_Sel == 9)
  {
    prepare_deepsleep();
  }
  getNTPtime(10);
  //showTime(timeinfo);

  if (timeinfo.tm_min != oldTimeinfo.tm_min || Color_Sel != old_Color) //Hat sich irgendwas verändert?
  {
    //Schlafen??
    if (timeinfo.tm_hour == 0 && timeinfo.tm_min == 0 && timeinfo.tm_sec == 0)
    {
      sleep();
    }

    GenerateOutputs(timeinfo.tm_hour, timeinfo.tm_min, O_ar::Cola.at(Color_Sel).at(0), O_ar::Cola.at(Color_Sel).at(1), O_ar::Cola.at(Color_Sel).at(2));
    oldTimeinfo = timeinfo;
    old_Color = Color_Sel;
  }

  rotary_time = millis();
  while (millis() < rotary_time + 1000 && Color_Sel == old_Color)
  {
    aState = digitalRead(Encode_PIN_DT); // Reads the "current" state of the outputA
    // If the previous and the current state of the outputA are different, that means a Pulse has occured
    if (aState != aLastState)
    {
      alteZeit = millis() + 500;
      // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
      if (digitalRead(Clock_PIN_CLK) != aState)
      {
        Serial.println("Rotary works");
        if (brightness < 245)
          brightness += 10;
      }
      else
      {
        if (brightness > 10)
          brightness -= 10;
      }
      Serial.print("Helligkeit: ");
      Serial.println(brightness);
    }
    aLastState = aState; // Updates the previous state of the outputA with the current state
    if (brightness != old_Brightness)
    {
      old_Brightness = brightness;

      strip.setBrightness(brightness);
      strip2.setBrightness(brightness);

      GenerateOutputs(timeinfo.tm_hour, timeinfo.tm_min, O_ar::Cola.at(Color_Sel).at(0), O_ar::Cola.at(Color_Sel).at(1), O_ar::Cola.at(Color_Sel).at(2));
    }
  }
}
