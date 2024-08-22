#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

#ifndef _countof
#define _countof(array) (sizeof(array) / sizeof(array[0]))
#endif

// KPI 데이터 관리 함수들
void insertKPIData(SQLHDBC hDbc);
void selectKPIData(SQLHDBC hDbc);
void updateKPIData(SQLHDBC hDbc, int kpiId, const char* kpiName, double targetValue, double actualValue, const char* assessmentDate);
void deleteKPIData(SQLHDBC hDbc, int kpiId);
void calculateKPIStatistics(SQLHDBC hDbc);
void generateReport(SQLHDBC hDbc);