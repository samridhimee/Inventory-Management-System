#include <iostream>
#include <string>
#include "sqlite3.h"

using namespace std;

sqlite3 *DB;
char *messageError;

// Connect Database
void connectDatabase()
{
    int rc = sqlite3_open("inventory.db", &DB);

    if (rc)
        cout << "Cannot open database!\n";
    else
        cout << "Database Connected Successfully!\n";
}

// Create Table
void createTable()
{
    string sql =
        "CREATE TABLE IF NOT EXISTS Products("
        "ProductID INTEGER PRIMARY KEY,"
        "ProductName TEXT NOT NULL,"
        "Quantity INTEGER,"
        "Price REAL);";

    int rc = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (rc != SQLITE_OK)
    {
        cout << "Error creating table\n";
        sqlite3_free(messageError);
    }
}

// Add Product
void addProduct()
{
    int id, quantity;
    double price;
    string name;

    cout << "\nEnter Product ID: ";
    cin >> id;
    cin.ignore();

    cout << "Enter Product Name: ";
    getline(cin, name);

    cout << "Enter Quantity: ";
    cin >> quantity;

    cout << "Enter Price: ";
    cin >> price;

    string sql =
        "INSERT INTO Products(ProductID,ProductName,Quantity,Price) VALUES(" +
        to_string(id) + ",'" + name + "'," +
        to_string(quantity) + "," +
        to_string(price) + ");";

    int rc = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (rc != SQLITE_OK)
    {
        cout << "Failed to add product\n";
        sqlite3_free(messageError);
    }
    else
    {
        cout << "Product Added Successfully!\n";
    }
}

// Callback Function
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        cout << azColName[i] << " : "
             << (argv[i] ? argv[i] : "NULL") << endl;
    }
    cout << "-------------------------\n";
    return 0;
}
// View All Products
void viewProducts()
{
    string sql = "SELECT * FROM Products;";

    int rc = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);

    if (rc != SQLITE_OK)
    {
        cout << "Error fetching data\n";
        sqlite3_free(messageError);
    }
}

// Search Product
void searchProduct()
{
    int id;

    cout << "\nEnter Product ID: ";
    cin >> id;

    string sql = "SELECT * FROM Products WHERE ProductID=" + to_string(id) + ";";

    int rc = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);

    if (rc != SQLITE_OK)
    {
        cout << "Search Failed\n";
        sqlite3_free(messageError);
    }
}

// Update Product
void updateProduct()
{
    int id, quantity;
    double price;

    cout << "\nEnter Product ID: ";
    cin >> id;

    cout << "Enter New Quantity: ";
    cin >> quantity;

    cout << "Enter New Price: ";
    cin >> price;

    string sql =
        "UPDATE Products SET Quantity=" + to_string(quantity) +
        ", Price=" + to_string(price) +
        " WHERE ProductID=" + to_string(id) + ";";

    int rc = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (rc != SQLITE_OK)
    {
        cout << "Update Failed\n";
        sqlite3_free(messageError);
    }
    else
    {
        cout << "Product Updated Successfully!\n";
    }
}

// Delete Product
void deleteProduct()
{
    int id;

    cout << "\nEnter Product ID: ";
    cin >> id;

    string sql =
        "DELETE FROM Products WHERE ProductID=" + to_string(id) + ";";

    int rc = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (rc != SQLITE_OK)
    {
        cout << "Delete Failed\n";
        sqlite3_free(messageError);
    }
    else
    {
        cout << "Product Deleted Successfully!\n";
    }
}
int main()
{
    connectDatabase();
    createTable();

    int choice;

    do
    {
        cout << "\n===================================";
        cout << "\n   INVENTORY MANAGEMENT SYSTEM";
        cout << "\n===================================";
        cout << "\n1. Add Product";
        cout << "\n2. View Products";
        cout << "\n3. Search Product";
        cout << "\n4. Update Product";
        cout << "\n5. Delete Product";
        cout << "\n6. Exit";

        cout << "\n\nEnter your choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
                addProduct();
                break;

            case 2:
                viewProducts();
                break;

            case 3:
                searchProduct();
                break;

            case 4:
                updateProduct();
                break;

            case 5:
                deleteProduct();
                break;

            case 6:
                cout << "\nThank you for using Inventory Management System.\n";
                break;

            default:
                cout << "\nInvalid Choice!\n";
        }

    } while(choice != 6);

    sqlite3_close(DB);

    return 0;
}