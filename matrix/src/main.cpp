

#include <netconfig.hpp> 
/*
netconfig.hpp is simply:
#define WIFI_SSID "ssid"

#define WIFI_PWD "pwd"

*/

#include <Arduino.h>
#include <matrix.hpp>
#include <network/networkmanager.hpp>
#include <clock.hpp>

#include <api/api.hpp>
/*
for mDNS on arch:
sudo systemctl start avahi-daemon

*/
CMatrix matrix = CMatrix(matrix_pin_t()); 
CNetworkManager NetworkManager("matrix", WIFI_SSID, WIFI_PWD);

CClock Clock;
ApiRequest weather("http://api.open-meteo.com/v1/forecast?latitude=43.895628&longitude=-78.839352&current=temperature_2m,relative_humidity_2m,apparent_temperature,precipitation,surface_pressure,wind_speed_10m&timezone=America%2FNew_York&forecast_days=1");

void setup () {
  matrix.Init();
  matrix.StaticText("init matrix");
  if(uint8_t err =  NetworkManager.Connect(); err != CNetworkManager::NET_SUCCESS){
     for(;;){
        matrix.printf("Network Error: %s", NetworkManager.GetErrorMessage(err));
        delay(400);
     }
       
  }
  Clock.Init();
 // matrix.printf("%s @ %s / %s.local", NetworkManager.GetSSID(), NetworkManager.GetIP().c_str(), NetworkManager.GetHost());
  
 
 
}


unsigned long nextWeather = 0;
void loop () {
  NetworkManager.OnLoop();

  if(millis() > nextWeather){
    weather.Get();
    nextWeather = millis() + 1000 * 60 * 15;
  }
  matrix.staticf("%s  | %sc",  Clock.GetTime(), weather.msg().disp.c_str());
  delay(50);
  
}