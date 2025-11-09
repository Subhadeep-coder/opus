#include "list_type.hpp"

#include <algorithm>

namespace opus
{
    namespace storage
    {

        ListType::ListType() = default;

        ListType::~ListType() = default;

        std::string ListType::getType() const
        {
            return "list";
        }

        int ListType::lpush(const std::string &val)
        {
            values.push_front(val);
            return values.size();
        }

        int ListType::rpush(const std::string &val)
        {
            values.push_back(val);
            return values.size();
        }

        std::optional<std::string> ListType::lpop()
        {
            if (values.empty())
                return std::nullopt;
            std::string val = values.front();
            values.pop_front();
            return val;
        }

        std::optional<std::string> ListType::rpop()
        {
            if (values.empty())
                return std::nullopt;
            std::string val = values.back();
            values.pop_back();
            return val;
        }

        int ListType::llen() const
        {
            return values.size();
        }

        std::vector<std::string> ListType::lrange(int start, int stop) const
        {
            std::vector<std::string> result;
            int size = values.size();

            // Handle negative indices
            if (start < 0)
                start = size + start;
            if (stop < 0)
                stop = size + stop;

            if (start < 0)
                start = 0;
            if (stop >= size)
                stop = size - 1;

            if (start > stop || start >= size)
                return result;

            auto it = values.begin();
            std::advance(it, start);

            for (int i = start; i <= stop && it != values.end(); ++i, ++it)
            {
                result.push_back(*it);
            }

            return result;
        }

        bool ListType::isEmpty() const
        {
            return values.empty();
        }

    } // namespace storage
} // namespace opus
