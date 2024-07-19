#include <api/location.hpp>

void LocationApi::Parse(ArduinoJson::DynamicJsonDocument& json) //todo get timezone
{
    m_location.city = json["city"].as<std::string>();
    m_location.lat = json["latitude"].as<float>();
    m_location.lng = json["longitude"].as<float>();
    m_location.timeoffset = json["timezone"]["offset"].as<long>();
    m_location.timeabbr = json["timezone"]["abbr"].as<std::string>();
    m_location.is_dst = json["timezone"]["is_dst"].as<bool>();

    //no validation >:(
    m_hasInit = true;
}

void LocationApi::Display(int type)
{
    matrix.displayf("%s (%s)", m_location.city.c_str(), m_location.timeabbr.c_str());
   // matrix.printf("%s %.3f/%.3f %s", m_location.city.c_str(), m_location.lat, m_location.lng, m_location.timeabbr.c_str());
}

ArduinoJson::V6215PB2::DeserializationError LocationApi::Deserialize(ArduinoJson::DynamicJsonDocument& json, const std::string& data)
{
      static ArduinoJson::StaticJsonDocument<128> filter = MakeFilter();
      LOG(data.c_str());
      LOG("parsing location");
    return ArduinoJson::deserializeJson(json, data); // ArduinoJson::DeserializationOption::Filter(filter)
}
/*
{
    "About Us": "https:\/\/ipwhois.io",
    "ip": "142.198.157.66",
    "success": true,
    "type": "IPv4",
    "continent": "North America",
    "continent_code": "NA",
    "country": "Canada",
    "country_code": "CA",
    "region": "Ontario",
    "region_code": "ON",
    "city": "Oshawa",
    "latitude": 43.8970929,
    "longitude": -78.8657912,
    "is_eu": false,
    "postal": "L1H 1A0",
    "calling_code": "1",
    "capital": "Ottawa",
    "borders": "US",
    "flag": {
        "img": "https:\/\/cdn.ipwhois.io\/flags\/ca.svg",
        "emoji": "\ud83c\udde8\ud83c\udde6",
        "emoji_unicode": "U+1F1E8 U+1F1E6"
    },
    "connection": {
        "asn": 577,
        "org": "Bell Canada",
        "isp": "Bell Canada",
        "domain": "bell.ca"
    },
    "timezone": {
        "id": "America\/Toronto",
        "abbr": "EST",
        "is_dst": false,
        "offset": -18000,
        "utc": "-05:00",
        "current_time": "2024-01-08T18:42:40-05:00"
    }
}
*/

ArduinoJson::StaticJsonDocument<256> LocationApi::MakeFilter()
{
     using namespace ArduinoJson;
    StaticJsonDocument<256> filter;
   
    auto tzf = filter["timezone"].createNestedObject();
     tzf["offset"] = true;
     tzf["abbr"] = true;
    tzf["is_dst"] = true;


     filter["latitude"] = true;
     filter["longitude"] = true;

    

     filter["city"] = true;
     
     
    return filter;
}
