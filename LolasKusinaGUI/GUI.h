    #pragma once
    #include <windows.h>
    #include <commctrl.h>
    #include <string>
    #include <vector>
    #include <map>
    #include "RestaurantCore.h"

    // Control IDs
    #define IDC_LOGIN_USERNAME     1001
    #define IDC_LOGIN_PASSWORD     1002
    #define IDC_LOGIN_BUTTON       1003
    #define IDC_LOGOUT_BUTTON      1004

    #define IDC_TAB_APPETIZERS     2001
    #define IDC_TAB_MAIN           2002
    #define IDC_TAB_DESSERTS       2003
    #define IDC_TAB_BEVERAGES      2004

    #define IDC_MENU_ITEM_BASE     3000
    #define IDC_QTY_UP_BASE        4000
    #define IDC_QTY_DOWN_BASE      5000
    #define IDC_QTY_DISPLAY_BASE   6000

    #define IDC_ORDER_LIST         7001
    #define IDC_BTN_REMOVE         7002
    #define IDC_BTN_CLEAR          7003
    #define IDC_BTN_CHECKOUT       7004

    #define IDC_PAYMENT_AMOUNT     8001
    #define IDC_CHANGE_AMOUNT      8002
    #define IDC_DISCOUNT_COMBO     8003
    #define IDC_STATIC_SUBTOTAL    8004

    #define IDC_KEY_0              9000
    #define IDC_KEY_1              9001
    #define IDC_KEY_2              9002
    #define IDC_KEY_3              9003
    #define IDC_KEY_4              9004
    #define IDC_KEY_5              9005
    #define IDC_KEY_6              9006
    #define IDC_KEY_7              9007
    #define IDC_KEY_8              9008
    #define IDC_KEY_9              9009
    #define IDC_KEY_CLEAR          9010
    #define IDC_KEY_DECIMAL        9011

    // Colors
    #define COLOR_PRIMARY      RGB(139, 69, 19)    // Saddle Brown
    #define COLOR_SECONDARY    RGB(205, 133, 63)   // Peru
    #define COLOR_ACCENT       RGB(255, 215, 0)    // Gold
    #define COLOR_SUCCESS      RGB(34, 139, 34)    // Forest Green
    #define COLOR_DANGER       RGB(220, 53, 69)    // Red
    #define COLOR_BACKGROUND   RGB(253, 245, 230)  // Old Lace
    #define COLOR_CARD         RGB(255, 250, 240)  // Floral White

    class RestaurantGUI {
    private:
        RestaurantCore* core;
        std::string currentCategory;
        int selectedQty;

        // Window handles
        HWND hwndMain, hwndLogin, hwndOrderList;
        HWND hwndSubtotalLabel, hwndDiscountCombo, hwndPaymentAmount, hwndChangeAmount;
        HWND hwndTabAppetizers, hwndTabMain, hwndTabDesserts, hwndTabBeverages;
        HWND hwndUsername, hwndPassword, hwndLoginBtn, hwndLogoutBtn;

        // Fonts
        HFONT hFontTitle, hFontNormal, hFontBold, hFontLarge;
        HFONT hMenuFont, hPriceFont, hEmojiFont;

        // Image handles
        std::map<int, HBITMAP> foodImages;
        HBITMAP hLoginBg;
        HBITMAP hMainBg;

        // Dynamic controls
        std::vector<HWND> menuButtonHandles;
        std::vector<HWND> keypadHandles;
        std::vector<HWND> qtyUpHandles;
        std::vector<HWND> qtyDownHandles;
        std::vector<HWND> qtyDisplayHandles;

    public:
        RestaurantGUI(RestaurantCore* restaurantCore);
        ~RestaurantGUI();

        bool CreateMainWindow(HINSTANCE hInstance);
        int Run();

    private:
        void CreateLoginWindow(HINSTANCE hInstance);
        void CreateModernLayout(HWND hwnd);
        void CreateMenuGrid(HWND hwnd);
        void CreateNumericKeypad(HWND hwnd, int startX, int startY);
        void CreatePaymentSection(HWND hwnd);
        void CreateQuantityControls(HWND hwnd, int x, int y, int index);

        // Image functions
        void LoadFoodImages();
        void DrawFoodImages(HDC hdc);

        void UpdateMenuDisplay();
        void UpdateOrderDisplay();
        void UpdateSubtotalDisplay();
        void UpdateVisualFeedback();

        void SwitchCategory(const std::string& category);
        void OnCommand(WPARAM wParam, LPARAM lParam);
        void OnPaint(HWND hwnd);

        void OnLogin();
        void OnLogout();
        void OnAddToOrder(int itemId, int quantity = 1);
        void OnRemoveFromOrder();
        void OnClearOrder();
        void OnCheckout();
        void OnQuantityChange(int buttonId, int change);

        std::string GetFoodEmoji(const std::string& itemName);
        void AnimateButtonPress(HWND button);

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK LoginProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };