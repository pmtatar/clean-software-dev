#include <iostream>
#include <string>
#include <map>

static const std::string SYSTEM_LOG_FILE = "system.log";

class LoginManager {
    bool _isLoggedIn;

    bool validatePassword(const std::string& password) {
        return password == "1234";
    }

    public:
    LoginManager() : _isLoggedIn(false) {}

    void login(const std::string& password) {
        _isLoggedIn = validatePassword(password);
    }

    void logout() {
        _isLoggedIn = false;
    }

    bool isLoggedIn() {
        return _isLoggedIn;
    }
};

class FileManager {
    std::map<std::string, std::string> files;

    public:
    bool hasFile(const std::string& name) {
        return files.find(name) != files.end();
    }

    void writeFile(const std::string& name, const std::string& content) {
        files[name] = content;
    }

   void appendFile(const std::string& name, const std::string& content) {
        files[name] += content;
    }

    void deleteFile(const std::string& name) {
        files.erase(name);
    }

    std::string readFile(const std::string& name) {
        return files[name];
    }
};

class TaxCalculator {
    const double taxRate;

    public:
    TaxCalculator(double taxRate) : taxRate(taxRate) {}

    double calculateTax(const double amount) {
        return amount * taxRate;
    }
};

class SystemLogger {
    FileManager& fileManager;

    public:
    SystemLogger(FileManager& fileManager) : fileManager(fileManager) {}

    void log(const std::string& message) {
        fileManager.appendFile(SYSTEM_LOG_FILE, message + "\n");
    }

    std::string getLog() {
        return fileManager.readFile(SYSTEM_LOG_FILE);
    }
};

int main() {
    LoginManager loginManager;
    FileManager fileManager;
    TaxCalculator taxCalculator(0.1);
    SystemLogger systemLogger(fileManager);

    auto exitOperation = [&systemLogger]() {
        std::cout << "System Log:\n" << systemLogger.getLog();
    };

    loginManager.login("1234");
    systemLogger.log("Login Status: " + std::to_string(loginManager.isLoggedIn()));

    if (!loginManager.isLoggedIn()) {
        exitOperation();
        return 1;
    }

    double calculatedTax = taxCalculator.calculateTax(100);
    systemLogger.log("Tax calculated: " + std::to_string(calculatedTax));

    exitOperation();
    return 0;
}
