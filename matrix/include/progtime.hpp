#pragma once
#include <include.hpp>
using time_ms_t = unsigned long;
using time_sec_t = uint32_t;

class CTime
{
public:
    CTime(float minutes) { set(minutes); }
    CTime(time_ms_t ms) : m_millis(ms) {}
    CTime(time_sec_t sec) { set(sec); }
    CTime(){ Update();}

    time_ms_t Update(){
        m_millis = millis();
        return m_millis;
    }

    inline void set(float minutes){
        m_millis = (float)(60000.f * minutes);
    }
    inline void set(time_ms_t ms){
        m_millis = ms;
    }

    inline void set(time_sec_t sec){
        m_millis = sec * 1000;
    }

    inline time_sec_t sec() const {
        return m_millis / 1000;
    }
    inline float min() const {
        return (float)sec() / 60.f;
    }
    inline time_ms_t ms() const {
        return m_millis;
    }

    bool operator==(const CTime& rhs) const {
        return m_millis == rhs.m_millis;
    }
    bool operator>(const CTime& rhs) const {
        return m_millis > rhs.m_millis;
    }
    bool operator<(const CTime& rhs) const {
        return m_millis < rhs.m_millis;
    }
    bool operator<=(const CTime& rhs) const {
        return m_millis <= rhs.m_millis;
    }
    bool operator>=(const CTime& rhs) const {
        return m_millis >= rhs.m_millis;
    }

    CTime operator+(const CTime& rhs) const {
       return CTime(m_millis + rhs.m_millis);
    }
    CTime operator-(const CTime& rhs) const {
        time_ms_t val = (m_millis >= rhs.m_millis) ? m_millis - rhs.m_millis : 0;
       return CTime(val);
    }

    void operator+=(const CTime& rhs) {
        m_millis += rhs.m_millis;
    }
    void operator-=(const CTime& rhs) {
        time_ms_t val = (m_millis >= rhs.m_millis) ? rhs.m_millis : m_millis;
        m_millis -= val;
    }
    static CTime now(){
        return CTime();
    }
private:
    time_ms_t m_millis;


};