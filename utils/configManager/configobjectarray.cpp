#include "configobjectarray.h"


bool AddConfigObjectComand::execute()
{
    return target->executeAddConfigObject(index);
}

bool AddConfigObjectComand::unExecute()
{
    return target->executeRemoveConfigObject(index);
}

QString AddConfigObjectComand::getExecutionLog()
{
    return QString("%1,%2,Add,Index:%3").arg(CommonMethod::getCurrentDateTime())
            .arg(target->objectName()).arg(index);
}

QString AddConfigObjectComand::getUnExecutionLog()
{
    return QString("%1,%2,Remove,Index:%3").arg(CommonMethod::getCurrentDateTime())
            .arg(target->objectName()).arg(index);
}

bool RemoveConfigObjectCommand::execute()
{
    return target->executeRemoveConfigObject(index);
}

bool RemoveConfigObjectCommand::unExecute()
{
    return target->executeAddConfigObject(index, oldConfigObj);
}

QString RemoveConfigObjectCommand::getExecutionLog()
{
    return QString("%1,%2,Remove,Index:%3").arg(CommonMethod::getCurrentDateTime())
            .arg(target->objectName()).arg(index);
}

QString RemoveConfigObjectCommand::getUnExecutionLog()
{
    return QString("%1,%2,Insert,Index:%3").arg(CommonMethod::getCurrentDateTime())
            .arg(target->objectName()).arg(index);
}




ConfigObjectArray::ConfigObjectArray(QString configObjClassName, QObject* parent):
    ConfigBase(ConfigElementInfo::ConfigObjArr, parent), configObjClassName(configObjClassName)
{
    templateObject = qobject_cast<ConfigObject*>(QObjectFactory::createObject(configObjClassName));
    if(templateObject == nullptr){
        qFatal("Create template object failed!");
        return;
    }
    if(!templateObject->getIsInit()){
        qFatal("You should call ConfigObject::init() in the constructor of class %s", configObjClassName.toStdString().c_str());
        return;
    }
    QStringList configNamesToShow = templateObject->getConfigNamesToShow();
    for (int i = 0; i < configNamesToShow.count(); i++) {
        auto elementType = templateObject->getConfigType(configNamesToShow[i]);
        if(elementType == ConfigElementInfo::ConfigArr ||
                elementType == ConfigElementInfo::ConfigObj ||
                elementType == ConfigElementInfo::ConfigObjArr){
            qFatal("The config type of %s in the class %s must be simple type!",
                   configNamesToShow[i].toStdString().c_str(), configObjClassName.toStdString().c_str());
            return;
        }
    }
}

bool ConfigObjectArray::read(const QJsonValue &jsonValue)
{
    QJsonArray jsonArr = jsonValue.toArray();
    bool result = true;
//    if(configObjects.count() > jsonArr.count()){
//        while (configObjects.count() > jsonArr.count()) {
//            ConfigObject* configObj = configObjects[jsonArr.count() - 1];
//            configObjects.removeAt(jsonArr.count() - 1);
//            if(configChangedSignalReceiver != nullptr){
//                configObj->connectConfigChangedSignalToSlot(configChangedSignalReceiver, configChangedReceiverSlotIndex, false);
//            }
//        }
//    }

    for (int i = 0; i < jsonArr.count(); i++) {
        if(i < configObjects.count()) {
            if(!configObjects[i]->read(jsonArr[i].toObject())){
                result = false;
            }
        }else {
            ConfigObject* configObj = qobject_cast<ConfigObject*>(QObjectFactory::createObject(configObjClassName, this));
            if(configObj == nullptr){
                return false;
            }
            if(!configObj->read(jsonArr[i].toObject())){
                result = false;
            }
            if(configChangedSignalReceiver != nullptr){
                configObj->connectConfigChangedSignalToSlot(configChangedSignalReceiver, configChangedReceiverSlotIndex, true);
            }
            configObjects.append(configObj);
            emit configInserted(configObjects.count() - 1, configObj);
        }
    }
    return result;
}

void ConfigObjectArray::write(QJsonValue &jsonValue)
{
    QJsonArray jsonArr;

    for(int i = 0; i < count(); i++){
        QJsonValue jsonObj;
        configObjects[i]->write(jsonObj);
        jsonArr.append(jsonObj);
    }
    jsonValue = QJsonValue(jsonArr);
}

bool ConfigObjectArray::connectConfigChangedSignalToSlotImpl(QObject *receiver, int slotIndex, bool connect)
{
    configChangedSignalReceiver = receiver;
    configChangedReceiverSlotIndex = slotIndex;
    for(int i = 0; i < metaObject()->methodCount(); i++){
        auto method = metaObject()->method(i);
        if(method.methodType() == QMetaMethod::Signal){
            if(QString(method.name()) == QString("configInserted") ||
                    QString(method.name()) == QString("configRemoved")) {
                if(connect){
                    QMetaObject::connect(this, method.methodIndex(), receiver, slotIndex);
                }else {
                    QMetaObject::disconnect(this, method.methodIndex(), receiver, slotIndex);
                }
            }
        }
    }
    for(int i = 0; i < configObjects.count(); i++){
        configObjects[i]->connectConfigChangedSignalToSlot(receiver, slotIndex, connect);
    }
    return true;
}

bool ConfigObjectArray::executeAddConfigObject(int index, ConfigObject* instance)
{
    if(index < 0 || index > this->count()){
        qCritical() << QString("Index %1 out of range [0, %2]").arg(index).arg(this->count());
        return false;
    }

#ifdef ConfigUnitTest
    qDebug() << QString("Add ConfigObject, index: %1 ").arg(index);
#endif

    if(instance == nullptr){
        instance = qobject_cast<ConfigObject*>(QObjectFactory::createObject(configObjClassName, this));
        if(instance == nullptr){
            return false;
        }
    }
    if(configChangedSignalReceiver != nullptr) {
        instance->connectConfigChangedSignalToSlot(configChangedSignalReceiver, configChangedReceiverSlotIndex, true);
    }
    configObjects.insert(index, instance);
    emit configInserted(index, instance);
    return true;
}

bool ConfigObjectArray::executeRemoveConfigObject(int index)
{
    if(index < 0 || index >= this->count()){
        qCritical() << QString("Index %1 out of range [0, %2)").arg(index).arg(this->count());
        return false;
    }

#ifdef ConfigUnitTest
    qDebug() << QString("Remove config, index: %1 ").arg(index);
#endif

    ConfigObject* configObj = this->at(index);
    configObjects.removeAt(index);
    if(configChangedSignalReceiver != nullptr){
        configObj->connectConfigChangedSignalToSlot(configChangedSignalReceiver, configChangedReceiverSlotIndex, false);
    }
    emit configRemoved(index);
    return true;
}
