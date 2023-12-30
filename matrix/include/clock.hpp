#include <include.hpp>
#include <matrix.hpp>
constexpr long  gmtOffset_sec = -18000;
constexpr int   daylightOffset_sec = 3600;

constexpr const char* ntp_server = "pool.ntp.org";

#define TIMEBUF_SIZE 64

class CClock
{
public:
    CClock(uint32_t m_timeout = 5000) : m_lastUpdate(0u), m_timeout(m_timeout) {}

    void Init(){
        configTime(gmtOffset_sec, daylightOffset_sec, ntp_server);

        uint8_t attempts = 0;
        while(!_getTime(&m_timeinfo)){
            attempts++;
            delay(400);
            if(attempts > 3){
                matrix.staticf("ntp attempt %d", attempts);
            }
        }

    }
    void Update(){
        unsigned long curTime = millis();
        if(m_lastUpdate + 500 > curTime) return;

        if(!_getTime(&m_timeinfo)){
           m_timeinfo.tm_hour = m_timeinfo.tm_min =  m_timeinfo.tm_sec = -1;
        }
        m_lastUpdate = curTime;

    }

    const char* GetTime(){
        if(millis() - m_lastUpdate > 1500) Update();
        UpdateTimeStr();
        return m_timebuf;
        //m_timestr.c_str();
    }
private:
    inline bool _getTime(tm* info) { //wrapper for custom timeout bc ill forget 
       return getLocalTime(info, m_timeout); 
    }
    void UpdateTimeStr(){
        
        snprintf(m_timebuf, TIMEBUF_SIZE, "%02i:%02i:%02i", m_timeinfo.tm_hour, m_timeinfo.tm_min, m_timeinfo.tm_sec);

        
    }

private:
    unsigned long m_lastUpdate;
    const uint32_t m_timeout;
    tm m_timeinfo;
    char m_timebuf[TIMEBUF_SIZE];
};

extern CClock Clock;