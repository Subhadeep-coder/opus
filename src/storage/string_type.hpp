#ifndef OPUS_STORAGE_STRING_TYPE_HPP
#define OPUS_STORAGE_STRING_TYPE_HPP

#include "base_datastructure.hpp"
#include <string>

namespace opus
{
    namespace storage
    {

        class StringType : public BaseDataStructure
        {
        private:
            std::string value;

        public:
            StringType();
            explicit StringType(const std::string &val);
            ~StringType() override;

            std::string getType() const override;

            void set(const std::string &val);
            std::string get() const;
        };

    } // namespace storage
} // namespace opus

#endif
