
#include "network/networkmanager.hpp"
#include "matrix.hpp"
#include "WiFi.h"
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#include <ElegantOTA.h>

#include <api/spotify.hpp>

#include <feature/feature.hpp>

void onOTAStart(){
    NetworkManager.SetOTAStatus(true);
}
unsigned long ota_progress_millis = 0;
void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    matrix.staticf("OTA: %u/%u ", current, final);
  }
}
void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    matrix.staticf("update success!");
  } else {
     matrix.staticf("update failed");
       delay(1000);
  }


  NetworkManager.SetOTAStatus(false);
}

uint8_t CNetworkManager::Connect(int32_t timeout)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pwd);
    int32_t max_timeout = timeout;
    while (WiFi.status() != WL_CONNECTED)
    {
        matrix.staticf("wifi: %d/%d", max_timeout - timeout, max_timeout);
        constexpr uint32_t connect_delay = 500;
        delay(connect_delay);
        timeout -= connect_delay;
        if (timeout <= 0 || WiFi.status() == WL_CONNECT_FAILED){
            return (WiFi.status() == WL_CONNECT_FAILED) ? WIFI_FAIL : WIFI_TIMEOUT;
        }
    }
    matrix.staticf("%s", GetIP().c_str());
    if (!MDNS.begin(hostname)){
        return MDNS_FAIL;
    }

    RegisterHandlers();

   


    ElegantOTA.onStart(&onOTAStart);
    ElegantOTA.onProgress(&onOTAProgress);
    ElegantOTA.onEnd(&onOTAEnd);
    
    
    ElegantOTA.begin(&m_server);    // Start AsyncElegantOTA

    m_server.begin();
    

    return NET_SUCCESS;
}
void CNetworkManager::OnLoop()
{
    ElegantOTA.loop();
    while(m_isInOTA){
        delay(25);
         ElegantOTA.loop();   
    }
    if(WiFi.status() != WL_CONNECTED){
        int warntime = 0;
        while(warntime < 5){
            warntime++;
            matrix.printf("lost %s %d/5", ssid, warntime); //todo
            delay(1000);
        }
        Error();
    }
}
void CNetworkManager::Error(const char* msg ){
    matrix.displayf("Error: %s", msg); //dumb
    Reset();
}
void CNetworkManager::Reset()
{
    WiFi.disconnect();
    matrix.displayf("RESET");
    
    delay(1000);
    ESP.restart();

}

const char *CNetworkManager::GetErrorMessage(uint8_t err) const
{
    switch (err)
    {
    case NetErrorCodes::NET_SUCCESS:
        return "no error";
    case NetErrorCodes::WIFI_FAIL:
        return "wifi fail";
    case NetErrorCodes::WIFI_TIMEOUT:
        return "wifi timeout";
    case NetErrorCodes::WEBSERVE_FAIL:
        return "webserve fail";
    case NetErrorCodes::MDNS_FAIL:
        return "mdns failed";
    case NetErrorCodes::NET_ERR_UNKN:
    default:
        return "unknown net error";
    }
}


std::string CNetworkManager::GetIP() const {
    return std::string(WiFi.localIP().toString().c_str()); //wtf
}

const char* homepage = 
"<html> <head><title>MATRIX32</title></head><body><h1>ESP MATRIX32</h1><a href=\"/update\">update</a> <a href=\"/reset\">reset</a> <a href=\"/dark\">dark</a> ></body> </html>";

void CNetworkManager::RegisterHandlers()
{

    auto tryParam = [](AsyncWebServerRequest *request, const char* name, std::string& toSet){
        if(!request || !name || !request->hasArg(name)) return false;

        //WHY!!!
        //2: warning: init-statement in selection statements only available with -std=c++17 or -std=gnu++17
        // if(auto arg =  request->getParam(name); arg){
         //   ^~~~
        auto arg =  request->getParam(name);
        if( arg){
            toSet = arg->value().c_str();
            return true;
        }
        return false;

    };
     m_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", homepage);
    });
    m_server.onNotFound(  [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "page not found sorry");
    });
      m_server.on("/bright", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Set Brightness to full");
         matrix.SetBrightness(1.0);
    });
     m_server.on("/dim", HTTP_GET, [](AsyncWebServerRequest *request) {
        matrix.SetBrightness(0.5);
        request->send(200, "text/plain", "Set Brightness to 0.5l");
    });
     m_server.on("/dark", HTTP_GET, [](AsyncWebServerRequest *request) {
         matrix.SetBrightness(0.0);
        request->send(200, "text/plain", "Set Brightness to 0");
    });

      m_server.on("/test", HTTP_GET, [tryParam](AsyncWebServerRequest *request) {
         
        
        std::string arg = "";
         
         if(tryParam(request, "exit", arg)){
            Features.ExitState();
            request->send(200, "text/plain", "set state to last");
         }
         else{
            Features.SetState(Feature_Developer);
            request->send(200, "text/plain", "set state to dev");
         }
         
     
        
    });
    m_server.on("/lock", HTTP_GET, [tryParam](AsyncWebServerRequest *request) {
        
         
        std::string val = "";
            
        if(tryParam(request, "unlock", val)){
            Features.Unlock();
        }
        else if(tryParam(request, "lock", val)){
            Features.Lock();
        }
        else {
            if(Features.IsLocked()) Features.Unlock();
            else Features.Lock();
        }
         request->send(200, "text/plain", (std::string("set feature lock -> ").append(std::to_string(Features.IsLocked()))).c_str() );

        
        
    });
    m_server.on("/test2", HTTP_GET, [](AsyncWebServerRequest *request) {
         request->send(200, "text/plain", "testing scroll");
          Features.Lock();
        matrix.StaticText("Jan. 24 2023", "4:55pm -3c");
        delay(8000);
      //  matrix.ScrollSplit("Mitski - My Body's Made Of Crushed Little Stars", "4:59");
        Features.Unlock();
       
    });
       m_server.on("/reset", HTTP_GET, [this](AsyncWebServerRequest *request) {
        this->Reset();
        request->redirect("/");
        request->send(200, "text/plain", "Bye");
    });
    
     m_server.on("/debugapi", HTTP_GET, [this](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", this->GetLastRequest().c_str());
    });

    Spotify.Init(m_server);
}