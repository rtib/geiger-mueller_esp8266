#include <U8g2lib.h>

class Display {
  public:
    Display(const char* version) {
      u8g2 = U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C(U8G2_R0, /* clock=*/ 5, /* data=*/ 4, /* reset=*/ 16);
      u8g2.begin();
      versionString = version;
    }

    void splash() {
      u8g2.clearBuffer();
      drawSplash();
      u8g2.sendBuffer();
    }

    void initScreen(int current, int from) {
      u8g2.clearBuffer();
      drawSplash();
      u8g2.drawFrame(8,25,114,5);
      u8g2.drawHLine(10, 27, 110 * (from - current + 1) / from);
      u8g2.sendBuffer();
    }

    void runDisplay(double exposure, double dose, unsigned long cpm) {
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_ncenR18_tf);
      u8g2.setCursor(0, 20);
      u8g2.print(exposure);
      u8g2.print(F(" \xB5Sv/h"));

      u8g2.setFont(u8g2_font_5x8_tf);
      u8g2.setCursor(0, 31);
      u8g2.print(dose); u8g2.print(F(" \xB5Sv"));

      u8g2.setCursor(75, 31);
      u8g2.print(F("CPM: ")); u8g2.print(cpm);
      u8g2.sendBuffer();  
    }
  private:
    U8G2 u8g2;
    const char* versionString;

    void drawSplash() {
      u8g2.setFont(u8g2_font_smart_patrol_nbp_tf);
      u8g2.drawStr(10, 10, "Geiger-M\xFCller");
      u8g2.setFont(u8g2_font_profont11_tr);
      u8g2.drawStr(20, 21, "Version");
      u8g2.drawStr(70, 21, versionString);
    }
};
