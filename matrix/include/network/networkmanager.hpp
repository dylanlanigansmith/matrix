#pragma once
#include <include.hpp>
#include <ESPAsyncWebServer.h>
#include <WiFiClientSecure.h>


struct net_config{
    std::string ssid;
    std::string pwd;

};

class CNetworkManager
{
    public:
        CNetworkManager(const char* hostname) : 
                            hostname(hostname), m_server(80), m_isInOTA(false) {}

        uint8_t Connect(int32_t timeout = 10000);


        void OnLoop();

        void Reset();
        enum NetErrorCodes : uint8_t{
            NET_ERR_UNKN = 0xFF,
            NET_SUCCESS = 0,
            WIFI_FAIL,
            WIFI_TIMEOUT,
            MDNS_FAIL,
            WEBSERVE_FAIL
        };

        const char* GetErrorMessage(uint8_t err) const;

        inline const char* GetSSID() const { return WiFi.SSID().c_str(); }
        inline const char* GetHost() const { return hostname; }

        std::string GetIP() const;

        void SetOTAStatus(bool s) { m_isInOTA = s; }
        inline bool InOTA() const { return m_isInOTA; }
        WiFiClientSecure& GetClient() { return m_client; }
        std::string& GetLastRequest() { return m_lastRequest; }
        void SetLastRequest(const std::string& url) { m_lastRequest = url; }


        bool GetConfig(net_config& cfg);

        bool WriteConfig(net_config& cfg);
        void Error(const char* msg = "NetError");

        void InitSetup();
    protected:
        void RegisterHandlers();


    private:
        const char* hostname;
       //should have local netconfig object

        AsyncWebServer m_server;

        bool m_isInOTA;
        WiFiClientSecure m_client;
        std::string m_lastRequest;
    
};

extern CNetworkManager NetworkManager;