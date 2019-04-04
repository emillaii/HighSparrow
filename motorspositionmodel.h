#ifndef MOTORSPOSITIONMODEL_H
#define MOTORSPOSITIONMODEL_H
#include <QObject>

typedef enum {
    MOTION_POSITION,
    MOVE_TO_BUTTON
} ModelType;


class MotorsPositionModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(int type READ type WRITE setType)
public:
    MotorsPositionModel(QObject *parent = 0);
    MotorsPositionModel(const QString &name, const double &value, const int &type, QObject *parent=0);

    QString name() const;
    void setName(const QString &name);

    double value() const;
    void setValue(double value);

    int type() const;
    void setType(int type);

signals:
    void nameChanged();
    void valueChanged();

private:
    QString m_name;
    double m_value;
    int m_type;
};

#endif // MOTORSPOSITIONMODEL_H
