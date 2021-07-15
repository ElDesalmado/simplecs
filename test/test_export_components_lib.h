#pragma once

#include <simplecs/components.hpp>

struct message_struct
{
    char string[64]{};
};

extern template class eld::impl::component_c<message_struct>;
