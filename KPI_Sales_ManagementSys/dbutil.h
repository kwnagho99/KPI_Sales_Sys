#ifndef DBUTIL_H
#define DBUTIL_H

#include <sql.h>
#include <sqlext.h>


// ���� ó�� �Լ�
void checkError(SQLRETURN retcode, SQLHANDLE handle, SQLSMALLINT type);

#endif
