#pragma once
#include "api.hpp"
#include <api_config.hpp>




struct location_t
{
    float lat;
    float lng;
    long timeoffset;
    
    std::string timeabbr; //smh
    std::string city;
    bool is_dst;
};

class LocationApi : public ApiRequest
{
public:
    LocationApi() : ApiRequest("http://ipwho.is"), m_hasInit(false) {}

    static LocationApi& Instance() {
        static LocationApi loc;
        if(!loc.m_hasInit){
            loc.Get();
            LOGF("location: %s %li lat: %f long: %f %s", loc.m_location.city.c_str(), loc.m_location.timeoffset, loc.m_location.lat, loc.m_location.lng, loc.m_location.timeabbr.c_str());
            loc.m_hasInit = true;
        }

        return loc;
    }
    virtual void Parse(ArduinoJson::DynamicJsonDocument& json) override;
    virtual void Display(int type = 0) override;
     virtual ArduinoJson::V6214PB2::DeserializationError Deserialize(ArduinoJson::DynamicJsonDocument& json, const std::string& data) override;

     float Latitude() const { return m_location.lat; }
     float Longitude() const { return m_location.lng; }
     long GMTOffset() const { return m_location.timeoffset; }
     const location_t& Location() { return m_location; } //again should be const but idfk/c

    inline bool isDST(){
        if (m_hasInit){
            return m_location.is_dst;
        }
    }
protected:
        ArduinoJson::StaticJsonDocument<256> MakeFilter();
private:
    bool m_hasInit;
    location_t m_location;
};
