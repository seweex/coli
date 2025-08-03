#include "coli/graphics/buffer.h"

#include <iostream>

namespace Coli::Graphics::Detail::inline OpenGL
{
    template <Graphics::OpenGL::BufferType Type>
    class BufferBindings final
    {
    public:
        [[noreturn]] static void fail_another_bound() {
            throw std::logic_error("Another buffer bound");
        }

        static inline GLuint current = 0;
    };

    void BufferFactory::fail_initialization_error() {
        throw std::runtime_error("Failed to create a OpenGL buffer");
    }

    void BufferFactory::fail_invalid_context() {
        throw std::invalid_argument("Invalid context");
    }

    void BufferFactory::destroy(Graphics::OpenGL::Context& context, GLuint handle) noexcept
    {
        if (handle != 0)
            glDeleteBuffers(1, &handle);

        (void)context;
    }

    GLuint BufferFactory::create(std::shared_ptr<Graphics::OpenGL::Context>& context)
    {
        if (!context)
            fail_invalid_context();

        context->verify_thread();
        context->verify_context();

        GLuint handle = 0;
        glCreateBuffers(1, &handle);

        if (handle == 0)
            fail_initialization_error();

        return handle;
    }
}

namespace Coli::Graphics::inline OpenGL
{
    template <BufferType Type, bool Mutable> requires (Detail::OpenGL::ValidBufferType<Type>::value)
    BasicBuffer<Type, Mutable>::BasicBuffer(BasicBuffer&&) noexcept(false) = default;

    template <BufferType Type, bool Mutable> requires (Detail::OpenGL::ValidBufferType<Type>::value)
    BasicBuffer<Type, Mutable>& BasicBuffer<Type, Mutable>::operator=(BasicBuffer&&) noexcept(false) = default;

    template <BufferType Type, bool Mutable> requires (Detail::OpenGL::ValidBufferType<Type>::value)
    BasicBuffer<Type, Mutable>::~BasicBuffer() noexcept = default;

    template <BufferType Type, bool Mutable> requires (Detail::OpenGL::ValidBufferType<Type>::value)
    void BasicBuffer<Type, Mutable>::fail_invalid_param(std::string_view msg) {
        throw std::invalid_argument(msg.data());
    }

    template <BufferType Type, bool Mutable> requires (Detail::OpenGL::ValidBufferType<Type>::value)
    BasicBuffer<Type, Mutable>::BasicBuffer(std::shared_ptr<OpenGL::Context> context)
    requires (Mutable) :
        resource_base (context, factory_type::create(context), &factory_type::destroy),
        mySize (0)
    {}

    template <BufferType Type, bool Mutable> requires (Detail::OpenGL::ValidBufferType<Type>::value)
    BasicBuffer<Type, Mutable>::BasicBuffer(std::shared_ptr<OpenGL::Context> context, void const* data, size_t size) :
        resource_base (context, factory_type::create(context), &factory_type::destroy),
        mySize (size)
    {
        if (!data)
            fail_invalid_param("Invalid data value");

        if (size == 0 || size > max_size())
            fail_invalid_param("Invalid size value");

        if constexpr (Mutable)
            glNamedBufferData(myHandle, size, data, GL_DYNAMIC_DRAW | GL_MAP_READ_BIT);
        else
            glNamedBufferStorage(myHandle, size, data, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT);
    }

    template <BufferType Type, bool Mutable> requires (Detail::OpenGL::ValidBufferType<Type>::value)
    size_t BasicBuffer<Type, Mutable>::size() const
    {
        if (is_valid())
            return mySize;
        else
            fail_call_on_invalid();
    }

    template <BufferType Type, bool Mutable> requires (Detail::OpenGL::ValidBufferType<Type>::value)
    void BasicBuffer<Type, Mutable>::update(void const* data, size_t size, size_t offset) requires (Mutable)
    {
        if (is_valid()) {
            if (!data)
                fail_invalid_param("Invalid data value");

            if (size == 0 || size > max_size() )
                fail_invalid_param("Invalid size value");

            if (mySize < offset || offset > max_size() - size)
                fail_invalid_param("Invalid buffer offset");

            if (mySize < size + offset)
            {
                auto const myData = glMapNamedBuffer(myHandle, GL_READ_ONLY);

                BasicBuffer copy { myContext, myData, std::max(size, mySize) };
                copy.update(data, size, offset);

                glUnmapNamedBuffer(myHandle);
                *this = std::move(copy);
            }
            else
                glNamedBufferSubData(myHandle, offset, size, data);
        }
        else
            fail_call_on_invalid();
    }

    template <BufferType Type, bool Mutable> requires (Detail::OpenGL::ValidBufferType<Type>::value)
    void BasicBuffer<Type, Mutable>::assign(void const* data, size_t size) requires (Mutable)
    {
        if (is_valid()) {
            if (!data)
                fail_invalid_param("Invalid data value");

            if (size == 0 || size > max_size() )
                fail_invalid_param("Invalid size value");

            glNamedBufferData(myHandle, mySize = size, data, GL_DYNAMIC_DRAW);
        }
        else
            fail_call_on_invalid();
    }

    template <BufferType Type, bool Mutable> requires (Detail::OpenGL::ValidBufferType<Type>::value)
    size_t BasicBuffer<Type, Mutable>::max_size() noexcept {
        return std::min(std::numeric_limits<GLintptr>::max(), std::numeric_limits<GLsizeiptr>::max());
    }

    template <BufferType Type, bool Mutable> requires (Detail::OpenGL::ValidBufferType<Type>::value)
    void BasicBuffer<Type, Mutable>::bind()
    {
        if (!is_valid())
            fail_call_on_invalid();

        if (Detail::OpenGL::BufferBindings<Type>::current == 0) {
            glBindBuffer(static_cast<GLuint>(Type), myHandle);
            Detail::OpenGL::BufferBindings<Type>::current = myHandle;
        }
        else
            Detail::OpenGL::BufferBindings<Type>::fail_another_bound();
    }

    template <BufferType Type, bool Mutable> requires (Detail::OpenGL::ValidBufferType<Type>::value)
    void BasicBuffer<Type, Mutable>::unbind() noexcept
    {
        if (myHandle != 0 &&
            Detail::OpenGL::BufferBindings<Type>::current == myHandle
        ) {
            glBindBuffer(static_cast<GLuint>(Type), 0);
            Detail::OpenGL::BufferBindings<Type>::current = 0;
        }
    }

    template class COLI_EXPORT BasicBuffer<BufferType::vertex, true>;
    template class COLI_EXPORT BasicBuffer<BufferType::index, true>;
    template class COLI_EXPORT BasicBuffer<BufferType::uniform, true>;

    template class COLI_EXPORT BasicBuffer<BufferType::vertex, false>;
    template class COLI_EXPORT BasicBuffer<BufferType::index, false>;
    template class COLI_EXPORT BasicBuffer<BufferType::uniform, false>;
}