#include <include.hpp>
#include <matrix.hpp>
#include <api/location.hpp>
constexpr long  gmtOffset_sec = -18000;
constexpr int   daylightOffset_sec = 3600;

constexpr const char* ntp_server = "pool.ntp.org";

#define TIMEBUF_SIZE 64
#define TIME_UPDATE_RATE 400
class CClock
{
public:
    CClock(uint32_t m_timeout = 5000) : m_lastUpdate(0u), m_timeout(m_timeout), m_use24hr(false) {}

    void Init(){
        configTime(LocationApi::Instance().GMTOffset(), daylightOffset_sec, ntp_server);

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
        if(m_lastUpdate + TIME_UPDATE_RATE > curTime) return;

        if(!_getTime(&m_timeinfo)){
           m_timeinfo.tm_hour = m_timeinfo.tm_min =  m_timeinfo.tm_sec = -1;
        }
        m_lastUpdate = curTime;

    }

    const char* GetTime(bool date = false){
        Update();
        UpdateTimeStr(date);
        return m_timebuf;
        //m_timestr.c_str();
    }

    tm& Time() { return m_timeinfo; } //should be const but idfk what c++ ver this is, some wack ass hybrid 
private:
    inline bool _getTime(tm* info) { //wrapper for custom timeout bc ill forget 
       return getLocalTime(info, m_timeout); 
    }
    void UpdateTimeStr(bool date = false){
        if(date){
            snprintf(m_datebuf, TIMEBUF_SIZE, "%i/%i/%i", m_timeinfo.tm_mon, m_timeinfo.tm_mday,  m_timeinfo.tm_year);
        }

        int hr =  m_timeinfo.tm_hour;
        if(m_use24hr)
            snprintf(m_timebuf, TIMEBUF_SIZE, "%02i:%02i", hr, m_timeinfo.tm_min);
        else{
            if(hr > 12)
                 snprintf(m_timebuf, TIMEBUF_SIZE, "%i:%02i:%02i PM", hr - 12, m_timeinfo.tm_min, m_timeinfo.tm_sec);
            
            else
               snprintf(m_timebuf, TIMEBUF_SIZE, "%i:%02i:%02i AM", hr, m_timeinfo.tm_min, m_timeinfo.tm_sec);
            
        }
        
        
    }

private:
    unsigned long m_lastUpdate;
    const uint32_t m_timeout;
    tm m_timeinfo;
    char m_timebuf[TIMEBUF_SIZE];
    char m_datebuf[TIMEBUF_SIZE];
    char m_dispbuf[TIMEBUF_SIZE];
    bool m_use24hr;
};

extern CClock Clock;