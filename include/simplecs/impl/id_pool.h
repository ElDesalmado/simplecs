#pragma once

#include <stack>
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

            const auto id = freed_.top();
            freed_.pop();
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

        bool is_free(const id_type &/*id*/) { return false; }

        bool reserve(const id_type &/*id*/) { return false; }

    private:
        id_type instances_;
        std::stack<id_type> freed_;
    };
}   // namespace eld::detail
