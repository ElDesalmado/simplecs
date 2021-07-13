
#include "c_core.h"

#include <algorithm>
#include <unordered_map>
#include <vector>
#include <iterator>

namespace eld::internal
{
    using entity_id = size_t;
    using component_id = size_t;

    /**
     * TODO:
     * - select from table
     * - get all components
     */

    /**
     * TODO:
     */
    class registry
    {
    public:
        using component_column = std::vector<entity_id>;

        std::vector<entity_id> select(const component_id *idArray, size_t arraySz)
        {
            // naive select implementation

            if (!idArray ||
                !arraySz ||
                componentTable_.empty())
                return {};

            using difference_type = std::iterator_traits<decltype(idArray)>::difference_type;

            auto inputBegin = idArray,
                 inputEnd = std::next(inputBegin, difference_type(arraySz));

            std::vector<component_id> input(inputBegin, inputEnd);
            std::sort(input.begin(), input.end());

            std::vector<component_column *> columns;

            // filter columns
            std::for_each(input.cbegin(), input.cend(),//
                          [this, columns](const component_id &componentId) mutable {
                              auto found = componentTable_.find(componentId);
                              if (found == componentTable_.cend())
                                  return;
                              columns.push_back(std::addressof(found->second));
                          });

            if (columns.empty())
                return {};

            // find intersections
            std::sort(columns.begin(), columns.end(), [](const component_column *lhs, const component_column *rhs) {
                return lhs->size() < rhs->size();
            });

            std::vector<entity_id> selected{};
//            auto *prevColumn = *columns.cbegin();
//
//            std::for_each(columns.cbegin(), columns.cend(), [&selected](const auto &column) {
//                std::vector<entity_id> temp{};
//                temp.reserve(selected.size());
//
//                std::set_intersection(prevColumn->cbegin(), prevColumn->cend(),
//                                      )
//
//            });
            return selected;
        }

    private:
        static registry registry_;

        std::unordered_map<component_id, component_column> componentTable_;
    };

    registry registry::registry_{};

}// namespace eld::internal
