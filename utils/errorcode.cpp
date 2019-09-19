#include "utils/errorcode.h"

ErrorBase::ErrorBase(QString name)
{
    error_source = name;
}

void ErrorBase::setName(QString name)
{
    this->error_source = name;
}

void ErrorBase::AppendError(QString error)
{
    error_content.append(error);
}

void ErrorBase::AppendLineError(QString error)
{
    error_content.append(error);
//    error_content.append("\r\n");
}

QString ErrorBase::GetCurrentError(int layer)
{
    layer++;
    foreach (ErrorBase* temp_error, parts) {
        if (temp_error) {
            QString temp_message = temp_error->GetCurrentError(layer);
            if(temp_message != "")
            {
                AppendError("\r\n");
                for (int i = 0; i < layer; ++i)
                    AppendError("   ");
                AppendLineError(temp_message);
            }
        }
    }

    if("" == error_content)
        return "";
    QString temp_error = error_source;
    temp_error.append(":");
    temp_error.append(error_content);
    error_content = "";
    return temp_error;
}
