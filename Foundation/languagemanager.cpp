#include "languagemanager.h"
#include <QTranslator>

LanguageManager::LanguageManager(QGuiApplication& _app, QQmlApplicationEngine& _engine):app(_app), engine(_engine)
{

}

void LanguageManager::switchLanguage(int lan)
{
    QTranslator translator;
    switch (lan) {
    case 1:
        translator.load(":/chinese.pm");
        break;
    case 2:
        translator.load(":/english.pm");
        break;
    }
    app.installTranslator(&translator);
    engine.retranslate();
}

