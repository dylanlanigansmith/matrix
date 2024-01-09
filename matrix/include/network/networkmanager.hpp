#pragma once
#include <include.hpp>
#include <ESPAsyncWebServer.h>
#include <WiFiClientSecure.h>


class CNetworkManager
{
    public:
        CNetworkManager(const char* hostname, const char* ssid, const char* pwd) : 
                            hostname(hostname), ssid(ssid), pwd(pwd), m_server(80), m_isInOTA(false) {}

        uint8_t Connect(int32_t timeout = 10000);


        void OnLoop();


        enum NetErrorCodes : uint8_t{
            NET_ERR_UNKN = 0xFF,
            NET_SUCCESS = 0,
            WIFI_FAIL,
            WIFI_TIMEOUT,
            MDNS_FAIL,
            WEBSERVE_FAIL
        };

        const char* GetErrorMessage(uint8_t err) const;

        inline const char* GetSSID() const { return ssid; }
        inline const char* GetHost() const { return hostname; }

        std::string GetIP() const;

        void SetOTAStatus(bool s) { m_isInOTA = s; }
        WiFiClientSecure& GetClient() { return m_client; }
    protected:
        void RegisterHandlers();


    private:
        const char* hostname;
        const char* ssid;
        const char* pwd;

        AsyncWebServer m_server;

        bool m_isInOTA;
        WiFiClientSecure m_client;
    
};

extern CNetworkManager NetworkManager;