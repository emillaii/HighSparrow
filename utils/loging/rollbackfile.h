#ifndef ROLLBACKFILE_H
#define ROLLBACKFILE_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>
#include <QDir>
#include <QTextCodec>
#include <QDebug>


/**
 * @brief The RollbackFile class
 * 性能测试：Release模式下，flushImmediately为true时，每条log约1.4us。flushImmediately为false时，每条log约5.6us。
 * 建议调试期间，flushImmediately设为true，系统稳定后改为false。
 */
class RollbackFile
{
public:
    explicit RollbackFile();
    /**
     * @brief init
     * @param folder 目录名，日志文件将位于folder/{currentDate}目录下，程序运行期间跨日期时，日志文件将重定位到新的{currentDate}目录下
     * @param fileName  文件名，不含扩展名
     * @param maxSize    单个文件最大大小，超出大小后，将该文件重命名，添加后缀.{index}，新的log写入到新建的文件中。
     * @param nBackupFile  保存的备份文件的最大数量，超出该数量时，将删除最早的日志文件
     * @param flushImmediately  每条log是否立即写入到文件，还是只写入流。建议调试期间设为true，系统稳定后改为false。
     */
    void init(QString folder, QString fileName, qint64 maxSize, int nBackupFile, bool flushImmediately);
    void appendLine(QString s);
    ~RollbackFile();

private:
    void initFileStream();
    void rollBack();
    void dateChanged();

    QDateTime dateForLogFolderName;
    QString folder;
    QString fileName;
    qint64 maxSize;
    qint64 currentSize;
    int nBackupFile;
    bool flushImmediately;
    QFile file;
    QTextStream stream;
    QMutex locker;
};

#endif // ROLLBACKFILE_H
