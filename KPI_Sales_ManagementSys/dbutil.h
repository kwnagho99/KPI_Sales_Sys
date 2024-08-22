#ifndef DBUTIL_H
#define DBUTIL_H

#include <sql.h>
#include <sqlext.h>


// 오류 처리 함수
void checkError(SQLRETURN retcode, SQLHANDLE handle, SQLSMALLINT type);

#endif
