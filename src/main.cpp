#include <iostream>
#include <string>
#include <memory>
#include "authentication/authentication.hpp"
#include "help.hpp"

int main(int argc, char *argv[]) {
    if (argc % 2 == 0 || argc < 3) {
        std::cerr << "Error: Missing or invalid arguments" << std::endl;
        print_usage();
        return 1;
    }

    std::string host, username;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i += 2) {
        std::string parameter = argv[i];
        std::string value = argv[i + 1];
        
        if (parameter == "-h") {
            host = value;
        }
        else if (parameter == "-U") {
            username = value;
        }
        else {
            std::cerr << "Error: Invalid argument '" << parameter << "'" << std::endl;
            print_usage();
            return 1;
        }
    }

    // Validate required parameters
    if (host.empty() || username.empty()) {
        std::cerr << "Error: Both host and username are required" << std::endl;
        print_usage();
        return 1;
    }

    try {
        // Get authentication manager instance
        auto& auth_manager = opus::auth::AuthenticationManager::getInstance();
        
        auto user = auth_manager.authenticateUser("config.yaml", username);
        
        if (!user) {
            std::cerr << "Error: Authentication failed for user '" << username << "'" << std::endl;
            return 1;
        }

        // Authentication successful
        std::cout << "Authentication successful!" << std::endl;
        std::cout << "User: " << user->username << std::endl;
        std::cout << "Role: " << user->role << std::endl;

        // Check basic permissions
        if (auth_manager.hasPermission(*user, "write")) {
            std::cout << "User has write permission" << std::endl;
        }
        
        std::cout << "\nOpus in-memory cache system starting..." << std::endl;
        std::cout << "Connected to: " << host << std::endl;

        // TODO: Start server

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
