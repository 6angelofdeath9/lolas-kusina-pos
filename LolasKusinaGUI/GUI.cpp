#include "GUI.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

// Constructor
RestaurantGUI::RestaurantGUI(RestaurantCore* restaurantCore)
    : core(restaurantCore), currentCategory("Appetizers"), selectedQty(1),
    hwndMain(nullptr), hwndLogin(nullptr), hwndOrderList(nullptr),
    hwndSubtotalLabel(nullptr), hwndDiscountCombo(nullptr),
    hwndPaymentAmount(nullptr), hwndChangeAmount(nullptr),
    hFontTitle(nullptr), hFontNormal(nullptr), hFontBold(nullptr),
    hFontLarge(nullptr), hMenuFont(nullptr), hPriceFont(nullptr), hEmojiFont(nullptr),
    hLoginBg(nullptr), hMainBg(nullptr) {
}


RestaurantGUI::~RestaurantGUI() {

    if (hFontTitle) DeleteObject(hFontTitle);
    if (hFontNormal) DeleteObject(hFontNormal);
    if (hFontBold) DeleteObject(hFontBold);
    if (hFontLarge) DeleteObject(hFontLarge);
    if (hMenuFont) DeleteObject(hMenuFont);
    if (hPriceFont) DeleteObject(hPriceFont);
    if (hEmojiFont) DeleteObject(hEmojiFont);

    if (hLoginBg) DeleteObject(hLoginBg);
    if (hMainBg) DeleteObject(hMainBg);


    for (auto& pair : foodImages) {
        if (pair.second) DeleteObject(pair.second);
    }

    // Dynamic controls
    for (HWND hwnd : menuButtonHandles) {
        if (hwnd) DestroyWindow(hwnd);
    }
    for (HWND hwnd : keypadHandles) {
        if (hwnd) DestroyWindow(hwnd);
    }
    for (HWND hwnd : qtyUpHandles) {
        if (hwnd) DestroyWindow(hwnd);
    }
    for (HWND hwnd : qtyDownHandles) {
        if (hwnd) DestroyWindow(hwnd);
    }
    for (HWND hwnd : qtyDisplayHandles) {
        if (hwnd) DestroyWindow(hwnd);
    }
}

void RestaurantGUI::LoadFoodImages() {
    //Load food images
    foodImages[1] = (HBITMAP)LoadImage(NULL, L"images/lumpia.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[2] = (HBITMAP)LoadImage(NULL, L"images/calamares.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[3] = (HBITMAP)LoadImage(NULL, L"images/chicharon.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[4] = (HBITMAP)LoadImage(NULL, L"images/adobo.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[5] = (HBITMAP)LoadImage(NULL, L"images/karekare.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[6] = (HBITMAP)LoadImage(NULL, L"images/sinigang.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[7] = (HBITMAP)LoadImage(NULL, L"images/bangus.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[8] = (HBITMAP)LoadImage(NULL, L"images/bulalo.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[9] = (HBITMAP)LoadImage(NULL, L"images/sisig.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[10] = (HBITMAP)LoadImage(NULL, L"images/halohalo.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[11] = (HBITMAP)LoadImage(NULL, L"images/lecheflan.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[12] = (HBITMAP)LoadImage(NULL, L"images/bukopandan.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[13] = (HBITMAP)LoadImage(NULL, L"images/buko_juice.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[14] = (HBITMAP)LoadImage(NULL, L"images/iced_tea.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[15] = (HBITMAP)LoadImage(NULL, L"images/coke.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[16] = (HBITMAP)LoadImage(NULL, L"images/gulaman.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[17] = (HBITMAP)LoadImage(NULL, L"images/fresh_Lumpia.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[18] = (HBITMAP)LoadImage(NULL, L"images/Cheesy_Dynamite.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[19] = (HBITMAP)LoadImage(NULL, L"images/Calamansi Juice.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
    foodImages[20] = (HBITMAP)LoadImage(NULL, L"images/CremadeLeche.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);


    hLoginBg = (HBITMAP)LoadImage(NULL, L"images/login_bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hMainBg = (HBITMAP)LoadImage(NULL, L"images/main_bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void RestaurantGUI::DrawFoodImages(HDC hdc) {
    const MenuItem* menu = core->getMenu();
    int menuSize = core->getMenuSize();

    std::vector<MenuItem> categoryItems;
    for (int i = 0; i < menuSize; i++) {
        if (menu[i].category == currentCategory) {
            categoryItems.push_back(menu[i]);
        }
    }


    const int buttonWidth = 200;
    const int buttonHeight = 80;
    const int spacing = 10;
    const int columns = 5;
    const int startX = 20;
    const int startY = 120;

    for (size_t i = 0; i < categoryItems.size(); i++) {
        int row = i / columns;
        int col = i % columns;
        int x = startX + col * (buttonWidth + spacing);
        int y = startY + row * (buttonHeight + 30 + spacing);


        auto it = foodImages.find(categoryItems[i].number);
        if (it != foodImages.end() && it->second) {
            HDC hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, it->second);
            BITMAP bm;
            GetObject(it->second, sizeof(bm), &bm);

            int imgSize = 40;
            int imgX = x + (buttonWidth - imgSize) / 2;
            int imgY = y + 5;

            StretchBlt(hdc, imgX, imgY, imgSize, imgSize,
                hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
            DeleteDC(hdcMem);
        }
    }
}

bool RestaurantGUI::CreateMainWindow(HINSTANCE hInstance) {
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES | ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icex);

    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(COLOR_BACKGROUND);
    wc.lpszClassName = L"LolasKusinaWindowClass";
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        return false;
    }

    hFontTitle = CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");

    hFontNormal = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

    hFontBold = CreateFont(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

    hFontLarge = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

    hMenuFont = CreateFont(11, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

    hPriceFont = CreateFont(10, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

    hEmojiFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI Emoji");

    // Load images
    LoadFoodImages();

    CreateLoginWindow(hInstance);
    return true;
}

void RestaurantGUI::CreateLoginWindow(HINSTANCE hInstance) {
    WNDCLASSEX wcLogin = { 0 };
    wcLogin.cbSize = sizeof(WNDCLASSEX);
    wcLogin.style = CS_HREDRAW | CS_VREDRAW;
    wcLogin.lpfnWndProc = LoginProc;
    wcLogin.hInstance = hInstance;
    wcLogin.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcLogin.hbrBackground = CreateSolidBrush(COLOR_PRIMARY);
    wcLogin.lpszClassName = L"LoginWindowClass";
    wcLogin.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wcLogin);

    hwndLogin = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        L"LoginWindowClass",
        L"🌺 LOLA'S KUSINA - LOGIN 🌺",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 600,
        NULL, NULL, hInstance, this
    );

    if (hwndLogin) {
        CreateWindow(L"STATIC", L"🌺 LOLA'S KUSINA 🌺",
            WS_VISIBLE | WS_CHILD | SS_CENTER,
            75, 160, 350, 50, hwndLogin, NULL, NULL, NULL);

        CreateWindow(L"STATIC", L"MGA LUTO NI LOLA",
            WS_VISIBLE | WS_CHILD | SS_CENTER,
            75, 215, 350, 30, hwndLogin, NULL, NULL, NULL);

        CreateWindow(L"STATIC", L"Username:",
            WS_VISIBLE | WS_CHILD,
            75, 270, 350, 25, hwndLogin, NULL, NULL, NULL);

        hwndUsername = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"admin",
            WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL,
            75, 300, 350, 35, hwndLogin, (HMENU)IDC_LOGIN_USERNAME, NULL, NULL);

        CreateWindow(L"STATIC", L"Password:",
            WS_VISIBLE | WS_CHILD,
            75, 350, 350, 25, hwndLogin, NULL, NULL, NULL);

        hwndPassword = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"admin",
            WS_VISIBLE | WS_CHILD | ES_LEFT | ES_PASSWORD | ES_AUTOHSCROLL,
            75, 380, 350, 35, hwndLogin, (HMENU)IDC_LOGIN_PASSWORD, NULL, NULL);

        hwndLoginBtn = CreateWindow(L"BUTTON", L"🔐 LOGIN",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            75, 445, 350, 45, hwndLogin, (HMENU)IDC_LOGIN_BUTTON, NULL, NULL);

        SendMessage(hwndUsername, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
        SendMessage(hwndPassword, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
        SendMessage(hwndLoginBtn, WM_SETFONT, (WPARAM)hFontBold, TRUE);

        ShowWindow(hwndLogin, SW_SHOW);
        UpdateWindow(hwndLogin);
    }
}

LRESULT CALLBACK RestaurantGUI::LoginProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    RestaurantGUI* pThis = nullptr;

    if (uMsg == WM_CREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<RestaurantGUI*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
    }
    else {
        pThis = reinterpret_cast<RestaurantGUI*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis) {
        switch (uMsg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_LOGIN_BUTTON) {
                pThis->OnLogin();
            }
            return 0;
        case WM_PAINT:
            pThis->OnPaint(hwnd);
            return 0;
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        case WM_DESTROY:
            if (!pThis->hwndMain) {
                PostQuitMessage(0);
            }
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK RestaurantGUI::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    RestaurantGUI* pThis = nullptr;

    if (uMsg == WM_CREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<RestaurantGUI*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
    }
    else {
        pThis = reinterpret_cast<RestaurantGUI*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis) {
        switch (uMsg) {
        case WM_COMMAND:
            pThis->OnCommand(wParam, lParam);
            return 0;
        case WM_PAINT:
            pThis->OnPaint(hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void RestaurantGUI::OnPaint(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);


    wchar_t className[40];
    GetClassName(hwnd, className, 40);
    bool isLogin = wcscmp(className, L"LoginWindowClass") == 0;

    if (isLogin) {
        // Draw login background
        if (hLoginBg) {
            HDC hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, hLoginBg);
            BITMAP bm;
            GetObject(hLoginBg, sizeof(bm), &bm);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            StretchBlt(hdc, 0, 0, clientRect.right, clientRect.bottom,
                hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
            DeleteDC(hdcMem);
        }
    }
    else {

        if (hMainBg) {
            HDC hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, hMainBg);
            BITMAP bm;
            GetObject(hMainBg, sizeof(bm), &bm);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            StretchBlt(hdc, 0, 0, clientRect.right, clientRect.bottom,
                hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
            DeleteDC(hdcMem);
        }


        DrawFoodImages(hdc);
    }

    EndPaint(hwnd, &ps);
}

void RestaurantGUI::OnLogin() {
    wchar_t username[50];
    wchar_t password[50];

    GetWindowText(hwndUsername, username, 50);
    GetWindowText(hwndPassword, password, 50);

    std::string strUsername = std::string(username, username + wcslen(username));
    std::string strPassword = std::string(password, password + wcslen(password));

    if (core->login(strUsername, strPassword)) {
        ShowWindow(hwndLogin, SW_HIDE);

        hwndMain = CreateWindowEx(
            0,
            L"LolasKusinaWindowClass",
            L"🌺 LOLA'S KUSINA - MGA LUTO NI LOLA 🌺",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 1400, 850,
            NULL, NULL, GetModuleHandle(NULL), this
        );

        if (hwndMain) {
            CreateModernLayout(hwndMain);
            UpdateMenuDisplay();
            UpdateOrderDisplay();

            ShowWindow(hwndMain, SW_SHOW);
            UpdateWindow(hwndMain);
        }
    }
    else {
        MessageBox(hwndLogin, L"Invalid username or password!\nTry: admin/admin", L"Login Failed", MB_OK | MB_ICONERROR);
    }
}

void RestaurantGUI::OnLogout() {
    int result = MessageBox(hwndMain, L"Are you sure you want to logout?", L"Confirm Logout", MB_YESNO | MB_ICONQUESTION);
    if (result == IDYES) {
        core->logout();
        DestroyWindow(hwndMain);
        hwndMain = nullptr;

        ShowWindow(hwndLogin, SW_SHOW);
        UpdateWindow(hwndLogin);

        SetWindowText(hwndUsername, L"admin");
        SetWindowText(hwndPassword, L"admin");
    }
}

void RestaurantGUI::CreateModernLayout(HWND hwnd) {
    menuButtonHandles.clear();
    keypadHandles.clear();
    qtyUpHandles.clear();
    qtyDownHandles.clear();
    qtyDisplayHandles.clear();

    // Header
    CreateWindow(L"STATIC", L"🌺 LOLA'S KUSINA - MGA LUTO NI LOLA 🌺",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        0, 10, 1400, 40, hwnd, NULL, NULL, NULL);

    // User info and logout
    std::string currentUser = core->getCurrentUser();
    std::wstring userText = L"👤 User: " + std::wstring(currentUser.begin(), currentUser.end());
    CreateWindow(L"STATIC", userText.c_str(),
        WS_VISIBLE | WS_CHILD | SS_RIGHT,
        1100, 15, 200, 25, hwnd, NULL, NULL, NULL);

    hwndLogoutBtn = CreateWindow(L"BUTTON", L"🚪 Logout",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        1310, 15, 80, 25, hwnd, (HMENU)IDC_LOGOUT_BUTTON, NULL, NULL);

    // Category tabs
    hwndTabAppetizers = CreateWindow(L"BUTTON", L"🍤 APPETIZERS",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        20, 60, 200, 40, hwnd, (HMENU)IDC_TAB_APPETIZERS, NULL, NULL);

    hwndTabMain = CreateWindow(L"BUTTON", L"🍲 MAIN DISHES",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        230, 60, 200, 40, hwnd, (HMENU)IDC_TAB_MAIN, NULL, NULL);

    hwndTabDesserts = CreateWindow(L"BUTTON", L"🍰 DESSERTS",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        440, 60, 200, 40, hwnd, (HMENU)IDC_TAB_DESSERTS, NULL, NULL);

    hwndTabBeverages = CreateWindow(L"BUTTON", L"🥤 BEVERAGES",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        650, 60, 200, 40, hwnd, (HMENU)IDC_TAB_BEVERAGES, NULL, NULL);

    SendMessage(hwndTabAppetizers, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
    SendMessage(hwndTabMain, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
    SendMessage(hwndTabDesserts, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
    SendMessage(hwndTabBeverages, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
    SendMessage(hwndLogoutBtn, WM_SETFONT, (WPARAM)hFontNormal, TRUE);

    CreateMenuGrid(hwnd);

    // Order section
    CreateWindow(L"STATIC", L"📋 CURRENT ORDER",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        20, 500, 1040, 30, hwnd, NULL, NULL, NULL);

    hwndOrderList = CreateWindowEx(WS_EX_CLIENTEDGE, L"LISTBOX", NULL,
        WS_VISIBLE | WS_CHILD | WS_VSCROLL | LBS_NOTIFY | LBS_HASSTRINGS,
        20, 540, 1040, 180, hwnd, (HMENU)IDC_ORDER_LIST, NULL, NULL);

    CreatePaymentSection(hwnd);

    // Action buttons 
    const int actionLeft = 1080;
    const int actionBtnWidth = 280;
    const int actionBtnHeight = 40;
    const int actionSpacing = 10;

    int y = 440;

    CreateWindow(L"BUTTON", L"🗑️ REMOVE SELECTED",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        actionLeft, y, actionBtnWidth, actionBtnHeight,
        hwnd, (HMENU)IDC_BTN_REMOVE, NULL, NULL);
    y += actionBtnHeight + actionSpacing;

    CreateWindow(L"BUTTON", L"🧹 CLEAR ORDER",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        actionLeft, y, actionBtnWidth, actionBtnHeight,
        hwnd, (HMENU)IDC_BTN_CLEAR, NULL, NULL);
    y += actionBtnHeight + actionSpacing;

    CreateWindow(L"BUTTON", L"💰 CHECKOUT",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        actionLeft, y, actionBtnWidth, 50,
        hwnd, (HMENU)IDC_BTN_CHECKOUT, NULL, NULL);
    y += 50 + actionSpacing;

    // Numeric keypad 
    const int keySize = 50;
    const int keySpacing = 5;
    const int keypadCols = 3;
    const int keypadWidth = keypadCols * keySize + (keypadCols - 1) * keySpacing;
    const int keypadX = actionLeft + (actionBtnWidth - keypadWidth) / 2; // Center keypad

    CreateNumericKeypad(hwnd, keypadX, y);

    SendMessage(hwndOrderList, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
    SendMessage(hwndTabAppetizers, WM_SETFONT, (WPARAM)hFontBold, TRUE);
}

void RestaurantGUI::CreateMenuGrid(HWND hwnd) {
    for (HWND btn : menuButtonHandles) if (btn) DestroyWindow(btn);
    for (HWND btn : qtyUpHandles) if (btn) DestroyWindow(btn);
    for (HWND btn : qtyDownHandles) if (btn) DestroyWindow(btn);
    for (HWND btn : qtyDisplayHandles) if (btn) DestroyWindow(btn);

    menuButtonHandles.clear();
    qtyUpHandles.clear();
    qtyDownHandles.clear();
    qtyDisplayHandles.clear();

    const MenuItem* menu = core->getMenu();
    int menuSize = core->getMenuSize();

    std::vector<MenuItem> categoryItems;
    for (int i = 0; i < menuSize; i++) {
        if (menu[i].category == currentCategory) {
            categoryItems.push_back(menu[i]);
        }
    }

    const int buttonWidth = 200;
    const int buttonHeight = 80;
    const int qtyHeight = 30;
    const int spacing = 10;
    const int columns = 5;
    const int startX = 20;
    const int startY = 120;

    for (size_t i = 0; i < categoryItems.size(); i++) {
        int row = i / columns;
        int col = i % columns;
        int x = startX + col * (buttonWidth + spacing);
        int y = startY + row * (buttonHeight + qtyHeight + spacing);

        int controlId = IDC_MENU_ITEM_BASE + static_cast<int>(i);
        HWND menuButton = CreateWindow(L"BUTTON", NULL,
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_MULTILINE,
            x, y, buttonWidth, buttonHeight,
            hwnd, (HMENU)controlId, NULL, NULL);

        std::string emoji = GetFoodEmoji(categoryItems[i].name);
        std::wostringstream buttonText;
        buttonText << emoji.c_str() << L" "
            << std::wstring(categoryItems[i].name.begin(), categoryItems[i].name.end()).c_str() << L"\n"
            << L"₱" << std::fixed << std::setprecision(2) << categoryItems[i].price;

        SetWindowText(menuButton, buttonText.str().c_str());
        SendMessage(menuButton, WM_SETFONT, (WPARAM)hMenuFont, TRUE);

        menuButtonHandles.push_back(menuButton);
        CreateQuantityControls(hwnd, x, y + buttonHeight, static_cast<int>(i));
    }
}

void RestaurantGUI::CreateQuantityControls(HWND hwnd, int x, int y, int index) {
    const int qtyWidth = 200;
    const int btnWidth = 35;
    const int spacing = 2;

    HWND qtyDown = CreateWindow(L"BUTTON", L"➖",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        x, y, btnWidth, 25, hwnd, (HMENU)(IDC_QTY_DOWN_BASE + index), NULL, NULL);

    HWND qtyDisplay = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"1",
        WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER,
        x + btnWidth + spacing, y, qtyWidth - 2 * (btnWidth + spacing), 25,
        hwnd, (HMENU)(IDC_QTY_DISPLAY_BASE + index), NULL, NULL);

    HWND qtyUp = CreateWindow(L"BUTTON", L"➕",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        x + qtyWidth - btnWidth, y, btnWidth, 25,
        hwnd, (HMENU)(IDC_QTY_UP_BASE + index), NULL, NULL);

    SendMessage(qtyDown, WM_SETFONT, (WPARAM)hEmojiFont, TRUE);
    SendMessage(qtyDisplay, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
    SendMessage(qtyUp, WM_SETFONT, (WPARAM)hEmojiFont, TRUE);

    qtyDownHandles.push_back(qtyDown);
    qtyDisplayHandles.push_back(qtyDisplay);
    qtyUpHandles.push_back(qtyUp);
}

void RestaurantGUI::CreatePaymentSection(HWND hwnd) {
    int startY = 730;

    hwndSubtotalLabel = CreateWindow(L"STATIC", L"Subtotal: ₱0.00 | Discount: ₱0.00 | Total: ₱0.00",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        20, startY, 1040, 30, hwnd, (HMENU)IDC_STATIC_SUBTOTAL, NULL, NULL);
    SendMessage(hwndSubtotalLabel, WM_SETFONT, (WPARAM)hFontBold, TRUE);

    CreateWindow(L"STATIC", L"DISCOUNT:",
        WS_VISIBLE | WS_CHILD,
        20, startY + 40, 100, 25, hwnd, NULL, NULL, NULL);

    hwndDiscountCombo = CreateWindow(L"COMBOBOX", NULL,
        WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
        120, startY + 40, 200, 200, hwnd, (HMENU)IDC_DISCOUNT_COMBO, NULL, NULL);

    SendMessage(hwndDiscountCombo, CB_ADDSTRING, 0, (LPARAM)L"None");
    SendMessage(hwndDiscountCombo, CB_ADDSTRING, 0, (LPARAM)L"Student (10%)");
    SendMessage(hwndDiscountCombo, CB_ADDSTRING, 0, (LPARAM)L"Senior (20%)");
    SendMessage(hwndDiscountCombo, CB_ADDSTRING, 0, (LPARAM)L"PWD (20%)");
    SendMessage(hwndDiscountCombo, CB_SETCURSEL, 0, 0);

    CreateWindow(L"STATIC", L"PAYMENT AMOUNT:",
        WS_VISIBLE | WS_CHILD,
        20, startY + 75, 150, 25, hwnd, NULL, NULL, NULL);

    hwndPaymentAmount = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"0.00",
        WS_VISIBLE | WS_CHILD | ES_RIGHT | ES_NUMBER,
        180, startY + 75, 120, 25, hwnd, (HMENU)IDC_PAYMENT_AMOUNT, NULL, NULL);

    CreateWindow(L"STATIC", L"CHANGE:",
        WS_VISIBLE | WS_CHILD,
        320, startY + 75, 80, 25, hwnd, NULL, NULL, NULL);

    hwndChangeAmount = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"0.00",
        WS_VISIBLE | WS_CHILD | ES_RIGHT | ES_READONLY,
        400, startY + 75, 120, 25, hwnd, (HMENU)IDC_CHANGE_AMOUNT, NULL, NULL);

    SendMessage(hwndDiscountCombo, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
    SendMessage(hwndPaymentAmount, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
    SendMessage(hwndChangeAmount, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
}

void RestaurantGUI::CreateNumericKeypad(HWND hwnd, int startX, int startY) {
    for (HWND btn : keypadHandles) if (btn) DestroyWindow(btn);
    keypadHandles.clear();

    const int keySize = 50;
    const int spacing = 5;

    const wchar_t* keys[] = { L"7", L"8", L"9", L"4", L"5", L"6", L"1", L"2", L"3", L"C", L"0", L"." };
    const int keyIds[] = { IDC_KEY_7, IDC_KEY_8, IDC_KEY_9, IDC_KEY_4, IDC_KEY_5, IDC_KEY_6,
                         IDC_KEY_1, IDC_KEY_2, IDC_KEY_3, IDC_KEY_CLEAR, IDC_KEY_0, IDC_KEY_DECIMAL };

    for (int i = 0; i < 12; i++) {
        int row = i / 3;
        int col = i % 3;
        int x = startX + col * (keySize + spacing);
        int y = startY + row * (keySize + spacing);

        HWND keyButton = CreateWindow(L"BUTTON", keys[i],
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            x, y, keySize, keySize,
            hwnd, (HMENU)keyIds[i], NULL, NULL);

        SendMessage(keyButton, WM_SETFONT, (WPARAM)hFontBold, TRUE);
        keypadHandles.push_back(keyButton);
    }
}

void RestaurantGUI::OnCommand(WPARAM wParam, LPARAM lParam) {
    int wmId = LOWORD(wParam);

    switch (wmId) {
    case IDC_TAB_APPETIZERS:
        SwitchCategory("Appetizers");
        SendMessage(hwndTabAppetizers, WM_SETFONT, (WPARAM)hFontBold, TRUE);
        SendMessage(hwndTabMain, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
        SendMessage(hwndTabDesserts, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
        SendMessage(hwndTabBeverages, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
        break;
    case IDC_TAB_MAIN:
        SwitchCategory("Main Dishes");
        SendMessage(hwndTabAppetizers, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
        SendMessage(hwndTabMain, WM_SETFONT, (WPARAM)hFontBold, TRUE);
        SendMessage(hwndTabDesserts, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
        SendMessage(hwndTabBeverages, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
        break;
    case IDC_TAB_DESSERTS:
        SwitchCategory("Desserts");
        SendMessage(hwndTabAppetizers, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
        SendMessage(hwndTabMain, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
        SendMessage(hwndTabDesserts, WM_SETFONT, (WPARAM)hFontBold, TRUE);
        SendMessage(hwndTabBeverages, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
        break;
    case IDC_TAB_BEVERAGES:
        SwitchCategory("Beverages");
        SendMessage(hwndTabAppetizers, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
        SendMessage(hwndTabMain, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
        SendMessage(hwndTabDesserts, WM_SETFONT, (WPARAM)hFontNormal, TRUE);
        SendMessage(hwndTabBeverages, WM_SETFONT, (WPARAM)hFontBold, TRUE);
        break;

    case IDC_LOGOUT_BUTTON:
        OnLogout();
        break;

    case IDC_MENU_ITEM_BASE:
    case IDC_MENU_ITEM_BASE + 1:
    case IDC_MENU_ITEM_BASE + 2:
    case IDC_MENU_ITEM_BASE + 3:
    case IDC_MENU_ITEM_BASE + 4:
    case IDC_MENU_ITEM_BASE + 5:
    case IDC_MENU_ITEM_BASE + 6:
    case IDC_MENU_ITEM_BASE + 7:
    case IDC_MENU_ITEM_BASE + 8:
    case IDC_MENU_ITEM_BASE + 9:
    case IDC_MENU_ITEM_BASE + 10:
    case IDC_MENU_ITEM_BASE + 11:
    case IDC_MENU_ITEM_BASE + 12:
    case IDC_MENU_ITEM_BASE + 13:
    case IDC_MENU_ITEM_BASE + 14:
    case IDC_MENU_ITEM_BASE + 15:
    case IDC_MENU_ITEM_BASE + 16:
    case IDC_MENU_ITEM_BASE + 17:
    case IDC_MENU_ITEM_BASE + 18:
    case IDC_MENU_ITEM_BASE + 19:
    case IDC_MENU_ITEM_BASE + 20:
    {
        int buttonIndex = wmId - IDC_MENU_ITEM_BASE;
        const MenuItem* menu = core->getMenu();
        int menuSize = core->getMenuSize();

        int currentIndex = 0;
        for (int i = 0; i < menuSize; i++) {
            if (menu[i].category == currentCategory) {
                if (currentIndex == buttonIndex) {
                    wchar_t qtyText[10];
                    GetWindowText(qtyDisplayHandles[buttonIndex], qtyText, 10);
                    int quantity = _wtoi(qtyText);
                    if (quantity < 1) quantity = 1;

                    OnAddToOrder(menu[i].number, quantity);
                    AnimateButtonPress((HWND)lParam);
                    break;
                }
                currentIndex++;
            }
        }
    }
    break;

    case IDC_QTY_UP_BASE:
    case IDC_QTY_UP_BASE + 1:
    case IDC_QTY_UP_BASE + 2:
    case IDC_QTY_UP_BASE + 3:
    case IDC_QTY_UP_BASE + 4:
    case IDC_QTY_UP_BASE + 5:
    case IDC_QTY_UP_BASE + 6:
    case IDC_QTY_UP_BASE + 7:
    case IDC_QTY_UP_BASE + 8:
    case IDC_QTY_UP_BASE + 9:
    case IDC_QTY_UP_BASE + 10:
    case IDC_QTY_UP_BASE + 11:
    case IDC_QTY_UP_BASE + 12:
    case IDC_QTY_UP_BASE + 13:
    case IDC_QTY_UP_BASE + 14:
    case IDC_QTY_UP_BASE + 15:
    case IDC_QTY_UP_BASE + 16:
    case IDC_QTY_UP_BASE + 17:
    case IDC_QTY_UP_BASE + 18:
    case IDC_QTY_UP_BASE + 19:
    case IDC_QTY_UP_BASE + 20:
        OnQuantityChange(wmId, 1);
        AnimateButtonPress((HWND)lParam);
        break;

    case IDC_QTY_DOWN_BASE:
    case IDC_QTY_DOWN_BASE + 1:
    case IDC_QTY_DOWN_BASE + 2:
    case IDC_QTY_DOWN_BASE + 3:
    case IDC_QTY_DOWN_BASE + 4:
    case IDC_QTY_DOWN_BASE + 5:
    case IDC_QTY_DOWN_BASE + 6:
    case IDC_QTY_DOWN_BASE + 7:
    case IDC_QTY_DOWN_BASE + 8:
    case IDC_QTY_DOWN_BASE + 9:
    case IDC_QTY_DOWN_BASE + 10:
    case IDC_QTY_DOWN_BASE + 11:
    case IDC_QTY_DOWN_BASE + 12:
    case IDC_QTY_DOWN_BASE + 13:
    case IDC_QTY_DOWN_BASE + 14:
    case IDC_QTY_DOWN_BASE + 15:
    case IDC_QTY_DOWN_BASE + 16:
    case IDC_QTY_DOWN_BASE + 17:
    case IDC_QTY_DOWN_BASE + 18:
    case IDC_QTY_DOWN_BASE + 19:
    case IDC_QTY_DOWN_BASE + 20:
        OnQuantityChange(wmId, -1);
        AnimateButtonPress((HWND)lParam);
        break;

    case IDC_KEY_0:
    case IDC_KEY_1:
    case IDC_KEY_2:
    case IDC_KEY_3:
    case IDC_KEY_4:
    case IDC_KEY_5:
    case IDC_KEY_6:
    case IDC_KEY_7:
    case IDC_KEY_8:
    case IDC_KEY_9:
    {
        wchar_t digit = L'0' + (wmId - IDC_KEY_0);
        wchar_t currentText[50];
        GetWindowText(hwndPaymentAmount, currentText, 50);

        if (wcscmp(currentText, L"0.00") == 0) {
            swprintf(currentText, 50, L"%c", digit);
        }
        else {
            size_t len = wcslen(currentText);
            if (len < 49) {
                currentText[len] = digit;
                currentText[len + 1] = L'\0';
            }
        }
        SetWindowText(hwndPaymentAmount, currentText);
        UpdateSubtotalDisplay();
        AnimateButtonPress((HWND)lParam);
    }
    break;

    case IDC_KEY_DECIMAL:
    {
        wchar_t currentText[50];
        GetWindowText(hwndPaymentAmount, currentText, 50);
        if (wcsstr(currentText, L".") == nullptr) {
            wcscat_s(currentText, L".");
            SetWindowText(hwndPaymentAmount, currentText);
        }
        AnimateButtonPress((HWND)lParam);
    }
    break;

    case IDC_KEY_CLEAR:
        SetWindowText(hwndPaymentAmount, L"0.00");
        AnimateButtonPress((HWND)lParam);
        break;

    case IDC_BTN_REMOVE:
        OnRemoveFromOrder();
        AnimateButtonPress((HWND)lParam);
        break;
    case IDC_BTN_CLEAR:
        OnClearOrder();
        AnimateButtonPress((HWND)lParam);
        break;
    case IDC_BTN_CHECKOUT:
        OnCheckout();
        AnimateButtonPress((HWND)lParam);
        break;

    case IDC_DISCOUNT_COMBO:
        if (HIWORD(wParam) == CBN_SELCHANGE) {
            UpdateSubtotalDisplay();
        }
        break;
    }
}

void RestaurantGUI::OnQuantityChange(int buttonId, int change) {
    int index = -1;

    if (change > 0) {
        index = buttonId - IDC_QTY_UP_BASE;
    }
    else {
        index = buttonId - IDC_QTY_DOWN_BASE;
    }

    if (index >= 0 && index < qtyDisplayHandles.size()) {
        wchar_t qtyText[10];
        GetWindowText(qtyDisplayHandles[index], qtyText, 10);
        int quantity = _wtoi(qtyText);

        quantity += change;
        if (quantity < 1) quantity = 1;
        if (quantity > 99) quantity = 99;

        SetWindowText(qtyDisplayHandles[index], std::to_wstring(quantity).c_str());
    }
}

void RestaurantGUI::OnAddToOrder(int itemId, int quantity) {
    core->addToOrder(itemId, quantity);
    UpdateOrderDisplay();

    std::wstring message = L"Item added to your order!\nQuantity: " + std::to_wstring(quantity);
    MessageBox(hwndMain, message.c_str(), L"Order Updated", MB_OK | MB_ICONINFORMATION);
}

void RestaurantGUI::OnRemoveFromOrder() {
    int selected = (int)SendMessage(hwndOrderList, LB_GETCURSEL, 0, 0);
    if (selected == LB_ERR) {
        MessageBox(hwndMain, L"Please select an item to remove", L"No Selection", MB_OK | MB_ICONWARNING);
        return;
    }

    core->removeFromOrder(selected);
    UpdateOrderDisplay();
}

void RestaurantGUI::OnClearOrder() {
    if (core->isOrderEmpty()) {
        MessageBox(hwndMain, L"Order is already empty", L"Clear Order", MB_OK | MB_ICONINFORMATION);
        return;
    }

    int result = MessageBox(hwndMain, L"Are you sure you want to clear the entire order?",
        L"Confirm Clear", MB_YESNO | MB_ICONQUESTION);

    if (result == IDYES) {
        core->clearOrder();
        UpdateOrderDisplay();
    }
}

void RestaurantGUI::OnCheckout() {
    if (core->isOrderEmpty()) {
        MessageBox(hwndMain, L"Please add items to your order before checkout",
            L"Empty Order", MB_OK | MB_ICONWARNING);
        return;
    }

    int discountIndex = (int)SendMessage(hwndDiscountCombo, CB_GETCURSEL, 0, 0);
    double subtotal = core->calculateSubtotal();
    double discount = core->calculateDiscount(subtotal, discountIndex);
    double grandTotal = subtotal - discount;

    wchar_t paymentText[50];
    GetWindowText(hwndPaymentAmount, paymentText, 50);
    double payment = _wtof(paymentText);

    if (payment < grandTotal) {
        std::wostringstream errorMsg;
        errorMsg << std::fixed << std::setprecision(2);
        errorMsg << L"Insufficient payment!\nTotal: ₱" << grandTotal
            << L"\nPayment: ₱" << payment;
        MessageBox(hwndMain, errorMsg.str().c_str(), L"Payment Error", MB_OK | MB_ICONERROR);
        return;
    }

    double change = payment - grandTotal;

    std::string receipt = core->generateReceipt(subtotal, discount, payment, change);
    MessageBox(hwndMain, std::wstring(receipt.begin(), receipt.end()).c_str(),
        L"🧾 Lola's Kusina - Receipt", MB_OK | MB_ICONINFORMATION);

    core->clearOrder();
    UpdateOrderDisplay();

    SetWindowText(hwndPaymentAmount, L"0.00");
    SetWindowText(hwndChangeAmount, L"0.00");

    UpdateVisualFeedback();
}

void RestaurantGUI::UpdateMenuDisplay() {
    CreateMenuGrid(hwndMain);
}

void RestaurantGUI::UpdateOrderDisplay() {
    SendMessage(hwndOrderList, LB_RESETCONTENT, 0, 0);

    const std::vector<MenuItem>& order = core->getOrder();
    const std::vector<int>& quantities = core->getQuantities();

    for (size_t i = 0; i < order.size(); i++) {
        double itemTotal = order[i].price * quantities[i];
        std::wostringstream orderItem;
        orderItem << std::fixed << std::setprecision(2);
        orderItem << quantities[i] << L" x "
            << std::wstring(order[i].name.begin(), order[i].name.end())
            << L" - ₱" << itemTotal;

        SendMessage(hwndOrderList, LB_ADDSTRING, 0, (LPARAM)orderItem.str().c_str());
    }

    UpdateSubtotalDisplay();
}

void RestaurantGUI::UpdateSubtotalDisplay() {
    double subtotal = core->calculateSubtotal();

    int discountIndex = (int)SendMessage(hwndDiscountCombo, CB_GETCURSEL, 0, 0);
    double discount = core->calculateDiscount(subtotal, discountIndex);
    double grandTotal = subtotal - discount;

    std::wostringstream subtotalStream;
    subtotalStream << std::fixed << std::setprecision(2);
    subtotalStream << L"Subtotal: ₱" << subtotal
        << L"  |  Discount: ₱" << discount
        << L"  |  Total: ₱" << grandTotal;

    SetWindowText(hwndSubtotalLabel, subtotalStream.str().c_str());

    if (hwndPaymentAmount) {
        wchar_t paymentText[50];
        GetWindowText(hwndPaymentAmount, paymentText, 50);
        double payment = _wtof(paymentText);

        if (payment > 0) {
            double change = payment - grandTotal;
            std::wostringstream changeStream;
            changeStream << std::fixed << std::setprecision(2);
            changeStream << change;
            SetWindowText(hwndChangeAmount, changeStream.str().c_str());
        }
        else {
            SetWindowText(hwndChangeAmount, L"0.00");
        }
    }
}

void RestaurantGUI::SwitchCategory(const std::string& category) {
    currentCategory = category;
    UpdateMenuDisplay();
}

std::string RestaurantGUI::GetFoodEmoji(const std::string& itemName) {
    if (itemName.find("Lumpiang") != std::string::npos) return "🥟";
    if (itemName.find("Calamares") != std::string::npos) return "🦑";
    if (itemName.find("Chicharon") != std::string::npos) return "🐷";
    if (itemName.find("Adobo") != std::string::npos) return "🍲";
    if (itemName.find("Kare-Kare") != std::string::npos) return "🥘";
    if (itemName.find("Sinigang") != std::string::npos) return "🍜";
    if (itemName.find("Bangus") != std::string::npos) return "🐟";
    if (itemName.find("Bulalo") != std::string::npos) return "🍖";
    if (itemName.find("Sisig") != std::string::npos) return "🔥";
    if (itemName.find("Halo-Halo") != std::string::npos) return "🍧";
    if (itemName.find("Leche Flan") != std::string::npos) return "🍰";
    if (itemName.find("Buko Pandan") != std::string::npos) return "🥥";
    if (itemName.find("Buko Juice") != std::string::npos) return "🥥";
    if (itemName.find("Iced Tea") != std::string::npos) return "🥤";
    if (itemName.find("Coke") != std::string::npos) return "🥤";
    if (itemName.find("Gulaman") != std::string::npos) return "🍋";
    if (itemName.find("Fresh Lumpia") != std::string::npos) return "🍋";
    if (itemName.find("Cheesy Dynamite") != std::string::npos) return "🍋";
    if (itemName.find("Calamansi") != std::string::npos) return "🍋";
    if (itemName.find("Crema de Leche") != std::string::npos) return "🍋";

    return "🍽️";
}

void RestaurantGUI::AnimateButtonPress(HWND button) {
    InvalidateRect(button, NULL, TRUE);
    UpdateWindow(button);
}

void RestaurantGUI::UpdateVisualFeedback() {

}

int RestaurantGUI::Run() {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}