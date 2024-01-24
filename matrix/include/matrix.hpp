#pragma once
#include <include.hpp>
#include <functional>

using TextUpdateFn = std::function<std::string()> ;

#define MATRIX_WIDTH 96
#define MATRIX_HEIGHT 8

#define NUM_PANELS 3
#define PANEL_WIDTH 32

struct matrix_pin_t
{
    uint8_t wr;
    uint8_t data;
    uint8_t cs1;
    uint8_t cs2;
    uint8_t cs3;

    matrix_pin_t(){
        wr = 26;
        data = 25;
        cs1 = 27;
        cs2 = 33;
        cs3 = 32;
    }
};


using scrollspeed = std::pair<uint32_t, uint8_t>;
class CMatrix
{
public:
    CMatrix(const matrix_pin_t& pins) ;
    ~CMatrix() {}

    bool Init();


    //should return time taken to scroll
    void ScrollText(const std::string& text);
    void ScrollText(const char* disp);
    void printf(const char* fmt, ...);
    void staticf(const char* fmt, ...);

    void displayf(const char* fmt, ...);
    void StaticText(const char* disp);
    inline bool HasInit() const { return m_is_init; }

    int GetTextWidth(const char* disp);
    int GetTextWidth(const char* disp, int *font_end, uint8_t font_height, uint8_t gutter_space = (uint8_t)1U);
    inline bool TextFits(const char* disp) {
        return GetTextWidth(disp) < MATRIX_WIDTH; //inconsistent
    }

    inline void SetNextSpeed(const scrollspeed& next) { m_nextSpeed = next; m_overrideSpeed = true;}
    inline void ResetSpeed() { m_nextSpeed = m_speed; m_overrideSpeed = false; }

    void SetBrightness(float amt);

    int ScrollSplit(const char* scrollmsg, const char* staticmsg, int type = 0);

    int ScrollSplit(const char* scrollmsg, TextUpdateFn staticfn); 
    int Display(const char* disp, bool large = true);

    bool StaticText(const char* disp_top, const char* disp_bot);

    int EstimateScrollTime(const char* disp, int* font_end = nullptr, uint8_t font_height = 8, uint8_t gutter_space = (uint8_t)1U);
private:
    
    inline scrollspeed GetScrollSpeed(){
        auto speed = (m_overrideSpeed) ? m_nextSpeed : m_speed;
        m_overrideSpeed = false;
        return speed;
    }
    

    int scroll(const char* disp, const byte *font = nullptr, int *font_end = nullptr, uint8_t font_height = 8, uint8_t gutter_space = (uint8_t)1U);

    bool static_text(const char* disp, bool clear, const byte *font, int *font_end, uint8_t font_height, int y_offset = 0, uint8_t gutter_space = (uint8_t)1U);
    bool static_text(const char* disp_top, const char* disp_bot, bool clear, const byte *font, int *font_end, uint8_t font_height, uint8_t gutter_space = (uint8_t)1U);
    void ClearAll();
    void RenderAll();
private:
    const matrix_pin_t m_pins;
    bool m_is_init;

    scrollspeed m_speed;
    scrollspeed m_nextSpeed;
    bool m_overrideSpeed;
};




extern CMatrix matrix;