#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "dbutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlext.h>


void checkError(SQLRETURN retcode, SQLHANDLE handle, SQLSMALLINT type) {
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        SQLWCHAR sqlState[6], message[256];
        SQLINTEGER nativeError;
        SQLSMALLINT textLength;

        SQLGetDiagRec(type, handle, 1, sqlState, &nativeError, message, sizeof(message), &textLength);
        wprintf(L"Error: %s\n", message);
        exit(-1);
    }
}
