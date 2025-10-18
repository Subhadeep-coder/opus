#include "authentication.hpp"
#include "hashing.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace opus
{
    namespace auth
    {
        // User implementation
        User::User(const std::string &username, const std::string &hashedPassword, const std::string &role)
            : username(username), hashedPassword(hashedPassword), role(role) {}

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

        AuthResult AuthenticationManager::registerUser(const std::string &config_file, const std::string &username, const std::string &plain_password, const std::string &role)
        {
            // Check if user already exists
            std::ifstream checkFile(config_file);
            if (!checkFile)
            {
                throw std::runtime_error("Error opening configuration file: " + config_file);
            }

            std::string line;
            bool userExists = false;

            while (std::getline(checkFile, line))
            {
                if (line.find("username:") != std::string::npos)
                {
                    std::string existingUsername = this->trim(line.substr(line.find(":") + 1));
                    existingUsername = this->trim(existingUsername.substr(1, existingUsername.length() - 2));
                    if (existingUsername == username)
                    {
                        userExists = true;
                        break;
                    }
                }
            }
            checkFile.close();

            if (userExists)
            {
                throw std::runtime_error("User already exists: " + username);
            }

            // Read entire file content
            std::ifstream inFile(config_file);
            if (!inFile)
            {
                throw std::runtime_error("Error opening configuration file: " + config_file);
            }

            std::vector<std::string> fileLines;
            int lastUserLineIndex = -1;
            int currentLine = 0;
            bool inUsersSection = false;

            while (std::getline(inFile, line))
            {
                fileLines.push_back(line);

                std::string trimmedLine = this->trim(line);

                if (trimmedLine == "users:")
                {
                    inUsersSection = true;
                }
                else if (inUsersSection && trimmedLine.find("- username:") != std::string::npos)
                {
                    // Find the role line that follows this username (it's 2 lines after username)
                    lastUserLineIndex = currentLine + 2;
                }
                else if (inUsersSection && !trimmedLine.empty() && trimmedLine[0] != '-' &&
                         trimmedLine.find("username:") == std::string::npos &&
                         trimmedLine.find("password:") == std::string::npos &&
                         trimmedLine.find("role:") == std::string::npos)
                {
                    // We've left the users section
                    inUsersSection = false;
                }

                currentLine++;
            }
            inFile.close();

            if (lastUserLineIndex == -1)
            {
                throw std::runtime_error("Could not find users section in configuration file");
            }

            std::string hashed_password = opus::auth::hash_password(plain_password);

            // Prepare new user entry with proper indentation
            std::vector<std::string> newUserLines = {
                "    - username: \"" + username + "\"",
                "      password: \"" + hashed_password + "\"",
                "      role: \"" + role + "\""};

            // Insert new user after the last user entry
            fileLines.insert(fileLines.begin() + lastUserLineIndex + 1,
                             newUserLines.begin(), newUserLines.end());

            // Write back to file
            std::ofstream outFile(config_file);
            if (!outFile)
            {
                throw std::runtime_error("Error opening configuration file for writing: " + config_file);
            }

            try
            {
                for (const auto &fileLine : fileLines)
                {
                    outFile << fileLine << "\n";
                }
                outFile.close();
            }
            catch (const std::exception &e)
            {
                throw std::runtime_error("Error writing to configuration file: " + std::string(e.what()));
            }

            return AuthResult::SUCCESS;
        }

        std::pair<AuthResult, std::optional<User>> AuthenticationManager::authenticateUser(const std::string &configFile, const std::string &target_username, const std::string &plainPassword) const
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
                            if (current_user->getUsername() == target_username)
                            {
                                if (!verifyPassword(plainPassword, current_user->getHashedPassword()))
                                {
                                    return {AuthResult::INVALID_CREDENTIALS, std::nullopt};
                                }
                                return {AuthResult::SUCCESS, *current_user};
                            }
                            current_user.reset();
                        }

                        // Get username
                        std::string username = trim(line.substr(line.find(":") + 1));
                        std::string cleanUsername = trim(username.substr(1, username.length() - 2));

                        // Read next line for password
                        std::string hashedPassword;
                        if (std::getline(inFile, line))
                        {
                            if (line.find("password:") != std::string::npos)
                            {
                                std::string password = trim(line.substr(line.find(":") + 1));
                                hashedPassword = trim(password.substr(1, password.length() - 2));
                            }
                        }

                        // Read next line for role
                        std::string role;
                        if (std::getline(inFile, line))
                        {
                            if (line.find("role:") != std::string::npos)
                            {
                                std::string roleStr = trim(line.substr(line.find(":") + 1));
                                role = trim(roleStr.substr(1, roleStr.length() - 2));
                            }
                        }

                        // Create user object with all information
                        current_user = std::make_unique<User>(cleanUsername, hashedPassword, role);
                    }
                }

                // Check last user
                if (current_user && current_user->getUsername() == target_username)
                {
                    if (!verifyPassword(plainPassword, current_user->getHashedPassword()))
                    {
                        return {AuthResult::INVALID_CREDENTIALS, std::nullopt};
                    }
                    return {AuthResult::SUCCESS, *current_user};
                }
            }
            catch (const std::exception &e)
            {
                return {AuthResult::SYSTEM_ERROR, std::nullopt};
            }
            return {AuthResult::USER_NOT_FOUND, std::nullopt};
        }

        bool AuthenticationManager::hasPermission(const User &user, const std::string &permission) const
        {
            // Use getter method instead of direct member access
            if (user.getRole() == "admin")
            {
                return true; // Admins have all permissions
            }

            if (user.getRole() == "standard")
            {
                // Standard users have basic read/write permissions
                return (permission == "read" || permission == "write");
            }

            return false;
        }

        bool AuthenticationManager::verifyPassword(
            const std::string &plainPassword,
            const std::string &storedHash) const
        {
            try
            {
                // Hash the provided password and compare with stored hash
                std::string hashedPassword = opus::auth::hash_password(plainPassword);
                return hashedPassword == storedHash;
            }
            catch (const std::exception &)
            {
                return false;
            }
        }
    }
}