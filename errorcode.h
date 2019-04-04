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

#endif // ERRORCODE_H
