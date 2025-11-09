#ifndef OPUS_STORAGE_SET_TYPE_HPP
#define OPUS_STORAGE_SET_TYPE_HPP

#include "base_datastructure.hpp"
#include <string>
#include <unordered_set>
#include <vector>

namespace opus
{
    namespace storage
    {

        class SetType : public BaseDataStructure
        {
        private:
            std::unordered_set<std::string> values;

        public:
            SetType();
            ~SetType() override;

            std::string getType() const override;

            int sadd(const std::string &value);
            int sadd(const std::vector<std::string> &members);

            bool sismember(const std::string &value) const;
            int srem(const std::string &value);
            int scard() const;
            std::vector<std::string> smembers() const;

            bool isEmpty() const;
        };

    } // namespace storage
} // namespace opus

#endif // OPUS_STORAGE_SET_TYPE_HPP
