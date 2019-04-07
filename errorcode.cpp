#include "errorcode.h"

ErrorBase::ErrorBase(QString name)
{
    error_source = name;
}

void ErrorBase::AppendError(QString error)
{
    error_content.append(error);
}

void ErrorBase::AppendLineError(QString error)
{
    error_content.append(error);
    error_content.append("\r\n");
}

QString ErrorBase::GetCurrentError()
{
    if("" == error_content)
        return "";
    QString temp_error = error_source;
    temp_error.append(":");
    temp_error.append(error_content);
    error_content = "";
    return temp_error;
}
