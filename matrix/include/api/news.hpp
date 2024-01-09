#include "api.hpp"
#include "api_config.hpp"
#include <vector>
#define NEWS_NUM_HEADLINES 8
#define NEWS_COUNTRY "ca"
#define NEWS_SOURCES "cbc-news,cnn,tech-crunch,bbc-news,reuters"
struct headline_t{
    std::string m_src;
    std::string m_title;
    int m_date[3] = {0,0,0};
    bool m_displayed;
    headline_t(const std::string& m_src, const std::string& m_title) : m_src(m_src), m_title(m_title), m_displayed(false){

    }
};

class NewsApi : public ApiRequest
{
public:
    NewsApi()   //https://open-meteo.com/en/docs
    : ApiRequest(ApiRequest::MakeURL(
        "http://newsapi.org/v2/top-headlines?sources=%s&pageSize=%i&apiKey=%s", NEWS_SOURCES, NEWS_NUM_HEADLINES, NEWSAPI_KEY 
    ), 2048) {}

    virtual void Parse(ArduinoJson::DynamicJsonDocument& json) override;
    virtual void Display(int type = 0) override;
    virtual ArduinoJson::V6214PB2::DeserializationError Deserialize(ArduinoJson::DynamicJsonDocument& json, const std::string& data) override;

    std::vector<headline_t>& headlines() { return m_headlines; }
protected:
    ArduinoJson::StaticJsonDocument<128> MakeFilter();
    
    std::vector<headline_t> m_headlines;
};



extern NewsApi Headlines;