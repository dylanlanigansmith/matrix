#pragma once
#include <include.hpp>
#include <matrix.hpp>
#include <progtime.hpp>
#include <matrix/sign.hpp>

#include <ArduinoJson.hpp>

struct apiheader_t
{
   
    const char* key;
    const char* val;
    bool use = false;
    inline bool ShouldAdd() const { return use; }
    apiheader_t() : use(false) {}
    apiheader_t(const char* key, const char* val) : key(key), val(val), use(true) {}
};

class ApiRequest
{
    public:
        ApiRequest(const char* m_url, const int m_size  = 1024) : m_url(m_url), m_size(m_size) {};
        static const char* MakeURL(const char* fmt, ...) {
            va_list args;
            va_start(args, fmt);
            char* buf = new char[512]; //too big?
            vsnprintf(buf, sizeof(char) * 512, fmt, args);

            
            va_end(args);
            return buf;
         }
        virtual std::string MakeURL();
        virtual void Get();
        virtual void Parse(ArduinoJson::DynamicJsonDocument& json);
        virtual void Display(int style = 0);
        inline sign_msg& msg() { return m_msg; }
        inline std::string& displaystr()  { return m_msg.disp; }

        inline const CTime& LastUpdate() const { return m_lastUpdate; }
    protected:

        inline void AddHeader(const apiheader_t& head){
            m_header = head;
        }
        inline bool HasHeader() const { return m_header.ShouldAdd();}
        bool request();

        virtual ArduinoJson::V6215PB2::DeserializationError Deserialize(ArduinoJson::DynamicJsonDocument& json, const std::string& data){
            return ArduinoJson::deserializeJson(json, data);
        }
    protected:
        CTime m_lastUpdate;
        const char* m_url;
        const int m_size;
        sign_msg m_msg;
        apiheader_t m_header;
};