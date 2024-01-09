#pragma once
#include <include.hpp>


#define MATRIX_WIDTH 96
#define MATRIX_HEIGHT 8

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



    void ScrollText(const std::string& text);
    void ScrollText(const char* disp);
    void printf(const char* fmt, ...);
    void staticf(const char* fmt, ...);

    void displayf(const char* fmt, ...);
    void StaticText(const char* disp);
    inline bool HasInit() const { return m_is_init; }

    int GetTextWidth(const char* disp);

    inline bool TextFits(const char* disp) {
        return GetTextWidth(disp) < MATRIX_WIDTH; //inconsistent
    }

    inline void SetNextSpeed(const scrollspeed& next) { m_nextSpeed = next; m_overrideSpeed = true;}
    inline void ResetSpeed() { m_nextSpeed = m_speed; m_overrideSpeed = false; }

    void SetBrightness(float amt);
private:
    


private:
    const matrix_pin_t m_pins;
    bool m_is_init;

    scrollspeed m_speed;
    scrollspeed m_nextSpeed;
    bool m_overrideSpeed;
};

extern CMatrix matrix;