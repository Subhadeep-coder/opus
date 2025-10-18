#include "authentication.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <unordered_map>

namespace opus
{
    namespace auth
    {

        // Static instance for singleton pattern
        AuthenticationManager &AuthenticationManager::getInstance()
        {
            static AuthenticationManager instance;
            return instance;
        }

        std::string AuthenticationManager::trim(const std::string &str) const
        {
            size_t first = str.find_first_not_of(" \t");
            if (first == std::string::npos)
            {
                return "";
            }
            size_t last = str.find_last_not_of(" \t");
            return str.substr(first, last - first + 1);
        }

        std::unique_ptr<User> AuthenticationManager::authenticateUser(const std::string &configFile, const std::string &target_username) const
        {

            std::ifstream inFile(configFile);
            if (!inFile)
            {
                throw std::runtime_error("Error opening configuration file: " + configFile);
            }

            std::string line;
            bool in_users_section = false;
            std::unique_ptr<User> current_user = nullptr; // reference -> https://en.cppreference.com/w/cpp/memory/unique_ptr.html

            try
            {
                while (std::getline(inFile, line))
                {
                    line = trim(line);

                    if (line == "users:")
                    {
                        in_users_section = true;
                        continue;
                    }

                    if (in_users_section && line.find("- username:") != std::string::npos)
                    {
                        if (current_user)
                        {
                            if (current_user->username == target_username)
                            {
                                return current_user;
                            }
                            current_user.reset();
                        }

                        current_user = std::make_unique<User>(); // reference -> https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique.html#Version_1
                        std::string username = trim(line.substr(line.find(":") + 1));
                        current_user->username = trim(username.substr(1, username.length() - 2));
                    }
                    else if (current_user && line.find("password:") != std::string::npos)
                    {
                        std::string password = trim(line.substr(line.find(":") + 1));
                        current_user->password = trim(password.substr(1, password.length() - 2));
                    }
                    else if (current_user && line.find("role:") != std::string::npos)
                    {
                        std::string role = trim(line.substr(line.find(":") + 1));
                        current_user->role = trim(role.substr(1, role.length() - 2));
                    }
                }

                // Check last user
                if (current_user && current_user->username == target_username)
                {
                    return current_user;
                }
            }
            catch (const std::exception &e)
            {
                throw std::runtime_error("Error parsing configuration file: " + std::string(e.what()));
            }

            return nullptr;
        }

        bool AuthenticationManager::hasPermission(const User &user, const std::string &permission) const
        {
            // TODO: Implement role-based permission checking using the configuration file
            // For now, basic implementation
            if (user.role == "admin")
            {
                return true; // Admins have all permissions
            }

            if (user.role == "standard")
            {
                // Standard users have basic read/write permissions
                return (permission == "read" || permission == "write");
            }

            return false;
        }

    }
}