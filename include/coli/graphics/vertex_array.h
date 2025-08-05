#ifndef COLI_GRAPHICS_VERTEX_ARRAY_H
#define COLI_GRAPHICS_VERTEX_ARRAY_H

#include "coli/geometry/vertex.h"
#include "coli/graphics/buffer.h"

namespace Coli::Graphics::inline OpenGL {
    class VertexArray;
}

namespace Coli::Graphics::Detail::inline OpenGL
{
    struct VertexAttributes final {
        size_t length;
        size_t offset;
        GLenum type;
    };

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
        [[noreturn]] static void fail_invalid_storage();

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

        /**
         * @brief Configures the newly created vertex array
         *
         * @param array The created vertex array object
         * @param vertices The vertices storage
         * @param indices The indices storage
         * @param vertexAttributes The vector of vertex attributes
         * @param vertexSize The size of the vertex
         *
         * @throw std::invalid_argument If vertices pointer is invalid
         * @throw std::invalid_argument If indices pointer is invalid
         * @throw std::invalid_argument If vertex attributes has size smaller than 2
         * @throw std::invalid_argument If vertex size is invalid
         */
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

namespace Coli::Graphics::inline OpenGL
{
    /// @brief OpenGL vertex array class
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
         * @brief Creates a valid vertex array
         *
         * @param context The valid context to check it is loaded
         * @param vertices The valid pointer to vertices to use
         * @param indices The valid pointer to indices to use
         * @param vertexExample The example of vertex type
         *
         * @throw std::invalid_argument If the context is invalid
         * @throw std::invalid_argument If the vertices pointer is invalid
         * @throw std::invalid_argument If the indices pointer is invalid
         *
         * @throw std::logic_error If calls not from the context creation thread
         * @throw std::runtime_error If initialization fails
         * @throw std::bad_alloc If allocation fails
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