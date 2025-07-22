#include <coli/game-engine.h>
#include <gtest/gtest.h>

using namespace Coli;

TEST(Graphics, Context)
{
    EXPECT_NO_THROW(Graphics::Context {});
}
