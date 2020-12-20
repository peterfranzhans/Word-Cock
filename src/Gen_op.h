#ifndef GEN_OP_H
#define GEN_OP_H
#include "EEPROM.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_NeoPixel.h>
#include <vector>
#include <inttypes.h>

using std::vector;

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
  vector<vector<int>> Cola = {rot, gelb, blau, orange, dunkelgelb, helleresgelb, rosa, weiss, hellblau};
} // namespace O_ar

class Gen_op
{
public:
  Gen_op(int LEDs = 110, int Pin = 4, neoPixelType Type = NEO_RGB + NEO_KHZ800, int minute = 0, int hour = 0, int g = 0, int r = 0, int b = 0, int Brightness = 125);
  Gen_op(Adafruit_NeoPixel strip, int minute = 0, int hour = 0, int g = 0, int r = 0, int b = 0, int Brightness = 125);

  void GenerateOutputs();

  vector<int> get_minute(int minute);

  vector<int> get_vor_nach(int minute);

  vector<int> get_hour(int hour, int minute);

  vector<int> get_halb(int minute);

  vector<int> get_outro(int minute);

  void set_param(int hour, int minute, int Colorselect, int Brightness);

private:
  Adafruit_NeoPixel strip;
  int hour;
  int minute;
  uint8_t g;
  uint8_t r;
  uint8_t b;
  int Color_Select;
  int Brightness;
};
#endif //GEN_OP_H