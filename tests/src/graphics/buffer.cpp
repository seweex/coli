#include <coli/game-engine.h>
#include <gtest/gtest.h>

#include <memory>
#include <thread>

using namespace Coli;

class BufferTest :
    public ::testing::Test
{
protected:
    void SetUp() override
    {
        try {
            settings = { "Context Testing", 640, 480 };
            context = std::make_shared<Graphics::Context>();
            window = std::make_unique<Graphics::Window>(context, settings);
        }
        catch (std::bad_alloc const&) {
            GTEST_SKIP() << "A 'std::bad_alloc' was thrown" << std::endl;
        }
    }

    void TearDown() override {
        context.reset();
        window.reset();
    }

    std::shared_ptr<Graphics::Context> context;
    std::unique_ptr<Graphics::Window> window;
    Graphics::Window::Settings settings { "Context Testing", 640, 480 };

    static constexpr size_t size = 40;
    static constexpr size_t offset = 20;

    static const inline size_t maxSize = Graphics::VertexBuffer::max_size();
    static const inline size_t invalidSize =
        maxSize == std::numeric_limits<size_t>::max() ?
        maxSize + 1 : 0;
};

#ifdef CREATE_BUFFER
#undef CREATE_BUFFER
#endif

#define CREATE_BUFFER \
std::unique_ptr<Graphics::VertexBuffer> buffer; \
try { \
    buffer = std::make_unique<Graphics::VertexBuffer>(context); \
} \
catch (std::exception const& e) { \
    GTEST_SKIP() << "An exception was thrown: " << e.what() << "." << std::endl; \
}

#ifdef CREATE_DATA
#undef CREATE_DATA
#endif

#define CREATE_DATA \
std::unique_ptr<std::byte[]> data; \
try { \
    data = std::make_unique<std::byte[]>(size); \
} \
catch (std::exception const& e) { \
    GTEST_SKIP() << "An exception was thrown: " << e.what() << "." << std::endl; \
}

/* Create */

TEST_F(BufferTest, CreateSuccess)
{
    EXPECT_NO_THROW({
        Graphics::VertexBuffer buffer (context);
        EXPECT_TRUE(buffer.is_valid());
    });
}

TEST_F(BufferTest, CreateInvalidContext)
{
    EXPECT_THROW(
        Graphics::VertexBuffer buffer ({}),
        std::invalid_argument);
}

TEST_F(BufferTest, CreateAnotherThread)
{
    auto createBuffer = [&] {
        EXPECT_THROW(
            Graphics::VertexBuffer buffer (context),
            std::logic_error);
    };

    std::thread thread(createBuffer);
    thread.join();
}

TEST_F(BufferTest, CreateNullptr)
{
    EXPECT_THROW(
        Graphics::VertexBuffer buffer (context, nullptr, size),
        std::invalid_argument);
}

TEST_F(BufferTest, CreateInvalidSize)
{
    CREATE_DATA;

    EXPECT_THROW(
       Graphics::VertexBuffer buffer (context, data.get(), invalidSize),
       std::invalid_argument);
}

/* Move */

TEST_F(BufferTest, MoveSuccess)
{
    CREATE_BUFFER;

    EXPECT_NO_THROW({
        Graphics::VertexBuffer other (std::move(*buffer));

        EXPECT_TRUE(other.is_valid());
        EXPECT_FALSE(buffer->is_valid());
    });
}

TEST_F(BufferTest, MoveAnotherThread)
{
    CREATE_BUFFER;

    auto moveBuffer = [&] {
        EXPECT_THROW(
            Graphics::VertexBuffer other (std::move(*buffer)),
            std::logic_error);
    };

    std::thread thread (moveBuffer);
    thread.join();
}

/* Data Assignment */

TEST_F(BufferTest, AssignDataSuccess)
{
    CREATE_BUFFER;
    CREATE_DATA;

    buffer->assign(data.get(), size);

    EXPECT_EQ(buffer->size(), size);
}

TEST_F(BufferTest, AssignDataOnInvalid)
{
    CREATE_BUFFER;
    CREATE_DATA;

    buffer->clear();

    EXPECT_THROW(
        buffer->assign(data.get(), size),
        std::invalid_argument);
}

TEST_F(BufferTest, AssignDataNullptr)
{
    CREATE_BUFFER;

    EXPECT_THROW(
        buffer->assign(nullptr, size),
        std::invalid_argument);
}

TEST_F(BufferTest, AssignDataInvalidSize)
{
    CREATE_BUFFER;
    CREATE_DATA;

    EXPECT_THROW(
        buffer->assign(data.get(), invalidSize),
        std::invalid_argument);
}

/* Data Updating */

TEST_F(BufferTest, UpdateDataSuccess)
{
    CREATE_BUFFER;
    CREATE_DATA;

    ASSERT_NO_THROW(buffer->assign(data.get(), size));
    EXPECT_NO_THROW(buffer->update(data.get(), size - offset, offset));
}

TEST_F(BufferTest, UpdateDataOnInvalid)
{
    CREATE_BUFFER;
    CREATE_DATA;

    buffer->clear();

    EXPECT_THROW(
        buffer->update(data.get(), size - offset, offset),
        std::invalid_argument);
}

TEST_F(BufferTest, UpdateDataNullptr)
{
    CREATE_BUFFER;
    CREATE_DATA;

    ASSERT_NO_THROW(buffer->assign(data.get(), size));
    EXPECT_THROW(
        buffer->update(nullptr, size - offset, offset),
        std::invalid_argument);
}

TEST_F(BufferTest, UpdateDataInvalidSize)
{
    CREATE_BUFFER;
    CREATE_DATA;

    ASSERT_NO_THROW(buffer->assign(data.get(), size));
    EXPECT_THROW(
       buffer->update(data.get(), invalidSize),
       std::invalid_argument);
}

TEST_F(BufferTest, UpdateDataInvalidSizePlusOffset)
{
    CREATE_BUFFER;
    CREATE_DATA;

    ASSERT_NO_THROW(buffer->assign(data.get(), size));
    EXPECT_THROW(
       buffer->update(data.get(), maxSize - offset + 1, offset),
       std::invalid_argument);
}

TEST_F(BufferTest, UpdateDataOffsetMoreThanCurrentSize)
{
    CREATE_BUFFER;
    CREATE_DATA;

    ASSERT_NO_THROW(buffer->assign(data.get(), size));
    EXPECT_THROW(
        buffer->update(data.get(), size, size + 1),
        std::invalid_argument);
}

/* Size */

TEST_F(BufferTest, GetSize)
{
    CREATE_BUFFER;
    CREATE_DATA;

    ASSERT_NO_THROW(buffer->assign(data.get(), size));

    EXPECT_EQ(buffer->size(), size);
}

/* Bind */

TEST_F(BufferTest, BindSuccess)
{
    CREATE_BUFFER;
    EXPECT_NO_THROW(Graphics::Binding binding { *buffer });
}

TEST_F(BufferTest, BindTwice)
{
    CREATE_BUFFER;

    EXPECT_NO_THROW({
        Graphics::Binding binding { *buffer };
        EXPECT_THROW(
            Graphics::Binding other { *buffer },
            std::logic_error
        );
    });
}