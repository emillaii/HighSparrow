#ifndef CONFIGMANAGERUNITTEST_H
#define CONFIGMANAGERUNITTEST_H

#include "configobject.h"
#include "configarray.h"
#include "configobjectarray.h"

class TestClassA: public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(int editableInt READ editableInt WRITE seteditableInt NOTIFY editableIntChanged)
    Q_PROPERTY(int readonlyInt READ readonlyInt WRITE setreadonlyInt NOTIFY readonlyIntChanged)
    Q_PROPERTY(int optionalInt READ optionalInt WRITE setoptionalInt NOTIFY optionalIntChanged)
    Q_PROPERTY(int hiddenInt READ hiddenInt WRITE sethiddenInt NOTIFY hiddenIntChanged)

    Q_PROPERTY(double editableDouble READ editableDouble WRITE seteditableDouble NOTIFY editableDoubleChanged)
    Q_PROPERTY(double readonlyDouble READ readonlyDouble WRITE setreadonlyDouble NOTIFY readonlyDoubleChanged)
    Q_PROPERTY(double optionalDouble READ optionalDouble WRITE setoptionalDouble NOTIFY optionalDoubleChanged)
    Q_PROPERTY(double hiddenDouble READ hiddenDouble WRITE sethiddenDouble NOTIFY hiddenDoubleChanged)

    int m_editableInt;

    int m_readonlyInt;

    int m_optionalInt;

    int m_hiddenInt;

    double m_editableDouble;

    double m_readonlyDouble;

    double m_optionalDouble;

    double m_hiddenDouble;

public:
    TestClassA(QObject* parent=nullptr):ConfigObject(parent)
    {
        hide("hiddenInt");
        hide("hiddenDouble");

        setReadOnlyProperty("readonlyDouble");
        setReadOnlyProperty("readonlyInt");

        QVariantList intOptions;
        for(int i = 0; i < 10; i++){
            intOptions.append(i + 4);
        }
        QVariantList doubleOptions;
        for(int i = 0; i < 10; i++){
            doubleOptions.append(i / 3.4);
        }
        setOptionalProperty("optionalInt", intOptions);
        setOptionalProperty("optionalDouble", doubleOptions);

        init();
    }
    int editableInt() const
    {
        return m_editableInt;
    }
    int readonlyInt() const
    {
        return m_readonlyInt;
    }

    int optionalInt() const
    {
        return m_optionalInt;
    }

    int hiddenInt() const
    {
        return m_hiddenInt;
    }

    double editableDouble() const
    {
        return m_editableDouble;
    }

    double readonlyDouble() const
    {
        return m_readonlyDouble;
    }

    double optionalDouble() const
    {
        return m_optionalDouble;
    }

    double hiddenDouble() const
    {
        return m_hiddenDouble;
    }

public slots:
    void seteditableInt(int editableInt)
    {
        if (m_editableInt == editableInt)
            return;

        m_editableInt = editableInt;
        emit editableIntChanged(m_editableInt);
    }
    void setreadonlyInt(int readonlyInt)
    {
        if (m_readonlyInt == readonlyInt)
            return;

        m_readonlyInt = readonlyInt;
        emit readonlyIntChanged(m_readonlyInt);
    }

    void setoptionalInt(int optionalInt)
    {
        if (m_optionalInt == optionalInt)
            return;

        m_optionalInt = optionalInt;
        emit optionalIntChanged(m_optionalInt);
    }

    void sethiddenInt(int hiddenInt)
    {
        if (m_hiddenInt == hiddenInt)
            return;

        m_hiddenInt = hiddenInt;
        emit hiddenIntChanged(m_hiddenInt);
    }

    void seteditableDouble(double editableDouble)
    {
        if (qFuzzyCompare(m_editableDouble, editableDouble))
            return;

        m_editableDouble = editableDouble;
        emit editableDoubleChanged(m_editableDouble);
    }

    void setreadonlyDouble(double readonlyDouble)
    {
        if (qFuzzyCompare(m_readonlyDouble, readonlyDouble))
            return;

        m_readonlyDouble = readonlyDouble;
        emit readonlyDoubleChanged(m_readonlyDouble);
    }

    void setoptionalDouble(double optionalDouble)
    {
        if (qFuzzyCompare(m_optionalDouble, optionalDouble))
            return;

        m_optionalDouble = optionalDouble;
        emit optionalDoubleChanged(m_optionalDouble);
    }

    void sethiddenDouble(double hiddenDouble)
    {
        if (qFuzzyCompare(m_hiddenDouble, hiddenDouble))
            return;

        m_hiddenDouble = hiddenDouble;
        emit hiddenDoubleChanged(m_hiddenDouble);
    }

signals:
    void editableIntChanged(int editableInt);
    void readonlyIntChanged(int readonlyInt);
    void optionalIntChanged(int optionalInt);
    void hiddenIntChanged(int hiddenInt);
    void editableDoubleChanged(double editableDouble);
    void readonlyDoubleChanged(double readonlyDouble);
    void optionalDoubleChanged(double optionalDouble);
    void hiddenDoubleChanged(double hiddenDouble);
};

class TestClassB: public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString editableString READ editableString WRITE seteditableString NOTIFY editableStringChanged)
    Q_PROPERTY(QString readonlyString READ readonlyString WRITE setreadonlyString NOTIFY readonlyStringChanged)
    Q_PROPERTY(QString optionalString READ optionalString WRITE setoptionalString NOTIFY optionalStringChanged)
    Q_PROPERTY(QString hiddenString READ hiddenString WRITE sethiddenString NOTIFY hiddenStringChanged)

    Q_PROPERTY(bool editableBool READ editableBool WRITE seteditableBool NOTIFY editableBoolChanged)
    Q_PROPERTY(bool readonlyBool READ readonlyBool WRITE setreadonlyBool NOTIFY readonlyBoolChanged)
    Q_PROPERTY(bool hiddenBool READ hiddenBool WRITE sethiddenBool NOTIFY hiddenBoolChanged)

    Q_PROPERTY(TestClassA* testClassA READ testClassA)
    Q_PROPERTY(ConfigArray* boolArray READ boolArray)
    Q_PROPERTY(ConfigObjectArray* testClassAArray READ testClassAArray)

    QString m_editableString;

    QString m_readonlyString;

    QString m_optionalString;

    QString m_hiddenString;

    bool m_editableBool;

    bool m_readonlyBool;

    bool m_hiddenBool;

    TestClassA* m_testClassA;

    ConfigArray* m_boolArray;

    ConfigObjectArray* m_testClassAArray;

public:
    TestClassB(QObject* parent=nullptr): ConfigObject(parent)
    {
        hide("hiddenString");
        hide("hiddenBool");

        setReadOnlyProperty("readonlyString");
        setReadOnlyProperty("readonlyBool");

        QVariantList stringOptions;
        for(int i = 0; i < 10; i++){
            stringOptions.append(QString::number(i) + "HiHi");
        }
        setOptionalProperty("optionalString", stringOptions);

        m_testClassA = new TestClassA;
        m_testClassA->setObjectName("m_testClassA");
        m_boolArray = new ConfigArray(ConfigElementInfo::Bool);
        m_boolArray->setObjectName("m_boolArray");
        m_testClassAArray = new ConfigObjectArray("TestClassA");
        m_testClassAArray->setObjectName("m_testClassAArray");
        init();
    }
    QString editableString() const
    {
        return m_editableString;
    }
    QString readonlyString() const
    {
        return m_readonlyString;
    }

    QString optionalString() const
    {
        return m_optionalString;
    }

    QString hiddenString() const
    {
        return m_hiddenString;
    }

    bool editableBool() const
    {
        return m_editableBool;
    }

    bool readonlyBool() const
    {
        return m_readonlyBool;
    }

    bool hiddenBool() const
    {
        return m_hiddenBool;
    }

    TestClassA* testClassA() const
    {
        return m_testClassA;
    }

    ConfigArray* boolArray() const
    {
        return m_boolArray;
    }

    ConfigObjectArray* testClassAArray() const
    {
        return m_testClassAArray;
    }

public slots:
    void seteditableString(QString editableString)
    {
        if (m_editableString == editableString)
            return;

        m_editableString = editableString;
        emit editableStringChanged(m_editableString);
    }
    void setreadonlyString(QString readonlyString)
    {
        if (m_readonlyString == readonlyString)
            return;

        m_readonlyString = readonlyString;
        emit readonlyStringChanged(m_readonlyString);
    }

    void setoptionalString(QString optionalString)
    {
        if (m_optionalString == optionalString)
            return;

        m_optionalString = optionalString;
        emit optionalStringChanged(m_optionalString);
    }

    void sethiddenString(QString hiddenString)
    {
        if (m_hiddenString == hiddenString)
            return;

        m_hiddenString = hiddenString;
        emit hiddenStringChanged(m_hiddenString);
    }

    void seteditableBool(bool editableBool)
    {
        if (m_editableBool == editableBool)
            return;

        m_editableBool = editableBool;
        emit editableBoolChanged(m_editableBool);
    }

    void setreadonlyBool(bool readonlyBool)
    {
        if (m_readonlyBool == readonlyBool)
            return;

        m_readonlyBool = readonlyBool;
        emit readonlyBoolChanged(m_readonlyBool);
    }

    void sethiddenBool(bool hiddenBool)
    {
        if (m_hiddenBool == hiddenBool)
            return;

        m_hiddenBool = hiddenBool;
        emit hiddenBoolChanged(m_hiddenBool);
    }

signals:
    void editableStringChanged(QString editableString);
    void readonlyStringChanged(QString readonlyString);
    void optionalStringChanged(QString optionalString);
    void hiddenStringChanged(QString hiddenString);
    void editableBoolChanged(bool editableBool);
    void readonlyBoolChanged(bool readonlyBool);
    void hiddenBoolChanged(bool hiddenBool);
};

class TestClassC: public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(ConfigArray* intArray READ intArray)
    Q_PROPERTY(ConfigArray* doubleArray READ doubleArray)
    Q_PROPERTY(ConfigArray* stringArray READ stringArray)
    Q_PROPERTY(TestClassB* testClassB READ testClassB)
    Q_PROPERTY(int intValue READ intValue WRITE setintValue NOTIFY intValueChanged)

    ConfigArray* m_intArray;

    ConfigArray* m_doubleArray;

    ConfigArray* m_stringArray;

    TestClassB* m_testClassB;

    int m_intValue;

public:
    TestClassC()
    {
        m_intArray = new ConfigArray(ConfigElementInfo::Int);
        m_intArray->setObjectName("m_intArray");
        m_doubleArray = new ConfigArray(ConfigElementInfo::Double);
        m_doubleArray->setObjectName("m_doubleArray");
        m_stringArray = new ConfigArray(ConfigElementInfo::Other);
        m_stringArray->setObjectName("m_stringArray");
        m_testClassB = new TestClassB;
        m_testClassB->setObjectName("m_testClassB");

        init();
    }

    ConfigArray* intArray() const
    {
        return m_intArray;
    }
    ConfigArray* doubleArray() const
    {
        return m_doubleArray;
    }
    ConfigArray* stringArray() const
    {
        return m_stringArray;
    }
    TestClassB* testClassB() const
    {
        return m_testClassB;
    }
    int intValue() const
    {
        return m_intValue;
    }
public slots:
    void setintValue(int intValue)
    {
        if (m_intValue == intValue)
            return;

        m_intValue = intValue;
        emit intValueChanged(m_intValue);
    }
signals:
    void intValueChanged(int intValue);
};

class TestClassD: public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(ConfigArray* readonlyIntArray READ readonlyIntArray)
    Q_PROPERTY(ConfigArray* readonlyBoolArray READ readonlyBoolArray)
    Q_PROPERTY(ConfigArray* readonlyDoubleArray READ readonlyDoubleArray)
    Q_PROPERTY(ConfigArray* readonlyStringArray READ readonlyStringArray)

    Q_PROPERTY(ConfigArray* optionalIntArray READ optionalIntArray)
    Q_PROPERTY(ConfigArray* optionalDoubleArray READ optionalDoubleArray)
    Q_PROPERTY(ConfigArray* optionalStringArray READ optionalStringArray)

    ConfigArray* m_readonlyIntArray;

    ConfigArray* m_readonlyBoolArray;

    ConfigArray* m_readonlyDoubleArray;

    ConfigArray* m_readonlyStringArray;

    ConfigArray* m_optionalIntArray;

    ConfigArray* m_optionalDoubleArray;

    ConfigArray* m_optionalStringArray;

public:
    TestClassD()
    {
        m_readonlyIntArray = new ConfigArray(ConfigElementInfo::Int);
        m_readonlyIntArray->setObjectName("m_readonlyIntArray");
        m_readonlyBoolArray = new ConfigArray(ConfigElementInfo::Bool);
        m_readonlyBoolArray->setObjectName("m_readonlyBoolArray");
        m_readonlyDoubleArray = new ConfigArray(ConfigElementInfo::Double);
        m_readonlyDoubleArray->setObjectName("m_readonlyDoubleArray");
        m_readonlyStringArray = new ConfigArray(ConfigElementInfo::Other);
        m_readonlyStringArray->setObjectName("m_readonlyStringArray");
        m_optionalIntArray = new ConfigArray(ConfigElementInfo::Int);
        m_optionalIntArray->setObjectName("m_optionalIntArray");
        m_optionalDoubleArray = new ConfigArray(ConfigElementInfo::Double);
        m_optionalDoubleArray->setObjectName("m_optionalDoubleArray");
        m_optionalStringArray = new ConfigArray(ConfigElementInfo::Other);
        m_optionalStringArray->setObjectName("m_optionalStringArray");

        m_readonlyIntArray->setReadOnlyProperty();
        m_readonlyBoolArray->setReadOnlyProperty();
        m_readonlyDoubleArray->setReadOnlyProperty();
        m_readonlyStringArray->setReadOnlyProperty();

        QVariantList intOptions;
        for (int i = 0; i < 10; i++) {
            intOptions.append(i);
        }
        QVariantList doubleOptions;
        for (int i = 0; i < 10; i++) {
            doubleOptions.append(i * 3.14);
        }
        QVariantList stringOptions;
        for (int i = 0; i < 10; i++) {
            stringOptions.append("sdfsd" + QString::number(i));
        }
        m_optionalIntArray->setOptionalProperty(intOptions);
        optionalDoubleArray()->setOptionalProperty(doubleOptions);
        optionalStringArray()->setOptionalProperty(stringOptions);

        init();
    }

    ConfigArray* readonlyIntArray() const
    {
        return m_readonlyIntArray;
    }
    ConfigArray* readonlyBoolArray() const
    {
        return m_readonlyBoolArray;
    }
    ConfigArray* readonlyDoubleArray() const
    {
        return m_readonlyDoubleArray;
    }
    ConfigArray* readonlyStringArray() const
    {
        return m_readonlyStringArray;
    }
    ConfigArray* optionalIntArray() const
    {
        return m_optionalIntArray;
    }
    ConfigArray* optionalDoubleArray() const
    {
        return m_optionalDoubleArray;
    }
    ConfigArray* optionalStringArray() const
    {
        return m_optionalStringArray;
    }
};

class TestClassE: public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(int a READ a WRITE setA NOTIFY aChanged)
    Q_PROPERTY(int editableInt READ editableInt WRITE seteditableInt NOTIFY editableIntChanged)
    Q_PROPERTY(int optionalInt READ optionalInt WRITE setoptionalInt NOTIFY optionalIntChanged)
    Q_PROPERTY(double optionalDouble READ optionalDouble WRITE setoptionalDouble NOTIFY optionalDoubleChanged)
    Q_PROPERTY(double editableDouble READ editableDouble WRITE seteditableDouble NOTIFY editableDoubleChanged)
    Q_PROPERTY(QString editableString READ editableString WRITE seteditableString NOTIFY editableStringChanged)
    Q_PROPERTY(QString optionalString READ optionalString WRITE setoptionalString NOTIFY optionalStringChanged)
    Q_PROPERTY(bool editableBool READ editableBool WRITE seteditableBool NOTIFY editableBoolChanged)

    int m_a;

    int m_editableInt;

    int m_optionalInt;

    double m_optionalDouble;

    double m_editableDouble;

    QString m_editableString;

    QString m_optionalString;

    bool m_editableBool;

public:
    TestClassE(QObject* parent=nullptr):ConfigObject(parent)
    {
        QVariantList options;
        for (int i = 0; i < 10; i++) {
            options.append(i);
        }
        setOptionalProperty("optionalDouble", options);
        setOptionalProperty("optionalInt", options);
        setOptionalProperty("optionalString", options);

        init();
    }


    int a() const
    {
        return m_a;
    }
    int editableInt() const
    {
        return m_editableInt;
    }

    int optionalInt() const
    {
        return m_optionalInt;
    }

    double optionalDouble() const
    {
        return m_optionalDouble;
    }

    double editableDouble() const
    {
        return m_editableDouble;
    }

    QString editableString() const
    {
        return m_editableString;
    }

    QString optionalString() const
    {
        return m_optionalString;
    }

    bool editableBool() const
    {
        return m_editableBool;
    }

public slots:

    void setA(int a)
    {
        if (m_a == a)
            return;

        m_a = a;
        emit aChanged(m_a);
    }
    void seteditableInt(int editableInt)
    {
        if (m_editableInt == editableInt)
            return;

        m_editableInt = editableInt;
        emit editableIntChanged(m_editableInt);
    }

    void setoptionalInt(int optionalInt)
    {
        if (m_optionalInt == optionalInt)
            return;

        m_optionalInt = optionalInt;
        emit optionalIntChanged(m_optionalInt);
    }

    void setoptionalDouble(double optionalDouble)
    {
        if (qFuzzyCompare(m_optionalDouble, optionalDouble))
            return;

        m_optionalDouble = optionalDouble;
        emit optionalDoubleChanged(m_optionalDouble);
    }

    void seteditableDouble(double editableDouble)
    {
        if (qFuzzyCompare(m_editableDouble, editableDouble))
            return;

        m_editableDouble = editableDouble;
        emit editableDoubleChanged(m_editableDouble);
    }

    void seteditableString(QString editableString)
    {
        if (m_editableString == editableString)
            return;

        m_editableString = editableString;
        emit editableStringChanged(m_editableString);
    }

    void setoptionalString(QString optionalString)
    {
        if (m_optionalString == optionalString)
            return;

        m_optionalString = optionalString;
        emit optionalStringChanged(m_optionalString);
    }

    void seteditableBool(bool editableBool)
    {
        if (m_editableBool == editableBool)
            return;

        m_editableBool = editableBool;
        emit editableBoolChanged(m_editableBool);
    }

signals:
    void aChanged(int a);
    void editableIntChanged(int editableInt);
    void optionalIntChanged(int optionalInt);
    void optionalDoubleChanged(double optionalDouble);
    void editableDoubleChanged(double editableDouble);
    void editableStringChanged(QString editableString);
    void optionalStringChanged(QString optionalString);
    void editableBoolChanged(bool editableBool);
};

#endif // CONFIGMANAGERUNITTEST_H


