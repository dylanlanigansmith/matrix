#include <include.hpp>
#include <functional>
#include <progtime.hpp>
using IntervalFn = std::function<void()> ;




class CInterval 
{
public:

    CInterval(const CTime& length, IntervalFn m_callback, bool dontInit = false) : m_duration(length), m_callback(m_callback) {
        if(dontInit){
             m_nextTime = CTime::now() + m_duration;
        }
    }
    
    
    bool Update(const CTime& curTime){
        bool ret = HasReached(curTime);
        if(ret){
            m_callback();
        }

        return ret;
    }

    bool HasReached(const CTime& curTime){
        if(curTime >= m_nextTime){
            m_nextTime = curTime + m_duration;
            return true;
        }

        return false;
    }


private:
    CTime m_duration;
    CTime m_nextTime;
    IntervalFn m_callback;

};