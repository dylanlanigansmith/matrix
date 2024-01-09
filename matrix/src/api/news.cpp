#include <api/news.hpp>
#include <ArduinoJson.hpp>
#include <algorithm>
void NewsApi::Parse(ArduinoJson::DynamicJsonDocument &json)
{
    /*
    {
      "articles": [
        {
        
          "author": "VOA Learning English",
          "title": "Big Space Missions Planned for 2024 - VOA Learning English",
          "description": "We take a look ahead at some of the biggest space missions to watch for in 2024.",
          "publishedAt": "2024-01-07T22:05:00Z",
        },
        {
         
          "author": "CBC.ca",
          "title": "Quebec, public sector unions reach agreement in principle on wages, working conditions - CBC.ca",
          "description": null,
          
          "publishedAt": "2024-01-07T21:31:14Z",
        },
        {
          "author": "Montreal Gazette",
          "title": "McGill grad imagines with music how falling into a black hole feels - Montreal Gazette",
          "description": null,
         
          "publishedAt": "2024-01-07T21:22:11Z",
          "content": null
        }
      ]
    }
    */
   m_headlines.clear();

  // m_headlines.erase(std::remove_if(m_headlines.begin(), m_headlines.end(), [](const headline_t& hl) { return hl.m_displayed == true; }), m_headlines.end());
    LOG("parsing headlines");
  for(int i = 0; i < NEWS_NUM_HEADLINES; ++i){
        m_headlines.push_back(headline_t(json["articles"][i]["author"].as<std::string>(),json["articles"][i]["title"].as<std::string>()
        ));
  }
}
ArduinoJson::StaticJsonDocument<128> NewsApi::MakeFilter()
{
     using namespace ArduinoJson; //:(
    StaticJsonDocument<128> filter;
    JsonObject filter_articles_0 = filter["articles"].createNestedObject();
    // filter_articles_0["source"]["name"] = true;
    filter_articles_0["author"] = true;
    filter_articles_0["title"] = true;
    // filter_articles_0["description"] = true;
    filter_articles_0["publishedAt"] = true;
    return filter;
}

ArduinoJson::V6214PB2::DeserializationError NewsApi::Deserialize(ArduinoJson::DynamicJsonDocument &json, const std::string &data)
{
    static ArduinoJson::StaticJsonDocument<128> filter = MakeFilter();
    return ArduinoJson::deserializeJson(json, m_msg.data, ArduinoJson::DeserializationOption::Filter(filter)); //data vs 
}

void NewsApi::Display(int type)
{
    if(!m_headlines.empty()){
        for(auto& hl : m_headlines){
             matrix.SetNextSpeed({70,1});
            matrix.printf("%s: %s", hl.m_src.c_str(), hl.m_title.c_str());
            hl.m_displayed = true;
        }        
    }
    else{
        matrix.StaticText("no new headlines");
    }
}

NewsApi Headlines;
