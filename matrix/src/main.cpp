

#include <api_config.hpp> 
/*
api_config.hpp is simply:

#define WIFI_SSID "ssid"

#define WIFI_PWD "pwd"

#define SPOTIFY_TOKEN "etc.."

*/

#include <Arduino.h>
#include <matrix.hpp>
#include <network/networkmanager.hpp>
#include <clock.hpp>
#include <feature/feature.hpp>
#include <api/api.hpp>

#include <api/spotify.hpp>
/*
for mDNS on arch:
sudo systemctl start avahi-daemon

*/
CMatrix matrix = CMatrix(matrix_pin_t()); 
CNetworkManager NetworkManager("matrix");

CClock Clock;
//ApiRequest weather("http://api.open-meteo.com/v1/forecast?latitude=43.895628&longitude=-78.839352&current=temperature_2m,relative_humidity_2m,apparent_temperature,precipitation,surface_pressure,wind_speed_10m&timezone=America%2FNew_York&forecast_days=1");

void setup () {

  #if DEBUG
    Serial.begin(115200);
    LOG("starting")
  #endif

  matrix.Init();
  matrix.StaticText("init matrix");
  uint8_t err =  NetworkManager.Connect();
  if(err != CNetworkManager::NET_SUCCESS){
    auto failTime = millis();
     matrix.printf("Network Error: %s", NetworkManager.GetErrorMessage(err));
      LOGF("Network Error: %s", NetworkManager.GetErrorMessage(err))
     NetworkManager.InitSetup();
     while((failTime + (1200 * 1000) ) > millis() ) {
       
        matrix.staticf("matrix.local %li", millis() / 1000);
        delay(250);
        //todo
     }
     NetworkManager.Error();
    return;
  }
  Clock.Init();
  LOGF("%s @ %s / %s.local", NetworkManager.GetSSID(), NetworkManager.GetIP().c_str(), NetworkManager.GetHost());
  
  Features.Init();
 LOG("init okay");
 
}



void loop () {
  NetworkManager.OnLoop();
  Features.OnLoop();
 
  // weather.msg().disp.c_str());
  delay(25);
  
}