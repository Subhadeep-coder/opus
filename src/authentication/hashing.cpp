#include "hashing.hpp"
#include <openssl/evp.h>
#include <iomanip>
#include <sstream>

namespace opus
{
    namespace auth
    {
        std::string hash_password(const std::string& password)
        {
            // Create a new EVP_MD_CTX for SHA-256
            EVP_MD_CTX* ctx = EVP_MD_CTX_new();
            if (ctx == nullptr) {
                throw std::runtime_error("Failed to create hash context");
            }

            // Initialize the context for SHA-256
            if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1) {
                EVP_MD_CTX_free(ctx);
                throw std::runtime_error("Failed to initialize hash context");
            }

            // Update the hash with the password data
            if (EVP_DigestUpdate(ctx, password.c_str(), password.length()) != 1) {
                EVP_MD_CTX_free(ctx);
                throw std::runtime_error("Failed to update hash");
            }

            // Finalize the hash
            unsigned char hash[EVP_MAX_MD_SIZE];
            unsigned int hash_len;
            if (EVP_DigestFinal_ex(ctx, hash, &hash_len) != 1) {
                EVP_MD_CTX_free(ctx);
                throw std::runtime_error("Failed to finalize hash");
            }

            // Clean up the context
            EVP_MD_CTX_free(ctx);

            // Convert the hash to a hexadecimal string
            std::stringstream ss;
            ss << std::hex << std::setfill('0');
            for (unsigned int i = 0; i < hash_len; ++i) {
                ss << std::setw(2) << static_cast<int>(hash[i]);
            }

            return ss.str();
        }
    }
}