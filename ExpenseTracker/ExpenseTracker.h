#pragma once

#include "resource.h"

// Define directives for buffer limits
#define MAX_NUMERICAL_LEN 50
#define MAX_BUFFER_LEN 256

// Message crackers
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);
void OnWindowShow(HWND hwnd, WPARAM wParam, LPARAM lParam);
LRESULT OnSize(HWND hwnd, WPARAM wParam, LPARAM lLow, LPARAM lHigh);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
LRESULT OnGetMinMax(HWND hwnd, LPMINMAXINFO lParam);

// Global graph size constraint
int maxGraphLen = 465;
int yCoordinate = 540;

// Static counters
static int currentIndex = 0;
static int totalAmount = 0;
static int totalFood = 0;
static int totalTransport = 0;
static int totalCommodities = 0;
static int totalVehicles = 0;
static int totalEssentials = 0;
static int totalServices = 0;

// Define directives for combobox handling
#define CAT_FOOD 0
#define CAT_TRANSPORT 1
#define CAT_COMMODITIES 2
#define CAT_VEHICLES 3
#define CAT_ESSENTIALS 4
#define CAT_SERVICES 5

// Combobox selected index
int selectedItem;

// Input data buffers
WCHAR* bufferContent;
WCHAR* bufferAmount;
WCHAR* bufferCategory;
int lenContent;
int lenAmount;
WCHAR currentDirectory[MAX_BUFFER_LEN];
WCHAR configDirectory[MAX_BUFFER_LEN];

// Output data buffer
WCHAR* bufferTotal;
int lenTotal;

// UI cosmetics
HWND mainTitle;
HWND sectionHeading1;
HWND sectionHeading2;
HWND sectionHeading3;

// UI controls
HWND cmbType;
HWND lblType;
HWND txtContent;
HWND lblContent;
HWND txtAmount;
HWND lblAmount;
HWND btnAdd;
HWND lstExpenses;
HWND txtTotal;
HWND lblTotal;

// Graph controls
HWND grphFood;
HWND grphTransport;
HWND grphCommodities;
HWND grphVehicles;
HWND grphEssentials;
HWND grphServices;
HWND ttipFood;
HWND ttipTransport;
HWND ttipCommodities;
HWND ttipVehicles;
HWND ttipEssentials;
HWND ttipServices;

// Functions related to graph drawing
void drawGraph(HWND hwnd, const int nTotal, const int nFood, const int nTransport,
	const int nCommodities, const int nVehicles,
	const int nEssentials, const int nServices);
HWND CreateToolTip(HWND hwndTool, PTSTR pszText);