#include "matrix.hpp"

#include <HT1632.h>
#include <font_8x4.h>
#include <font_8x5.h>
#include <images.h>
#include <font_5x4.h>

CMatrix::CMatrix(const matrix_pin_t &pins) : m_pins(pins), m_is_init(false)
{

}

bool CMatrix::Init()
{

    // void begin(uint8_t pinCS1, uint8_t pinWR,  uint8_t pinDATA);
    HT1632.begin(m_pins.cs1, m_pins.cs2, m_pins.cs3, m_pins.wr, m_pins.data);
    m_is_init = true;


    return m_is_init;
}
void CMatrix::ScrollText(const char* disp)
{
    int wd;
  wd = HT1632.getTextWidth(disp, FONT_8X4_END, FONT_8X4_HEIGHT);
  
  int i = 0 + wd; //+ wd = start from right
  while (i > -wd) {
    //this scrolls
    HT1632.renderTarget(0);
    HT1632.clear();
    HT1632.drawText(disp, i, 0, FONT_8X4, FONT_8X4_END, FONT_8X4_HEIGHT);
    HT1632.render();

    HT1632.renderTarget(1);
    HT1632.clear();
    HT1632.drawText(disp, i - 32, 0, FONT_8X4, FONT_8X4_END, FONT_8X4_HEIGHT);
    HT1632.render();

    HT1632.renderTarget(2);
    HT1632.clear();
    HT1632.drawText(disp, i - 64, 0, FONT_8X4, FONT_8X4_END, FONT_8X4_HEIGHT);
    HT1632.render();

    i = i - 1; //scroll speed at 1px / 100ms
    delay(100);

  }
     //OG: if i > -wd, i = 0
}
void CMatrix::ScrollText(const std::string &text)
{
  ScrollText(text.c_str());
}

void CMatrix::printf(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  char buf[512]; //too big?
  vsnprintf(buf, sizeof(char) * 512, fmt, args);

  ScrollText(buf);
  va_end(args);
}
void CMatrix::staticf(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  char buf[512]; //too big?
  vsnprintf(buf, sizeof(char) * 512, fmt, args);

  StaticText(buf);
  va_end(args);
}
void CMatrix::StaticText(const char *disp)
{
  int wd;
  wd = GetTextWidth(disp);

  int offset = (wd > MATRIX_WIDTH) ? (6) : (MATRIX_WIDTH - wd) / 2;
  int pos = 0 ;//wd / 2; //why the fuck

  HT1632.renderTarget(0);
  HT1632.clear();
  HT1632.drawText(disp, pos + offset , 0, FONT_8X4, FONT_8X4_END, FONT_8X4_HEIGHT);
  HT1632.render();

  HT1632.renderTarget(1);
  HT1632.clear();
  HT1632.drawText(disp, pos - 32 + offset, 0, FONT_8X4, FONT_8X4_END, FONT_8X4_HEIGHT);
  HT1632.render();

  HT1632.renderTarget(2);
  HT1632.clear();
  HT1632.drawText(disp, pos - 64 + offset, 0, FONT_8X4, FONT_8X4_END, FONT_8X4_HEIGHT);
  HT1632.render();
  delay(50); //legacy
  
}

int CMatrix::GetTextWidth(const char *disp)
{
    return HT1632.getTextWidth(disp, FONT_8X4_END, FONT_8X4_HEIGHT);
}
