#include <coli/game-engine.h>
#include <gtest/gtest.h>

using namespace Coli;

TEST(API, Version)
{
#if defined(COLI_VERSION_MAJOR) && defined(COLI_VERSION_MINOR) && defined(COLI_VERSION_PATCH)
    EXPECT_TRUE(true);
#else
    EXPECT_TRUE(false);
#endif
}