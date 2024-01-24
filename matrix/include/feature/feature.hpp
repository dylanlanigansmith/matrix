#include <api/api.hpp>
#include <interval.hpp>
#include <unordered_map>


enum FeatureStates : uint8_t
{
    Feature_Error = 0xFF,
    Feature_Default = 0,
    Feature_Weather,
    Feature_NowPlaying,
    Feature_Headline,
    Feature_Locked,
    Feature_Developer
};

class CFeatureManager{
    public:
    CFeatureManager() : m_state(Feature_Default), m_prevState(Feature_Default) {}

    void Init();
    void OnLoop();


//private these
    void DisplayDefault();
    void NowPlaying();
    void NewHeadline();
    void DeveloperState();
    
   
    void Lock() { m_state = Feature_Locked; }
    void Unlock() { m_state = Feature_Default; }
    inline bool IsLocked() const { return m_state == Feature_Locked; }
    uint8_t GetState() const { return m_state; }
    bool SetState(uint8_t state) { if(IsLocked()) return false; m_prevState = m_state; m_state = state; return true;}
    bool ExitState() { if(IsLocked()) return false; m_state = (m_prevState != m_state) ? m_prevState : Feature_Default; return true;} 
    protected:
       
        uint8_t m_state;
         uint8_t m_prevState;
        std::unordered_map<const char*, CInterval> m_intervals;

};




extern CFeatureManager Features;
