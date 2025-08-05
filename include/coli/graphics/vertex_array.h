#ifndef COLI_GRAPHICS_VERTEX_ARRAY_H
#define COLI_GRAPHICS_VERTEX_ARRAY_H

#include "coli/graphics/buffer.h"

namespace Coli::Graphics::Detail::inline OpenGL
{
    /**
     * @brief A factory class for creating OpenGL vertex arrays
     *
     * @warning This class and all its methods are not supposed to be used by the user
     * @note This class does not allow instances
     */
    class VertexArrayFactory final
    {
        [[noreturn]] static void fail_initialization_error();
        [[noreturn]] static void fail_invalid_context();

    public:
        /**
         * @brief Destroys the vertex array
         *
         * @param context The context that was used to create a vertex array
         * @param handle The vertex array handle
         */
        static void destroy(Graphics::OpenGL::Context& context, GLuint handle) noexcept;

        /**
         * @brief Creates an OpenGL vertex array
         *
         * @param context The valid context to check it loaded
         *
         * @throw std::invalid_argument If the context is invalid
         * @throw std::logic_error If calls not from the context creation thread
         * @throw std::runtime_error If initialization fails
         *
         * @return A valid raw handle to the newly created vertex array
         */
        [[nodiscard]] static GLuint create(std::shared_ptr<Graphics::OpenGL::Context>& context);

        using deleter_type = decltype(&destroy);
        using resource_type = ResourceBase<GLuint, deleter_type>;

        VertexArrayFactory() = delete;

        VertexArrayFactory(const VertexArrayFactory&) = delete;
        VertexArrayFactory(VertexArrayFactory&&) = delete;

        VertexArrayFactory& operator=(const VertexArrayFactory&) = delete;
        VertexArrayFactory& operator=(VertexArrayFactory&&) = delete;
    };
}

namespace Coli::Graphics::inline OpenGL
{
    /// @brief OpenGL vertex array class
    class COLI_EXPORT VertexArray final :
        public Detail::OpenGL::VertexArrayFactory::resource_type
    {
        using factory_type = Detail::OpenGL::VertexArrayFactory;
        using resource_base = typename factory_type::resource_type;

    public:
        /**
         * @brief Creates a valid vertex array
         *
         * @param context The valid context to check it is loaded
         * @param vertices The valid pointer to vertices to use
         * @param indices The valid pointer to indices to use
         *
         * @throw std::invalid_argument If the context is invalid
         * @throw std::invalid_argument If the vertices pointer is invalid
         * @throw std::invalid_argument If the indices pointer is invalid
         *
         * @throw std::logic_error If calls not from the context creation thread
         * @throw std::runtime_error If initialization fails
         */
        VertexArray(
            std::shared_ptr<Context> context,
            std::shared_ptr<VertexStorage> vertices,
            std::shared_ptr<IndexStorage> indices
        );

        /**
         * @brief Accepts a moved vertex array
         *
         * @throw std::logic_error If moved from other thread
         */
        VertexArray(VertexArray&&) noexcept(false);

        /**
         * @copydoc VertexArray(VertexArray&&)
         * @return A reference to itself
         */
        VertexArray& operator=(VertexArray&&) noexcept(false);

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        /// @brief Deletes the vertex array
        ~VertexArray() noexcept;

        /**
         * @brief Gets the currently used vertices storage
         *
         * @throw std::invalid_argument If calls on invalid object
         *
         * @return Weak pointer to used vertices storage
         */
        [[nodiscard]] std::weak_ptr<VertexStorage>
        get_vertices();

        /**
         * @brief Gets the currently used indices storage
         *
         * @throw std::invalid_argument If calls on invalid object
         *
         * @return Weak pointer to used indices storage
         */
        [[nodiscard]] std::weak_ptr<IndexStorage>
        get_indices();

        /**
         * @brief Binds the vertex array to the graphic pipeline
         *
         * @throw std::invalid_argument If calls on the invalid object
         * @throw std::logic_error If calls not from the context creation thread
         * @throw std::logic_error If another vertex array is bound
         */
        void bind();

        /// @brief Unbinds the bound vertex array
        void unbind() noexcept;

        /// @copydoc Coli::Graphics::Detail::OpenGL::ResourceBase::clear
        void clear() noexcept;

    private:
        std::shared_ptr<VertexStorage> myVertices;
        std::shared_ptr<IndexStorage> myIndices;
    };
}

#endif