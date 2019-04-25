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

#include <qstring.h>

#define DEVICES_MANAGER_NAME "DevicesMannager"

typedef enum {
    TipNonblock = 1<1,
    WarningBlock = 1<2,
    ErrorCanSkip = 1<3,
    ErrorCanRetry = 1<4,
    ErrorMustStop = 1<5
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
    void setName(QString name);
    void AppendError(QString error);
    void AppendLineError(QString error = "");
    virtual QString GetCurrentError();
    void setCurrentErrorLevel(ErrorLevel current_level);
    virtual ErrorLevel GetCurrentErrorLevel();
};
#endif // ERRORCODE_H
