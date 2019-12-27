#ifndef CONFIGBASE_H
#define CONFIGBASE_H

#include <QObject>
#include <QVariant>
#include <QJsonValue>

#define ConfigUnitTest 1

class ConfigElementInfo: public QObject
{
    Q_OBJECT

public:
    enum Type
    {
        ConfigArr,
        ConfigObj,
        ConfigObjArr,

        Bool,     //UI will create a Checkbox to visualize the corresponding config
        Int,      //UI will create a TextField with a IntValidator to visualize the corresponding config
        Double,   //UI will create a TextField with a DoubleValidator to visualize the corresponding config
        Other
    };
    Q_ENUM(Type)

    enum Property
    {
        Optional,   //UI will create a ComboBox to visualize the corresponding config
        ReadOnly    //UI item`s enable will be set to false
    };
    Q_ENUM(Property)

    static Type getType(QVariant::Type qvType)
    {
        if (qvType == QVariant::Int ||
                qvType == QVariant::UInt ||
                qvType == QVariant::LongLong ||
                qvType == QVariant:: ULongLong) {
            return Int;
        }else if (qvType == QVariant::Double) {
            return Double;
        }else if (qvType == QVariant::Bool) {
            return Bool;
        }else {
            return Other;
        }
    }
};

class ConfigFile;

class ConfigBase: public QObject
{
    Q_OBJECT

public:
    ConfigBase(ConfigElementInfo::Type type, QObject* parent=nullptr):
        QObject(parent), m_configType(type)
    {

    }

    Q_INVOKABLE ConfigElementInfo::Type configType() const
    {
        return m_configType;
    }


    void connectConfigChangedSignalToSlot(QObject* receiver, int slotIndex, bool connect)
    {
        // Qt did not check if signal was connected to slot.
        // If we connect the signal and slot more than once,
        // one signal emiting would cause multi slot called.
        QString receiverSlotID = createReceiverSlotId(receiver, slotIndex);
        if(configChangedSignalConnectionStates.contains(receiverSlotID) &&
                configChangedSignalConnectionStates[receiverSlotID] == connect){
            return;
        }
        if(connectConfigChangedSignalToSlotImpl(receiver, slotIndex, connect)){
            configChangedSignalConnectionStates[receiverSlotID] = connect;
        }
    }
    virtual bool read(const QJsonValue& jsonValue) = 0;
    virtual void write(QJsonValue& jsonValue) = 0;

protected:
    virtual bool connectConfigChangedSignalToSlotImpl(QObject* receiver, int slotIndex, bool connect) = 0;


private:
    QString createReceiverSlotId(QObject* receiver, int slotIndex)
    {
        // Todo  would crash when running on 32-bit os.
        qint64* receiverAddress = reinterpret_cast<qint64*>(&receiver);
        return QString::number(*receiverAddress) + QString::number(slotIndex);
    }

private:
    friend class ConfigFile;

    ConfigElementInfo::Type m_configType;
    QMap<QString, bool> configChangedSignalConnectionStates;
};

#endif // CONFIGBASE_H
