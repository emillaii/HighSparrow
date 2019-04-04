#ifndef AAHEADMODULE_H
#define AAHEADMODULE_H

#include <QObject>
#include <propertybase.h>
#include <visionmodule.h>
#include <aaheadparameters.h>

class AAHeadModule : public QObject
{
    Q_OBJECT
public:
    AAHeadModule();
    AAHeadParameters aaModuleParams;
public slots:


private:
    VisionModule * visionModule;
};

#endif // AAHEADMODULE_H
