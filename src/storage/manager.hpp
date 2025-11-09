// Storage abstraction for opus
#ifndef OPUS_STORAGE_MANAGER_HPP
#define OPUS_STORAGE_MANAGER_HPP

#include <memory>
#include <string>
#include <system_error>
#include <vector>

namespace opus
{
    namespace storage
    {

        class IStorage
        {
        public:
            virtual ~IStorage() = default;

            // Save data for a key. On failure ec is set and the function returns false.
            virtual bool save(const std::string &key, const std::string &data, std::error_code &ec) = 0;

            // Load data for a key. Returns false and sets ec on failure.
            virtual bool load(const std::string &key, std::string &out, std::error_code &ec) = 0;

            // Remove a key. Returns true if removed or false + ec on error.
            virtual bool remove(const std::string &key, std::error_code &ec) = 0;

            // List keys under an optional prefix. Returns empty vector + ec on error.
            virtual std::vector<std::string> list(const std::string &prefix, std::error_code &ec) = 0;
        };

        // Factory for the filesystem-backed implementation.
        // `basePath` is the directory under which keys are stored. Keys are treated as
        // relative paths (sanitized) so "user/123" => $basePath/user/123
        std::unique_ptr<IStorage> make_filesystem_storage(const std::string &basePath);

    } // namespace storage
} // namespace opus

#endif // OPUS_STORAGE_MANAGER_HPP
