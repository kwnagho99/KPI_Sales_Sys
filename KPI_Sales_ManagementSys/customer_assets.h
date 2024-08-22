#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

#ifndef _countof
#define _countof(array) (sizeof(array) / sizeof(array[0]))
#endif

// �� �ڻ� ������ ���� �Լ���
void insertCustomerAssetData(SQLHDBC hDbc);
void selectCustomerAssetData(SQLHDBC hDbc);
void updateCustomerAssetData(SQLHDBC hDbc, int assetId, const char* customerName, double assetValue, const char* assetType, const char* acquisitionDate);
void deleteCustomerAssetData(SQLHDBC hDbc, int assetId);
void filterCustomerAssets(SQLHDBC hDbc);

