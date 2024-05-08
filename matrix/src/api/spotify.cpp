#include <api/spotify.hpp>
#include <HTTPClient.h>
#include <SpotifyArduino.h>
#include <SpotifyArduinoCert.h>
#include <api_config.hpp>





CSpotifyIntegration Spotify(SPOTIFY_CLIENT, SPOTIFY_SECRET);


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
void CSpotifyIntegration::Init(AsyncWebServer & server){
    m_current.isPlaying = false;
    auto& client = NetworkManager.GetClient();

    client.setCACert(spotify_server_cert);
    m_hasToken = !m_refreshToken.empty();
    
    #ifndef SPOTIFY_GETTOKEN
   //  m_hasToken = true;
    #endif
   
            server.on("/spotify", HTTP_GET, [this](AsyncWebServerRequest *request) {
            char webpage[800];
            sprintf(webpage, webpageTemplate, m_clientid, m_callbackURI, m_scope);
            request->send(200, "text/html", webpage);
        });
        server.on("/callback", HTTP_GET, [this](AsyncWebServerRequest *request) {
            String code = "";
            const char *refreshToken = NULL;
            for (uint8_t i = 0; i < request->args(); i++)
            {
                if (request->argName(i) == "code")
                {
                code = request->arg(i);
                refreshToken = m_spotify.requestAccessTokens(code.c_str(), m_callbackURI);
                }
            }

           
            if (refreshToken != NULL)
            {
                int err = 0;
                this->m_init = true;
                this->m_lastAuth.Update();
                m_refreshToken = refreshToken;
                const char* msg = "Wrote: %s Token: \n %s";
               
                net_config nc;
                if(NetworkManager.GetConfig(nc)){
                    nc.spot_auth = m_refreshToken;
                    NetworkManager.WriteConfig(nc);
                    m_spotify.setRefreshToken(m_refreshToken.c_str());
                }
                char buf[512];
                snprintf(buf, 512, msg, (err) ? "BAD" : "OK",  refreshToken);
                request->send(200, "text/plain", buf);

            }
            else
            {
                request->send(404, "text/plain", "Failed to load token, check serial monitor");
            }
        });
    if(m_hasToken){
        m_spotify.setRefreshToken(m_refreshToken.c_str());
        m_init = true;
        this->m_lastAuth.Update();
        LOG("set spotify refresh token");
    } else {
        matrix.Display("No Spotify Token!");
        LOG("No spotify token?");
    }
}

int CSpotifyIntegration::Update()
{
    m_current.isPlaying = false;
    
    return m_spotify.getCurrentlyPlaying([](CurrentlyPlaying cur){
        Spotify.m_current =  cur;
        Spotify.m_currentUpdate.Update();
    }, "CA");

}

void CSpotifyIntegration::Display(int type)
{
    matrix.staticf("now playing:"); 
    delay(800);
    matrix.SetNextSpeed({50,1});
    matrix.printf("%s - %s ",Spotify.m_current.trackName, Spotify.m_current.artists[0].artistName);
}
void CSpotifyIntegration::SetToken(const std::string &tok)
{
    this->m_refreshToken = tok;
    LOG("Set Token! ");
    LOG(this->m_refreshToken.c_str());
}
bool CSpotifyIntegration::IsPlaying()
{

    if(!m_init)
        return false;
    return m_current.isPlaying;
}

bool CSpotifyIntegration::HasAuth()
{
    return m_init;
    
}
