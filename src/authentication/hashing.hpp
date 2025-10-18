#ifndef OPUS_HASHING_HPP
#define OPUS_HASHING_HPP

#include <string>

namespace opus
{
    namespace auth
    {
        /**
         * @brief Hashes a password using SHA-256
         * @param password The password string to hash
         * @return The hexadecimal representation of the SHA-256 hash
         */
        std::string hash_password(const std::string& password);
    }
}

#endif