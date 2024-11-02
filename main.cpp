#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <sstream>
#include <vector>

using namespace std;

void main_menu();

void gotoRowCol(int rpos, int cpos) {
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}

vector<string> splitString(const string& input, char delimiter) {
    vector<string> result;
    stringstream ss(input);
    string item;

    while (getline(ss, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}

// Function to clear console screen
void clearConsole() {
    system("cls");
}

// Function to disable echoing of password input
void disableEcho() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & ~ENABLE_ECHO_INPUT);
}

// Function to enable echoing of password input
void enableEcho() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode | ENABLE_ECHO_INPUT);
}

struct Review {
    string product_name;
    int rating;
};

struct ProductInfo {
    string name;
    float price;
    float average_rating;
};

class Product {
protected:
    string name;
    float price;

public:
    // Function to add a product to the file
    void add_product() {
        clearConsole();
        gotoRowCol(2, 70);  // Position cursor for admin action
        cout << "Admin is adding products." << endl;

        while (true) {
            string name;
            float price;
            cout << "Enter product name: ";
            cin >> name;
            cout << "Enter product price: ";
            cin >> price;

            ofstream product_file("products.txt", ios::app);
            product_file << name << " " << price << endl;
            product_file.close();

            cout << "Product added successfully!" << endl;

            char add_more_choice;
            cout << "Do you want to add another product? (y/n): ";
            cin >> add_more_choice;
            if (add_more_choice == 'n' || add_more_choice == 'N') {
                break;
            }
        }
    }

    // Function to delete a product from the file
    void delete_product() {
        clearConsole();
        display_products();
        gotoRowCol(2, 70);  // Position cursor for admin action
        cout << "Admin is deleting products." << endl;
        while (true) {
            string name;
            cout << "Enter product name to delete: ";
            cin >> name;

            ifstream product_file("products.txt");
            ofstream temp_file("temp.txt");

            string line;
            bool found = false;
            while (getline(product_file, line)) {
                vector<string> result = splitString(line, ' ');
                if (result[0] != name) {
                    temp_file << line << endl;
                } else {
                    found = true;
                }
            }

            product_file.close();
            temp_file.close();

            remove("products.txt");
            rename("temp.txt", "products.txt");

            if (found) {
                cout << "Product deleted successfully!" << endl;
            } else {
                cout << "Product not found!" << endl;
            }

            char delete_more_choice;
            cout << "Do you want to delete another product? (y/n): ";
            cin >> delete_more_choice;
            if (delete_more_choice == 'n' || delete_more_choice == 'N') {
                break;
            }
        }
    }

    // Function to display all products
    void display_products() {
        gotoRowCol(5, 59);  // Position cursor for admin action
        cout << "All store products are displaying" << endl;


        //below code
//        string line, prod_name;
//        int z = 0, total_rating = 0, rating = 0, avg_rating = 0;
//        ifstream prods("products.txt");
//        while(getline(prods, line))
//        {
//            total_rating = 0;
//            avg_rating = 0;
//            rating = 0;
//            z = 0;
//            vector<string> result = splitString(line, ' ');
//            prod_name = result[0];
//            cout << "\n********************************\n";
//            cout << "Name: " << prod_name << endl;
//            cout << "Price: " << result[1] << endl;
//
//            ifstream reviews("reviews.txt");
//            while(getline(reviews, line))
//            {
//
//                vector<string> result = splitString(line, '|');
//                if(prod_name == result[0])
//                {
//                    z++;
//                    rating = stoi(result[1]);
//                    total_rating += rating;
//                    avg_rating = total_rating/z;
//
//                }
//            }
//            if(z >= 1)
//            {
//                cout << "Rating: " << avg_rating << endl;
//            }
//        }
        //above code


        vector<Review> reviews;
        string line;

        // Read reviews from file
        ifstream reviews_file("reviews.txt");
        while (getline(reviews_file, line)) {
            vector<string> result = splitString(line, '|');
            Review review;
            review.product_name = result[0];
            stringstream ss(result[1]);
            ss >> review.rating;
            reviews.push_back(review);
        }
        reviews_file.close();

        // Store products with average ratings
        vector<ProductInfo> products_info;
        ifstream products("products.txt");
        while (getline(products, line)) {
            vector<string> result = splitString(line, ' ');
            ProductInfo product;
            product.name = result[0];
            stringstream ss(result[1]);
            ss >> product.price;

            // Calculate average rating
            int sum = 0;
            int count = 0;
            for (const auto& review : reviews) {
                if (review.product_name == product.name) {
                    sum += review.rating;
                    count++;
                }
            }
            product.average_rating = (count > 0) ? static_cast<float>(sum) / count : 0.0;

            products_info.push_back(product);
        }
        products.close();

        // Sort products by average rating
        for (size_t i = 0; i < products_info.size() - 1; ++i) {
            for (size_t j = i + 1; j < products_info.size(); ++j) {
                if (products_info[i].average_rating < products_info[j].average_rating) {
                    swap(products_info[i], products_info[j]);
                }
            }
        }

        // Display sorted products with labels
        int count = 0;
        for (const auto& product : products_info) {
            string label;
            if (count == 0) {
                label = "Epicurean Delight";
            } else if (count == 1) {
                label = "Chef's Choice";
            } else if (count == 2) {
                label = "Savory Supreme";
            }


            cout << "\n***********************" << endl;
            cout << "Name: " << product.name << " (" << label << ")" << endl;
            cout << "Price: " << product.price << endl;
            cout << "Rating: " << product.average_rating << endl;

            count++;
        }
//        system("pause");
    }

    void order_confirmed()
    {
        string user_name, line, item[50];
        int quantity, temp, total_price= 0, i = 0;
        ifstream cart("cart.txt");

        while(getline(cart, line))
        {
            vector<string> result = splitString(line, ',');
            user_name = result[0];
            item[i] = result[1];
            quantity += stoi(result[2]);
            temp = stoi(result[3]);
            total_price += temp;


            ofstream sold_history("sold_history.txt", ios::app);
            sold_history << user_name << "," << item[i] << "," << temp << endl;
            sold_history.close();

            i++;
        }


        cart.close();

        system("cls");
        cout << "Congratulation " << user_name << " your order has been confirmed." << endl;
        cout << "Your Items: " << endl;
        for(int a = 0; a < i; a++)
        {
            cout << a+1 << ". " << item[a] << endl;
        }
        cout << "Receipt:\nTotal Price:(items_price + 50(delivery charges)): " << total_price + 50<< endl;


        string address;
        cout << "Enter your address: ";
        cin >> address;


        cout << "Your order is on the way." << endl;
        cout << "Thanks for choosing our service." << endl;



        string x;
        cout << "Press any key to continue...";
        _getch();
        main_menu();
    }

    // Friend function declaration to access private members for review purposes
    friend void add_review(const string& product_name);
};

// Function to add a review for a purchased product (Friend Function)
void add_review(const string& product_name) {
    int rating;
    cout << "Enter rating (1-5): ";
    cin >> rating;

    ofstream review_file("reviews.txt", ios::app);
    review_file << product_name << "|" << rating << endl;
    review_file.close();

    cout << "Review added successfully!" << endl;
}
class User : public Product {
private:
    vector<pair<string, int>> cart; // Pair of product name and quantity
    string username;

public:
    // Constructor to initialize username
    User(const string& user) : username(user) {}

    // Function to handle user menu
   // Function to handle user menu
void user_menu() {
    clearConsole();
    gotoRowCol(1, 1);
    cout << "Enter your username: ";
    cin >> username;
    gotoRowCol(3, 60);
    cout << "\033[1;31m"; // Bold and red text
    cout << "Welcome ";
    cout << "\033[4m"; // Underline
    cout << username;
    cout << "\033[0m"; // Reset all formatting
    cout << "\033[1;31m"; // Bold and red text
    cout << "! Happy Shopping!";
    cout << "\033[0m"; // Reset all formatting
    cout << endl;

    float total_price = 0.0;
    bool buy_more = true;

    // Clear the common cart file at the beginning of the session
    ofstream clear_cart("cart.txt", ios::trunc);
    clear_cart.close();

    while (buy_more) {
        display_products(); // Display products before each purchase

        string item_name;
        int quantity, rating;
        cout << "\nEnter item you want to buy (or type 'goto' to goto cart): ";
        cin >> item_name;

        if (item_name == "goto") {
            break; // Exit the user menu
        }

        cout << "Enter quantity: ";
        cin >> quantity;

        ifstream product_file("products.txt");
        string line;
        bool found = false;
        while (getline(product_file, line)) {
            stringstream ss(line);
            ss >> name; // Read product name
            ss >> price; // Read product price

            if (name == item_name) {
                found = true;
                cart.push_back(make_pair(name, quantity)); // Add product and quantity to cart
                total_price += price * quantity; // Update total price based on quantity
                ofstream user_cart("cart.txt", ios::app); // Common cart file
                user_cart << username << "," << name << "," << quantity << "," << price * quantity << endl;
                user_cart.close();
                cout << item_name << " (Quantity: " << quantity << ") purchased successfully!" << endl;
                add_review(name); // Call add_review after a successful purchase
                break;
            }
        }
        if (!found) {
            cout << "Invalid Input" << endl;
        }

        display_cart(); // Display cart contents after each purchase

        char buy_more_choice;
        cout << "\nPress Y/y to buy more or F/f to confirm the order: ";
        cin >> buy_more_choice;
        system("cls");
        if (buy_more_choice == 'n' || buy_more_choice == 'N') {
            buy_more = false;
        } else if (buy_more_choice == 'f' || buy_more_choice == 'F') {
            order_confirmed();
        }
    }

    // Display cart contents and total price after shopping session
    if (!cart.empty()) {
        clearConsole();
        cout << "\nThank you for shopping with us!\n";
        cout << "Your Cart:\n";
        for (const auto& item : cart) {
            cout << "Product: " << item.first << " | Quantity: " << item.second << endl;
        }
        cout << "\nTotal Price: $" << total_price << endl;
    }

    // Allow user to manage cart (remove products)
    manage_cart();
}

    // Function to manage the cart (remove products)
  // Function to manage the cart (remove products)
void manage_cart() {
    char manage_choice;
    do {
        cout << "\nDo you want to manage your cart? (y/n): ";
        cin >> manage_choice;

        if (manage_choice == 'y' || manage_choice == 'Y') {
            clearConsole();
            display_cart();

            string product_name;
            cout << "\nEnter the product name to remove from cart (or 'exit' to go back): ";
            cin >> product_name;

            if (product_name == "exit") {
                break;
            }

            bool found = false;
            for (auto it = cart.begin(); it != cart.end(); ++it) {
                if (it->first == product_name) {
                    cart.erase(it);
                    found = true;
                    break;
                }
            }

            if (found) {
                cout << product_name << " removed from cart." << endl;
            } else {
                cout << "Product not found in cart." << endl;
            }

            // Update the cart file with the remaining items
            ofstream update_cart("cart.txt", ios::trunc);
            for (const auto& item : cart) {
                update_cart << username << "," << item.first << "," << item.second << endl;
            }
            update_cart.close();

            display_cart(); // Display updated cart after deletion
        }
    } while (manage_choice == 'y' || manage_choice == 'Y');
}

    // Function to display the cart contents
    void display_cart() {
        clearConsole();
        cout << "\nYour Cart:\n";
        float total_price = 0.0;


        string user, line1, items[50];
        int quan = 0, tpr, x = 0;
        ifstream cart("cart.txt");

        while(getline(cart, line1))
        {
            vector<string> result = splitString(line1, ',');
            user = result[0];
            items[x] = result[1];
            quan = stoi(result[2]);
            tpr = stoi(result[3]);


            cout << "\nUser: " << user << endl;
            cout << "Item: " << items[x] << endl;
            cout << "Quantity: " << quan << endl;
            cout << "Price: " << tpr << endl;

            x++;
        }

        cart.close();
//
//        user_cart.close();

    }
};


class Admin : public Product {
public:
    // Function to display all reviews
    void display_reviews() {
        clearConsole();
        cout << "All the reviews are displaying" << endl;

        string line;
        ifstream reviews_file("reviews.txt");

        while (getline(reviews_file, line)) {
            vector<string> result = splitString(line, '|');
            cout << "\n***********************" << endl;
            cout << "Product: " << result[0] << endl;
            cout << "Rating: " << result[1] << endl;
        }
        reviews_file.close();
        system("pause");
    }

    // Overriding the admin_login to provide specific admin authentication
    void admin_login() {
        string user, password;
        bool user_is_valid = false, pass_is_valid = false;

        while (true) {
            int x = 0, i = 0;
            clearConsole();
            cout << "Enter username: ";
            cin >> user;
            cout << "Enter password: ";
            disableEcho();
            cin >> password;
            enableEcho();

            string text;
            ifstream users_file("admins.txt");

            while (getline(users_file, text)) {
                i++;
                if (text == user) {
                    user_is_valid = true;
                    break;
                }
            }

            string p_text;
            ifstream user_password_file("admins_password.txt");

            while (getline(user_password_file, p_text)) {
                x++;
                if (p_text == password) {
                    pass_is_valid = true;
                    break;
                }
            }

            if (i == x && user_is_valid && pass_is_valid) {
                clearConsole();
                cout << "\nYou are logged in as " << user << endl;
                admin_menu();
                break;
            } else {
                cout << "\nInvalid username or password. " << endl;
                continue;
            }
        }
        system("pause");
    }

    // Function to handle admin menu
    void admin_menu() {
        int admin_option;
        do {
            clearConsole();
            cout << "Welcome, Admin!" << endl;
            cout << "\nSelect your option:" << endl;
            cout << "1. Add Product" << endl;
            cout << "2. Delete Product" << endl;
            cout << "3. Display Products" << endl;
            cout << "4. Display Reviews" << endl;
            cout << "5. Exit to Main Menu" << endl;
            cout << "\nEnter option: ";
            cin >> admin_option;

            switch (admin_option) {
                case 1:
                    add_product();
                    break;
                case 2:
                    delete_product();
                    break;
                case 3:
                    system("cls");
                    display_products();
                    cout << "press any key to continue.." << endl;
                    getch();
                    break;
                case 4:
                    display_reviews();
                    break;
                case 5:
                    break; // Exit to main menu
                default:
                    cout << "Invalid option. Please try again." << endl;
                    break;
            }
        } while (admin_option != 5);
    }
};


void main_menu()
{
    system("cls");
    string username;
    int option;

    do {
        clearConsole();
        gotoRowCol(1, 50);
          cout << "\033[1;31mWelcome to our \033[1;4mBites and Bytes\033[0m\033[1;31m !\033[0m" << endl;
         gotoRowCol(3, 35);
        cout << "\033[1;31mDeliciousness brought to you by Huzaira, Seher Bano, Ashir, and Hamza!\033[0m" << endl;
        cout << "\nSelect your option:" << endl;
        cout << "1. User Menu" << endl;
        cout << "2. Admin Menu" << endl;
        cout << "3. Exit" << endl;
        cout << "\nEnter option: ";
        cin >> option;

        switch (option) {
            case 1: {
                User user(username);
                user.user_menu();
                break;
            }
            case 2: {
                Admin admin;
                admin.admin_login();
                break;
            }
            case 3:
                break; // Exit the program
            default:
                cout << "Invalid option. Please try again." << endl;
                break;
        }
    } while (option != 3);
}

// Main function to drive the program
int main() {
    // Prompt user to choose an option
    main_menu();
    return 0;
}
