#ifndef COLI_GRAPHICS_VERTEX_ARRAY_H
#define COLI_GRAPHICS_VERTEX_ARRAY_H

#include "coli/geometry/vertex.h"
#include "coli/graphics/buffer.h"

/// @brief Namespace for the all OpenGL-based stuff.
namespace Coli::Graphics::inline OpenGL
{
    class VertexArray;
}

/**
 * @brief For internal details.
 * @note The user should not use this namespace.
 */
namespace Coli::Graphics::Detail::inline OpenGL
{
    struct VertexAttributes final {
        size_t length;
        size_t offset;
        GLenum type;
    };

    class VertexArrayFactory final
    {
        [[noreturn]] static void fail_initialization_error();
        [[noreturn]] static void fail_invalid_context();
        [[noreturn]] static void fail_invalid_storage();

    public:
        static void destroy(Graphics::OpenGL::Context& context, GLuint handle) noexcept;

        [[nodiscard]] static GLuint create(std::shared_ptr<Graphics::OpenGL::Context>& context);

        static void configure(
            Graphics::OpenGL::VertexArray& array,
            std::shared_ptr<VertexStorage>& vertices,
            std::shared_ptr<IndexStorage>& indices,
            std::vector<VertexAttributes> const& vertexAttributes,
            size_t vertexSize
        );

        using deleter_type = decltype(&destroy);
        using resource_type = ResourceBase<GLuint, deleter_type>;

        VertexArrayFactory() = delete;

        VertexArrayFactory(const VertexArrayFactory&) = delete;
        VertexArrayFactory(VertexArrayFactory&&) = delete;

        VertexArrayFactory& operator=(const VertexArrayFactory&) = delete;
        VertexArrayFactory& operator=(VertexArrayFactory&&) = delete;
    };
}

/// @brief Namespace for the all OpenGL-based stuff.
namespace Coli::Graphics::inline OpenGL
{
    /**
     * @brief OpenGL vertex array class.
     * @detail Represent the vertex array objects.
     *
     * @note Does not allow multithreading. You should use
     * this class from the thread where you created the context.
     */
    class COLI_EXPORT VertexArray final :
        public Detail::OpenGL::VertexArrayFactory::resource_type
    {
        using factory_type = Detail::OpenGL::VertexArrayFactory;
        using resource_base = typename factory_type::resource_type;

        template <class VertexTy>
        void configure()
        {
            using traits_type = Geometry::VertexTraits<VertexTy>;
            constexpr size_t max_vertex_attributes = 3; // pos + uv + normal

            auto floatEnumerator = [] <typename Ty> (Ty&&) -> GLenum {
                if constexpr (std::same_as<Ty, float>)
                    return GL_FLOAT;
                else if constexpr (std::same_as<Ty, double>)
                    return GL_DOUBLE;
                else
                    static_assert(false, "invalid floating point type");
            };

            std::vector<Detail::OpenGL::VertexAttributes> attributes;
            attributes.reserve(max_vertex_attributes);

            attributes.emplace_back(
                traits_type::position_length(),
                traits_type::position_offset(),
                floatEnumerator(typename traits_type::position_float_type{})
            );

            attributes.emplace_back(
                traits_type::texcoord_length(),
                traits_type::texcoord_offset(),
                floatEnumerator(typename traits_type::texcoord_float_type{})
            );

            if constexpr (traits_type::has_normal::value)
                attributes.emplace_back(
                    traits_type::normal_length(),
                    traits_type::normal_offset(),
                    floatEnumerator(typename traits_type::texcoord_float_type{})
                );

            factory_type::configure(*this, myVertices,
                myIndices, attributes, traits_type::stride());
        }

        class Binding;

    public:
        /**
         * @brief Creates vertex array.
         * @detail Creates a valid vertex array.
         *
         * @param context The valid context to check it is loaded;
         * @param vertices Valid pointer to vertices to use;
         * @param indices Valid pointer to indices to use;
         * @param vertexExample Vertex example.
         *
         * @throw std::invalid_argument If the context is invalid;
         * @throw std::invalid_argument If the vertices pointer is invalid;
         * @throw std::invalid_argument If the indices pointer is invalid;
         *
         * @throw std::logic_error If calls not from the context creation thread;
         * @throw std::runtime_error If initialization fails;
         * @throw std::bad_alloc If allocation fails.
         */
        template <class VertexTy>
        VertexArray(
            std::shared_ptr<Context> context,
            std::shared_ptr<VertexStorage> vertices,
            std::shared_ptr<IndexStorage> indices,
            [[maybe_unused]] VertexTy const& vertexExample
        ) :
            resource_base(context, factory_type::create(context), &factory_type::destroy),
            myVertices (std::move(vertices)),
            myIndices  (std::move(indices))
        {
            configure<VertexTy>();
        }

        /**
         * @brief Moves vertex array.
         * @detail Moves the vertex array.
         *
         * @throw std::logic_error If moved from other thread;
         */
        VertexArray(VertexArray&&) noexcept(false);

        /// @copydoc VertexArray(VertexArray&&)
        VertexArray& operator=(VertexArray&&) noexcept(false);

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        /**
         * @brief Destroys buffer.
         * @details Destroys the buffer.
         */
        ~VertexArray() noexcept;

        /**
         * @brief Returns vertex buffer.
         * @details Returns the currently used vertex buffer.
         *
         * @throw std::invalid_argument If calls on invalid object.
         *
         * @return Unexpired weak pointer to used vertices storage.
         */
        [[nodiscard]] std::weak_ptr<VertexStorage>
        get_vertices();

        /**
         * @brief Returns index buffer.
         * @details Returns the currently used index buffer.
         *
         * @throw std::invalid_argument If calls on invalid object.
         *
         * @return Unexpired weak pointer to used index storage.
         */
        [[nodiscard]] std::weak_ptr<IndexStorage>
        get_indices();

        /**
         * @brief Binds vertex array.
         * @brief Binds the vertex array to the graphic pipeline.
         *
         * @throw std::invalid_argument If calls on the invalid object;
         * @throw std::logic_error If calls not from the context creation thread;
         * @throw std::logic_error If another vertex array is bound.
         */
        void bind();

        /**
         * @brief Unbinds vertex array.
         * @brief Unbinds the vertex array from the graphic pipeline if
         * the bound vertex array is current.
         */
        void unbind() noexcept;

        /// @copydoc Coli::Graphics::Detail::OpenGL::ResourceBase::clear
        void clear() noexcept;

    private:
        std::shared_ptr<VertexStorage> myVertices;
        std::shared_ptr<IndexStorage> myIndices;
    };
}

#endif