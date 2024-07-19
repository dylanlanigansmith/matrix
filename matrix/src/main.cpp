

#include <api_config.hpp> 


#include <Arduino.h>
#include <matrix.hpp>
#include <network/networkmanager.hpp>
#include <clock.hpp>
#include <feature/feature.hpp>
#include <api/api.hpp>

#include <api/spotify.hpp>

//for mDNS on arch:
//sudo systemctl start avahi-daemon



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
 /*




#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <WiFi.h>

#include <ESPmDNS.h>
#endif

#include <WiFiClient.h>
#include <WiFiClientSecure.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <SpotifyArduino.h>
// Library for connecting to the Spotify API

// Install from Github
// https://github.com/witnessmenow/spotify-api-arduino

// including a "spotify_server_cert" variable
// header is included as part of the SpotifyArduino libary


#include <ArduinoJson.h>
// Library used for parsing Json from the API responses

// Search for "Arduino Json" in the Arduino Library manager
// https://github.com/bblanchon/ArduinoJson

//------- Replace the following! ------

char ssid[] = "BELL004";         // your network SSID (name)
char password[] = "324A153EF11F"; // your network password

char clientId[] = SPOTIFY_CLIENT;     // Your client ID of your spotify APP
char clientSecret[] = SPOTIFY_SECRET; // Your client Secret of your spotify APP (Do Not share this!)

char scope[] = "user-read-playback-state%20user-modify-playback-state";

//#define USE_IP_ADDRESS 0 //comment this out if you want to use MDNS

#ifdef USE_IP_ADDRESS
char callbackURItemplate[] = "%s%s%s";
char callbackURIProtocol[] = "http%3A%2F%2F"; // "http://"
char callbackURIAddress[] = "%2Fcallback%2F"; // "/callback/"
char callbackURI[100];
#else
char callbackURI[] = "http%3A%2F%2Fmatrix.local%2Fcallback%2F";
#endif

//------- ---------------------- ------

#if defined(ESP8266)
ESP8266WebServer server(80);
#elif defined(ESP32)
AsyncWebServer server(80);
#endif

WiFiClientSecure client;
SpotifyArduino spotify(client, clientId, clientSecret);


void setup()
{

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  IPAddress ipAddress = WiFi.localIP();
  Serial.println(ipAddress);

  if (MDNS.begin("matrix"))
  {
    Serial.println("MDNS responder started");
  }

  // Handle HTTPS Verification
#if defined(ESP8266)
  client.setFingerprint(SPOTIFY_FINGERPRINT); // These expire every few months
#elif defined(ESP32)
#include <SpotifyArduinoCert.h>
  client.setCACert(spotify_server_cert);
#endif
  // ... or don't!
  //client.setInsecure();

  // If you want to enable some extra debugging
  // uncomment the "#define SPOTIFY_DEBUG" in SpotifyArduino.h

#ifdef USE_IP_ADDRESS
  // Building up callback URL using IP address.
  sprintf(callbackURI, callbackURItemplate, callbackURIProtocol, ipAddress.toString().c_str(), callbackURIAddress);
#endif


        server.on("/spotify", HTTP_GET, [&](AsyncWebServerRequest *request) {
          const char *webpageTemplate =
    R"(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
  </head>
  <body>
    <div>
     <a href="https://accounts.spotify.com/authorize?client_id=%s&response_type=code&redirect_uri=%s&scope=%s">spotify Auth</a>
    </div>
  </body>
</html>
)";
           char webpage[800];
  sprintf(webpage, webpageTemplate, clientId, "http://matrix.local/callback", scope);
     request->send(200, "text/html", webpage);
        });
        server.on("/callback", HTTP_GET, [](AsyncWebServerRequest *request) {
            String code = "";
            const char *refreshToken = NULL;
            for (uint8_t i = 0; i < request->args(); i++)
            {
                if (request->argName(i) == "code")
                {
                code = request->arg(i);
                refreshToken = code.c_str();
                }
            }

           
            if (refreshToken != NULL)
            {
                
                request->send(200, "text/plain", refreshToken);

            }
            else
            {
                request->send(404, "text/plain", "Failed to load token, check serial monitor");
            }
        });

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
#if defined(ESP8266)
  MDNS.update();
#endif

  
}
*/