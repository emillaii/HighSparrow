#ifndef ERRORCODE_H
#define ERRORCODE_H
#include <QString>

typedef enum {
    OK = 300,
    INIT_COMPONENT_ERROR,
    AA_ZSCAN_DFOV,
    AA_MTF_TIMEOUT,
    PR_OBJECT_NOT_FOUND,
    GENERIC_ERROR
} ErrorCode;


struct ErrorCodeStruct
{
    ErrorCode code;
    QString errorMessage;
};

#include <qlist.h>
#include <qstring.h>

#define DEVICES_MANAGER_NAME "DevicesMannager"

typedef enum {
    TipNonblock = 0,
    WarningBlock = 1,
    ContinueOrGiveUp = 2,
    ContinueOrRetry = 3,
    RetryOrStop = 4,
    ErrorMustStop = 5
} ErrorLevel;


class ErrorBase
{
public:
    ErrorBase(QString name = "Unnaming");
private:
    ErrorLevel error_level;
    QString error_source;
    QString error_content;
protected:
    QList<ErrorBase*> parts;
public:
    bool is_debug = true;
public:
    void setName(QString name);
    void AppendError(QString error);
    void AppendLineError(QString error = "");
    QString GetCurrentError();
};
#endif // ERRORCODE_H
