#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "customer_assets.h"
#include "dbutil.h"
#include <stdio.h>
#include <sql.h>
#include <sqlext.h>

void insertCustomerAssetData(SQLHDBC hDbc) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    char sqlQuery[512];
    SQLWCHAR sqlQueryW[512];
    char inputLine[512];

    printf("Enter customer asset data (format: asset_id,customer_name,asset_value,asset_type,acquisition_date):\n");

    while (fgets(inputLine, sizeof(inputLine), stdin)) {
        if (inputLine[0] == '\n' || inputLine[0] == '\r') {
            break;
        }
        int assetId;
        char customerName[50];
        double assetValue;
        char assetType[50];
        char acquisitionDate[20];

        if (sscanf_s(inputLine, "%d,%49[^,],%lf,%49[^,],%19[^,]", &assetId, customerName, (unsigned)_countof(customerName), &assetValue, assetType, (unsigned)_countof(assetType), acquisitionDate, (unsigned)_countof(acquisitionDate)) == 5) {
            sprintf_s(sqlQuery, sizeof(sqlQuery), "INSERT INTO CUSTOMER_ASSETS (asset_id, customer_name, asset_value, asset_type, acquisition_date) VALUES (%d, '%s', %f, '%s', '%s')", assetId, customerName, assetValue, assetType, acquisitionDate);
            MultiByteToWideChar(CP_ACP, 0, sqlQuery, -1, sqlQueryW, 512);

            retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
            checkError(retcode, hDbc, SQL_HANDLE_DBC);

            retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
            checkError(retcode, hStmt, SQL_HANDLE_STMT);

            printf("Customer asset data inserted successfully.\n");

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        }
        else {
            printf("Invalid input format. Please try again.\n");
        }
        printf("Enter next customer asset data (or press Enter to finish):\n");
    }
}



void selectCustomerAssetData(SQLHDBC hDbc) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    SQLWCHAR sqlQueryW[] = L"SELECT asset_id, customer_name, asset_value, asset_type, acquisition_date FROM Customer_Assets";

    SQLINTEGER assetId;
    SQLWCHAR customerName[50];
    SQLDOUBLE assetValue;
    SQLWCHAR assetType[50];
    SQLWCHAR acquisitionDate[20];

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);

    retcode = SQLBindCol(hStmt, 1, SQL_C_SLONG, &assetId, 0, NULL);
    retcode = SQLBindCol(hStmt, 2, SQL_C_WCHAR, customerName, sizeof(customerName), NULL);
    retcode = SQLBindCol(hStmt, 3, SQL_C_DOUBLE, &assetValue, 0, NULL);
    retcode = SQLBindCol(hStmt, 4, SQL_C_WCHAR, assetType, sizeof(assetType), NULL);
    retcode = SQLBindCol(hStmt, 5, SQL_C_WCHAR, acquisitionDate, sizeof(acquisitionDate), NULL);

    printf("Customer Asset Data:\n");
    while (SQLFetch(hStmt) != SQL_NO_DATA) {
        wprintf(L"ID: %d, Customer Name: %ls, Asset Value: %.2f, Asset Type: %ls, Acquisition Date: %ls\n",
            assetId, customerName, assetValue, assetType, acquisitionDate);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void updateCustomerAssetData(SQLHDBC hDbc, int assetId, const char* customerName, double assetValue, const char* assetType, const char* acquisitionDate) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    char sqlQuery[512];
    SQLWCHAR sqlQueryW[512];

    // SQL 쿼리 작성
    sprintf_s(sqlQuery, sizeof(sqlQuery),
        "UPDATE Customer_Assets SET customer_name = '%s', asset_value = %f, asset_type = '%s', acquisition_date = TO_DATE('%s', 'YYYY-MM-DD') WHERE asset_id = %d",
        customerName, assetValue, assetType, acquisitionDate, assetId);

    // 쿼리 문자열을 와이드 문자열로 변환
    MultiByteToWideChar(CP_ACP, 0, sqlQuery, -1, sqlQueryW, 512);

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    // 쿼리 실행
    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);

    printf("Customer asset data updated successfully.\n");

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void deleteCustomerAssetData(SQLHDBC hDbc, int assetId) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    char sqlQuery[256];
    SQLWCHAR sqlQueryW[256];

    // SQL 쿼리 작성
    sprintf_s(sqlQuery, sizeof(sqlQuery), "DELETE FROM Customer_Assets WHERE asset_id = %d", assetId);

    // 쿼리 문자열을 와이드 문자열로 변환
    MultiByteToWideChar(CP_ACP, 0, sqlQuery, -1, sqlQueryW, 256);

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    // 쿼리 실행
    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);

    printf("Customer asset data deleted successfully.\n");

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void filterCustomerAssets(SQLHDBC hDbc) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    SQLWCHAR sqlQueryW[512];
    SQLINTEGER customerId;
    SQLWCHAR customerName[50];
    SQLDOUBLE assetValue;
    double minAssetValue;

    // 사용자로부터 최소 자산 가치를 입력받기
    printf("Enter minimum asset value to filter: ");
    scanf_s("%lf", &minAssetValue);

    // SQL 쿼리 작성 (최소 자산 가치 이상인 고객만 필터링)
    swprintf(sqlQueryW, sizeof(sqlQueryW) / sizeof(SQLWCHAR),
        L"SELECT asset_id, customer_name, asset_value FROM CUSTOMER_ASSETS WHERE asset_value >= %f", minAssetValue);

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

    printf("Customer Assets with Asset Value >= %.2f:\n", minAssetValue);
    while (SQLFetch(hStmt) != SQL_NO_DATA) {
        retcode = SQLGetData(hStmt, 1, SQL_C_SLONG, &customerId, 0, NULL);
        retcode = SQLGetData(hStmt, 2, SQL_C_WCHAR, customerName, sizeof(customerName), NULL);
        retcode = SQLGetData(hStmt, 3, SQL_C_DOUBLE, &assetValue, 0, NULL);

        wprintf(L"ID: %d, Name: %ls, Asset Value: %.2f\n", customerId, customerName, assetValue);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}
