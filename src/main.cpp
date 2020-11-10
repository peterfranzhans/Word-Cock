#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <time.h>
#include "EEPROM.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_NeoPixel.h>
#include <vector>
using std::vector;
#define PIN 4

const char* ssid = "Bleibmalocka-LAN";
const char* password = "21744762451772810918";


const char* NTP_SERVER = "de.pool.ntp.org";
const char* TZ_INFO    = "CET-1CEST-3,M3.5.0/02:00:00,M10.5.0/03:00:00";  // enter your time zone (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)

tm timeinfo;
time_t now;
long unsigned lastNTPtime;
unsigned long lastEntryTime;
Adafruit_NeoPixel strip(110, PIN, NEO_RGB + NEO_KHZ800);
int minute;
int hour;

namespace O_ar
{
  /**                 H      A      L       B **/
  vector<int> halb= {65,   64,  63,  62};

    /**            E    S       I        S       T **/
    vector<int> intr= {109,   108,    106,    105,    104};
    /**            F    Ü       N        F       **/
    vector<int> min_5={102, 101, 100, 99};
    /**             Z     E      H         N **/
    vector<int> min_10={88,   89,  90,  91};
    /**            V   I   E   R   T   E   L**/
    vector<int> min_15={83, 82, 81, 80, 79, 78, 77};
    
    vector<vector<int>> mins= {min_5,min_10,min_15};

    /**            N    A     C   H**/
    vector<int> nach={68,  69,  70,  71};
    /**         V   O   R**/
    vector<int> vor={72, 73, 74};
    vector<vector<int>> vor_nach= {vor,nach};
                  /**E   I   N   S**/
    vector<int> hours_1={46,  47,  48,  49}; 
                  /**Z    W   E   I**/
    vector<int> hours_2={44,   45,  46,  47};

                  /**D   R   E   I**/
    vector<int> hours_3={42, 41,   40, 39};
                  /**V  I     E   R**/
    vector<int> hours_4={29,  30,  31,  32};
                  /**F  Ü   N   F**/
    vector<int> hours_5={36,35,34, 33};
                  /**S  E  C   H  S **/
    vector<int> hours_6={1, 2,  3,  4, 5};
                  /**S  I E  B  E N**/
    vector<int> hours_7={49,50,51,52,53,54};
                      /**A  C H  T**/
    vector<int> hours_8={20,  19,  18,  17};
                      /**N  E U N**/
    vector<int> hours_9={25,  26,  27,  28};
                      /**Z  E H N**/
    vector<int> hours_10={16, 15 , 14,  13};
                      /** E L  F**/
    vector<int> hours_11={22,   23,  24};
                      /**Z  W Ö L F**/
    vector<int> hours_12={ 60,  59,  58,  57,  56};
                /**U  H R**/  
   vector<vector<int>> hours={hours_1,hours_2,hours_3,hours_4,hours_5,hours_6,hours_7,hours_8,hours_9,hours_10,hours_11,hours_12};
   vector<int> o_Uhr= {8,9,10};
    


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

}

bool getNTPtime(int sec) {

  {
    uint32_t start = millis();
    do {
      time(&now);
      localtime_r(&now, &timeinfo);
      Serial.print(".");
      delay(10);
    } while (((millis() - start) <= (1000 * sec)) && (timeinfo.tm_year < (2016 - 1900)));
    if (timeinfo.tm_year <= (2016 - 1900)) return false;  // the NTP call was not successful
    Serial.print("now ");  Serial.println(now);
    char time_output[30];
    strftime(time_output, 30, "%a  %d-%m-%y %T", localtime(&now));
    Serial.println(time_output);
    Serial.println();
  }
  return true;
}

void showTime(tm localTime) {
  Serial.print(localTime.tm_mday);
  Serial.print('/ spipiz');
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
  if (localTime.tm_wday == 0)   Serial.println(7);
  else Serial.println(localTime.tm_wday);
}

vector<int> get_minute()
    {
      if(minute%10 == minute%15 == minute%5)
      {
      return vector<int>(0);
      }
        if((minute%10) == minute%15 && minute %5 != minute%10) //5 nach, 5 vor halb, 5 nach halb, 5 vor
        {
            return O_ar::min_5;
        }else if((minute%10)== (minute%5) && (minute%15)>=5)  //10 nach, 10 vor halb, 10 nach halb, 10 vor
        {
            return O_ar::min_10;
        }else if((minute%15) < (minute%10))  //10 nach, 10 vor halb, 10 nach halb, 10 vor
        {
            return O_ar::min_15;
        }
     return O_ar::intr;  
    };
vector<int> get_vor_nach()
    {
        /* vor is 0 nach is 1 in vector*/
        if((minute>=20 && minute <=30) || (minute>=50 && minute <=60))
        {
            return O_ar::vor_nach.at(0);
        }
        else
        {
            return O_ar::vor_nach.at(1);
        }
        
    };
vector<int> get_hour()     //Nacharbeit nötig. Übergange bei 12, 13 und 24 zu 1 checken sowie array position
    {
      int t_hour= hour;
      if(hour>= 12)
      {
        t_hour = hour-12;
      }
      if(minute>=45)
        {
        return O_ar::hours.at(t_hour); //11:45 viertel vor 12  hour +1
        }
        else return O_ar::hours.at(t_hour-1);
    };
vector<int> get_halb()
{
  if (minute >= 20 && minute < 45)
  {
  return O_ar::halb;
  }
  return vector<int> (0);
}
 void GenerateOutputs(int hour, int minute,uint8_t r,uint8_t g,uint8_t b)
    {
      Serial.println("Generate Outputs \n");
        strip.clear();
        Serial.println("Generate Outputs vor minute \n");
        for(int i: get_minute())
        {
        strip.setPixelColor(i,strip.Color(255, 0, 0));
        }
        
        for(int i: get_halb())
        {
        strip.setPixelColor(i,strip.Color(r, g, b));
        }
        
        Serial.println("Generate Outputs vor hour\n");
        
        for(int i: get_hour())
        {
        strip.setPixelColor(i,strip.Color(r, g, b));
        }
        Serial.println("Generate Outputs vor nach\n");
        for(int i: get_vor_nach())
        {
        strip.setPixelColor(i,strip.Color(r, g, b));
        }
        Serial.println("Generate Outputs vor intno \n");
        for (int i: O_ar::intr)
        {
        strip.setPixelColor(i,strip.Color(r, g, b));
        }
        Serial.println("Generate Outputs vor o_arr \n");
        for (int i:O_ar::o_Uhr)
        {
        strip.setPixelColor(i,strip.Color(r, g, b));
        }
        
        strip.show();
        Serial.println("show erfolgreich \n");
    };
    /*
    void gen_output()
    {
        strip.clear();
        hour=10;
        minute = 10;
        for(int i: get_minute())
        {
        strip.setPixelColor(i,strip.Color(255, 0, 0));
        }
        for(int i: get_hour())
        {
        strip.setPixelColor(i,strip.Color(255, 0, 0));
        }
        for(int i: get_vor_nach())
        {
        strip.setPixelColor(i,strip.Color(255, 0, 0));
        }
        for (int i: O_ar::intr)
        {
        strip.setPixelColor(i,strip.Color(255, 0, 0));
        }
        for (int i:O_ar::o_Uhr)
        {
        strip.setPixelColor(i,strip.Color(255, 0, 0));
        }
        strip.show();
    };

*/

    


void setup() {
  Serial.begin(115200);
  Serial.println("\n\nNTP Time Test\n");
  WiFi.begin(ssid, password);

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    if (++counter > 100) ESP.restart();
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
  showTime(timeinfo);
  lastNTPtime = time(&now);
  lastEntryTime = millis();
        #if defined (__AVR_ATtiny85__)
        if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
        #endif
    strip.begin();
    strip.clear();
    strip.setBrightness(50);
    strip.show(); // Initialize all pixels to 'off'
    Serial.println("Setup ist fertig");
}

void loop() {
  
  //getNTPtime(10);
  //showTime(timeinfo);
  Serial.println("Stunden");
  while (Serial.available() == 0) { 
  }
  hour = Serial.read();
  Serial.println("Minuten");
  while (Serial.available() == 0) { 
  }
  minute = Serial.read();

  hour = timeinfo.tm_hour;
  minute = timeinfo.tm_min;
  Serial.println("hour = " );
  Serial.println(hour);
  Serial.println("\n");
    Serial.println("Minute = " );
  Serial.println(minute);
  GenerateOutputs(hour, minute,128,128,0);

  delay(1000);
}



