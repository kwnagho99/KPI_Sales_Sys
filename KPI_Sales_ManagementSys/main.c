#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "sales.h"
#include "kpi.h"
#include "customer_assets.h"
#include "dbutil.h"
#include <sql.h>
#include <sqlext.h>

void displayMenu() {
    printf("Welcome to the Business Management System\n");
    printf("Please choose an option:\n");
    printf("1. Input Sales Data\n");
    printf("2. View Sales Data\n");
    printf("3. Input KPI Data\n");
    printf("4. View KPI Data\n");
    printf("5. Input Customer Asset Data\n");
    printf("6. View Customer Asset Data\n");
    printf("7. Calculate Monthly Sales\n"); // 수정된 기능
    printf("8. Filter Customer Assets\n"); // 수정된 기능
    printf("9. Generate KPI Report\n"); // 수정된 기능
    printf("10. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    SQLHENV hEnv = NULL;
    SQLHDBC hDbc = NULL;
    SQLRETURN retcode;

    // ODBC 환경 초기화
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    checkError(retcode, hEnv, SQL_HANDLE_ENV);

    retcode = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    checkError(retcode, hEnv, SQL_HANDLE_ENV);

    retcode = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    // 데이터베이스 연결
    retcode = SQLConnect(hDbc, (SQLWCHAR*)L"XE", SQL_NTS,
        (SQLWCHAR*)L"c##kwangho", SQL_NTS, (SQLWCHAR*)L"0219", SQL_NTS);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    int choice;
    do {
        displayMenu();
        scanf_s("%d", &choice);
        getchar();
        switch (choice) {
        case 1:
            insertSalesData(hDbc);
            //selectSalesData(hDbc);
            break;
        case 2:
            selectSalesData(hDbc);
            break;
        case 3:
            insertKPIData(hDbc);
            //selectKPIData(hDbc);
            break;
        case 4:
            selectKPIData(hDbc);
            break;
        case 5:
            insertCustomerAssetData(hDbc);
          //selectCustomerAssetData(hDbc);
            break;
        case 6:
            selectCustomerAssetData(hDbc);
            break;
        case 7:
            calculateMonthlySales(hDbc); // 수정된 기능
            break;
        case 8:
            filterCustomerAssets(hDbc); // 수정된 기능
            break;
        case 9:
            generateReport(hDbc); // 수정된 기능
            break;
        case 10:
            printf("Exiting the program.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    } while (choice != 10);

    // 데이터베이스 연결 종료
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    printf("Disconnected from database.\n");
    return 0;
}
