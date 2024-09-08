#include "CConfig.h"

CConfig::CConfig() {}
CConfig::~CConfig() {}

void CConfig::load(const std::string file) {
    using namespace nlohmann;
    std::ifstream ifs(file);
    if (ifs.is_open()) {
        try {
            json jf = json::parse(ifs);
            from_json(jf, *this);

        } catch (const std::exception& ex) {
            throw std::runtime_error(ex.what());
        }
    } else {
        // Create a json file
        logprintf("%s does not exist... try creating it", file.c_str());

        this->host = "127.0.0.1";
        this->api_port = 8080;
        this->ws_port = 8081;
        this->token = "";
        this->threadCount = 6;

        json jf(*this);
        auto data = jf.dump(4);

        std::ofstream ofs(file);
        if (ofs.is_open()) {
            ofs << data;
        } else {
            throw std::runtime_error("Cannot open config file " + file);
        }
    }
}

std::string CConfig::getHost() { return host; }
std::uint16_t CConfig::getApiPort() { return api_port; }
std::uint16_t CConfig::getWSPort() { return ws_port; }
std::string CConfig::getToken() { return token; }
int CConfig::getThreadCount() { return threadCount; }
void CConfig::to_json(nlohmann::json& j, const CConfig& p) {
    j = nlohmann::json{{"host", p.host},
                       {"api_port", p.api_port},
                       {"ws_port", p.ws_port},
                       {"token", p.token},
                       {"thread_count", p.threadCount}};
}

void CConfig::from_json(const nlohmann::json& j, CConfig& p) {
    j.at("host").get_to(p.host);
    j.at("api_port").get_to(p.api_port);
    j.at("ws_port").get_to(p.ws_port);
    j.at("token").get_to(p.token);
    j.at("thread_count").get_to(p.threadCount);
}

void to_json(nlohmann::json& j, const CConfig& p) { CConfig::to_json(j, p); }

void from_json(const nlohmann::json& j, CConfig& p) {
    CConfig::from_json(j, p);
}