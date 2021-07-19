
#include "simplecs/c_api/c_core.hpp"

#include <gtest/gtest.h>

#include <functional>

class TestDestructor
{
public:
    template<typename Callable>
    TestDestructor(Callable &&callable)   //
      : onDestroy_(std::forward<Callable>(callable))
    {
    }

    ~TestDestructor()
    {
        if (onDestroy_)
            onDestroy_();
    }

private:
    std::function<void()> onDestroy_;
};

/**
 * Explicitly deallocate created component
 */
TEST(c_api_component_storage, deallocation_test_explicit)
{
    using namespace eld;

    struct Destructor
    {
        static void destroy(Destructor *destructor, TestDestructor *pObject)
        {
            EXPECT_TRUE(destructor);
            ASSERT_TRUE(pObject);

            pObject->~TestDestructor();
        }
    };

    Destructor destructor;

    const auto storageParams =
        c_api::storage_params{ sizeof(TestDestructor),
                               reinterpret_cast<void (*)(c_api::callable *, c_api::object *)>(
                                   (void *)&Destructor::destroy),
                               reinterpret_cast<c_api::callable *>(&destructor) };

    c_api::type_descriptor newTypeDescriptor{};

    {
        auto error = c_api::init_component_storage(storageParams, newTypeDescriptor, nullptr);
        ASSERT_FALSE(bool(error));
    }

    c_api::component_pointer pointer{};

    {
        auto error = c_api::allocate_component(newTypeDescriptor, pointer);
        ASSERT_FALSE(bool(error));
        EXPECT_EQ(pointer.componentDescriptor.typeDescriptor, newTypeDescriptor);
        EXPECT_EQ(pointer.componentDescriptor.handle, c_api::handle{ 0 });
        ASSERT_TRUE(pointer.pObject);
    }

    int n = 0;
    new (pointer.pObject) TestDestructor([&n]() { n = 42; });

    {
        auto error = c_api::deallocate_component(pointer.componentDescriptor);
        ASSERT_FALSE(bool(error));
    }

    ASSERT_EQ(n, 42);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}