#ifndef LANGUAGECONFIG_H
#define LANGUAGECONFIG_H

#include "../configManager/configobject.h"

class LanguageConfig: public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(int language READ language WRITE setLanguage NOTIFY languageChanged)

public:
    enum Language
    {
        Chinese,
        English
    };
    Q_ENUM(Language)

    LanguageConfig(QObject* parent=nullptr):ConfigObject (parent)
    {
        init();
    }

    void publishLanguage()
    {
        emit languageChanged(m_language);
    }


    int language() const
    {
        return m_language;
    }

public slots:
    void setLanguage(int language)
    {
        if (m_language == language)
            return;

        m_language = language;
        emit languageChanged(m_language);
    }

signals:
    void languageChanged(int language);

private:
    int m_language;
};


#endif // LANGUAGECONFIG_H
