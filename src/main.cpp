#include <iostream>
#include <string>
#include <memory>
#include "authentication/authentication.hpp"
#include "help.hpp"

int main(int argc, char *argv[])
{
    if (argc % 2 == 0 || argc < 3)
    {
        std::cerr << "Error: Missing or invalid arguments" << std::endl;
        print_usage();
        return 1;
    }

    std::string host, username;

    // Parse command line arguments
    for (int i = 1; i < argc; i += 2)
    {
        std::string parameter = argv[i];
        std::string value = argv[i + 1];

        if (parameter == "-h")
        {
            host = value;
        }
        else if (parameter == "-U")
        {
            username = value;
        }
        else
        {
            std::cerr << "Error: Invalid argument '" << parameter << "'" << std::endl;
            print_usage();
            return 1;
        }
    }

    // Validate required parameters
    if (host.empty() || username.empty())
    {
        std::cerr << "Error: Both host and username are required" << std::endl;
        print_usage();
        return 1;
    }

    try
    {

        // Command -> opus au -h localhost
        // username: subhadeep
        // password: abcd

        // Command: opus -h localhost -nU shamsemagi

        // Get authentication manager instance
        auto &auth_manager = opus::auth::AuthenticationManager::getInstance();
        // auto regResult = auth_manager.registerUser("config.yaml", "diptadip", "nigga", "standard");
        // if (regResult == opus::auth::AuthResult::SUCCESS)
        // {
        //     std::cout << "User registered successfully!\n";
        // }

        std::string password;
        std::cout << "Enter password for " << username << ": ";
        std::getline(std::cin, password);
        auto [result, user] = auth_manager.authenticateUser("config.yaml", username, password);

        switch (result)
        {
        case opus::auth::AuthResult::SUCCESS:
            std::cout << "Authentication successful!\n";
            if (auth_manager.hasPermission(*user, "read"))
            {
                std::cout << "User has read permission\n";
            }
            break;
        case opus::auth::AuthResult::INVALID_CREDENTIALS:
            std::cerr << "Invalid password!\n";
            return 1;
        case opus::auth::AuthResult::USER_NOT_FOUND:
            std::cerr << "User not found!\n";
            return 1;
        case opus::auth::AuthResult::SYSTEM_ERROR:
            std::cerr << "System error during authentication!\n";
            return 1;
        }

        std::cout << "\nOpus in-memory cache system starting..." << std::endl;

        // TODO: Start server

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
