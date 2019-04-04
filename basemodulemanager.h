#ifndef BASEMODULEMANAGER_H
#define BASEMODULEMANAGER_H

#include <QObject>
#include "baslerpyloncamera.h"
#include "visionmodule.h"
#include "wordoplight.h"
#include "propertybase.h"
#include "LontryLight.h"

class BaseModuleManager : public PropertyBase
{
    Q_OBJECT
    int m_downlookLighting;

    int m_uplookLighting;

    int m_lightPanelLighting;

public:
    explicit BaseModuleManager(QObject *parent = nullptr);
    ~BaseModuleManager();

    Q_PROPERTY(int downlookLighting READ downlookLighting WRITE setDownlookLighting NOTIFY lightingValueChanged);
    Q_PROPERTY(int uplookLighting READ uplookLighting WRITE setUplookLighting NOTIFY lightingValueChanged);
    Q_PROPERTY(int lightPanelLighting READ lightPanelLighting WRITE setLightPanelLighting NOTIFY lightPanelValueChanged);

    BaslerPylonCamera * pylonDownlookCamera = Q_NULLPTR;
    BaslerPylonCamera * pylonUplookCamera = Q_NULLPTR;
    VisionModule * visionModule = Q_NULLPTR;
    WordopLight * lightingModule = Q_NULLPTR;
    LontryLight * lightPanel = Q_NULLPTR;

    Q_INVOKABLE void testVision();

    int downlookLighting() const
    {
        return m_downlookLighting;
    }

    int uplookLighting() const
    {
        return m_uplookLighting;
    }

    int lightPanelLighting() const
    {
        return m_lightPanelLighting;
    }

signals:
    void lightingValueChanged(int downlookLighting);

    void lightPanelValueChanged(int lightPanelLighting);

public slots:
    void setDownlookLighting(int downlookLighting)
    {
        if (m_downlookLighting == downlookLighting)
            return;

        m_downlookLighting = downlookLighting;
        emit lightingValueChanged(m_downlookLighting);
    }
    void setUplookLighting(int uplookLighting)
    {
        if (m_uplookLighting == uplookLighting)
            return;

        m_uplookLighting = uplookLighting;
        emit lightingValueChanged(m_uplookLighting);
    }
    void setLightPanelLighting(int lightPanelLighting)
    {
        if (m_lightPanelLighting == lightPanelLighting)
            return;

        m_lightPanelLighting = lightPanelLighting;
        emit lightPanelValueChanged(m_lightPanelLighting);
    }
};

#endif // BASEMODULEMANAGER_H
