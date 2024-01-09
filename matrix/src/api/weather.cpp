#include <api/weather.hpp>







void WeatherApi::Parse(ArduinoJson::DynamicJsonDocument &json)
{
      //https://open-meteo.com/en/docs
  //  matrix.ScrollText(json.getData()->asString().c_str());
    m_lastUpdate.Update();

    m_weather.apparent_temp = json["current"]["apparent_temperature"].as<float>();
    m_weather.temp = json["current"]["temperature_2m"].as<float>();
    m_weather.precipitation = json["current"]["precipitation"].as<int>();
    m_weather.humidity =  json["current"]["relative_humidity_2m"].as<int>();
    m_weather.wind_speed = json["current"]["wind_speed_10m"].as<float>();
    m_weather.surface_pressure = json["current"]["surface_pressure"].as<float>();

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
}

void WeatherApi::Display(int type)
{
  matrix.printf("%0.1f c / %0.1f c, %0.1f kmh, %dmm, %d/100, %0.1f hPa", m_weather.apparent_temp, m_weather.temp, m_weather.wind_speed, m_weather.precipitation, m_weather.humidity, m_weather.surface_pressure);
}
WeatherApi Weather;