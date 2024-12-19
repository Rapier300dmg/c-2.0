#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

class Contact {
private:
    char* fullName; 
    std::string homePhone;
    std::string workPhone;
    std::string mobilePhone;
    std::string additionalInfo;

public:
    Contact(const char* name, const std::string& home, const std::string& work,
        const std::string& mobile, const std::string& info)
        : homePhone(home), workPhone(work), mobilePhone(mobile), additionalInfo(info) {
        fullName = new char[strlen(name) + 1];
        strcpy(fullName, name);
    }

    Contact(const Contact& other)
        : homePhone(other.homePhone), workPhone(other.workPhone),
        mobilePhone(other.mobilePhone), additionalInfo(other.additionalInfo) {
        fullName = new char[strlen(other.fullName) + 1];
        strcpy(fullName, other.fullName);
    }

    Contact& operator=(const Contact& other) {
        if (this == &other) return *this;

        delete[] fullName;

        fullName = new char[strlen(other.fullName) + 1];
        strcpy(fullName, other.fullName);

        homePhone = other.homePhone;
        workPhone = other.workPhone;
        mobilePhone = other.mobilePhone;
        additionalInfo = other.additionalInfo;

        return *this;
    }

    ~Contact() {
        delete[] fullName;
    }

    const char* getFullName() const {
        return fullName;
    }

    void print() const {
        std::cout << "Name: " << fullName << std::endl;
        std::cout << "Home Phone: " << homePhone << std::endl;
        std::cout << "Work Phone: " << workPhone << std::endl;
        std::cout << "Mobile Phone: " << mobilePhone << std::endl;
        std::cout << "Additional Info: " << additionalInfo << std::endl;
    }

    void saveToFile(std::ofstream& out) const {
        out << fullName << '\n' << homePhone << '\n' << workPhone << '\n'
            << mobilePhone << '\n' << additionalInfo << '\n';
    }

    static Contact loadFromFile(std::ifstream& in) {
        std::string name, home, work, mobile, info;
        std::getline(in, name);
        std::getline(in, home);
        std::getline(in, work);
        std::getline(in, mobile);
        std::getline(in, info);
        return Contact(name.c_str(), home, work, mobile, info);
    }
};

class PhoneBook {
private:
    std::vector<Contact> contacts;

public:
    void addContact(const Contact& contact) {
        contacts.push_back(contact);
    }

    void removeContact(const std::string& name) {
        auto it = std::remove_if(contacts.begin(), contacts.end(), [&name](const Contact& c) {
            return name == c.getFullName();
            });

        if (it != contacts.end()) {
            contacts.erase(it, contacts.end());
            std::cout << "Contact removed.\n";
        }
        else {
            std::cout << "Contact not found.\n";
        }
    }

    void searchContact(const std::string& name) const {
        for (const auto& contact : contacts) {
            if (name == contact.getFullName()) {
                contact.print();
                return;
            }
        }
        std::cout << "Contact not found.\n";
    }

    void showAllContacts() const {
        for (const auto& contact : contacts) {
            contact.print();
            std::cout << "--------------------------\n";
        }
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out) {
            std::cerr << "Error opening file for writing.\n";
            return;
        }

        out << contacts.size() << '\n';
        for (const auto& contact : contacts) {
            contact.saveToFile(out);
        }
        std::cout << "Contacts saved to file.\n";
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) {
            std::cerr << "Error opening file for reading.\n";
            return;
        }

        size_t count;
        in >> count;
        in.ignore(); 

        contacts.clear();
        for (size_t i = 0; i < count; ++i) {
            contacts.push_back(Contact::loadFromFile(in));
        }
        std::cout << "Contacts loaded from file.\n";
    }
};

int main() {
    PhoneBook phoneBook;

    while (true) {
        std::cout << "\nPhone Book Menu:\n";
        std::cout << "1. Add Contact\n";
        std::cout << "2. Remove Contact\n";
        std::cout << "3. Search Contact\n";
        std::cout << "4. Show All Contacts\n";
        std::cout << "5. Save to File\n";
        std::cout << "6. Load from File\n";
        std::cout << "7. Exit\n";
        std::cout << "Choose an option: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(); 

        if (choice == 1) {
            char name[100];
            std::string home, work, mobile, info;

            std::cout << "Enter full name: ";
            std::cin.getline(name, 100);

            std::cout << "Enter home phone: ";
            std::getline(std::cin, home);

            std::cout << "Enter work phone: ";
            std::getline(std::cin, work);

            std::cout << "Enter mobile phone: ";
            std::getline(std::cin, mobile);

            std::cout << "Enter additional info: ";
            std::getline(std::cin, info);

            phoneBook.addContact(Contact(name, home, work, mobile, info));

        }
        else if (choice == 2) {
            std::string name;
            std::cout << "Enter full name of contact to remove: ";
            std::getline(std::cin, name);
            phoneBook.removeContact(name);

        }
        else if (choice == 3) {
            std::string name;
            std::cout << "Enter full name of contact to search: ";
            std::getline(std::cin, name);
            phoneBook.searchContact(name);

        }
        else if (choice == 4) {
            phoneBook.showAllContacts();

        }
        else if (choice == 5) {
            std::string filename;
            std::cout << "Enter filename to save contacts: ";
            std::getline(std::cin, filename);
            phoneBook.saveToFile(filename);

        }
        else if (choice == 6) {
            std::string filename;
            std::cout << "Enter filename to load contacts: ";
            std::getline(std::cin, filename);
            phoneBook.loadFromFile(filename);

        }
        else if (choice == 7) {
            break;

        }
        else {
            std::cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}
