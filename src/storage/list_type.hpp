#ifndef OPUS_STORAGE_LIST_TYPE_HPP
#define OPUS_STORAGE_LIST_TYPE_HPP

#include "base_datastructure.hpp"
#include <string>
#include <list>
#include <vector>
#include <optional>

namespace opus
{
    namespace storage
    {

        class ListType : public BaseDataStructure
        {
        private:
            std::list<std::string> values;

        public:
            ListType();
            ~ListType() override;

            std::string getType() const override;

            int lpush(const std::string &val);
            int rpush(const std::string &val);

            std::optional<std::string> lpop();
            std::optional<std::string> rpop();

            int llen() const;
            std::vector<std::string> lrange(int start, int stop) const;

            bool isEmpty() const;
        };

    } // namespace storage
} // namespace opus

#endif // OPUS_STORAGE_LIST_TYPE_HPP
