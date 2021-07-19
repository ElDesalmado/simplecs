#include "simplecs/generic/registry.h"

#include <gtest/gtest.h>
#include <vector>

namespace
{
    const int testVal = 2;
    const std::string testString = "tmp";

}   // namespace

struct entity_type
{
    using descriptor = size_t;
    descriptor descriptor_ = 0;
};
struct Component
{
    size_t id = 0;
};

struct Impl
{
    template<typename... ComponentsT>
    using selection_type = std::tuple<ComponentsT &...>;

    Impl(int val, const std::string &tmp)
    {
        EXPECT_EQ(val, ::testVal);
        EXPECT_EQ(tmp, ::testString);
    }
    entity_type create_entity() { return { 1 }; };
    entity_type create_entity(const entity_type::descriptor &descriptor) { return { descriptor }; };
    template<typename... ComponentsT>
    selection_type<Component> add_components(const entity_type::descriptor &,
                                             ComponentsT &&...components)
    {
        _components = { components... };
        auto result = vectorToTuple<1>(_components);
        return result;
    }

private:
    std::vector<Component> _components;

private:
    template<typename T, std::size_t... Indices>
    selection_type<Component> vectorToTupleHelper(std::vector<T> &v,
                                                  std::index_sequence<Indices...>)
    {
        return std::make_tuple(std::ref(v[Indices])...);
    }

    template<std::size_t N, typename T>
    selection_type<Component> vectorToTuple(std::vector<T> &v)
    {
        assert(v.size() >= N);
        return vectorToTupleHelper(v, std::make_index_sequence<N>());
    }
};

enum class entity_descriptor
{
};

struct Traits
{
    using entity_type = ::entity_type;
};

class TestRegistry : public testing::Test
{
protected:
    void SetUp() override { Test::SetUp(); }
    void TearDown() override { Test::TearDown(); }
    simplecs::generic::registry<Impl, Traits> registry =
        simplecs::make_registry<Impl, Traits>(::testVal, ::testString);
};

TEST_F(TestRegistry, create_entity_empty)
{
    auto entity = registry.create_entity();
    EXPECT_EQ(entity.descriptor_, 1);
}

TEST_F(TestRegistry, create_entity_entity_descriptor)
{
    auto entity = registry.create_entity(3);
    EXPECT_EQ(entity.descriptor_, 3);
}

TEST_F(TestRegistry, create_entity_add_ten_components)
{
    auto entity = registry.create_entity(3);
    auto components = registry.add_components(entity.descriptor_, Component{ 1 });
    ASSERT_EQ(std::tuple_size_v<decltype(components)>, 1);
    Component component1;
    std::tie(component1) = components;
    EXPECT_EQ(component1.id, 1);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
