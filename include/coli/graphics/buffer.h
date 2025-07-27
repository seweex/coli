#ifndef COLI_GRAPHICS_BUFFER_H
#define COLI_GRAPHICS_BUFFER_H

#include "coli/graphics/resource.h"

namespace Coli::Graphics::inline OpenGL
{
    /// @brief Enumerator for qualifying the buffer/storage data type
    enum class BufferType
    {
        /// @brief Make the buffer store vertices
        vertex = GL_ARRAY_BUFFER,

        /// @brief Make the buffer store indices for indexing vertices
        index = GL_ELEMENT_ARRAY_BUFFER,

        /// @brief Make the buffer store shaders uniform data
        uniform = GL_UNIFORM_BUFFER
    };
}

namespace Coli::Graphics::Detail::inline OpenGL
{
    template <Graphics::OpenGL::BufferType Type>
    struct ValidBufferType : public std::bool_constant<
        (Type == Graphics::OpenGL::BufferType::vertex) ||
        (Type == Graphics::OpenGL::BufferType::index) ||
        (Type == Graphics::OpenGL::BufferType::uniform)
    > {};

    /**
     * @brief A factory class for creating OpenGL buffers
     *
     * @warning This class and all its methods are not supposed to be used by the user
     * @note This class does not allow instances
     */
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
     * @brief The OpenGL buffer/storage class
     *
     * @tparam Type The qualification of the storing data
     * @tparam Mutable Allows you to change the buffer data
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
         * @brief Creates an empty valid buffer
         *
         * @param context The valid context to check it is loaded
         *
         * @throw std::invalid_argument If the context is invalid
         * @throw std::logic_error If calls not from the context creation thread
         * @throw std::runtime_error If initialization fails
         *
         * @warning You can not create an empty storage (immutable buffer)
         */
        BasicBuffer(std::shared_ptr<OpenGL::Context> context) requires (Mutable);

        /**
         * @brief Creates a valid buffer/storage and completes it with
         * your data
         *
         * @param context The valid context to check it is loaded
         * @param data The valid pointer to the initial data
         * @param size The non-zero size in bytes of the initial data
         *
         * @throw std::invalid_argument If the context is invalid
         * @throw std::invalid_argument If the data pointer is nullptr
         * @throw std::invalid_argument If the data size is 0 or more than max_size()
         *
         * @throw std::logic_error If calls not from the context creation thread
         * @throw std::runtime_error If initialization fails
         */
        BasicBuffer(
            std::shared_ptr<OpenGL::Context> context,
            void const* data,
            size_t size
        );

        /**
         * @brief Accepts a moved buffer/storage
         *
         * @throw std::logic_error If moved from other thread
         */
        BasicBuffer(BasicBuffer&&) noexcept(false);

        /**
         * @copydoc BasicBuffer(BasicBuffer&&)
         * @return A reference to itself
         */
        BasicBuffer& operator=(BasicBuffer&&) noexcept(false);

        BasicBuffer(const BasicBuffer&) = delete;
        BasicBuffer& operator=(const BasicBuffer&) = delete;

        /// @brief Deletes the buffer
        ~BasicBuffer() noexcept;

        /**
         * @brief Gets the size of the storing data
         *
         * @throw std::invalid_argument If calls on the invalid object
         *
         * @return The size of the currently storing data
         */
        [[nodiscard]] size_t size() const;

        /**
         * @brief Updates the buffer data with other
         * @note If there is not enough memory the buffers reallocates it
         *
         * @param data The valid pointer to the new data
         * @param size The non-zero size in bytes of the new data
         * @param offset The offset off the currently storing data begin in bytes
         *
         * @throw std::invalid_argument If calls on the invalid object
         * @throw std::invalid_argument If the data pointer is nullptr
         * @throw std::invalid_argument If the data size is 0 or more than max_size()
         * @throw std::invalid_argument If 'size' + 'offset' is more than max_size()
         * @throw std::invalid_argument If the data offset is more than the currently storing data size
         */
        void update(void const* data, size_t size, size_t offset = 0) requires (Mutable);

        /**
         * @brief Frees the current buffer memory,
         * allocates a new one, and fills it with your data
         *
         * @param data The valid pointer to the new data
         * @param size The non-zero size in bytes of the new data
         *
         * @throw std::invalid_argument If calls on the invalid object
         * @throw std::invalid_argument If the data pointer is nullptr
         * @throw std::invalid_argument If the data size is 0 or more than max_size()
         */
        void assign(void const* data, size_t size) requires (Mutable);

        /**
         * @brief Gets the max size of the buffer data
         * @return The max buffer data size value
         */
        [[nodiscard]] static size_t max_size() noexcept;

        /**
         * @brief Binds the buffer to the graphic pipeline
         *
         * @throw std::invalid_argument If calls on the invalid object
         * @throw std::logic_error If calls not from the context creation thread
         * @throw std::logic_error If another buffer is bound
         */
        void bind();

        /// @brief Unbinds the bound buffer
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

    using VertexBuffer = BasicBuffer<BufferType::vertex, true>;
    using IndexBuffer = BasicBuffer<BufferType::index, true>;
    using UniformBuffer = BasicBuffer<BufferType::uniform, true>;

    using VertexStorage = BasicBuffer<BufferType::vertex, false>;
    using IndexStorage = BasicBuffer<BufferType::index, false>;
    using UniformStorage = BasicBuffer<BufferType::uniform, false>;
}

#endif