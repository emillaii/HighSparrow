#include "motorspositionmodel.h"

MotorsPositionModel::MotorsPositionModel(QObject *parent)
    : QObject(parent)
{
}

MotorsPositionModel::MotorsPositionModel(const QString &name, const double &value, const int &type, QObject *parent)
    : QObject(parent), m_name(name), m_value(value), m_type(type)
{

}

QString MotorsPositionModel::name() const
{
    return m_name;
}

double MotorsPositionModel::value() const
{
    return m_value;
}

void MotorsPositionModel::setName(const QString &name)
{
    if (name != m_name) {
        m_name = name;
        emit nameChanged();
    }
}

void MotorsPositionModel::setValue(double value)
{
    if (value != m_value) {
        m_value = value;
        emit valueChanged();
    }
}

int MotorsPositionModel::type() const
{
    return m_type;
}

void MotorsPositionModel::setType(int type)
{
    m_type = type;
}
