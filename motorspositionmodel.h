#ifndef MOTORSPOSITIONMODEL_H
#define MOTORSPOSITIONMODEL_H
#include <QObject>

class MotorsPositionModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
public:
    MotorsPositionModel(QObject *parent = 0);
    MotorsPositionModel(const QString &name, const double &value, QObject *parent=0);

    QString name() const;
    void setName(const QString &name);

    double value() const;
    void setValue(double value);

signals:
    void nameChanged();
    void valueChanged();
private:
    QString m_name;
    double m_value;
};

#endif // MOTORSPOSITIONMODEL_H
