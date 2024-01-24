#pragma once
#include <include.hpp>

struct matrix_img
{
    struct {
        uint8_t w,h;
    } size;
    const byte* img;

    matrix_img(const byte* img, uint8_t w, uint8_t h) : img(img) {
        size.w = w; size.h = h;
    }
};

#define MATRIX_IMAGE(name, w, h, ...) const byte __IMG_##name [] = __VA_ARGS__ ; const matrix_img img_##name (__IMG_##name, w, h );





MATRIX_IMAGE(weather_snowflake, 9,8, {0b00000000, 0b00001000, 0b00101010, 0b00010100, 0b01101011, 0b00010100, 0b00101010, 0b00001000, 0b00000000});


