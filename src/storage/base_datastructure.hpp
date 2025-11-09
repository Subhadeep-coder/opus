#ifndef OPUS_STORAGE_BASE_DATASTRUCTURE_HPP
#define OPUS_STORAGE_BASE_DATASTRUCTURE_HPP

#include <string>

namespace opus
{
    namespace storage
    {

        class BaseDataStructure
        {
        public:
            virtual ~BaseDataStructure() = default;
            virtual std::string getType() const = 0;
        };

    } // namespace storage
} // namespace opus

#endif // OPUS_STORAGE_BASE_DATASTRUCTURE_HPP
