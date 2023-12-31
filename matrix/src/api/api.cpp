#include "api/api.hpp"

#include <HTTPClient.h>
void ApiRequest::Get() 
{
    request();
}

void ApiRequest::request() 
{
    HTTPClient http;
    ArduinoJson::DynamicJsonDocument json(m_size);
    MakeURL();
    if(!http.begin(m_url)){
        //fail

    }
    
    auto status = http.GET();
    if(status <= 0){
        
        return http.end();
    }
    matrix.StaticText("req ok");
    m_msg.data = http.getString().c_str(); //wtf
    auto error = ArduinoJson::deserializeJson(json, m_msg.data);
    http.end();
    if(error){

        return;
    }

    Parse(json);

}
void ApiRequest::Parse(ArduinoJson::DynamicJsonDocument& json)
{
    //https://open-meteo.com/en/docs
  //  matrix.ScrollText(json.getData()->asString().c_str());

    std::string temperature = json["current"]["apparent_temperature"].as<std::string>();

    m_msg.disp = json["current"]["temperature_2m"].as<std::string>();
    /*
        {
  "latitude": 43.898594,
  "longitude": -78.85666,
  "generationtime_ms": 0.05698204040527344,
  "utc_offset_seconds": -18000,
  "timezone": "America/New_York",
  "timezone_abbreviation": "EST",
  "elevation": 96,
  "current_units": {
    "time": "iso8601",
    "interval": "seconds",
    "temperature_2m": "°C",
    "relative_humidity_2m": "%",
    "apparent_temperature": "°C",
    "precipitation": "mm",
    "surface_pressure": "hPa",
    "wind_speed_10m": "km/h"
  },
  "current": {
    "time": "2023-12-31T02:30",
    "interval": 900,
    "temperature_2m": -0.4,
    "relative_humidity_2m": 83,
    "apparent_temperature": -4.1,
    "precipitation": 0,
    "surface_pressure": 1001.7,
    "wind_speed_10m": 7.7
  }
}
    */
    matrix.printf("%s/%s C  [%s ] %skmh", temperature.c_str(),json["current"]["temperature_2m"].as<std::string>().c_str(), json["current"]["precipitation"].as<std::string>().c_str(),   json["current"]["wind_speed_10m"].as<std::string>().c_str());
   

}
void ApiRequest::MakeURL()
{

}