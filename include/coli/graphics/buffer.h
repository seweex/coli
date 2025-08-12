#ifndef COLI_GRAPHICS_BUFFER_H
#define COLI_GRAPHICS_BUFFER_H

#include "coli/graphics/resource.h"

/// @brief Namespace for the all OpenGL-based stuff.
namespace Coli::Graphics::inline OpenGL
{
    /**
     * @brief Buffer data type.
     * @details Enumerator for qualifying the buffer data type.
     */
    enum class BufferType
    {
        /// @brief Make the buffer store vertices.
        vertex = GL_ARRAY_BUFFER,

        /// @brief Make the buffer store indices for indexing vertices.
        index = GL_ELEMENT_ARRAY_BUFFER,

        /// @brief Make the buffer store shaders uniform data.
        uniform = GL_UNIFORM_BUFFER
    };
}

/**
 * @brief For internal details.
 * @note The user should not use this namespace.
 */
namespace Coli::Graphics::Detail::inline OpenGL
{
    template <Graphics::OpenGL::BufferType Type>
    struct ValidBufferType : public std::bool_constant<
        (Type == Graphics::OpenGL::BufferType::vertex) ||
        (Type == Graphics::OpenGL::BufferType::index) ||
        (Type == Graphics::OpenGL::BufferType::uniform)
    > {};

    class BufferFactory final
    {
        [[noreturn]] static void fail_initialization_error();
        [[noreturn]] static void fail_invalid_context();

    public:
        /**
         * @brief Destroys the buffer
         *
         * @param context The context that was used to create a buffer/storage
         * @param handle The buffer/storage handle
         */
        static void destroy(Graphics::OpenGL::Context& context, GLuint handle) noexcept;

        /**
         * @brief Creates an OpenGL buffer/storage
         *
         * @param context The valid context to check it loaded
         *
         * @throw std::invalid_argument If the context is invalid
         * @throw std::logic_error If calls not from the context creation thread
         * @throw std::runtime_error If initialization fails
         *
         * @return A valid raw handle to the newly created buffer/storage
         */
        [[nodiscard]] static GLuint
        create(std::shared_ptr<Graphics::OpenGL::Context>& context);

        using deleter_type = decltype(&destroy);
        using resource_type = OpenGL::ResourceBase<GLuint, deleter_type>;

        BufferFactory() = delete;

        BufferFactory(const BufferFactory&) = delete;
        BufferFactory(BufferFactory&&) = delete;

        BufferFactory& operator=(const BufferFactory&) = delete;
        BufferFactory& operator=(BufferFactory&&) = delete;
    };
}

namespace Coli::Graphics::inline OpenGL
{
    /**
     * @brief OpenGL buffer class.
     * @detail Represent the buffer of qualified data.
     *
     * @tparam Type Qualification of the stored data.
     * @tparam Mutable Allows you to change the buffer data.
     *
     * @note Does not allow multithreading. You should use
     * this class from the thread where you created the context.
     */
    template <BufferType Type, bool Mutable>
        requires (Detail::OpenGL::ValidBufferType<Type>::value)
    class COLI_EXPORT BasicBuffer final :
        public Detail::OpenGL::BufferFactory::resource_type
    {
        using factory_type = Detail::OpenGL::BufferFactory;
        using resource_base = typename factory_type::resource_type;

        [[noreturn]] static void fail_invalid_param(std::string_view msg);

    public:
        /**
         * @brief Creates empty buffer.
         * @detail Cerates a valid buffer without any data.
         *
         * @param context The valid context to check it is loaded.
         *
         * @throw std::invalid_argument If the context is invalid;
         * @throw std::logic_error If calls not from the context creation thread;
         * @throw std::runtime_error If initialization fails.
         *
         * @note You can create an empty buffer only if it's mutable.
         */
        BasicBuffer(std::shared_ptr<OpenGL::Context> context) requires (Mutable);

        /**
         * @brief Creates buffer.
         * @detail Creates a valid buffer with user's data.
         *
         * @param context The valid context to check it is loaded;
         * @param data Valid pointer to the initial data;
         * @param size Valid size of the initial data, in bytes;
         *
         * @throw std::invalid_argument If the context is invalid;
         * @throw std::invalid_argument If the data pointer is nullptr;
         * @throw std::invalid_argument If the data size is 0 or more than max_size();
         *
         * @throw std::logic_error If calls not from the context creation thread;
         * @throw std::runtime_error If initialization fails.
         */
        BasicBuffer(
            std::shared_ptr<OpenGL::Context> context,
            void const* data,
            size_t size
        );

        /**
         * @brief Moves buffer.
         * @detail Moves the buffer.
         *
         * @throw std::logic_error If moved from other thread;
         */
        BasicBuffer(BasicBuffer&&) noexcept(false);

        /// @copydoc BasicBuffer(BasicBuffer&&)
        BasicBuffer& operator=(BasicBuffer&&) noexcept(false);

        BasicBuffer(const BasicBuffer&) = delete;
        BasicBuffer& operator=(const BasicBuffer&) = delete;

        /**
         * @brief Destroys buffer.
         * @details Destroys the buffer.
         */
        ~BasicBuffer() noexcept;

        /**
         * @brief Returns data size.
         * @details Returns the size of the stored data, in bytes.
         *
         * @throw std::invalid_argument If calls on the invalid object.
         *
         * @return Size of the currently stored data.
         */
        [[nodiscard]] size_t size() const;

        /**
         * @breif Updates buffer data.
         * @details Updates the stored buffer data with others.
         * If there is not enough memory the buffers reallocates it
         *
         * @param data Valid pointer to the new data;
         * @param size Valid size of the new data, in bytes;
         * @param offset Offset off the currently stored data beginning, in bytes.
         *
         * @throw std::invalid_argument If calls on the invalid object;
         * @throw std::invalid_argument If the data pointer is nullptr;
         * @throw std::invalid_argument If the data size is 0 or more than max_size();
         * @throw std::invalid_argument If 'size' + 'offset' is more than max_size();
         * @throw std::invalid_argument If the data offset is more than the currently storing data size.
         */
        void update(void const* data, size_t size, size_t offset = 0) requires (Mutable);

        /**
         * @brief Recreates buffer data.
         * @detail Frees the current buffer memory,
         * allocates a new one, and fills it with user's data.
         *
         * @param data Valid pointer to the new data;
         * @param size Valid size of the new data, in bytes.
         *
         * @throw std::invalid_argument If calls on the invalid object;
         * @throw std::invalid_argument If the data pointer is nullptr;
         * @throw std::invalid_argument If the data size is 0 or more than max_size().
         */
        void assign(void const* data, size_t size) requires (Mutable);

        /**
         * @brief Returns max data size.
         * @brief Returns the max possible size of buffer data.
         *
         * @return Max buffer data size value.
         */
        [[nodiscard]] static size_t max_size() noexcept;

        /**
         * @brief Binds buffer.
         * @brief Binds the buffer to the graphic pipeline.
         *
         * @throw std::invalid_argument If calls on the invalid object;
         * @throw std::logic_error If calls not from the context creation thread;
         * @throw std::logic_error If another buffer is bound.
         */
        void bind();

        /**
         * @brief Unbinds buffer.
         * @brief Unbinds the buffer from the graphic pipeline if
         * the bound buffer is current.
         */
        void unbind() noexcept;

    private:
        size_t mySize;
    };

#if COLI_BUILD
#else
    extern template class COLI_EXPORT BasicBuffer<BufferType::vertex, true>;
    extern template class COLI_EXPORT BasicBuffer<BufferType::index, true>;
    extern template class COLI_EXPORT BasicBuffer<BufferType::uniform, true>;

    extern template class COLI_EXPORT BasicBuffer<BufferType::vertex, false>;
    extern template class COLI_EXPORT BasicBuffer<BufferType::index, false>;
    extern template class COLI_EXPORT BasicBuffer<BufferType::uniform, false>;
#endif

    /// @brief Mutable buffer storing vertices.
    using VertexBuffer = BasicBuffer<BufferType::vertex, true>;

    /// @brief Mutable buffer storing indices.
    using IndexBuffer = BasicBuffer<BufferType::index, true>;

    /// @brief Mutable buffer for uniform data.
    using UniformBuffer = BasicBuffer<BufferType::uniform, true>;

    /// @brief Immutable buffer storing vertices.
    using VertexStorage = BasicBuffer<BufferType::vertex, false>;

    /// @brief Immutable buffer storing indices.
    using IndexStorage = BasicBuffer<BufferType::index, false>;

    /// @brief Immutable buffer for uniform data.
    using UniformStorage = BasicBuffer<BufferType::uniform, false>;
}

#endif