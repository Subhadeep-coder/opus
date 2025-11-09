#include <unordered_map>
#include <memory>
#include <optional>
#include <stdexcept>

#include "base_datastructure.hpp"
#include "string_type.hpp"
#include "list_type.hpp"
#include "set_type.hpp"

namespace opus
{
    namespace storage
    {
        class CacheManager
        {
        private:
            std::unordered_map<std::string, std::unique_ptr<BaseDataStructure>> store;

            template <typename T>
            T *getAs(const std::string &key)
            {
                auto it = store.find(key);
                if (it == store.end())
                {
                    return nullptr;
                }
                T *ptr = dynamic_cast<T *>(it->second.get());
                if (!ptr)
                {
                    throw std::runtime_error("WRONGTYPE: Operation against a key holding the wrong kind of value");
                }
                return ptr;
            }

            template <typename T>
            T *getOrCreate(const std::string &key)
            {
                auto it = store.find(key);
                if (it == store.end())
                {
                    auto newObj = std::make_unique<T>();
                    T *ptr = newObj.get();
                    store[key] = std::move(newObj);
                    return ptr;
                }

                T *ptr = dynamic_cast<T *>(it->second.get());
                if (!ptr)
                {
                    throw std::runtime_error("WRONGTYPE: Operation against a key holding the wrong kind of value");
                }
                return ptr;
            }

        public:
            void set(const std::string &key, const std::string &value)
            {
                store[key] = std::make_unique<StringType>(value);
            }

            std::optional<std::string> get(const std::string &key)
            {
                StringType *str = getAs<StringType>(key);
                if (!str)
                    return std::nullopt;
                return str->get();
            }

            int lpush(const std::string &key, const std::string &value)
            {
                ListType *list = getOrCreate<ListType>(key);
                return list->lpush(value);
            }

            int rpush(const std::string &key, const std::string &value)
            {
                ListType *list = getOrCreate<ListType>(key);
                return list->rpush(value);
            }

            std::optional<std::string> lpop(const std::string &key)
            {
                ListType *list = getAs<ListType>(key);
                if (!list)
                    return std::nullopt;

                auto result = list->lpop();
                if (list->isEmpty())
                {
                    store.erase(key);
                }
                return result;
            }

            std::optional<std::string> rpop(const std::string &key)
            {
                ListType *list = getAs<ListType>(key);
                if (!list)
                    return std::nullopt;

                auto result = list->rpop();
                if (list->isEmpty())
                {
                    store.erase(key);
                }
                return result;
            }

            int llen(const std::string &key)
            {
                ListType *list = getAs<ListType>(key);
                return list ? list->llen() : 0;
            }

            std::vector<std::string> lrange(const std::string &key, int start, int stop)
            {
                ListType *list = getAs<ListType>(key);
                if (!list)
                    return {};
                return list->lrange(start, stop);
            }

            int sadd(const std::string &key, const std::string &value)
            {
                SetType *set = getOrCreate<SetType>(key);
                return set->sadd(value);
            }

            int sadd(const std::string &key, const std::vector<std::string> &values)
            {
                SetType *set = getOrCreate<SetType>(key);
                return set->sadd(values);
            }

            bool sismember(const std::string &key, const std::string &value)
            {
                SetType *set = getAs<SetType>(key);
                return set ? set->sismember(value) : false;
            }

            int srem(const std::string &key, const std::string &value)
            {
                SetType *set = getAs<SetType>(key);
                if (!set)
                    return 0;

                int result = set->srem(value);
                if (set->isEmpty())
                {
                    store.erase(key);
                }
                return result;
            }

            int scard(const std::string &key)
            {
                SetType *set = getAs<SetType>(key);
                return set ? set->scard() : 0;
            }

            std::vector<std::string> smembers(const std::string &key)
            {
                SetType *set = getAs<SetType>(key);
                if (!set)
                    return {};
                return set->smembers();
            }

            bool exists(const std::string &key) const
            {
                return store.find(key) != store.end();
            }

            bool del(const std::string &key)
            {
                return store.erase(key) > 0;
            }

            std::optional<std::string> type(const std::string &key) const
            {
                auto it = store.find(key);
                if (it == store.end())
                    return std::nullopt;
                return it->second->getType();
            }

            void clear()
            {
                store.clear();
            }

            size_t dbsize() const
            {
                return store.size();
            }
        };

    }
}
