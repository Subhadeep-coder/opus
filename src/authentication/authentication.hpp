#ifndef OPUS_AUTHENTICATION_HPP
#define OPUS_AUTHENTICATION_HPP

#include <string>
#include <memory>
#include <optional>
#include <unordered_map>
#include "hashing.hpp"

namespace opus
{
    namespace auth
    {
        /**
         * @brief Authentication result codes
         */
        enum class AuthResult
        {
            SUCCESS,             // Authentication successful
            INVALID_CREDENTIALS, // Username or password incorrect
            USER_NOT_FOUND,      // User doesn't exist
            SYSTEM_ERROR         // System-level error occurred
        };

        /**
         * @brief Represents a user in the system with authentication and role information
         */
        class User
        {
        public:
            User(const std::string &username, const std::string &hashedPassword, const std::string &role);

            const std::string &getUsername() const { return username; }
            const std::string &getRole() const { return role; }
            const std::string &getHashedPassword() const { return hashedPassword; }

            void setUsername(std ::string username)
            {
                this->username = username;
            }
            void setRole(std ::string role)
            {
                this->role = role;
            }
            void setPassword(std ::string password)
            {
                this->hashedPassword = password;
            }

        private:
            std::string username;       // User's unique identifier
            std::string hashedPassword; // Stored hashed password
            std::string role;           // User's role (admin, standard, etc.)
        };

        /**
         * @brief Authentication manager class to handle user operations
         */
        class AuthenticationManager
        {
        public:
            /**
             * @brief Get the singleton instance of AuthenticationManager
             * @return Reference to the AuthenticationManager instance
             */
            static AuthenticationManager &getInstance();

            /**
             * @brief Register a new user
             * @param username The username to register
             * @param plainPassword The plain text password (will be hashed)
             * @param role The user's role
             * @return AuthResult indicating the result of the registration
             */
            AuthResult registerUser(const std::string &configFile, const std::string &username, const std::string &plainPassword, const std::string &role);

            /**
             * @brief Authenticate a user with username and password
             * @param username The username to authenticate
             * @param plainPassword The plain text password to verify
             * @return pair of AuthResult and optional User object
             */
            std::pair<AuthResult, std::optional<User>> authenticateUser(const std::string &configFile, const std::string &username, const std::string &plainPassword) const;

            /**
             * @brief Check if user has specific permission
             * @param user User to check permissions for
             * @param permission Permission to check
             * @return true if user has permission, false otherwise
             */
            bool hasPermission(const User &user, const std::string &permission) const;

        private:
            AuthenticationManager() = default;
            ~AuthenticationManager() = default;

            AuthenticationManager(const AuthenticationManager &) = delete;
            AuthenticationManager &operator=(const AuthenticationManager &) = delete;

            /**
             * @brief Helper function to trim whitespace from strings
             * @param str String to trim
             * @return Trimmed string
             */
            std::string trim(const std::string &str) const;

            /**
             * @brief Verify if a plain password matches a stored hash
             * @param plainPassword The password to verify
             * @param storedHash The stored hash to compare against
             * @return true if password matches, false otherwise
             */
            bool verifyPassword(const std::string &plainPassword, const std::string &storedHash) const;

            // Add storage for users (in memory for now, could be extended to database)
            std::unordered_map<std::string, User> users;
        };

    } // namespace auth
} // namespace opus

#endif // OPUS_AUTHENTICATION_HPP
