#include <iostream>
#include <string>
#include <memory>
#include "authentication/authentication.hpp"
#include "command/parser.hpp"
#include "command/init.hpp"

/**
 * @brief Handles user registration flow
 */
bool handle_registration(const std::string &config_path, const std::string &username, const std::string &host)
{
    std::string password, confirm_password;

    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    std::cout << "Confirm password: ";
    std::getline(std::cin, confirm_password);

    // Validate password match
    if (password != confirm_password)
    {
        std::cerr << "Error: Passwords do not match\n";
        return false;
    }

    // Validate password strength (basic example)
    if (password.length() < 8)
    {
        std::cerr << "Error: Password must be at least 8 characters long\n";
        return false;
    }

    try
    {
        auto &auth_manager = opus::auth::AuthenticationManager::getInstance();
        auto result = auth_manager.registerUser(config_path, username, password, "standard");

        if (result == opus::auth::AuthResult::SUCCESS)
        {
            std::cout << "✓ User registered successfully!\n";
            std::cout << "You can now login with: opus -h " << host
                      << " -U " << username << "\n";
            return true;
        }
        else
        {
            std::cerr << "✗ Registration failed\n";
            return false;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "✗ Registration error: " << e.what() << "\n";
        return false;
    }
}

/**
 * @brief Handles user login flow
 */
bool handle_login(const std::string &config_path, const std::string &username)
{
    std::string password;

    std::cout << "Enter password for " << username << ": ";
    std::getline(std::cin, password);

    try
    {
        auto &auth_manager = opus::auth::AuthenticationManager::getInstance();
        auto [result, user] = auth_manager.authenticateUser(config_path, username, password);

        switch (result)
        {
        case opus::auth::AuthResult::SUCCESS:
            std::cout << "✓ Authentication successful!\n";
            std::cout << "Welcome, " << username << "!\n";

            // Check permissions
            if (auth_manager.hasPermission(*user, "read"))
            {
                std::cout << "Permissions: Read access granted\n";
            }
            return true;

        case opus::auth::AuthResult::INVALID_CREDENTIALS:
            std::cerr << "✗ Invalid password!\n";
            return false;

        case opus::auth::AuthResult::USER_NOT_FOUND:
            std::cerr << "✗ User not found!\n";
            std::cerr << "Hint: Use -nU to register a new user\n";
            return false;

        case opus::auth::AuthResult::SYSTEM_ERROR:
            std::cerr << "✗ System error during authentication!\n";
            return false;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "✗ Authentication error: " << e.what() << "\n";
        return false;
    }

    return false;
}

int main(int argc, char *argv[])
{
    try
    {
        // Initialize and get the parser
        auto parser = opus::command::initialize_parser();

        // Parse command-line arguments
        if (!parser->parse(argc, argv))
        {
            return 1;
        }

        // Validate that either -U or -nU is present
        if (!parser->has("-U") && !parser->has("-nU"))
        {
            std::cerr << "Error: Either -U (login) or -nU (register) must be specified\n";
            parser->print_usage();
            return 1;
        }

        // Extract values
        std::string host = parser->get("-h").value();
        std::string username;
        bool is_registration = false;

        if (parser->has("-nU"))
        {
            username = parser->get("-nU").value();
            is_registration = true;
        }
        else
        {
            username = parser->get("-U").value();
        }

        // Port with default fallback
        int port = 5432; // default from config
        if (auto port_opt = parser->get_as<int>("-p"))
        {
            port = port_opt.value();

            if (port < 1 || port > 65535)
            {
                std::cerr << "Error: Port must be between 1 and 65535\n";
                return 1;
            }
        }

        bool verbose = parser->has("--verbose");

        // Display configuration if verbose
        if (verbose)
        {
            std::cout << "\n=== Connection Configuration ===\n";
            std::cout << "Host: " << host << "\n";
            std::cout << "Port: " << port << "\n";
            std::cout << "Username: " << username << "\n";
            std::cout << "Mode: " << (is_registration ? "Registration" : "Login") << "\n";
            std::cout << "================================\n\n";
        }

        // Path to config file
        const std::string config_path = "config.yaml";

        // Handle registration or login
        bool auth_success = false;

        if (is_registration)
        {
            auth_success = handle_registration(config_path, username, host);
        }
        else
        {
            auth_success = handle_login(config_path, username);
        }

        if (!auth_success)
        {
            return 1;
        }

        // Start the server/application
        std::cout << "\n🚀 Opus in-memory cache system starting...\n";
        std::cout << "Connected to " << host << ":" << port << "\n\n";

        // TODO: Start server logic here
        while (true)
        {
            std::string query;
            std::cout << "opus > ";
            std::getline(std::cin, query);
            if (query == "quit")
            {
                return 0;
            }
        }

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
