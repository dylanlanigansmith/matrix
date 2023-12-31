#include <include.hpp>
#include <matrix.hpp>

#include <matrix/sign.hpp>

#include <ArduinoJson.hpp>

class ApiRequest
{
    public:
        ApiRequest(const char* m_url, const int m_size  = 1024) : m_url(m_url), m_size(m_size) {};

        virtual void MakeURL();
        virtual void Get();
        virtual void Parse(ArduinoJson::DynamicJsonDocument& json);

        inline sign_msg& msg() { return m_msg; }
        inline std::string& displaystr()  { return m_msg.disp; }
    protected:
        void request();
    private:
        const char* m_url;
        const int m_size;
        sign_msg m_msg;
};