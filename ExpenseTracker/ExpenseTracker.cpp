// This software makes use of some icons from the Farm-Fresh Icon Set from FatCow Web Hosting,
// licensed under the Creative Commons Attribution 3.0 License:
// http://www.fatcow.com/free-icons
// © Copyright 2016 FatCow Web Hosting.
// ExpenseTracker.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ExpenseTracker.h"
#include <windowsX.h>
#include <winuser.h>
#include <commctrl.h>
#include <stdio.h>

#pragma comment(lib, "ComCtl32.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
bool isNotif = 0;
int interval = 0;
int sizeI;
WCHAR* bufferI = NULL;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_EXPENSETRACKER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EXPENSETRACKER));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EXPENSETRACKER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_EXPENSETRACKER));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable
	const int ScreenX = (GetSystemMetrics(SM_CXSCREEN) - 530) / 2;
	const int ScreenY = (GetSystemMetrics(SM_CYSCREEN) - 620) / 2;
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		ScreenX, ScreenY, 530, 620, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	GetCurrentDirectory(MAX_BUFFER_LEN, currentDirectory);
	wsprintf(configDirectory, L"%s\\data.ini", currentDirectory);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_SHOWWINDOW, OnWindowShow);
		HANDLE_MSG(hWnd, WM_SIZE, OnSize);
		HANDLE_MSG(hWnd, WM_GETMINMAXINFO, OnGetMinMax);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	// Some magic to apply visual style that I don't understand yet
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

	// Use system font
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	// Spawn controls
	mainTitle = CreateWindowEx(0, L"STATIC", L"",
		WS_CHILD | WS_VISIBLE | SS_BITMAP, 0, 0, 1920, 51,
		hWnd, NULL, hInst, NULL);
	HANDLE hImage = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP7),
		IMAGE_BITMAP, 1920, 51, LR_DEFAULTCOLOR);
	SendMessage(mainTitle, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);
	sectionHeading1 = CreateWindowEx(0, L"STATIC", L"",
		WS_CHILD | WS_VISIBLE | SS_BITMAP, 15, 60, 500, 30,
		hWnd, NULL, hInst, NULL);
	hImage = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP8),
		IMAGE_BITMAP, 500, 30, LR_DEFAULTCOLOR);
	SendMessage(sectionHeading1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);
	sectionHeading2 = CreateWindowEx(0, L"STATIC", L"",
		WS_CHILD | WS_VISIBLE | SS_BITMAP, 15, 165, 500, 30,
		hWnd, NULL, hInst, NULL);
	hImage = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP9),
		IMAGE_BITMAP, 500, 30, LR_DEFAULTCOLOR);
	SendMessage(sectionHeading2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);
	sectionHeading3 = CreateWindowEx(0, L"STATIC", L"",
		WS_CHILD | WS_VISIBLE | SS_BITMAP, 15, 465, 500, 30,
		hWnd, NULL, hInst, NULL);
	hImage = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP10),
		IMAGE_BITMAP, 500, 30, LR_DEFAULTCOLOR);
	SendMessage(sectionHeading3, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);
	lblType = CreateWindowEx(0, L"STATIC", L"Expense category:", WS_CHILD | WS_VISIBLE, 25, 100, 120, 25, hWnd, NULL, hInst, NULL);
	cmbType = CreateWindowEx(WS_EX_CLIENTEDGE, L"COMBOBOX", L"Essentials", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 25, 130, 120, 25, hWnd, NULL, hInst, NULL);
	SendMessage(cmbType, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(cmbType, CB_ADDSTRING, 0, (LPARAM)L"Food");
	SendMessage(cmbType, CB_ADDSTRING, 0, (LPARAM)L"Transportation");
	SendMessage(cmbType, CB_ADDSTRING, 0, (LPARAM)L"Commodities");
	SendMessage(cmbType, CB_ADDSTRING, 0, (LPARAM)L"Personal vehicles");
	SendMessage(cmbType, CB_ADDSTRING, 0, (LPARAM)L"Necessities");
	SendMessage(cmbType, CB_ADDSTRING, 0, (LPARAM)L"Services");
	txtContent = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 155, 130, 120, 25, hWnd, NULL, hInst, NULL);
	SendMessage(txtContent, WM_SETFONT, WPARAM(hFont), TRUE);
	lblContent = CreateWindowEx(0, L"STATIC", L"Expense description:", WS_CHILD | WS_VISIBLE, 155, 100, 120, 25, hWnd, NULL, hInst, NULL);
	txtAmount = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 285, 130, 120, 25, hWnd, NULL, hInst, NULL);
	SendMessage(txtAmount, WM_SETFONT, WPARAM(hFont), TRUE);
	lblAmount = CreateWindowEx(0, L"STATIC", L"Amount of money spent:", WS_CHILD | WS_VISIBLE, 285, 100, 120, 25, hWnd, NULL, hInst, NULL);
	SendMessage(lblAmount, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(lblContent, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(lblType, WM_SETFONT, WPARAM(hFont), TRUE);
	btnAdd = CreateWindowEx(0, L"BUTTON", L"Add", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON, 415, 130, 75, 25, hWnd, (HMENU)IDC_BTNADD, hInst, NULL);
	SendMessage(btnAdd, WM_SETFONT, WPARAM(hFont), TRUE);
	lstExpenses = CreateWindowEx(0, WC_LISTVIEW, _T("List View"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_TABSTOP,
		25, 205, 465, 250, hWnd, (HMENU)IDC_LISTVIEW, hInst, NULL);
	LVCOLUMN listColumn;
	listColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	listColumn.fmt = LVCFMT_LEFT;
	listColumn.cx = 130;
	listColumn.pszText = _T("Category");
	ListView_InsertColumn(lstExpenses, 0, &listColumn);
	listColumn.fmt = LVCFMT_LEFT;
	listColumn.cx = 200;
	listColumn.pszText = _T("Description");
	ListView_InsertColumn(lstExpenses, 1, &listColumn);
	listColumn.fmt = LVCFMT_RIGHT;
	listColumn.cx = 120;
	listColumn.pszText = _T("Amount");
	ListView_InsertColumn(lstExpenses, 2, &listColumn);
	SetWindowLong(lstExpenses, GWL_STYLE, GetWindowLong(lstExpenses, GWL_STYLE) | LVS_REPORT);
	txtTotal = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_READONLY | 
		ES_AUTOHSCROLL | ES_CENTER, 160, 505, 120, 25, hWnd, NULL, hInst, NULL);
	lblTotal = CreateWindowEx(0, L"STATIC", L"Total:", WS_CHILD | WS_VISIBLE | SS_CENTER, 
		125, 505, 50, 25, hWnd, NULL, hInst, NULL);
	SendMessage(txtTotal, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(lblTotal, WM_SETFONT, WPARAM(hFont), TRUE);
	return true;
}

void OnWindowShow(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	WCHAR* bufferSelected = new WCHAR[2];
	WCHAR* bufferItem = new WCHAR[MAX_BUFFER_LEN];
	WCHAR* bufferSubItem = new WCHAR[MAX_BUFFER_LEN];
	WCHAR* bufferNumeric = new WCHAR[MAX_NUMERICAL_LEN];
	int nTemp = 0;
	GetPrivateProfileString(L"general", L"selected", L"0", bufferSelected, 2, configDirectory);
	nTemp = _wtoi(bufferSelected);
	ComboBox_SetCurSel(cmbType, nTemp);
	GetPrivateProfileString(L"general", L"count", L"0", bufferSelected, 2, configDirectory);
	nTemp = _wtoi(bufferSelected);
	currentIndex = nTemp;
	int i;
	for (i = 0; i < nTemp; i++) {
		wsprintf(bufferItem, L"item%d", i);
		LV_ITEM listviewItem;
		listviewItem.mask = LVIF_TEXT | LVIF_PARAM;
		listviewItem.iItem = i;
		listviewItem.iSubItem = 0;
		GetPrivateProfileString(bufferItem, L"type", L"6", bufferSubItem, MAX_BUFFER_LEN, configDirectory);
		GetPrivateProfileString(bufferItem, L"amount", L"0", bufferNumeric, MAX_BUFFER_LEN, configDirectory);
		int nType = _wtoi(bufferSubItem);
		int nIncrease = _wtoi(bufferNumeric);
		switch (nType) {
		case CAT_FOOD:
			wsprintf(bufferSubItem, L"Food");
			totalFood += nIncrease;
			break;
		case CAT_TRANSPORT:
			wsprintf(bufferSubItem, L"Transportation");
			totalTransport += nIncrease;
			break;
		case CAT_COMMODITIES:
			wsprintf(bufferSubItem, L"Commodities");
			totalCommodities += nIncrease;
			break;
		case CAT_VEHICLES:
			wsprintf(bufferSubItem, L"Personal Vehicles");
			totalVehicles += nIncrease;
			break;
		case CAT_ESSENTIALS:
			wsprintf(bufferSubItem, L"Necessities");
			totalEssentials += nIncrease;
			break;
		case CAT_SERVICES:
			wsprintf(bufferSubItem, L"Services");
			totalServices += nIncrease;
			break;
		default:
			wsprintf(bufferSubItem, L"Error reading data");
			break;
		}
		listviewItem.pszText = bufferSubItem;
		listviewItem.lParam = i;
		ListView_InsertItem(lstExpenses, &listviewItem);
		listviewItem.mask = LVIF_TEXT;
		listviewItem.iSubItem = 1;
		GetPrivateProfileString(bufferItem, L"description", L"Error reading data", bufferSubItem, MAX_BUFFER_LEN, configDirectory);
		listviewItem.pszText = bufferSubItem;
		ListView_SetItem(lstExpenses, &listviewItem);
		listviewItem.iSubItem = 2;
		listviewItem.pszText = bufferNumeric;
		ListView_SetItem(lstExpenses, &listviewItem);
	}
	GetPrivateProfileString(L"general", L"total", L"0", bufferSubItem, MAX_BUFFER_LEN, configDirectory);
	SetWindowText(txtTotal, bufferSubItem);
	totalAmount += _wtoi(bufferSubItem);
	drawGraph(hwnd, totalAmount, totalFood, totalTransport, totalCommodities,
		totalVehicles, totalEssentials, totalServices);
	delete[] bufferSelected;
	delete[] bufferItem;
	delete[] bufferSubItem;
}

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify) {
	switch (id) {
	case IDC_BTNADD:
		bool isValid = true;
		lenContent = GetWindowTextLength(txtContent);
		lenAmount = GetWindowTextLength(txtAmount);
		bufferContent = new WCHAR[lenContent + 1];
		bufferAmount = new WCHAR[lenAmount + 1];
		bufferTotal = new WCHAR[MAX_NUMERICAL_LEN];
		bufferCategory = new WCHAR[20];
		GetWindowText(txtContent, bufferContent, lenContent + 1);
		GetWindowText(txtAmount, bufferAmount, lenAmount + 1);
		int i = 0;
		for (i; i < lenAmount; i++) {
			if ((bufferAmount[i] < 48) || (bufferAmount[i] > 57)) isValid = false;
		}
		if (bufferContent[0] == '\0' || bufferAmount[0] == '\0') isValid = false;
		if (isValid == false) {
			MessageBox(hWnd, L"You need to enter a numerical amount\ngreater than 0 in the Amount field, and\n"
				"cannot leave the Content field blank!", L"Error!", MB_OK | MB_ICONWARNING);
		}
		else {
			totalAmount += _wtoi(bufferAmount);
			selectedItem = ComboBox_GetCurSel(cmbType);
			WCHAR* bufferSelected = new WCHAR[2];
			wsprintf(bufferSelected, L"%d", selectedItem);
			WritePrivateProfileString(L"general", L"selected", bufferSelected, configDirectory);
			switch (selectedItem) {
			case CAT_FOOD:
				wsprintf(bufferCategory, L"Food");
				totalFood += _wtoi(bufferAmount);
				break;
			case CAT_TRANSPORT:
				wsprintf(bufferCategory, L"Transportation");
				totalTransport += _wtoi(bufferAmount);
				break;
			case CAT_COMMODITIES:
				wsprintf(bufferCategory, L"Commodities");
				totalCommodities += _wtoi(bufferAmount);
				break;
			case CAT_VEHICLES:
				wsprintf(bufferCategory, L"Personal Vehicles");
				totalVehicles += _wtoi(bufferAmount);
				break;
			case CAT_ESSENTIALS:
				wsprintf(bufferCategory, L"Necessities");
				totalEssentials += _wtoi(bufferAmount);
				break;
			case CAT_SERVICES:
				wsprintf(bufferCategory, L"Services");
				totalServices += _wtoi(bufferAmount);
				break;
			default:
				break;
			}
			WCHAR* bufferIndex = new WCHAR[MAX_BUFFER_LEN];
			wsprintf(bufferIndex, L"item%d", currentIndex);
			WritePrivateProfileString(bufferIndex, L"type", bufferSelected, configDirectory);
			WritePrivateProfileString(bufferIndex, L"description", bufferContent, configDirectory);
			WritePrivateProfileString(bufferIndex, L"amount", bufferAmount, configDirectory);
			LV_ITEM listviewItem;
			listviewItem.mask = LVIF_TEXT | LVIF_PARAM;
			listviewItem.iItem = currentIndex;
			listviewItem.iSubItem = 0;
			listviewItem.pszText = bufferCategory;
			listviewItem.lParam = currentIndex;
			ListView_InsertItem(lstExpenses, &listviewItem);
			listviewItem.mask = LVIF_TEXT;
			listviewItem.iSubItem = 1;
			listviewItem.pszText = bufferContent;
			ListView_SetItem(lstExpenses, &listviewItem);
			listviewItem.iSubItem = 2;
			listviewItem.pszText = bufferAmount;
			ListView_SetItem(lstExpenses, &listviewItem);
			currentIndex++;
			wsprintf(bufferTotal, L"%d", totalAmount);
			SetWindowText(txtTotal, bufferTotal);
			drawGraph(hWnd, totalAmount, totalFood, totalTransport, totalCommodities,
					totalVehicles, totalEssentials, totalServices);
			wsprintf(bufferIndex, L"%d", currentIndex);
			WritePrivateProfileString(L"general", L"count", bufferIndex, configDirectory);
			WritePrivateProfileString(L"general", L"total", bufferTotal, configDirectory);
			delete[] bufferSelected;
			delete[] bufferIndex;
		}
		delete[] bufferCategory;
		delete[] bufferAmount;
		delete[] bufferContent;
		delete[] bufferTotal;
		break;
	}
}

void drawGraph(HWND hwnd, const int nTotal, const int nFood, const int nTransport,
	const int nCommodities, const int nVehicles,
	const int nEssentials, const int nServices) {
	double blockSize = maxGraphLen;
	int xCoordinate = 25;
	double percentage;
	WCHAR* bufferTooltip = new WCHAR[MAX_BUFFER_LEN];
	HBITMAP hImage;
	if (nFood > 0) {
		percentage = ((double)nFood / (double)nTotal) * 100.0;
		blockSize = (maxGraphLen / 100.0) * percentage;
		if (grphFood == NULL) {
			grphFood = CreateWindowEx(0, L"STATIC", L"",
				WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY, xCoordinate, yCoordinate,
				blockSize, 30, hwnd, (HMENU)IDC_GRPHFOOD, hInst, NULL);
			wsprintf(bufferTooltip, L"Food: %d (%d%c)", totalFood, (int)((percentage > 100) ? (percentage) : (percentage + 0.5)), 37);
			ttipFood = CreateToolTip(grphFood, bufferTooltip);
		}
		else {
			SetWindowPos(grphFood, NULL, xCoordinate, yCoordinate, blockSize, 30, SWP_NOZORDER);
			DestroyWindow(ttipFood);
			wsprintf(bufferTooltip, L"Food: %d (%d%c)", totalFood, (int)((percentage > 100) ? (percentage) : (percentage + 0.5)), 37);
			ttipFood = CreateToolTip(grphFood, bufferTooltip);
		}
		HANDLE hImage = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP3),
			IMAGE_BITMAP, (int)((blockSize >= maxGraphLen) ? (blockSize) : (blockSize + 0.5)), 30, LR_DEFAULTCOLOR);
		SendMessage(grphFood, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hImage);
		xCoordinate += (int)((blockSize >= maxGraphLen) ? (blockSize) : (blockSize + 0.5));
	}
	if (nTransport > 0) {
		percentage = ((double)nTransport / (double)nTotal) * 100.0;
		blockSize = (maxGraphLen / 100.0) * percentage;
		if (grphTransport == NULL) {
			grphTransport = CreateWindowEx(0, L"STATIC", L"",
				WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY, xCoordinate, yCoordinate, blockSize, 30,
				hwnd, NULL, hInst, NULL);
			wsprintf(bufferTooltip, L"Transport: %d (%d%c)", totalTransport, (int)((percentage > 100) ? (percentage) : (percentage + 0.5)), 37);
			ttipTransport = CreateToolTip(grphTransport, bufferTooltip);
		}
		else {
			SetWindowPos(grphTransport, NULL, xCoordinate, yCoordinate, blockSize, 30, SWP_NOZORDER);
			DestroyWindow(ttipTransport);
			wsprintf(bufferTooltip, L"Transport: %d (%d%c)", totalTransport, (int)((percentage > 100) ? (percentage) : (percentage + 0.5)), 37);
			ttipTransport = CreateToolTip(grphTransport, bufferTooltip);
		}
		HANDLE hImage = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP5),
			IMAGE_BITMAP, (int)((blockSize >= maxGraphLen) ? (blockSize) : (blockSize + 0.5)), 30, LR_DEFAULTCOLOR);
		SendMessage(grphTransport, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hImage);
		xCoordinate += (int)((blockSize >= maxGraphLen) ? (blockSize) : (blockSize + 0.5));
	}
	if (nCommodities) {
		percentage = ((double)nCommodities / (double)nTotal) * 100.0;
		blockSize = (maxGraphLen / 100.0) * percentage;
		if (grphCommodities == NULL) {
			grphCommodities = CreateWindowEx(0, L"STATIC", L"",
				WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY, xCoordinate, yCoordinate, blockSize, 30,
				hwnd, NULL, hInst, NULL);
			wsprintf(bufferTooltip, L"Commodities: %d (%d%c)", totalCommodities, (int)((percentage > 100) ? (percentage) : (percentage + 0.5)), 37);
			ttipCommodities = CreateToolTip(grphCommodities, bufferTooltip);
		}
		else {
			SetWindowPos(grphCommodities, NULL, xCoordinate, yCoordinate, blockSize, 30, SWP_NOZORDER);
			DestroyWindow(ttipCommodities);
			wsprintf(bufferTooltip, L"Commodities: %d (%d%c)", totalCommodities, (int)((percentage > 100) ? (percentage) : (percentage + 0.5)), 37);
			ttipCommodities = CreateToolTip(grphCommodities, bufferTooltip);
		}
		HANDLE hImage = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP1),
			IMAGE_BITMAP, (int)((blockSize >= maxGraphLen) ? (blockSize) : (blockSize + 0.5)), 30, LR_DEFAULTCOLOR);
		SendMessage(grphCommodities, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);
		xCoordinate += (int)((blockSize >= maxGraphLen) ? (blockSize) : (blockSize + 0.5));
	}
	if (nVehicles) {
		percentage = ((double)nVehicles / (double)nTotal) * 100.0;
		blockSize = (maxGraphLen / 100.0) * percentage;
		if (grphVehicles == NULL) {
			grphVehicles = CreateWindowEx(0, L"STATIC", L"",
				WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY, xCoordinate, yCoordinate, blockSize, 30,
				hwnd, NULL, hInst, NULL);
			wsprintf(bufferTooltip, L"Personal Vehicles: %d (%d%c)", totalVehicles, (int)((percentage > 100) ? (percentage) : (percentage + 0.5)), 37);
			ttipVehicles = CreateToolTip(grphVehicles, bufferTooltip);
		}
		else {
			SetWindowPos(grphVehicles, NULL, xCoordinate, yCoordinate, blockSize, 30, SWP_NOZORDER);
			DestroyWindow(ttipVehicles);
			wsprintf(bufferTooltip, L"Personal Vehicles: %d (%d%c)", totalVehicles, (int)((percentage > 100) ? (percentage) : (percentage + 0.5)), 37);
			ttipVehicles = CreateToolTip(grphVehicles, bufferTooltip);
		}
		HANDLE hImage = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP6),
			IMAGE_BITMAP, (int)((blockSize >= maxGraphLen) ? (blockSize) : (blockSize + 0.5)), 30, LR_DEFAULTCOLOR);
		SendMessage(grphVehicles, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);
		xCoordinate += (int)((blockSize >= maxGraphLen) ? (blockSize) : (blockSize + 0.5));
	}
	if (nEssentials > 0) {
		percentage = ((double)nEssentials / (double)nTotal) * 100.0;
		blockSize = (maxGraphLen / 100.0) * percentage;
		if (grphEssentials == NULL) {
			grphEssentials = CreateWindowEx(0, L"STATIC", L"",
				WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY, xCoordinate, yCoordinate, blockSize, 30,
				hwnd, NULL, hInst, NULL);
			wsprintf(bufferTooltip, L"Necessities: %d (%d%c)", totalEssentials, (int)((percentage > 100) ? (percentage) : (percentage + 0.5)), 37);
			ttipEssentials = CreateToolTip(grphEssentials, bufferTooltip);
		}
		else {
			SetWindowPos(grphEssentials, NULL, xCoordinate, yCoordinate, blockSize, 30, SWP_NOZORDER);
			DestroyWindow(ttipEssentials);
			wsprintf(bufferTooltip, L"Necessities: %d (%d%c)", totalEssentials, (int)((percentage > 100) ? (percentage) : (percentage + 0.5)), 37);
			ttipEssentials = CreateToolTip(grphEssentials, bufferTooltip);
		}
		HANDLE hImage = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP2),
			IMAGE_BITMAP, (int)((blockSize >= maxGraphLen) ? (blockSize) : (blockSize + 0.5)), 30, LR_DEFAULTCOLOR);
		SendMessage(grphEssentials, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);
		xCoordinate += (int)((blockSize >= maxGraphLen) ? (blockSize) : (blockSize + 0.5));
	}
	if (nServices > 0) {
		percentage = ((double)nServices / (double)nTotal) * 100.0;
		blockSize = (maxGraphLen / 100.0) * percentage;
		if (grphServices == NULL) {
			grphServices = CreateWindowEx(0, L"STATIC", L"",
				WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY, xCoordinate, yCoordinate, blockSize, 30,
				hwnd, NULL, hInst, NULL);
			wsprintf(bufferTooltip, L"Services: %d (%d%c)", totalServices, (int)((percentage > 100) ? (percentage) : (percentage + 0.5)), 37);
			ttipServices = CreateToolTip(grphServices, bufferTooltip);
		}
		else {
			SetWindowPos(grphServices, NULL, xCoordinate, yCoordinate, blockSize, 30, SWP_NOZORDER);
			DestroyWindow(ttipServices);
			wsprintf(bufferTooltip, L"Services: %d (%d%c)", totalServices, (int)((percentage > 100) ? (percentage) : (percentage + 0.5)), 37);
			ttipServices = CreateToolTip(grphServices, bufferTooltip);
		}
		HANDLE hImage = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP4),
			IMAGE_BITMAP, (int)((blockSize >= maxGraphLen) ? (blockSize) : (blockSize + 0.5)), 30, LR_DEFAULTCOLOR);
		SendMessage(grphServices, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);
	}
	InvalidateRect(hwnd, nullptr, false);
}

LRESULT OnSize(HWND hwnd, WPARAM wParam, LPARAM lLow, LPARAM lHigh) {
	RECT rcClient;
	GetClientRect(hwnd, &rcClient);
	SetWindowPos(txtAmount, NULL, rcClient.right - 230, 130, 120, 25, SWP_NOZORDER);
	SetWindowPos(lblAmount, NULL, rcClient.right - 230, 100, 120, 25, SWP_NOZORDER);
	SetWindowPos(btnAdd, NULL, rcClient.right - 100, 130, 75, 25, SWP_NOZORDER);
	SetWindowPos(txtContent, NULL, 155, 130, (rcClient.right - 240) - 155, 25, SWP_NOZORDER);
	SetWindowPos(lblContent, NULL, 155, 100, (rcClient.right - 240) - 155, 25, SWP_NOZORDER);
	SetWindowPos(lstExpenses, NULL, 25, 205, rcClient.right - 50,
		rcClient.bottom - 335, SWP_NOZORDER);
	SetWindowPos(sectionHeading3, NULL, 15, rcClient.bottom - 120, 500, 30, SWP_NOZORDER);
	SetWindowPos(txtTotal, NULL, ((rcClient.right - rcClient.left) - 120) / 2 + 35, rcClient.bottom - 85, 
		120, 30, SWP_NOZORDER);
	SetWindowPos(lblTotal, NULL, ((rcClient.right - rcClient.left) - 120) / 2, rcClient.bottom - 80,
		30, 25, SWP_NOZORDER);
	ListView_SetColumnWidth(lstExpenses, 1, ((rcClient.right - rcClient.left) - 300));
	ListView_SetColumnWidth(lstExpenses, 2, 120);
	maxGraphLen = (rcClient.right - rcClient.left) - 50;
	yCoordinate = rcClient.bottom - 45;
	drawGraph(hwnd, totalAmount, totalFood, totalTransport, totalCommodities,
		totalVehicles, totalEssentials, totalServices);
	return 0;
}

LRESULT OnGetMinMax(HWND hwnd, LPMINMAXINFO lParam) {
	// The graph glitches out at any width lower than 530 for some reson
	// I'll have to cheese through it with this until I find out the exact cause
	MINMAXINFO* mmi = (MINMAXINFO*)lParam;
	mmi->ptMinTrackSize.x = 530;
	mmi->ptMinTrackSize.y = 450;
	mmi->ptMaxTrackSize.x = 1920;
	mmi->ptMaxTrackSize.y = 1080;
	return 0;
}

HWND CreateToolTip(HWND hwndTool, PTSTR pszText) {
	if (!hwndTool || !pszText)
	{
		return FALSE;
	}

	HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
		TTS_ALWAYSTIP | WS_POPUP,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hwndTool, NULL,
		hInst, NULL);
	if (!hwndTool || !hwndTip)
	{
		return (HWND)NULL;
	}

	TOOLINFO toolInfo = { 0 };
	toolInfo.cbSize = sizeof(TOOLINFO);
	toolInfo.hwnd = GetParent(hwndTool);
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.uId = (UINT_PTR)hwndTool;
	toolInfo.lpszText = pszText;
	SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&toolInfo);
	SendMessage(hwndTip, TTM_ACTIVATE, TRUE, 0);
	return hwndTip;
}

void OnPaint(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);
	EndPaint(hWnd, &ps);
}

void OnDestroy(HWND hwnd) {
	PostQuitMessage(0);
}