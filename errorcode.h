#ifndef ERRORCODE_H
#define ERRORCODE_H
#include <QString>

typedef enum {
    OK = 300,
    INIT_COMPONENT_ERROR,
    AA_ZSCAN_DFOV,
    AA_MTF_TIMEOUT
} ErrorCode;


struct ErrorCodeStruct
{
    ErrorCode code;
    QString errorMessage;
};

#include <qstring.h>

#define DEVICES_MANAGER_NAME "DevicesMannager"

typedef enum {
    Warning,
    Error
} ErrorLevel;


class ErrorBase
{
public:
    ErrorBase(QString name = "Unnaming");
private:
    ErrorLevel error_level;
    QString error_source;
    QString error_content;
public:
    void AppendError(QString error);
    void AppendLineError(QString error = "");
    QString GetCurrentError();
};
#endif // ERRORCODE_H
