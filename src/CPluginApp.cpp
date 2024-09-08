#include "CPluginApp.h"

#include "CBot.h"
#include "CConfig.h"

CPluginApp::CPluginApp() {}

CPluginApp::~CPluginApp() {
    delete upConfig;
    delete upBot;
}

void CPluginApp::init() {
    upConfig = new CConfig();
    upBot = new CBot();
}

std::string CPluginApp::getVersion() { return std::string("1.0.0 BETA"); }
std::string CPluginApp::getAuther() { return std::string("Blume"); }

void CPluginApp::printPluginMessage() {
    logprintf("   _______   __  ______    ____  _  _________  ____  ______");
    logprintf("  / __/ _ | /  |/  / _ \\  / __ \\/ |/ / __/ _ )/ __ \\/_  __/");
    logprintf(" _\\ \\/ __ |/ /|_/ / ___/ / /_/ /    / _// _  / /_/ / / /   ");
    logprintf("/___/_/ |_/_/  /_/_/     \\____/_/|_/___/____/\\____/ /_/    ");

    logprintf("-------------------------------------");
    logprintf("SAMP-ONEBOT PLUGIN LOAD!");
    logprintf("Auther: %s", this->getAuther().c_str());
    logprintf("Current version: %s", this->getVersion().c_str());
    logprintf("Visit https://github.com/81Vm3 !");
    logprintf("-------------------------------------");
}

CConfig& CPluginApp::getConfig() { return *upConfig; }
CBot& CPluginApp::getBot() { return *upBot; }