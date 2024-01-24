#include <api/api.hpp>
#include <interval.hpp>
#include <unordered_map>

enum FeatureStates : uint8_t
{
    Feature_Error = 0xFF,
    Feature_Default = 0,
    Feature_Weather,
    Feature_NowPlaying,
    Feature_Headline
};

class CFeatureManager{
    public:
    CFeatureManager() : m_state(Feature_Default) {}

    void Init();
    void OnLoop();

    void DisplayDefault();
    void NowPlaying();
    void NewHeadline();
    protected:
        uint8_t m_state;
        std::unordered_map<const char*, CInterval> m_intervals;

};


extern CFeatureManager Features;
