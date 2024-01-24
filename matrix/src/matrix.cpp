#include "matrix.hpp"

#include <HT1632.h>
#include <font_8x4.h>
#include <font_8x5.h>
#include <images.h>
#include <font_5x4.h>



CMatrix::CMatrix(const matrix_pin_t &pins) : m_pins(pins), m_is_init(false)
{
  m_speed = {100u, 1u};
  m_nextSpeed = m_speed;
  m_overrideSpeed = false;
}

bool CMatrix::Init()
{

  // void begin(uint8_t pinCS1, uint8_t pinWR,  uint8_t pinDATA);
  HT1632.begin(m_pins.cs1, m_pins.cs2, m_pins.cs3, m_pins.wr, m_pins.data);
  m_is_init = true;

  return m_is_init;
}
void CMatrix::ScrollText(const char *disp)
{
  int wd;
  wd = HT1632.getTextWidth(disp, FONT_8X4_END, FONT_8X4_HEIGHT);

  int i = 0 + MATRIX_WIDTH; //+ wd = start from right

  auto speed = GetScrollSpeed();

  while (i > -wd)
  {
    // this scrolls
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

    i = i - speed.second; // scroll speed at 1px / 100ms
    delay(speed.first);
  }

  // OG: if i > -wd, i = 0
}

int CMatrix::ScrollSplit(const char *scrollmsg, const char *staticmsg, int type)
{
  int static_wd = GetTextWidth(staticmsg), wd = GetTextWidth(scrollmsg);

  if (static_wd > (PANEL_WIDTH)) 
    return -1;

 // bool static_split = static_wd > PANEL_WIDTH; //always true lets just not use panel 2

  auto speed = GetScrollSpeed();

  int scroll_end = wd;
  int scroll_start = MATRIX_WIDTH - PANEL_WIDTH;
  constexpr int padding = 8;
  int i = scroll_start + padding;
  int time = 0;

  int stat_offset = (PANEL_WIDTH - static_wd) / 2;
  if(stat_offset < 0 || stat_offset > PANEL_WIDTH / 2) stat_offset = 0;
  while (i > -wd)
  {
    for (int t = 0; t < (NUM_PANELS ); t++)
    {
      if(t == 2) continue; //dont fucking ask
      HT1632.renderTarget(t);
      HT1632.clear();
      int x = i - (t * PANEL_WIDTH);
      HT1632.drawText(scrollmsg, x, 0, FONT_8X4, FONT_8X4_END, FONT_8X4_HEIGHT);
     HT1632.render();
    }

   

    // do static overtop (wasteful but this library is designed for a 15 year old microcontroller with no features)
    HT1632.renderTarget(2);
    HT1632.clear();
    HT1632.drawText(staticmsg, stat_offset, 0, FONT_8X4, FONT_8X4_END, FONT_8X4_HEIGHT);
    HT1632.render();
    i = i - speed.second; // scroll speed at 1px / 100ms
    time += speed.first;
    delay(speed.first);
  }

  return time;
}

int CMatrix::ScrollSplit(const char* scrollmsg, TextUpdateFn staticfn)
{

  auto static_str = std::string();
  int static_wd = 0, stat_offset = 0, wd = GetTextWidth(scrollmsg);

  auto GetStaticText = [this, staticfn](int& wd, int& offset){
    auto str = staticfn();
    if(str.empty()) str = "0";
    wd = GetTextWidth(str.c_str()) ;
    wd = (wd > PANEL_WIDTH) ? PANEL_WIDTH : wd;
    offset = (PANEL_WIDTH - wd) / 2;
    if(offset < 0 || offset > PANEL_WIDTH / 2) offset = 0;
    return str;
  };
  

  

  
  static_str = GetStaticText(static_wd, stat_offset);
  if (static_wd > (PANEL_WIDTH)) 
    return -1;

 // bool static_split = static_wd > PANEL_WIDTH; //always true lets just not use panel 2

  auto speed = GetScrollSpeed();

  int scroll_end = wd;
  int scroll_start = MATRIX_WIDTH - PANEL_WIDTH;
  constexpr int padding = 8;
  int i = scroll_start + padding;
  int time = 0;

  

  
  while (i > -wd)
  {
    for (int t = 0; t < (NUM_PANELS ); t++)
    {
      if(t == 2) continue; //dont fucking ask
      HT1632.renderTarget(t);
      HT1632.clear();
      int x = i - (t * PANEL_WIDTH);
      HT1632.drawText(scrollmsg, x, 0, FONT_8X4, FONT_8X4_END, FONT_8X4_HEIGHT);
     HT1632.render();
    }

   
    static_str = GetStaticText(static_wd, stat_offset);
    // do static overtop (wasteful but this library is designed for a 15 year old microcontroller with no features)
    HT1632.renderTarget(2);
    HT1632.clear();
    HT1632.drawText(static_str.c_str(), stat_offset, 0, FONT_8X4, FONT_8X4_END, FONT_8X4_HEIGHT);
    HT1632.render();
    i = i - speed.second; // scroll speed at 1px / 100ms
    time += speed.first;
    delay(speed.first);
  }

  return time;
}

int CMatrix::Display(const char* disp, bool large)
{
  if(large)
    return scroll(disp, FONT_8X4, FONT_8X4_END, FONT_8X4_HEIGHT);

  return scroll(disp, FONT_5X4, FONT_5X4_END, FONT_5X4_HEIGHT);
}

bool CMatrix::StaticText(const char* disp_top, const char* disp_bot) //NOT GONNA FUCKIN WORK IDIOT FONT IS FUCKIN BACKWARDS WTF
{

  bool ret = static_text(disp_top, disp_bot, true, FONT_5X4, FONT_5X4_END, FONT_5X4_HEIGHT) ;

  delay(GetScrollSpeed().first); //idk how this can be consistent

  //fuck this whole thing is messy
  
  return ret; //both texts seemed to likely fit on screen 
}

bool CMatrix::static_text(const char* disp, bool clear, const byte *font, int *font_end, uint8_t font_height, int y_offset, uint8_t gutter_space)
{
   int wd;
  wd = GetTextWidth(disp, font_end, font_height, gutter_space);
  auto speed = (m_overrideSpeed) ? m_nextSpeed : m_speed;
  int offset = (wd > MATRIX_WIDTH) ? (6) : (MATRIX_WIDTH - wd) / 2;
  int pos = 0; // wd / 2; //why the fuck

  for (int t = 0; t < (NUM_PANELS); ++t)
  {
    HT1632.renderTarget(t);
    if(clear) HT1632.clear();
    
    HT1632.drawText(disp, pos - (t * PANEL_WIDTH) + offset, y_offset, font,  font_end, font_height);
    if(clear) HT1632.render();
  }

  return (wd <= MATRIX_WIDTH);
}
bool CMatrix::static_text(const char* disp_top, const char* disp_bot, bool clear, const byte *font, int *font_end, uint8_t font_height, uint8_t gutter_space)
{
   int wd_top = GetTextWidth(disp_top, font_end, font_height, gutter_space), wd_bot = GetTextWidth(disp_bot, font_end, font_height, gutter_space);

  auto speed = (m_overrideSpeed) ? m_nextSpeed : m_speed;
  int offset_top = (wd_top > MATRIX_WIDTH) ? (6) : (MATRIX_WIDTH - wd_top) / 2;
  int offset_bot = (wd_bot > MATRIX_WIDTH) ? (6) : (MATRIX_WIDTH - wd_bot) / 2;

  if(font_height >= MATRIX_HEIGHT) return false; //bad this function wasnt supposed to do two at once 
  int y_top = 0, y_bot = (MATRIX_HEIGHT / 2) + 1;

  int pos = 0; // wd / 2; //why the fuck

  for (int t = 0; t < (NUM_PANELS); ++t)
  {
    HT1632.renderTarget(t);
    if(clear) HT1632.clear();
    
    HT1632.drawText(disp_top, pos - (t * PANEL_WIDTH) + offset_top, y_top, font,  font_end, font_height);
    HT1632.drawText(disp_bot, pos - (t * PANEL_WIDTH) + offset_bot, y_bot, font,  font_end, font_height);
    if(clear) HT1632.render();
  }

  return (wd_top <= MATRIX_WIDTH) && (wd_bot <= MATRIX_WIDTH);
}
int CMatrix::scroll(const char *disp, const byte *font, int *font_end, uint8_t font_height, uint8_t gutter_space)
{
  const byte* scroll_font = (font != nullptr) ? font : FONT_8X4;
  if (font_end == nullptr) font_end  = FONT_8X4_END;
  int wd;
  wd = HT1632.getTextWidth(disp, font_end, font_height);

  int i = 0 + MATRIX_WIDTH; //+ wd = start from right

  auto speed = GetScrollSpeed();
  int total_delay = 0;
  while (i > -wd)
  {
    // this scrolls
    for (int t = 0; t < (NUM_PANELS); ++t)
    {
      HT1632.renderTarget(t);
      HT1632.clear();
      HT1632.drawText(disp, i - (t * PANEL_WIDTH), 0, scroll_font,  font_end, font_height);
      HT1632.render();
    }
    i = i - speed.second; // scroll speed at 1px / 100ms
    total_delay += speed.first;
    delay(speed.first);
  }

  return total_delay;
}

void CMatrix::ClearAll()
{
  for (int t = 0; t < (NUM_PANELS); ++t)
  {
    HT1632.renderTarget(t);
    HT1632.clear();
  }

}

void CMatrix::RenderAll()
{
  for (int t = 0; t < (NUM_PANELS); ++t)
  {
    HT1632.renderTarget(t);
    HT1632.render();
  }
}



void CMatrix::ScrollText(const std::string &text)
{
  ScrollText(text.c_str());
}

void CMatrix::printf(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  char buf[512]; // too big?
  vsnprintf(buf, sizeof(char) * 512, fmt, args);

  ScrollText(buf);
  va_end(args);
}
void CMatrix::displayf(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  char buf[512]; // too big?
  vsnprintf(buf, sizeof(char) * 512, fmt, args);

  int width = GetTextWidth(buf);
  if (width > MATRIX_WIDTH - 1)
    ScrollText(buf);
  else
  {
    
    StaticText(buf);
   // delay(EstimateScrollTime(buf) / 2); //bug here
    m_overrideSpeed = false;
  }

  va_end(args);
}
void CMatrix::staticf(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  char buf[512]; // too big?
  vsnprintf(buf, sizeof(char) * 512, fmt, args);

  StaticText(buf);
  va_end(args);
}
void CMatrix::StaticText(const char *disp)
{
  int wd;
  wd = GetTextWidth(disp);
  auto speed = (m_overrideSpeed) ? m_nextSpeed : m_speed; //fix
  int offset = (wd > MATRIX_WIDTH) ? (6) : (MATRIX_WIDTH - wd) / 2;
  int pos = 0; // wd / 2; //why the fuck

  HT1632.renderTarget(0);
  HT1632.clear();
  HT1632.drawText(disp, pos + offset, 0, FONT_8X4, FONT_8X4_END, FONT_8X4_HEIGHT);
  HT1632.render();

  HT1632.renderTarget(1);
  HT1632.clear();
  HT1632.drawText(disp, pos - 32 + offset, 0, FONT_8X4, FONT_8X4_END, FONT_8X4_HEIGHT);
  HT1632.render();

  HT1632.renderTarget(2);
  HT1632.clear();
  HT1632.drawText(disp, pos - 64 + offset, 0, FONT_8X4, FONT_8X4_END, FONT_8X4_HEIGHT);
  HT1632.render();
  delay(m_speed.first); // legacy use EstimateScrollTime
  m_overrideSpeed = false;
}

int CMatrix::EstimateScrollTime(const char* disp, int* font_end, uint8_t font_height, uint8_t gutter_space)
{
    if(!disp) return 0;
    if (font_end == nullptr){
      font_end = FONT_8X4_END; 
      font_height = FONT_8X4_HEIGHT;
    } 
  
  int wd = GetTextWidth(disp, font_end, font_height, gutter_space);
  if(wd < 1) return 0;
  auto speed = (m_overrideSpeed) ? m_nextSpeed : m_speed; //dont clear bc we arent using scrollspeed
  int i = MATRIX_WIDTH;
  if(speed.second == 1){
    return (i - (-wd)) * speed.first; //should really add a min/max sorta thing 
  }



   //bad bad bad but never gonna happen
   
    int time = 0;
    while (i > -wd)
    {
      i = i - speed.second; // scroll speed at 1px / 100ms

      time += speed.first;
    }

    return time;
}

int CMatrix::GetTextWidth(const char *disp)
{
  return HT1632.getTextWidth(disp, FONT_8X4_END, FONT_8X4_HEIGHT);
}

int CMatrix::GetTextWidth(const char* disp, int *font_end, uint8_t font_height, uint8_t gutter_space)
{
  return HT1632.getTextWidth(disp, font_end, font_height, gutter_space);
}

void CMatrix::SetBrightness(float amt)
{
  int brightness = static_cast<int>(float(amt * 16.f));
  uint8_t val = std::max(std::min(brightness, 16), 0);
  for (int i = 0; i < NUM_PANELS; ++i)
  {
    HT1632.renderTarget(i);
    HT1632.setBrightness(val);
  }
}
