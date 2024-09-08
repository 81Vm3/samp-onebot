#ifndef CPLUGINAPP_H
#define CPLUGINAPP_H

#include <memory>
#include <twobot.hh>

class CConfig;
class CBot;

class CPluginApp
{
public:
    static CPluginApp& getInstance()
    {
        static CPluginApp instance; // Guaranteed to be destroyed.
                                    // Instantiated on first use.
        return instance;
    }

    void init();

    std::string getVersion();
    std::string getAuther();
    
    void printPluginMessage();
    CConfig& getConfig();
    CBot& getBot();

private:
    CPluginApp();
    ~CPluginApp();

    CPluginApp(CPluginApp const&) = delete; // Don't Implement
    void operator=(CPluginApp const&) = delete; // Don't Implement

    // configs
    CConfig* upConfig;
    CBot* upBot;
};

#endif // CPLUGINAPP_H
