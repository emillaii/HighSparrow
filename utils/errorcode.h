#ifndef ERRORCODE_H
#define ERRORCODE_H
#include <QString>

typedef enum {
    OK = 300,
    INIT_COMPONENT_ERROR,
    AA_ZSCAN_DFOV,
    AA_MTF_TIMEOUT,
    PR_OBJECT_NOT_FOUND,
    GENERIC_ERROR,
    PR_OBJECT_SCORE_FAIL,
    GLUE_INSPECTION_FAIL,
    SMALL_HOLE_DETECTION_FAIL
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
    ErrorMustStop = 2
} ErrorLevel;


class ErrorBase
{
public:
    ErrorBase(QString name = "Unnaming");
private:
    ErrorLevel error_level = TipNonblock;
    QString error_source = "";
    QString error_content = "";
protected:
    QList<ErrorBase*> parts;
public:
    bool is_debug = false;
public:
    void setName(QString name);
    void AppendError(QString error);
    void AppendLineError(QString error = "");
    QString GetCurrentError(int layer = 0);
};
#endif // ERRORCODE_H
