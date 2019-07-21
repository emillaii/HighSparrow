#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include <QMutex>

class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
    void setImage(QImage img);
    QImage img;
    QMutex mutex;
};
#endif // IMAGEPROVIDER_H
