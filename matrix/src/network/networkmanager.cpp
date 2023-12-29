
#include "network/networkmanager.hpp"
#include "matrix.hpp"
#include "WiFi.h"
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#include <ElegantOTA.h>


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
  }

  delay(700);
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

    if (!MDNS.begin(hostname)){
        return MDNS_FAIL;
    }

    m_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "ESP32 Matrix");
    });


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
        for(;;){
            matrix.printf("lost connection to %s", ssid); //todo
            delay(1000);
        }
    }
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