#include "api/api.hpp"
#include <network/networkmanager.hpp>
#include <HTTPClient.h>

void ApiRequest::Get() 
{
  //https://arduinojson.org/v5/assistant/
    request();
}

bool ApiRequest::request() 
{
    HTTPClient http;
    ArduinoJson::DynamicJsonDocument json(m_size);
    auto url = MakeURL();
    LOGF("try request %s", url.c_str());
    NetworkManager.SetLastRequest(url);
    if(!http.begin(url.c_str())){
        //fail
        LOGF("http:;begin failed %s", url);
        return false;
    }
    if(m_header.ShouldAdd()){
      http.addHeader(m_header.key, m_header.val);
    }
   // 
    auto status = http.GET();
    if(status <= 0){
        LOGF("http code %d", status);
       
        http.end();
        return false;
    }

    
    m_msg.data = http.getString().c_str(); //wtf
   // LOG(m_msg.data.c_str());
    auto error = Deserialize(json, m_msg.data);
    http.end();
    if(error){
        LOGF("parse error %d", error);
         
        return false;
    }

    Parse(json);

    return true;

}
void ApiRequest::Parse(ArduinoJson::DynamicJsonDocument& json)
{
  
}
void ApiRequest::Display(int style)
{
  matrix.ScrollText("ApiRequest::Display()");
}
std::string ApiRequest::MakeURL()
{
  return m_url;
}