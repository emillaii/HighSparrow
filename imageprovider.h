#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QObject>

class ImageProvider : public QObject
{
    Q_OBJECT
public:
    explicit ImageProvider(QObject *parent = nullptr);

signals:

public slots:
};

#endif // IMAGEPROVIDER_H