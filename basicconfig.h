#ifndef BASICCONFIG_H
#define BASICCONFIG_H

#include "utils/configManager/configfile.h"
#include "utils/loging/logconfig.h"
#include "utils/languageManager/languagemanager.h"

class BasicConfig: public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(LanguageConfig* languageConfig READ languageConfig)
    Q_PROPERTY(LogConfig* logConfig READ logConfig)

    LanguageConfig* m_languageConfig;

    LogConfig* m_logConfig;

public:
    BasicConfig()
    {
        m_languageConfig = new LanguageConfig;
        m_logConfig = new LogConfig;

        init();
    }

    LanguageConfig* languageConfig() const
    {
        return m_languageConfig;
    }
    LogConfig* logConfig() const
    {
        return m_logConfig;
    }
};

#endif // BASICCONFIG_H
