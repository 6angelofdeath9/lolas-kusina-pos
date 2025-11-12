#include "GUI.h"
#include "RestaurantCore.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES | ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icex);

    
    RestaurantCore restaurant;

    
    RestaurantGUI gui(&restaurant);
    if (gui.CreateMainWindow(hInstance)) {
        return gui.Run();
    }

    return 0;
}