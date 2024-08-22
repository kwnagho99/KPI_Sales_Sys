#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "sales.h"
#include "dbutil.h"
#include <stdio.h>
#include <sql.h>
#include <sqlext.h>


void insertSalesData(SQLHDBC hDbc) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    char sqlQuery[512];
    SQLWCHAR sqlQueryW[512];
    char inputLine[512];

    printf("Enter sales data (format: sale_id,product_name,sale_date,amount):\n");

    while (fgets(inputLine, sizeof(inputLine), stdin)) {
        if (inputLine[0] == '\n' || inputLine[0] == '\r') {
            break;
        }

        int saleId;
        char productName[50];
        char saleDate[20];
        double amount;

        if (sscanf_s(inputLine, "%d,%49[^,],%19[^,],%lf", &saleId, productName, (unsigned)_countof(productName), saleDate, (unsigned)_countof(saleDate), &amount) == 4) {
            sprintf_s(sqlQuery, sizeof(sqlQuery), "INSERT INTO SALES (sale_id, product_name, sale_date, amount) VALUES (%d, '%s', '%s', %f)", saleId, productName, saleDate, amount);
            MultiByteToWideChar(CP_ACP, 0, sqlQuery, -1, sqlQueryW, 512);

            retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
            checkError(retcode, hDbc, SQL_HANDLE_DBC);

            retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
            checkError(retcode, hStmt, SQL_HANDLE_STMT);

            printf("Sales data inserted successfully.\n");

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        }
        else {
            printf("Invalid input format. Please try again.\n");
        }

        printf("Enter next sales data (or press Enter to finish):\n");
    }
}

void selectSalesData(SQLHDBC hDbc) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    SQLWCHAR sqlQueryW[] = L"SELECT sale_id, product_name, sale_date, amount FROM SALES";

    SQLINTEGER saleId = 0;
    SQLWCHAR productName[50] = { 0 };
    SQLDOUBLE amount = 0.0;
    SQLWCHAR saleDate[20] = { 0 };

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);

    retcode = SQLBindCol(hStmt, 1, SQL_C_SLONG, &saleId, 0, NULL);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);
    retcode = SQLBindCol(hStmt, 2, SQL_C_WCHAR, productName, sizeof(productName) / sizeof(SQLWCHAR), NULL);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);
    retcode = SQLBindCol(hStmt, 3, SQL_C_WCHAR, saleDate, sizeof(saleDate) / sizeof(SQLWCHAR), NULL);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);
    retcode = SQLBindCol(hStmt, 4, SQL_C_DOUBLE, &amount, 0, NULL);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);

    printf("Sales Data:\n");
    while (SQLFetch(hStmt) != SQL_NO_DATA) {
        wprintf(L"ID: %d, Product: %ls, Amount: %.2f, Sale Date: %ls\n",
            saleId, productName, amount, saleDate);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}



void updateSalesData(SQLHDBC hDbc, int saleId, const char* productName, double amount, const char* saleDate) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    char sqlQuery[512];
    SQLWCHAR sqlQueryW[512];

    sprintf_s(sqlQuery, sizeof(sqlQuery),
        "UPDATE SALES SET product_name = '%s', amount = %f, sale_date = TO_DATE('%s', 'YYYY-MM-DD') WHERE sale_id = %d",
        productName, amount, saleDate, saleId);

    MultiByteToWideChar(CP_ACP, 0, sqlQuery, -1, sqlQueryW, 512);

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);

    printf("Sales data updated successfully.\n");

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}


void deleteSalesData(SQLHDBC hDbc, int saleId) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    char sqlQuery[256];
    SQLWCHAR sqlQueryW[256];

    sprintf_s(sqlQuery, sizeof(sqlQuery), "DELETE FROM SALES WHERE sale_id = %d", saleId);

    MultiByteToWideChar(CP_ACP, 0, sqlQuery, -1, sqlQueryW, 256);

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    checkError(retcode, hStmt, SQL_HANDLE_STMT);

    printf("Sales data deleted successfully.\n");

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}


void calculateMonthlySales(SQLHDBC hDbc) {
    SQLHSTMT hStmt;
    SQLRETURN retcode;
    SQLWCHAR sqlQueryW[512];
    SQLDOUBLE totalSales = 0.0;
    int month, year;

    printf("Enter the month (1-12): ");
    scanf_s("%d", &month);
    printf("Enter the year (e.g., 2024): ");
    scanf_s("%d", &year);

    swprintf(sqlQueryW, sizeof(sqlQueryW) / sizeof(SQLWCHAR),
        L"SELECT SUM(amount) FROM SALES WHERE EXTRACT(MONTH FROM sale_date) = %d AND EXTRACT(YEAR FROM sale_date) = %d",
        month, year);

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    if (retcode != SQL_SUCCESS) {
        printf("Failed to allocate statement handle.\n");
        return;
    }

    retcode = SQLExecDirect(hStmt, sqlQueryW, SQL_NTS);
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        SQLFetch(hStmt);
        SQLGetData(hStmt, 1, SQL_C_DOUBLE, &totalSales, 0, NULL);
        printf("Total Sales for %02d/%d: %.2f\n", month, year, totalSales);
    }
    else {
        printf("Failed to execute query.\n");
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

