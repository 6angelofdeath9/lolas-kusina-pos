#include "RestaurantCore.h"

RestaurantCore::RestaurantCore() : isLoggedIn(false), currentUser("") {
    // Initialize users
    users.push_back(User("admin", "admin", "Administrator"));
    users.push_back(User("cashier", "1234", "Cashier"));
}

bool RestaurantCore::login(const string& username, const string& password) {
    for (const auto& user : users) {
        if (user.username == username && user.password == password) {
            isLoggedIn = true;
            currentUser = username;
            return true;
        }
    }
    return false;
}

void RestaurantCore::logout() {
    isLoggedIn = false;
    currentUser = "";
}

const MenuItem* RestaurantCore::findMenuItem(int itemNumber) const {
    for (int i = 0; i < MENU_SIZE; ++i) {
        if (menu[i].number == itemNumber) {
            return &menu[i];
        }
    }
    return nullptr;
}

void RestaurantCore::addToOrder(int itemNumber, int quantity) {
    if (const MenuItem* item = findMenuItem(itemNumber)) {
      
        for (size_t i = 0; i < customerOrder.size(); ++i) {
            if (customerOrder[i].number == itemNumber) {
                orderQuantities[i] += quantity;
                return;
            }
        }

        
        customerOrder.push_back(*item);
        orderQuantities.push_back(quantity);
    }
}

void RestaurantCore::removeFromOrder(int index) {
    if (index >= 0 && index < customerOrder.size()) {
        customerOrder.erase(customerOrder.begin() + index);
        orderQuantities.erase(orderQuantities.begin() + index);
    }
}

void RestaurantCore::updateOrderQuantity(int index, int newQuantity) {
    if (index >= 0 && index < customerOrder.size() && newQuantity > 0) {
        orderQuantities[index] = newQuantity;
    }
}

void RestaurantCore::clearOrder() {
    customerOrder.clear();
    orderQuantities.clear();
}

bool RestaurantCore::isOrderEmpty() const {
    return customerOrder.empty();
}

double RestaurantCore::calculateSubtotal() const {
    double subtotal = 0.0;
    for (size_t i = 0; i < customerOrder.size(); ++i) {
        subtotal += customerOrder[i].price * orderQuantities[i];
    }
    return subtotal;
}

double RestaurantCore::calculateDiscount(double subtotal, int discountChoice) const {
    switch (discountChoice) {
    case 1: return subtotal * 0.10; // Student
    case 2: return subtotal * 0.20; // Senior
    case 3: return subtotal * 0.20; // PWD
    default: return 0.0;
    }
}

string RestaurantCore::generateReceipt(double subtotal, double discount, double payment, double change) const {
    stringstream receipt;
    receipt << fixed << setprecision(2);

    receipt << "=========================================\n";
    receipt << "           LOLAS KUSINA\n";
    receipt << "          \"MGA LUTO NI LOLA\"\n";
    receipt << "=========================================\n";
    receipt << "ITEM                QTY   PRICE   TOTAL\n";
    receipt << "-----------------------------------------\n";

    for (size_t i = 0; i < customerOrder.size(); ++i) {
        double itemTotal = customerOrder[i].price * orderQuantities[i];
        receipt << customerOrder[i].name << "\t\t"
            << orderQuantities[i] << "\tPHP " << customerOrder[i].price
            << "\tPHP " << itemTotal << endl;
    }

    receipt << "-----------------------------------------\n";
    receipt << "Subtotal:\t\t\tPHP " << subtotal << endl;
    receipt << "Discount:\t\t\tPHP " << discount << endl;
    receipt << "-----------------------------------------\n";
    receipt << "GRAND TOTAL:\t\t\tPHP " << (subtotal - discount) << endl;
    receipt << "Payment:\t\t\tPHP " << payment << endl;
    receipt << "Change:\t\t\t\tPHP " << change << endl;
    receipt << "=========================================\n";
    receipt << "  SALAMAT PO! MARAMING SALAMAT!\n";
    receipt << "Server: " << currentUser << endl;
    receipt << "=========================================\n";

    return receipt.str();
}
