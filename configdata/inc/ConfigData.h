#ifndef CONFIG_DATA_H
#define CONFIG_DATA_H

#include "ConfigStruct.h"

class ConfigData
{
public:
    static ConfigData& Instance();
    bool ReadPaniniConfig(PaniniConfigInfo *psPaniniConfig);

private:
    ConfigData() = default;
    ConfigData(const ConfigData&) = delete;
    ConfigData& operator=(const ConfigData&) = delete;
    ~ConfigData() = default;
    TiXmlDocument xmlDoc;
};

#endif //CONFIG_DATA_H