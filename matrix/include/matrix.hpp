#pragma once
#include <include.hpp>
#include <cstdarg>

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
    void StaticText(const char* disp);
    inline bool HasInit() const { return m_is_init; }

    int GetTextWidth(const char* disp);
private:
    


private:
    const matrix_pin_t m_pins;
    bool m_is_init;
};

extern CMatrix matrix;