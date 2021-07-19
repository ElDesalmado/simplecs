
#include "simplecs/c_api/c_core.hpp"
#include "simplecs/registry.hpp"

#include <iostream>

class Constructible
{
public:
    Constructible(int c, double d)
        : c(c),
        d(d)
    {
        std::cout << this->c << " " << this->d << std::endl;
    }

    ~Constructible()
    {
        std::cout << this->c << " " << this->d << std::endl;
    }
private:
    int c;
    double d;
};

int main()
{
    auto storage = simplecs::eld::get_component_storage<Constructible>();
    auto vecEmplaced = storage.emplace(std::forward_as_tuple(4, 8.15));

    (void)vecEmplaced.size();

    return 0;
}