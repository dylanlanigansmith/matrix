#include "feature/feature.hpp"
#include <api/spotify.hpp>
#include <api/weather.hpp>
#include <api_config.hpp>
#include <api/news.hpp>
#include <clock.hpp>


CFeatureManager Features;

void CFeatureManager::OnLoop() 
{
    CTime curTime;
    static CTime lastUpdate = curTime;
    if(curTime.ms() > lastUpdate.ms() + TIME_UPDATE_RATE){
        for(auto& interval : m_intervals){
            if( interval.second.Update(curTime)){
                    LOGF("CFeatureManager: ran %s", interval.first);
                    break;
            }
        }
    }
    switch(m_state)
    {
        case Feature_NowPlaying:
            NowPlaying(); break;
        case Feature_Default:
        default:
            DisplayDefault(); break;
    }
   
}

void CFeatureManager::DisplayDefault()
{
     matrix.staticf("%s    %.1fc", Clock.GetTime(), Weather.get().apparent_temp) ;
}

using songtime_t =  std::pair<uint32_t, uint32_t>;
inline songtime_t GetSongTime(long ms){
    ms /= 1000;
    songtime_t ret = {0u,0u};
    ret.first = ms / 60;
    ret.second = (ms % 60);
    return ret;
}
void CFeatureManager::NowPlaying()
{
    if(!Spotify.m_current.trackName){
    matrix.StaticText("spotify");
    LOG("we got null spotify"); return;
    }
    if(!Spotify.m_current.isPlaying) return;

    
    long pms = Spotify.m_current.progressMs + ( CTime() - Spotify.m_currentUpdate).ms();
   auto prog = GetSongTime(pms);
   auto dur = GetSongTime(Spotify.m_current.durationMs);
   if(pms >= Spotify.m_current.durationMs){
        Spotify.Update(); return;
   } 

   static CTime nextSwap = CTime() + CTime(time_sec_t(5));

   static int info = 1;
  
   const char* disp = (info) ? Spotify.m_current.trackName : Spotify.m_current.artists[0].artistName;
   static int timeWidth = matrix.GetTextWidth(" 06:66");
    
   int dispW = matrix.GetTextWidth(disp);
   
    bool canFit = (dispW ) < MATRIX_WIDTH;
    bool canFitBoth = (dispW + timeWidth) < MATRIX_WIDTH;
    matrix.SetNextSpeed({75,1});
    if(canFit){
        if(canFitBoth){
             matrix.displayf("%s %02lu:%02lu", disp, prog.first, prog.second );
        }
        else  matrix.displayf("%s", disp);
    }
    else matrix.displayf("%s", disp);
   
    matrix.ResetSpeed();
    auto curTime = CTime();
     if(curTime > nextSwap){
        nextSwap = curTime + CTime(time_sec_t(10));
        info = (info > 0) ? 0 : 1;
   }

    //imagine a callback during scrolling that lets me update buffer
    //sounds like a mess 
    

   //matrix.printf("%s - %s %02lu:%02lu / %02lu:%02lu ",Spotify.m_current.trackName, Spotify.m_current.artists[0].artistName, prog.first, prog.second, dur.first, dur.second);
}

void CFeatureManager::Init()
{
    LocationApi::Instance().Display();
    
    m_intervals.emplace("UpdateSpotify", CInterval(CTime(time_sec_t(15)), 
        [this](){
            LOG("UpdateSpotify");
            if(!Spotify.HasAuth()) return;
            long prevDur = Spotify.m_current.durationMs;
            Spotify.Update();
            if(Spotify.m_current.durationMs != prevDur && Spotify.IsPlaying()){
                Spotify.Display();
            }
           if(Spotify.IsPlaying()){
                m_state = Feature_NowPlaying;
           }
           else if( m_state == Feature_NowPlaying){
             m_state = Feature_Default;
           }
        } 
    ));

     m_intervals.emplace("UpdateWeather", CInterval(CTime(15.f), 
        [this](){
            LOG("UpdateWeather");
            Weather.Get();
            matrix.SetNextSpeed({50,1});
            Weather.Display();  
             matrix.ResetSpeed();
        } 
    ));

    m_intervals.emplace("UpdateHeadlines", CInterval(CTime(45.f), [this](){
        bool prev_heads = !Headlines.headlines().empty();
        Headlines.Get();
        if(!prev_heads){
            Headlines.Display();
        }
    }
    
    ));
     m_intervals.emplace("ShowHeadlines", CInterval(CTime(10.f), [this](){
        
        Headlines.Display(); //this should be a state!!!!!!!
    }
    
    ));


      m_intervals.emplace("CheckBrightness", CInterval(CTime(20.f), [this](){
        int hr = Clock.Time().tm_hour;
        
        if( hr >= 21 || hr <= 6 ){
            matrix.SetBrightness(0.35);
        }
        else{
            matrix.SetBrightness(1.0);
        }
        
    }
    
    ));
}