#include "string_type.hpp"

#include <utility>

namespace opus
{
    namespace storage
    {

        StringType::StringType() = default;

        StringType::StringType(const std::string &val) : value(val) {}

        StringType::~StringType() = default;

        std::string StringType::getType() const
        {
            return "string";
        }

        void StringType::set(const std::string &val)
        {
            value = val;
        }

        std::string StringType::get() const
        {
            return value;
        }

    } // namespace storage
} // namespace opus
