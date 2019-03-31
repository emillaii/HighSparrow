#include "motorspositionmodel.h"

MotorsPositionModel::MotorsPositionModel(QObject *parent)
    : QObject(parent)
{
}

MotorsPositionModel::MotorsPositionModel(const QString &name, const double &value, QObject *parent)
    : QObject(parent), m_name(name), m_value(value)
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
