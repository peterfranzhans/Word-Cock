#include "Gen_op.h"

Gen_op::Gen_op(int LEDs, int Pin, neoPixelType Type, int minute, int hour, int g, int r, int b, int Brightness)
    : strip(LEDs, Pin, Type), hour(hour), minute(minute), g(g), r(r), b(b), Color_Select(0), Brightness(Brightness)
{
  strip.begin();
  strip.clear();
  strip.setBrightness(Brightness);
  strip.show(); // Initialize all pixels to 'off'
}

Gen_op::Gen_op(Adafruit_NeoPixel strip, int minute, int hour, int g, int r, int b, int Brightness)
    : strip(strip), hour(hour), minute(minute), g(g), r(r), b(b), Brightness(Brightness)
{
  strip.begin();
  strip.clear();
  strip.setBrightness(Brightness);
  strip.show(); // Initialize all pixels to 'off'
}

void Gen_op::GenerateOutputs()
{
  strip.clear();

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
}

vector<int> Gen_op::get_minute(int minute)
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
}

vector<int> Gen_op::get_vor_nach(int minute)
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
}

vector<int> Gen_op::get_hour(int hour, int minute)
{
  int t_hour = hour;
  if (hour > 12)
  {
    t_hour = hour - 12;
  }

  if (minute >= 20)
  {
    Serial.print(t_hour + 1);
    return O_ar::hours.at(t_hour); //11:20 10 vor halb 12  hour +1
  }
  else
    Serial.print(t_hour);
  return O_ar::hours.at(t_hour - 1);
}

vector<int> Gen_op::get_halb(int minute)
{
  if (minute >= 20 && minute < 45)
  {
    Serial.print("halb ");
    return O_ar::halb;
  }
  return vector<int>(0);
}

vector<int> Gen_op::get_outro(int minute) //Nacharbeit nötig. Übergange bei 12, 13 und 24 zu 1 checken sowie array position
{
  if (minute >= 0 && minute < 5)
  {
    Serial.print("Uhr");
    return O_ar::o_Uhr;
  }
  return vector<int>(0, 0);
}

void Gen_op::set_param(int hour, int minute, int Colorselect, int Brightness)
{
  if (Brightness != this->Brightness)
  {
    this->Brightness = Brightness;
    strip.setBrightness(Brightness);
  }
  this->hour = hour;
  this->minute = minute;
  this->r = O_ar::Cola.at(Color_Select).at(0);
  this->g = O_ar::Cola.at(Color_Select).at(1);
  this->b = O_ar::Cola.at(Color_Select).at(2);
  this->Color_Select = Color_Select;
  GenerateOutputs();
}
