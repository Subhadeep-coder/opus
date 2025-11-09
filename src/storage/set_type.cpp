#include "set_type.hpp"

#include <algorithm>

namespace opus
{
    namespace storage
    {

        SetType::SetType() = default;

        SetType::~SetType() = default;

        std::string SetType::getType() const
        {
            return "set";
        }

        int SetType::sadd(const std::string &value)
        {
            auto result = values.insert(value);
            return result.second ? 1 : 0;
        }

        int SetType::sadd(const std::vector<std::string> &members)
        {
            int count = 0;
            for (const auto &member : members)
            {
                count += sadd(member);
            }
            return count;
        }

        bool SetType::sismember(const std::string &value) const
        {
            return values.find(value) != values.end();
        }

        int SetType::srem(const std::string &value)
        {
            return values.erase(value);
        }

        int SetType::scard() const
        {
            return values.size();
        }

        std::vector<std::string> SetType::smembers() const
        {
            return std::vector<std::string>(values.begin(), values.end());
        }

        bool SetType::isEmpty() const
        {
            return values.empty();
        }

    } // namespace storage
} // namespace opus
