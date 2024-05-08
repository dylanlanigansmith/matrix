#include "api.hpp"
#include <SpotifyArduino.h>
#include <ESPAsyncWebServer.h>
#include <network/networkmanager.hpp>

class CSpotifyIntegration
{

    //https://api.spotify.com/v1/me/player/currently-playing?market=CA

    public: 
        CSpotifyIntegration(const char* m_clientid, const char* m_clientsecret) 
            : m_clientid(m_clientid), m_clientsecret(m_clientsecret), m_callbackURI("http://matrix.local/callback"), m_scope("user-read-playback-state%20user-modify-playback-state"),
             m_init(false), m_spotify(NetworkManager.GetClient(), m_clientid, m_clientsecret ) {}
            
        void Init(AsyncWebServer& server);

        bool HasAuth();
        bool IsPlaying();
        CurrentlyPlaying m_current;
        CTime m_currentUpdate;
        int Update();

        void Display(int type = 0);
        void SetToken(const std::string& tok);
        std::string GetToken() const { return m_refreshToken; }
    protected:
        const char* m_clientid;
        const char* m_clientsecret;
        const char* m_callbackURI;
        const char* m_scope ;

        std::string m_refreshToken;
        bool m_hasToken;
        bool m_init;
        CTime m_lastAuth;
        SpotifyArduino m_spotify;
        
};

extern CSpotifyIntegration Spotify;