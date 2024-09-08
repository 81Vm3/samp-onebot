#ifndef CCONFIG_HPP
#define CCONFIG_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include "logprintf.h"

class CConfig {
private:
    std::string host;
    std::uint16_t api_port;
    std::uint16_t ws_port;
    std::string token;
    int threadCount;
    
public:
    CConfig(/* args */);
    ~CConfig();

    void load(const std::string file);

    static void to_json(nlohmann::json& j, const CConfig& p);
    static void from_json(const nlohmann::json& j, CConfig& p);

    std::string getHost();
    std::uint16_t getApiPort();
    std::uint16_t getWSPort();
    std::string getToken();
    int getThreadCount();
};

void to_json(nlohmann::json& j, const CConfig& p);
void from_json(nlohmann::json& j, const CConfig& p);

#endif