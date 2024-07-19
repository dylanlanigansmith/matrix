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

    if(NetworkManager.InOTA() || IsLocked()){
        m_state = Feature_Locked; delay(50); return;
    }
    if(curTime.ms() > lastUpdate.ms() + TIME_UPDATE_RATE){
        for(auto& interval : m_intervals){
            if( interval.second.Update(curTime)){
                    LOGF("CFeatureManager: ran %s", interval.first);
                    break;
            }
        }
    }
    /*
    Anything that displays text should be a state (do as I say not as I do) (im you... fix yr shit)

    -update funcs /  intervals should be moved Async
    -tbh this would be a good excuse to learn RTOS
    
    2024.7.19: rewrite this shit it sucks ass

    */
    switch(m_state)
    {
        case Feature_NowPlaying:
            NowPlaying(); break;
        case Feature_Headline:
            NewHeadline(); //not imp
            break;
        case Feature_Locked:
            break;
        case Feature_Developer: //todo
            DeveloperState(); break;
        case Feature_Default:
        default:
            DisplayDefault(); break;
    }
   
}
void CFeatureManager::NewHeadline()
{
    //bigger fish to fry
    /*
    
    scroll half with static thing 

    */

    Headlines.Display(2); 

    m_state = Feature_Default; //make function and remember last state! also enforce locks
}

void CFeatureManager::DeveloperState()
{
    auto startTime = millis();
    
    matrix.ScrollSplit("Mitski - My Body's Made Of Crushed Little Stars", [this, startTime](){

        
        return std::to_string(   (millis() - startTime ) / 1000);
    });
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
inline songtime_t UpdateSongTime(){
    long ms = Spotify.m_current.progressMs + ( CTime() - Spotify.m_currentUpdate).ms();
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
        Spotify.Update(); return; //sus
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
             matrix.displayf("%s %02lu:%02lu", disp, prog.first, prog.second ); //should be staticf
        }
        else  matrix.displayf("%s", disp);
    }
    else{
        matrix.ScrollSplit(disp, [this](){
            auto prog = UpdateSongTime();
            int tb_size = 16;
            char tb[tb_size];
            if(snprintf(tb, tb_size, "%02lu:%02lu", prog.first, prog.second) >= tb_size){
                return std::string("buf");
            };
            return std::string(tb);
        });
    } 
   
    matrix.ResetSpeed();
    auto curTime = CTime();
     if(curTime > nextSwap){
        nextSwap = curTime + CTime(time_sec_t(10));
        info = (info > 0) ? 0 : 1;
   }

    //imagine a callback during scrolling that lets me update buffer
    //sounds like a mess 
    //we are doing that now! 

   //matrix.printf("%s - %s %02lu:%02lu / %02lu:%02lu ",Spotify.m_current.trackName, Spotify.m_current.artists[0].artistName, prog.first, prog.second, dur.first, dur.second);
}

void CFeatureManager::Init()
{
   
    matrix.SetNextSpeed({30,1});
    LocationApi::Instance().Display();
    
    m_intervals.emplace("UpdateSpotify", CInterval(CTime(time_sec_t(15)), 
        [this](){
            LOG("UpdateSpotify");
            
            if(!Spotify.HasAuth()) return;
            long prevDur = Spotify.m_current.durationMs;
            Spotify.Update();
            if(Spotify.m_current.durationMs != prevDur && Spotify.IsPlaying()){
                Spotify.Display(); //sus
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
        if(!prev_heads && NEWS_DISPLAY_ON_BOOT){ //display headlines on boot
            Headlines.Display(2);
        }
    }
    
    ));
     m_intervals.emplace("ShowHeadlines", CInterval(CTime(10.f), [this](){
        //amt thing sorta sucks
        m_state = Feature_Headline;
       
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