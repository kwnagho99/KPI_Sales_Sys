#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "kpi.h"
#include "dbutil.h"
#include <stdio.h>
#include <sql.h>
#include <sqlext.h>
#include <float.h>  // DBL_MAX 사용을 위해 추가
void insertKPIData(SQLHDBC hDbc) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    char sqlQuery[512];
    SQLWCHAR sqlQueryW[512];
    char inputLine[512];

    printf("Enter KPI data (format: kpi_id,kpi_name,kpi_value,target_value,measurement_date):\n");

    while (fgets(inputLine, sizeof(inputLine), stdin)) {
        // 입력된 줄이 빈 줄이면 루프 종료
        if (inputLine[0] == '\n') {
            break;
        }

        int kpiId;
        char kpiName[50];
        double kpiValue, targetValue;
        char measurementDate[20];

        if (sscanf_s(inputLine, "%d,%49[^,],%lf,%lf,%19[^,]", &kpiId, kpiName, (unsigned)_countof(kpiName), &kpiValue, &targetValue, measurementDate, (unsigned)_countof(measurementDate)) == 5) {
            sprintf_s(sqlQuery, sizeof(sqlQuery), "INSERT INTO KPI (kpi_id, kpi_name, kpi_value, target_value, measurement_date) VALUES (%d, '%s', %f, %f, '%s')", kpiId, kpiName, kpiValue, targetValue, measurementDate);
            MultiByteToWideChar(CP_ACP, 0, sqlQuery, -1, sqlQueryW, 512);

            retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
            checkError(retcode, hDbc, SQL_HANDLE_DBC);

            retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
            checkError(retcode, hStmt, SQL_HANDLE_STMT);

            printf("KPI data inserted successfully.\n");

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        }
        else {
            printf("Invalid input format. Please try again.\n");
        }

        printf("Enter next KPI data (or press Enter to finish):\n");
    }
}


void selectKPIData(SQLHDBC hDbc) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    SQLWCHAR sqlQueryW[] = L"SELECT KPI_ID, KPI_NAME, KPI_VALUE, TARGET_VALUE, MEASUREMENT_DATE FROM KPI";

    SQLINTEGER kpiId;
    SQLWCHAR kpiName[100];
    SQLDOUBLE kpiValue, targetValue;
    SQLWCHAR measurementDate[20];

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);

    retcode = SQLBindCol(hStmt, 1, SQL_C_SLONG, &kpiId, 0, NULL);
    retcode = SQLBindCol(hStmt, 2, SQL_C_WCHAR, kpiName, sizeof(kpiName), NULL);
    retcode = SQLBindCol(hStmt, 3, SQL_C_DOUBLE, &kpiValue, 0, NULL);
    retcode = SQLBindCol(hStmt, 4, SQL_C_DOUBLE, &targetValue, 0, NULL);
    retcode = SQLBindCol(hStmt, 5, SQL_C_WCHAR, measurementDate, sizeof(measurementDate), NULL);

    printf("KPI Data:\n");
    while (SQLFetch(hStmt) != SQL_NO_DATA) {
        wprintf(L"ID: %d, Name: %ls, Value: %.2f, Target: %.2f, Date: %ls\n",
            kpiId, kpiName, kpiValue, targetValue, measurementDate);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void updateKPIData(SQLHDBC hDbc, int kpiId, const char* kpiName, double kpiValue, double targetValue, const char* measurementDate) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    char sqlQuery[512];
    SQLWCHAR sqlQueryW[512];

    // SQL 쿼리 작성
    sprintf_s(sqlQuery, sizeof(sqlQuery),
        "UPDATE KPI SET KPI_NAME = '%s', KPI_VALUE = %f, TARGET_VALUE = %f, MEASUREMENT_DATE = TO_DATE('%s', 'YYYY-MM-DD') WHERE KPI_ID = %d",
        kpiName, kpiValue, targetValue, measurementDate, kpiId);

    // 쿼리 문자열을 와이드 문자열로 변환
    MultiByteToWideChar(CP_ACP, 0, sqlQuery, -1, sqlQueryW, 512);

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    // 쿼리 실행
    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);

    printf("KPI data updated successfully.\n");

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}


void deleteKPIData(SQLHDBC hDbc, int kpiId) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    char sqlQuery[256];
    SQLWCHAR sqlQueryW[256];

    // SQL 쿼리 작성
    sprintf_s(sqlQuery, sizeof(sqlQuery), "DELETE FROM KPI WHERE kpi_id = %d", kpiId);

    // 쿼리 문자열을 와이드 문자열로 변환
    MultiByteToWideChar(CP_ACP, 0, sqlQuery, -1, sqlQueryW, 256);

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    // 쿼리 실행
    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);

    printf("KPI data deleted successfully.\n");

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void calculateKPIStatistics(SQLHDBC hDbc) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    SQLWCHAR sqlQueryW[] = L"SELECT value FROM KPI";
    SQLDOUBLE kpiValue;
    SQLDOUBLE sum = 0.0;
    SQLDOUBLE max = -DBL_MAX;
    SQLDOUBLE min = DBL_MAX;
    int count = 0;

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    if (retcode != SQL_SUCCESS) {
        printf("Failed to allocate statement handle.\n");
        return;
    }

    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        printf("Failed to execute query.\n");
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    while (SQLFetch(hStmt) != SQL_NO_DATA) {
        retcode = SQLGetData(hStmt, 1, SQL_C_DOUBLE, &kpiValue, 0, NULL);
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            sum += kpiValue;
            if (kpiValue > max) max = kpiValue;
            if (kpiValue < min) min = kpiValue;
            count++;
        }
    }

    if (count > 0) {
        double average = sum / count;
        printf("KPI Statistics:\n");
        printf("Average: %.2f\n", average);
        printf("Max: %.2f\n", max);
        printf("Min: %.2f\n", min);
    }
    else {
        printf("No KPI data available.\n");
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void generateReport(SQLHDBC hDbc) {
    FILE* reportFile;
    fopen_s(&reportFile, "report.txt", "w");
    if (reportFile == NULL) {
        printf("Failed to create report file.\n");
        return;
    }

    fprintf(reportFile, "KPI Statistics Report\n\n");

    // KPI 통계 작성
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    SQLWCHAR sqlQueryW[] = L"SELECT kpi_value FROM KPI";
    SQLDOUBLE kpiValue;
    SQLDOUBLE sum = 0.0;
    SQLDOUBLE max = -DBL_MAX;
    SQLDOUBLE min = DBL_MAX;
    int count = 0;

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    if (retcode != SQL_SUCCESS) {
        fprintf(reportFile, "Failed to allocate statement handle.\n");
        fclose(reportFile);
        return;
    }

    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        fprintf(reportFile, "Failed to execute query.\n");
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        fclose(reportFile);
        return;
    }

    while (SQLFetch(hStmt) != SQL_NO_DATA) {
        retcode = SQLGetData(hStmt, 1, SQL_C_DOUBLE, &kpiValue, 0, NULL);
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            sum += kpiValue;
            if (kpiValue > max) max = kpiValue;
            if (kpiValue < min) min = kpiValue;
            count++;
        }
    }

    if (count > 0) {
        double average = sum / count;
        fprintf(reportFile, "KPI Statistics:\n");
        fprintf(reportFile, "Average: %.2f\n", average);
        fprintf(reportFile, "Max: %.2f\n", max);
        fprintf(reportFile, "Min: %.2f\n", min);
    }
    else {
        fprintf(reportFile, "No KPI data available.\n");
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    fprintf(reportFile, "\nReport generated successfully.\n");
    fclose(reportFile);

    printf("Report generated and saved to report.txt.\n");
}


