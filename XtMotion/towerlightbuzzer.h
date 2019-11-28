#ifndef TOWERLIGHTBUZZER_H
#define TOWERLIGHTBUZZER_H

#include <QObject>
#include "XtGeneralOutput.h"
#include "towerlightbuzzerparameter.h"

///
/// \brief The TowerLightBuzzer class
/// 蜂鸣器的打开，闪烁与关闭之间，可以任意切换
/// 三色灯的常亮，闪烁之间，可以任意切换
///
class TowerLightBuzzer: public QObject
{
    Q_OBJECT

public:
    enum TowerLightColor
    {
        Red,
        Green,
        Yellow
    };
    Q_FLAGS(TowerLightColor)

    TowerLightBuzzer();

    void init(XtGeneralOutput* buzzerOut,
              XtGeneralOutput* redOut,
              XtGeneralOutput* greenOut,
              XtGeneralOutput* yellowOut,
              int buzzerBlinkInterval,
              int buzzerBlinkDuration,
              int towerLightBlinkInterval,
              int towerLightBlinkDuration);

    void openBuzzer();
    void blinkBuzzer();
    void closeBuzzer();


    ///
    /// \param color 可以使用|操作符，打开多个颜色
    ///
    void switchColor(TowerLightColor color);
    void blinkColor(TowerLightColor color);

public:
    TowerLightBuzzerParameter parameters;

private:
    XtGeneralOutput* buzzerOut;
    XtGeneralOutput* redOut;
    XtGeneralOutput* greenOut;
    XtGeneralOutput* yellowOut;
    int buzzerBlinkInterval;
    int buzzerBlinkDuration;
    int towerLightBlinkInterval;
    int towerLightBlinkDuration;
};

#endif // TOWERLIGHTBUZZER_H
