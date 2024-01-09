#include "api.hpp"
#include <api_config.hpp>
#include "location.hpp"

struct weather_t
{
    //Celsius
    float temp;
    float apparent_temp;
    //mm
    int precipitation;
    //kmh
    float wind_speed;
    //%
    int humidity;
    //hPa
    int surface_pressure;

};




class WeatherApi : public ApiRequest
{
public:
    WeatherApi()   //https://open-meteo.com/en/docs
    : ApiRequest("http://api.open-meteo.com/v1/forecast?latitude=43.895628&longitude=-78.839352&current=temperature_2m,relative_humidity_2m,apparent_temperature,precipitation,surface_pressure,wind_speed_10m&timezone=America%2FNew_York&forecast_days=1"
        ) {}

    virtual void Parse(ArduinoJson::DynamicJsonDocument& json) override;
    virtual void Display(int type = 0) override;
    [[nodiscard]] inline const weather_t& get() const { return m_weather; }

    virtual std::string MakeURL() override{
        static auto url = ApiRequest::MakeURL("http://api.open-meteo.com/v1/forecast?latitude=%f&longitude=%f&current=temperature_2m,relative_humidity_2m,apparent_temperature,precipitation,surface_pressure,wind_speed_10m&timezone=America%2FNew_York&forecast_days=1",
        LocationApi::Instance().Latitude(), LocationApi::Instance().Longitude()
         );
        return url;
    }
private:
    weather_t m_weather;
    
};

extern WeatherApi Weather;