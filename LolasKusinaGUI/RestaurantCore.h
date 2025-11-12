    #pragma once
    #include <iostream>
    #include <vector>
    #include <string>
    #include <sstream>
    #include <iomanip>
    #include <map>

    using namespace std;

    struct MenuItem {
        int number;
        string name;
        string category;
        double price;
        string imagePath;
    };

    class User {
    public:
        string username;
        string password;
        string role;

        User(const string& uname, const string& pwd, const string& userRole)
            : username(uname), password(pwd), role(userRole) {
        }
    };

    class RestaurantCore {
    private:
        vector<MenuItem> customerOrder;
        vector<int> orderQuantities;
        vector<User> users;
        bool isLoggedIn;
        string currentUser;

        const int MENU_SIZE = 20;
        MenuItem menu[20] = {
            {1, "Lumpiang Shanghai", "Appetizers", 120.00, "images/lumpia.bmp"},
            {2, "Crispy Calamares", "Appetizers", 150.00, "images/calamares.bmp"},
            {3, "Chicharon Bulaklak", "Appetizers", 130.00, "images/chicharon.bmp"},
            {4, "Chicken Adobo", "Main Dishes", 180.00, "images/adobo.bmp"},
            {5, "Beef Kare-Kare", "Main Dishes", 220.00, "images/karekare.bmp"},
            {6, "Sinigang na Baboy", "Main Dishes", 200.00, "images/sinigang.bmp"},
            {7, "Grilled Bangus", "Main Dishes", 160.00, "images/bangus.bmp"},
            {8, "Bulalo", "Main Dishes", 220.00, "images/bulalo.bmp"},
            {9, "Pork Sisig", "Main Dishes", 190.00, "images/sisig.bmp"},
            {10, "Halo-Halo", "Desserts", 100.00, "images/halohalo.bmp"},
            {11, "Leche Flan", "Desserts", 90.00, "images/lecheflan.bmp"},
            {12, "Buko Pandan", "Desserts", 85.00, "images/bukopandan.bmp"},
            {13, "Buko Juice", "Beverages", 60.00, "images/buko_juice.bmp"},
            {14, "Iced Tea", "Beverages", 50.00, "images/iced_tea.bmp"},
            {15, "Coke", "Beverages", 45.00, "images/coke.bmp"},
            {16, "Gulaman", "Beverages", 150.00, "images/gulaman.bmp"},
            {17, "Fresh Lumpia", "Appetizers", 120.00, "images/fresh_Lumpia.bmp"},
            {18, "Cheesy Dynamite", "Appetizers", 150.00, "images/Cheesy_Dynamite.bmp"},
            {19, "Calamansi Juice", "Beverages", 150.00, "images/Calamansi Juice.bmp"},
            {20, "Crema de Leche", "Desserts", 150.00, "images/CremadeLeche.bmp"},
        };

    public:
        RestaurantCore();

        // Login functions
        bool login(const string& username, const string& password);
        void logout();
        bool getLoginStatus() const { return isLoggedIn; }
        string getCurrentUser() const { return currentUser; }

        // Menu functions
        const MenuItem* getMenu() const { return menu; }
        int getMenuSize() const { return MENU_SIZE; }
        const MenuItem* findMenuItem(int itemNumber) const;

        // Order functions
        const vector<MenuItem>& getOrder() const { return customerOrder; }
        const vector<int>& getQuantities() const { return orderQuantities; }
        void addToOrder(int itemNumber, int quantity);
        void removeFromOrder(int index);
        void updateOrderQuantity(int index, int newQuantity);
        void clearOrder();
        bool isOrderEmpty() const;

        // Calculation functions
        double calculateSubtotal() const;
        double calculateDiscount(double subtotal, int discountChoice) const;
        string generateReceipt(double subtotal, double discount, double payment, double change) const;

        // Image paths
        string getLogoPath() const { return "images/logo.png"; }
        string getLoginBgPath() const { return "images/login_bg.jpg"; }
    };
