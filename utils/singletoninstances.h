#ifndef SINGLETONINSTANCES_H
#define SINGLETONINSTANCES_H

#include "./uiHelper/uioperation.h"
#include "./configManager/configmanager.h"

///
/// \brief singleton instances
///
class SI
{
public:
    static UIOperation ui;
    static ConfigManager cfgManager;
};

#endif // SINGLETONINSTANCES_H
