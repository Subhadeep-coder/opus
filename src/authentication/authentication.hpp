#ifndef OPUS_AUTHENTICATION_HPP
#define OPUS_AUTHENTICATION_HPP

#include <string>
#include <memory>

namespace opus
{
    namespace auth
    {

        /**
         * @brief Represents a user in the system with authentication and role information
         */
        struct User
        {
            std::string username; // User's unique identifier
            std::string password; // User's password (to be encrypted in future)
            std::string role;     // User's role (admin, standard, etc.)

            User() = default;
            ~User() = default;

            User(const User &) = delete;            // Prevent copying
            User &operator=(const User &) = delete; // Prevent copy assignment

            User(User &&) = default;            // Allow moving
            User &operator=(User &&) = default; // Allow move assignment
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
             * @brief Authenticate a user from configuration file
             * @param configFile Path to the configuration file
             * @param username Username to authenticate
             * @return std::unique_ptr<User> if user is found, nullptr otherwise
             */
            std::unique_ptr<User> authenticateUser(const std::string &configFile, const std::string &username) const;

            /**
             * @brief Check if user has specific permission
             * @param user User to check permissions for
             * @param permission Permission to check
             * @return true if user has permission, false otherwise
             */
            bool hasPermission(const User &user, const std::string &permission) const;

        private:
            AuthenticationManager() = default; // Private constructor for singleton
            ~AuthenticationManager() = default;

            AuthenticationManager(const AuthenticationManager &) = delete;
            AuthenticationManager &operator=(const AuthenticationManager &) = delete;

            /**
             * @brief Helper function to trim whitespace from strings
             * @param str String to trim
             * @return Trimmed string
             */
            std::string trim(const std::string &str) const;
        };

    } // namespace auth
} // namespace opus

#endif // OPUS_AUTHENTICATION_HPP
