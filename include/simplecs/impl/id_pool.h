#pragma once

#include <stack>
#include <set>
#include <cassert>
#include <type_traits>

namespace eld::detail
{
    template<typename IdT>
    class id_pool
    {
    public:
        using id_type = IdT;

        id_type next_available()
        {
            if (freed_.empty())
                return instances_++;

            auto idIter = freed_.cbegin();
            const auto id = *idIter;
            freed_.erase(idIter);

            return id;
        }

        void free(const id_type &id)
        {
            // TODO: assertions
            if (instances_ - 1 == id)
            {
                --instances_;
                return;
            }

            freed_.template emplace(id);
        }

        bool is_free(const id_type & id)
        {
            if (id >= instances_)
                return true;

            return freed_.find(id) != freed_.cend();
        }

        bool reserve(const id_type & id)
        {
            if (!is_free(id))
                return false;

            auto findResult = freed_.find(id);
            if(findResult != freed_.cend())
            {
                freed_.erase(findResult);
                return true;
            }

            for(id_type freeId = instances_; freeId < id; ++freeId)
                freed_.emplace(freeId);

            instances_ = id + 1;

            return true;
        }

    private:
        id_type instances_;
        std::set<id_type> freed_;
    };
}   // namespace eld::detail
