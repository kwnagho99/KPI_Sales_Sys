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
    printf("---------------------------------\n");
    printf("Welcome to the Business Management System\n");
    printf("---------------------------------\n");
    printf("Please choose an option:\n");
    printf("1. �������� ������ �Է�\n");
    printf("2. �������� ������ ��ȸ\n");
    printf("3. �������� ������ ����\n");
    printf("4. �������� ������ ����\n");
    printf("5. KPI ������ �Է�\n");
    printf("6. KPI ������ ��ȸ\n");
    printf("7. KPI ������ ����\n");
    printf("8. KPI ������ ����\n");
    printf("9. CRM ������ �Է�\n");
    printf("10. CRM ������ ��ȸ\n");
    printf("11. CRM ������ ����\n");
    printf("12. CRM ������ ����\n");
    printf("13. ���� �������� ���\n");
    printf("14. CRM ���͸�\n");
    printf("15. KPI ���� ����\n");
    printf("16. ����\n");
    printf("---------------------------------\n");
    printf("Enter your choice: ");
}

int main() {
    SQLHENV hEnv = NULL;
    SQLHDBC hDbc = NULL;
    SQLRETURN retcode;

    // ODBC ȯ�� �ʱ�ȭ
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    checkError(retcode, hEnv, SQL_HANDLE_ENV);

    retcode = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    checkError(retcode, hEnv, SQL_HANDLE_ENV);

    retcode = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    // �����ͺ��̽� ����
    retcode = SQLConnect(hDbc, (SQLWCHAR*)L"XE", SQL_NTS,
        (SQLWCHAR*)L"c##kwangho", SQL_NTS, (SQLWCHAR*)L"0219", SQL_NTS);
    checkError(retcode, hDbc, SQL_HANDLE_DBC);

    int choice;
    do {
        displayMenu();
        scanf_s("%d", &choice);
        getchar(); // �޴� ���� �� �Է� ���� ����

        printf("---------------------------------\n");

        switch (choice) {
        case 1:
            // �������� ������ �Է� �� ��ȸ
            insertSalesData(hDbc);
            selectSalesData(hDbc);
            break;
        case 2:
            // �������� ������ ��ȸ
            selectSalesData(hDbc);
            break;
        case 3:
        {
            // �������� ������ ����
            int performanceId;
            char salesRepName[50];
            double salesAmount;
            int clientsAcquired;
            char reportingDate[20];

            printf("Enter Sales Performance ID to update: ");
            scanf_s("%d", &performanceId);
            printf("Enter new Sales Rep Name: ");
            scanf_s("%s", salesRepName, (unsigned int)sizeof(salesRepName));
            printf("Enter new Sales Amount: ");
            scanf_s("%lf", &salesAmount);
            printf("Enter new Clients Acquired: ");
            scanf_s("%d", &clientsAcquired);
            printf("Enter new Reporting Date (YYYY-MM-DD): ");
            scanf_s("%s", reportingDate, (unsigned int)sizeof(reportingDate));

            updateSalesData(hDbc, performanceId, salesRepName, salesAmount, clientsAcquired, reportingDate);
            break;
        }
        case 4:
        {
            int performanceId;
            // �������� ������ ����
            printf("Enter Sales Performance ID to delete: ");
            scanf_s("%d", &performanceId);
            deleteSalesData(hDbc, performanceId);
            break;
        }
        case 5:
            // KPI ������ �Է� �� ��ȸ
            insertKPIData(hDbc);
            selectKPIData(hDbc);
            break;
        case 6:
            // KPI ������ ��ȸ
            selectKPIData(hDbc);
            break;
        case 7: {
            // KPI ������ ����
            int kpiId;
            char kpiName[50];
            double kpiValue, targetValue;
            char measurementDate[20];

            printf("Enter KPI ID to update: ");
            scanf_s("%d", &kpiId);
            printf("Enter new KPI Name: ");
            scanf_s("%s", kpiName, (unsigned int)sizeof(kpiName));
            printf("Enter new KPI Value: ");
            scanf_s("%lf", &kpiValue);
            printf("Enter new Target Value: ");
            scanf_s("%lf", &targetValue);
            printf("Enter new Measurement Date (YYYY-MM-DD): ");
            scanf_s("%s", measurementDate, (unsigned int)sizeof(measurementDate));

            updateKPIData(hDbc, kpiId, kpiName, kpiValue, targetValue, measurementDate);
            break;
        }
        case 8: {
            // KPI ������ ����
            int kpiId;
            printf("Enter KPI ID to delete: ");
            scanf_s("%d", &kpiId);
            deleteKPIData(hDbc, kpiId);
            break;
        }

        case 9:
            // CRM ������ �Է� �� ��ȸ
            insertCustomerAssetData(hDbc);
            selectCustomerAssetData(hDbc);
            break;
        case 10:
            // CRM ������ ��ȸ
            selectCustomerAssetData(hDbc);
            break;
        case 11:
        {
            // CRM ������ ����
            int assetId;
            char customerName[50];
            double assetValue;
            char assetType[50];
            char acquisitionDate[20];

            printf("Enter Asset ID to update: ");
            scanf_s("%d", &assetId);
            printf("Enter new Customer Name: ");
            scanf_s("%s", customerName, (unsigned int)sizeof(customerName));
            printf("Enter new Asset Value: ");
            scanf_s("%lf", &assetValue);
            printf("Enter new Asset Type: ");
            scanf_s("%s", assetType, (unsigned int)sizeof(assetType));
            printf("Enter new Acquisition Date (YYYY-MM-DD): ");
            scanf_s("%s", acquisitionDate, (unsigned int)sizeof(acquisitionDate));

            updateCustomerAssetData(hDbc, assetId, customerName, assetValue, assetType, acquisitionDate);
            break;
        }
        case 12:
        {
            int assetId;
            // CRM ������ ����
            printf("Enter Asset ID to delete: ");
            scanf_s("%d", &assetId);
            deleteCustomerAssetData(hDbc, assetId);
            break;
        }
        case 13:
            // ���� �������� ���
            calculateMonthlySales(hDbc);
            break;
        case 14:
        {
            // CRM ���͸�
            double minAssetValue;
            printf("Enter minimum asset value to filter: ");
            scanf_s("%lf", &minAssetValue);
            filterCustomerAssets(hDbc, minAssetValue);
            break;
        }
        case 15:
            // KPI ���� ����
            generateReport(hDbc);
            break;
        case 16:
            printf("���α׷��� �����մϴ�.\n");
            break;
        default:
            printf("�߸��� �����Դϴ�. �ٽ� �õ��� �ּ���.\n");
            break;
        }

        printf("---------------------------------\n");

    } while (choice != 16);

    // �����ͺ��̽� ���� ����
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    printf("�����ͺ��̽� ���� ����.\n");
    return 0;
}
