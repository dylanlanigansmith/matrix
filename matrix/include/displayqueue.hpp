#include <Arduino.h>
#include <queue>
#include <string>

class CDisplayQueue
{
    public:
        CDisplayQueue(){}

    private:
        std::queue<std::string> m_queue;
};