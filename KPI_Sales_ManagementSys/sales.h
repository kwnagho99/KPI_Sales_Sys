#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

#ifndef _countof
#define _countof(array) (sizeof(array) / sizeof(array[0]))
#endif
// 영업 실적 데이터 관리 함수들
void insertSalesData(SQLHDBC hDbc);
void selectSalesData(SQLHDBC hDbc);
void updateSalesData(SQLHDBC hDbc, int performanceId, const char* salesRepName, double salesAmount, int clientsAcquired, const char* reportingDate);
void deleteSalesData(SQLHDBC hDbc, int performanceId);
void calculateMonthlySales(SQLHDBC hDbc);

