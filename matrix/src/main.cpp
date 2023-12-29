

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


/*
for mDNS on arch:
sudo systemctl start avahi-daemon

*/
CMatrix matrix = CMatrix(matrix_pin_t()); 
CNetworkManager NetworkManager("matrix", WIFI_SSID, WIFI_PWD);

CClock Clock;

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
  matrix.printf("%s @ %s / %s.local", NetworkManager.GetSSID(), NetworkManager.GetIP().c_str(), NetworkManager.GetHost());

}



void loop () {
  NetworkManager.OnLoop();

  matrix.StaticText(Clock.GetTime());
  delay(100);
  
}