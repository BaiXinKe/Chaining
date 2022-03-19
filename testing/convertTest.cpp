#include "../src/auxiliary/ReadSetting.hpp"
#include "gtest/gtest.h"

TEST(unitConversion, convert)
{
    using namespace Chaining::auxiliary;
    EXPECT_EQ(settings::GetUnitConversionValue(""), 1024 * 1024);
    EXPECT_EQ(settings::GetUnitConversionValue("1MB"), 1024 * 1024);
    EXPECT_EQ(settings::GetUnitConversionValue("1GB"), 1024 * 1024 * 1024);
    EXPECT_EQ(settings::GetUnitConversionValue("12GB"), static_cast<size_t>(12) * 1024 * 1024 * 1024);
    EXPECT_EQ(settings::GetUnitConversionValue("0MB"), 0);
    EXPECT_EQ(settings::GetUnitConversionValue("100"), 100 * 1024 * 1024);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return ::RUN_ALL_TESTS();
}