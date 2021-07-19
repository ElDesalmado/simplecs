//
//#include "simplecs/c_api/c_core.hpp"
//
#include <gtest/gtest.h>
//
//#include <vector>
//
//using namespace eld::c_api;
//
//TEST(c_api_registry, reg_components_empty)
//{
//    entity_descriptor entity{ 0 };
//    std::vector<component_descriptor> componentsEntity{};
//    std::vector<reg_error> regResult{};
//
////    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//
//    ASSERT_EQ(regResult, std::vector<reg_error>(componentsEntity.size(), reg_error::success));
//}
//
//TEST(c_api_registry, reg_components_one_component_descriptor_nullptr)
//{
//    entity_descriptor entity{ 0 };
//    std::vector<reg_error> regResult{};
////    ASSERT_EXIT((register_components(entity, nullptr, 1, regResult.data()), exit(0)),
////                ::testing::ExitedWithCode(0),
////                ".*");
//}
//
//TEST(c_api_registry, reg_components_one_component_reg_error_nullptr)
//{
//    entity_descriptor entity{ 0 };
//    std::vector<component_descriptor> componentsEntity{};
////    ASSERT_EXIT((register_components(entity, componentsEntity.data(), 1, nullptr), exit(0)),
////                ::testing::ExitedWithCode(0),
////                ".*");
//}
//
//TEST(c_api_registry, reg_components_one)
//{
//    const size_t componentSize = 1;
//    entity_descriptor entity{ 0 };
//    std::vector<component_descriptor> componentsEntity{ component_descriptor{ 0 } };
//    std::vector<reg_error> regResult(componentSize);
//
////    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//
//    ASSERT_EQ(regResult, std::vector<reg_error>(componentsEntity.size(), reg_error::success));
//}
//
//TEST(c_api_registry, reg_components_ten_success)
//{
//    // TODO: clear for all tests from the bottom
//    const size_t componentSize = 10;
//    entity_descriptor entity{ 0 };
//
//    std::vector<component_descriptor> componentsEntity{};
//    componentsEntity.reserve(componentSize);
//    for (size_t i = 0; i < componentSize; ++i)
//    {
//        componentsEntity.push_back(component_descriptor{ i });
//    }
//    std::vector<reg_error> regResult(componentSize);
//
////    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//
//    ASSERT_EQ(regResult, std::vector<reg_error>(componentsEntity.size(), reg_error::success));
//}
//
//TEST(c_api_registry, reg_components_ten_already_registered)
//{
//    entity_descriptor entity{ 0 };
//    auto componentDescriptor = std::make_unique<component_descriptor>();
//    componentDescriptor->typeId = 1;
//    register_components(entity, componentDescriptor.get(), 1, nullptr);
//
//    const size_t componentSize = 10;
//
//    std::vector<component_descriptor> componentsEntity(componentSize, component_descriptor{ 1 });
//    std::vector<reg_error> regResult(componentSize);
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//
//    ASSERT_EQ(
//        regResult,
//        std::vector<reg_error>(componentsEntity.size(), reg_error::component_already_registered));
//}
//
//TEST(c_api_registry, reg_components_ten_two_already_registered)
//{
//    entity_descriptor entity{ 0 };
//    auto componentDescriptor = std::make_unique<component_descriptor>();
//    componentDescriptor->typeId = 0;
//    register_components(entity, componentDescriptor.get(), 1, nullptr);
//
//    const size_t componentSize = 10;
//    size_t iter = 1;
//    std::vector<component_descriptor> componentsEntity = {
//        component_descriptor{ 0 },      component_descriptor{ 0 },
//        component_descriptor{ ++iter }, component_descriptor{ ++iter },
//        component_descriptor{ ++iter }, component_descriptor{ ++iter },
//        component_descriptor{ ++iter }, component_descriptor{ ++iter },
//        component_descriptor{ ++iter }, component_descriptor{ ++iter }
//    };
//    std::vector<reg_error> regResult(componentSize);
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//    auto error = reg_error::component_already_registered;
//    auto success = reg_error::success;
//
//    std::vector<reg_error> expect = { error,   error,   success, success, success,
//                                      success, success, success, success, success };
//    ASSERT_EQ(regResult, expect);
//
//    componentsEntity = { component_descriptor{ ++iter }, component_descriptor{ ++iter },
//                         component_descriptor{ 0 },      component_descriptor{ 0 },
//                         component_descriptor{ ++iter }, component_descriptor{ ++iter },
//                         component_descriptor{ ++iter }, component_descriptor{ ++iter },
//                         component_descriptor{ ++iter }, component_descriptor{ ++iter } };
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//
//    expect = {
//        success, success, error, error, success, success, success, success, success, success
//    };
//    ASSERT_EQ(regResult, expect);
//
//    componentsEntity = { component_descriptor{ ++iter }, component_descriptor{ ++iter },
//                         component_descriptor{ ++iter }, component_descriptor{ ++iter },
//                         component_descriptor{ 0 },      component_descriptor{ 0 },
//                         component_descriptor{ ++iter }, component_descriptor{ ++iter },
//                         component_descriptor{ ++iter }, component_descriptor{ ++iter } };
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//
//    expect = {
//        success, success, success, success, error, error, success, success, success, success
//    };
//    ASSERT_EQ(regResult, expect);
//
//    componentsEntity = { component_descriptor{ ++iter }, component_descriptor{ ++iter },
//                         component_descriptor{ ++iter }, component_descriptor{ ++iter },
//                         component_descriptor{ ++iter }, component_descriptor{ ++iter },
//                         component_descriptor{ 0 },      component_descriptor{ 0 },
//                         component_descriptor{ ++iter }, component_descriptor{ ++iter } };
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//
//    expect = {
//        success, success, success, success, success, success, error, error, success, success
//    };
//    ASSERT_EQ(regResult, expect);
//
//    componentsEntity = { component_descriptor{ ++iter }, component_descriptor{ ++iter },
//                         component_descriptor{ ++iter }, component_descriptor{ ++iter },
//                         component_descriptor{ ++iter }, component_descriptor{ ++iter },
//                         component_descriptor{ ++iter }, component_descriptor{ ++iter },
//                         component_descriptor{ 0 },      component_descriptor{ 0 } };
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//
//    expect = {
//        success, success, success, success, success, success, success, success, error, error
//    };
//    ASSERT_EQ(regResult, expect);
//}
//
//TEST(c_api_registry, unreg_components_empty)
//{
//    entity_descriptor entity{ 0 };
//    std::vector<component_descriptor> componentsEntity{};
//    std::vector<unreg_error> unregResult{};
//
//    unregister_components(entity,
//                          componentsEntity.data(),
//                          componentsEntity.size(),
//                          unregResult.data());
//
//    ASSERT_EQ(unregResult, std::vector<unreg_error>(componentsEntity.size(), unreg_error::success));
//}
//
//TEST(c_api_registry, unreg_components_one_component_descriptor_nullptr)
//{
//    entity_descriptor entity{ 0 };
//    std::vector<unreg_error> unregResult{};
//    ASSERT_EXIT((unregister_components(entity, nullptr, 1, unregResult.data()), exit(0)),
//                ::testing::ExitedWithCode(0),
//                ".*");
//}
//
//TEST(c_api_registry, unreg_components_one_component_reg_error_nullptr)
//{
//    entity_descriptor entity{ 0 };
//    std::vector<component_descriptor> componentsEntity{};
//    ASSERT_EXIT((register_components(entity, componentsEntity.data(), 1, nullptr), exit(0)),
//                ::testing::ExitedWithCode(0),
//                ".*");
//}
//
//TEST(c_api_registry, unreg_components_one_success)
//{
//    const size_t componentSize = 1;
//    entity_descriptor entity{ 0 };
//    std::vector<component_descriptor> componentsEntity{ component_descriptor{ 0 } };
//    std::vector<reg_error> regResult(componentSize);
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//
//    std::vector<unreg_error> unregResult(componentSize);
//
//    unregister_components(entity,
//                          componentsEntity.data(),
//                          componentsEntity.size(),
//                          unregResult.data());
//
//    ASSERT_EQ(unregResult, std::vector<unreg_error>(componentsEntity.size(), unreg_error::success));
//}
//
//TEST(c_api_registry, unreg_components_one_component_not_found)
//{
//    const size_t componentSize = 1;
//    entity_descriptor entity{ 0 };
//    std::vector<component_descriptor> componentsEntity{ component_descriptor{ 0 } };
//    std::vector<reg_error> regResult(componentSize);
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//
//    std::vector<unreg_error> unregResult(componentSize);
//
//    auto component = component_descriptor{ 1 };
//    unregister_components(entity, &component, componentsEntity.size(), unregResult.data());
//
//    ASSERT_EQ(unregResult,
//              std::vector<unreg_error>(componentsEntity.size(), unreg_error::component_not_found));
//}
//
//TEST(c_api_registry, unreg_components_one_component_not_registered)
//{
//    const size_t componentSize = 1;
//    entity_descriptor entity{ 0 };
//    std::vector<component_descriptor> componentsEntity{ component_descriptor{ 0 } };
//    std::vector<reg_error> regResult(componentSize);
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//
//    entity_descriptor entity2{ 1 };
//    componentsEntity = { component_descriptor{ 1 } };
//    register_components(entity2,
//                        componentsEntity.data(),
//                        componentsEntity.size(),
//                        regResult.data());
//
//    std::vector<unreg_error> unregResult(componentSize);
//    unregister_components(entity,
//                          componentsEntity.data(),
//                          componentsEntity.size(),
//                          unregResult.data());
//
//    ASSERT_EQ(
//        unregResult,
//        std::vector<unreg_error>(componentsEntity.size(), unreg_error::component_not_registered));
//}
//
//TEST(c_api_registry, unreg_components_ten_success)
//{
//    const size_t componentSize = 10;
//    entity_descriptor entity{ 0 };
//
//    std::vector<component_descriptor> componentsEntity{};
//    componentsEntity.reserve(componentSize);
//    for (size_t i = 0; i < componentSize; ++i)
//    {
//        componentsEntity.push_back(component_descriptor{ i });
//    }
//    std::vector<reg_error> regResult(componentSize);
//
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//
//    ASSERT_EQ(regResult, std::vector<reg_error>(componentsEntity.size(), reg_error::success));
//    std::vector<unreg_error> unregResult(componentSize);
//    unregister_components(entity,
//                          componentsEntity.data(),
//                          componentsEntity.size(),
//                          unregResult.data());
//    ASSERT_EQ(unregResult, std::vector<unreg_error>(componentsEntity.size(), unreg_error::success));
//}
//
//TEST(c_api_registry, unreg_components_ten_component_not_found)
//{
//    const size_t componentSize = 10;
//    entity_descriptor entity{ 0 };
//
//    std::vector<component_descriptor> componentsEntity{};
//    componentsEntity.reserve(componentSize);
//    for (size_t i = 0; i < componentSize; ++i)
//    {
//        componentsEntity.push_back(component_descriptor{ i });
//    }
//    std::vector<reg_error> regResult(componentSize);
//
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//
//    ASSERT_EQ(regResult, std::vector<reg_error>(componentsEntity.size(), reg_error::success));
//    std::vector<unreg_error> unregResult(componentSize);
//    std::vector<component_descriptor> unregComponentsEntity{};
//    unregComponentsEntity.reserve(componentSize);
//    for (size_t i = 10; i < 20; ++i)
//    {
//        unregComponentsEntity.push_back(component_descriptor{ i });
//    }
//    unregister_components(entity,
//                          unregComponentsEntity.data(),
//                          unregComponentsEntity.size(),
//                          unregResult.data());
//    ASSERT_EQ(
//        unregResult,
//        std::vector<unreg_error>(unregComponentsEntity.size(), unreg_error::component_not_found));
//}
//
//TEST(c_api_registry, unreg_components_ten_component_not_registered)
//{
//    const size_t componentSize = 10;
//    entity_descriptor entity{ 0 };
//
//    std::vector<component_descriptor> componentsEntity{};
//    componentsEntity.reserve(componentSize);
//    for (size_t i = 0; i < componentSize; ++i)
//    {
//        componentsEntity.push_back(component_descriptor{ i });
//    }
//    std::vector<reg_error> regResult(componentSize);
//
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//
//    ASSERT_EQ(regResult, std::vector<reg_error>(componentsEntity.size(), reg_error::success));
//
//    entity_descriptor entity2{ 1 };
//    std::vector<component_descriptor> componentsEntity2{};
//    componentsEntity2.reserve(componentSize);
//    for (size_t i = 10; i < 20; ++i)
//    {
//        componentsEntity2.push_back(component_descriptor{ i });
//    }
//    register_components(entity2,
//                        componentsEntity2.data(),
//                        componentsEntity2.size(),
//                        regResult.data());
//    ASSERT_EQ(regResult, std::vector<reg_error>(componentsEntity2.size(), reg_error::success));
//
//    std::vector<unreg_error> unregResult(componentSize);
//    unregister_components(entity,
//                          componentsEntity2.data(),
//                          componentsEntity2.size(),
//                          unregResult.data());
//    ASSERT_EQ(
//        unregResult,
//        std::vector<unreg_error>(componentsEntity2.size(), unreg_error::component_not_registered));
//}
//
//TEST(c_api_registry, reg_components_ten_two_component_not_found)
//{
//    const size_t componentSize = 10;
//    entity_descriptor entity{ 0 };
//    std::vector<component_descriptor> componentsEntity{};
//    componentsEntity.reserve(componentSize);
//    for (size_t i = 0; i < componentSize; ++i)
//    {
//        componentsEntity.push_back(component_descriptor{ i });
//    }
//    std::vector<reg_error> regResult(componentSize);
//
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//
//    ASSERT_EQ(regResult, std::vector<reg_error>(componentsEntity.size(), reg_error::success));
//
//    std::vector<unreg_error> unregResult(componentSize);
//    std::vector<component_descriptor> unregComponentsEntity = {
//        component_descriptor{ 11 }, component_descriptor{ 12 }, component_descriptor{ 2 },
//        component_descriptor{ 3 },  component_descriptor{ 4 },  component_descriptor{ 5 },
//        component_descriptor{ 6 },  component_descriptor{ 7 },  component_descriptor{ 8 },
//        component_descriptor{ 9 }
//    };
//    unregister_components(entity,
//                          unregComponentsEntity.data(),
//                          unregComponentsEntity.size(),
//                          unregResult.data());
//    auto success = unreg_error::success;
//    auto error = unreg_error::component_not_found;
//    std::vector<unreg_error> expect(unregComponentsEntity.size(), success);
//    expect[0] = error;
//    expect[1] = error;
//    ASSERT_EQ(unregResult, expect);
//
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//    unregComponentsEntity[0] = component_descriptor{ 0 };
//    unregComponentsEntity[1] = component_descriptor{ 1 };
//    unregComponentsEntity[2] = component_descriptor{ 11 };
//    unregComponentsEntity[3] = component_descriptor{ 12 };
//    unregister_components(entity,
//                          unregComponentsEntity.data(),
//                          unregComponentsEntity.size(),
//                          unregResult.data());
//    expect[0] = success;
//    expect[1] = success;
//    expect[2] = error;
//    expect[3] = error;
//    ASSERT_EQ(unregResult, expect);
//
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//    unregComponentsEntity[2] = component_descriptor{ 2 };
//    unregComponentsEntity[3] = component_descriptor{ 3 };
//    unregComponentsEntity[4] = component_descriptor{ 11 };
//    unregComponentsEntity[5] = component_descriptor{ 12 };
//    unregister_components(entity,
//                          unregComponentsEntity.data(),
//                          unregComponentsEntity.size(),
//                          unregResult.data());
//    expect[2] = success;
//    expect[3] = success;
//    expect[4] = error;
//    expect[5] = error;
//    ASSERT_EQ(unregResult, expect);
//
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//    unregComponentsEntity[4] = component_descriptor{ 4 };
//    unregComponentsEntity[5] = component_descriptor{ 5 };
//    unregComponentsEntity[6] = component_descriptor{ 11 };
//    unregComponentsEntity[7] = component_descriptor{ 12 };
//    unregister_components(entity,
//                          unregComponentsEntity.data(),
//                          unregComponentsEntity.size(),
//                          unregResult.data());
//    expect[4] = success;
//    expect[5] = success;
//    expect[6] = error;
//    expect[7] = error;
//    ASSERT_EQ(unregResult, expect);
//
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//    unregComponentsEntity[6] = component_descriptor{ 6 };
//    unregComponentsEntity[7] = component_descriptor{ 7 };
//    unregComponentsEntity[8] = component_descriptor{ 11 };
//    unregComponentsEntity[9] = component_descriptor{ 12 };
//    unregister_components(entity,
//                          unregComponentsEntity.data(),
//                          unregComponentsEntity.size(),
//                          unregResult.data());
//    expect[6] = success;
//    expect[7] = success;
//    expect[8] = error;
//    expect[9] = error;
//    ASSERT_EQ(unregResult, expect);
//}
//
//TEST(c_api_registry, reg_components_ten_component_not_registered)
//{
//    const size_t componentSize = 10;
//    entity_descriptor entity{ 0 };
//    std::vector<component_descriptor> componentsEntity{};
//    componentsEntity.reserve(componentSize);
//    for (size_t i = 0; i < componentSize; ++i)
//    {
//        componentsEntity.push_back(component_descriptor{ i });
//    }
//    std::vector<reg_error> regResult(componentSize);
//
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//    ASSERT_EQ(regResult, std::vector<reg_error>(componentsEntity.size(), reg_error::success));
//
//    entity_descriptor entity2{ 1 };
//    std::vector<component_descriptor> componentsEntity2{};
//    componentsEntity2.reserve(componentSize);
//    for (size_t i = 10; i < 12; ++i)
//    {
//        componentsEntity2.push_back(component_descriptor{ i });
//    }
//    register_components(entity2,
//                        componentsEntity2.data(),
//                        componentsEntity2.size(),
//                        regResult.data());
//
//    std::vector<unreg_error> unregResult(componentSize);
//    std::vector<component_descriptor> unregComponentsEntity = {
//        component_descriptor{ 10 }, component_descriptor{ 11 }, component_descriptor{ 2 },
//        component_descriptor{ 3 },  component_descriptor{ 4 },  component_descriptor{ 5 },
//        component_descriptor{ 6 },  component_descriptor{ 7 },  component_descriptor{ 8 },
//        component_descriptor{ 9 }
//    };
//    unregister_components(entity,
//                          unregComponentsEntity.data(),
//                          unregComponentsEntity.size(),
//                          unregResult.data());
//    auto success = unreg_error::success;
//    auto error = unreg_error::component_not_registered;
//    std::vector<unreg_error> expect(unregComponentsEntity.size(), success);
//    expect[0] = error;
//    expect[1] = error;
//    ASSERT_EQ(unregResult, expect);
//
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//    unregComponentsEntity[0] = component_descriptor{ 0 };
//    unregComponentsEntity[1] = component_descriptor{ 1 };
//    unregComponentsEntity[2] = component_descriptor{ 10 };
//    unregComponentsEntity[3] = component_descriptor{ 11 };
//    unregister_components(entity,
//                          unregComponentsEntity.data(),
//                          unregComponentsEntity.size(),
//                          unregResult.data());
//    expect[0] = success;
//    expect[1] = success;
//    expect[2] = error;
//    expect[3] = error;
//    ASSERT_EQ(unregResult, expect);
//
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//    unregComponentsEntity[2] = component_descriptor{ 2 };
//    unregComponentsEntity[3] = component_descriptor{ 3 };
//    unregComponentsEntity[4] = component_descriptor{ 10 };
//    unregComponentsEntity[5] = component_descriptor{ 11 };
//    unregister_components(entity,
//                          unregComponentsEntity.data(),
//                          unregComponentsEntity.size(),
//                          unregResult.data());
//    expect[2] = success;
//    expect[3] = success;
//    expect[4] = error;
//    expect[5] = error;
//    ASSERT_EQ(unregResult, expect);
//
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//    unregComponentsEntity[4] = component_descriptor{ 4 };
//    unregComponentsEntity[5] = component_descriptor{ 5 };
//    unregComponentsEntity[6] = component_descriptor{ 10 };
//    unregComponentsEntity[7] = component_descriptor{ 11 };
//    unregister_components(entity,
//                          unregComponentsEntity.data(),
//                          unregComponentsEntity.size(),
//                          unregResult.data());
//    expect[4] = success;
//    expect[5] = success;
//    expect[6] = error;
//    expect[7] = error;
//    ASSERT_EQ(unregResult, expect);
//
//    register_components(entity, componentsEntity.data(), componentsEntity.size(), regResult.data());
//    unregComponentsEntity[6] = component_descriptor{ 6 };
//    unregComponentsEntity[7] = component_descriptor{ 7 };
//    unregComponentsEntity[8] = component_descriptor{ 10 };
//    unregComponentsEntity[9] = component_descriptor{ 11 };
//    unregister_components(entity,
//                          unregComponentsEntity.data(),
//                          unregComponentsEntity.size(),
//                          unregResult.data());
//    expect[6] = success;
//    expect[7] = success;
//    expect[8] = error;
//    expect[9] = error;
//    ASSERT_EQ(unregResult, expect);
//}
//
///**
// * 1. Register components for several entities
// * 2. Select all entities with a set of components
// */
//TEST(c_api_registry, selec_entities)
//{
//    using namespace eld::c_api;
//
//    std::vector<component_descriptor> componentsEntity0{ component_descriptor{ 0 },   //
//                                                         component_descriptor{ 1 },   //
//                                                         component_descriptor{ 2 },   //
//                                                         component_descriptor{ 3 },   //
//                                                         component_descriptor{ 4 },   //
//                                                         component_descriptor{ 5 } },
//        componentsEntity1{ component_descriptor{ 2 },   //
//                           component_descriptor{ 3 },
//                           component_descriptor{ 5 },
//                           component_descriptor{ 6 } };
//
//    entity_descriptor entity0{ 0 },   //
//        entity1{ 1 };
//
//    std::vector<reg_error> regResultExpected(componentsEntity0.size(), reg_error::success),
//        regResult = regResultExpected;
//
//    register_components(entity0,
//                        componentsEntity0.data(),
//                        componentsEntity0.size(),
//                        regResult.data());
//
//    ASSERT_EQ(regResult, regResultExpected);
//
//    regResultExpected = std::vector(componentsEntity1.size(), reg_error::success),
//    regResult = regResultExpected;
//
//    register_components(entity1,
//                        componentsEntity1.data(),
//                        componentsEntity1.size(),
//                        regResult.data());
//
//    ASSERT_EQ(regResult, regResultExpected);
//
//    entity_selection entitySelection{};
//
//    std::vector<component_descriptor> selectComponents{ component_descriptor{ 2 },
//                                                        component_descriptor{ 3 },
//                                                        component_descriptor{ 5 } };
//
//    select_entities_by_components(selectComponents.data(),
//                                  selectComponents.size(),
//                                  entitySelection);
//
//    ASSERT_NE(entitySelection.array, nullptr);
//    EXPECT_EQ(entitySelection.length, 2);
//
//    free_entity_selection(entitySelection);
//    EXPECT_EQ(entitySelection.handle, -1);
//    EXPECT_EQ(entitySelection.array, nullptr);
//    EXPECT_EQ(entitySelection.length, 0);
//}
//
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
