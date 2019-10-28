//QT多语言配置

//1. 在.pro文件中加入一行：TRANSLATIONS = chinese.ts english.ts
//    声明要生成的翻译文件（文本格式）
//2. 工具->外部->QT语言家->更新翻译
//    将会提取所有用 tr("") 或者 qsTr("")包围的字符串，生成对应的ts文件
//3. 用Linguist软件（安装QT套装时会自动安装）打开ts文件，编辑翻译
//4. 用Linguist软件发布翻译，生成对应的.qm文件
//    文件->发布
//5. 将.qm文件添加到项目的Resource中，以便发布文件中包含该二进制文件

//增加、删除、更改需要翻译的字符串后，需要重新执行以上2-4步，无更改的字符串无需重新编辑。
//需要切换语言时，调用 LanguageManager.switchLanguage方法


#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>


class LanguageManager
{
    Q_OBJECT

    public:
        LanguageManager(QGuiApplication& app, QQmlApplicationEngine& engine);
        Q_INVOKABLE void switchLanguage(int lan);

    private:
        QGuiApplication& app;
        QQmlApplicationEngine& engine;
};

#endif // LANGUAGEMANAGER_H
