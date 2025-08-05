#ifndef COLI_GRAPHICS_SHADER_H
#define COLI_GRAPHICS_SHADER_H

#include "coli/graphics/resource.h"

namespace Coli::Graphics::inline OpenGL
{
    /// @brief Enumerator for qualifying the shader stage
    enum class ShaderType
    {
        /// @brief Make the shader do vertex work
        vertex = GL_VERTEX_SHADER,

        /// @brief Make the shader do fragment work
        fragment = GL_FRAGMENT_SHADER
    };
}

namespace Coli::Graphics::Detail::inline OpenGL
{
    class ProgramFactory;

    template <Graphics::OpenGL::ShaderType Type>
    struct ValidShaderType : public std::bool_constant<
        (Type == Graphics::OpenGL::ShaderType::vertex) ||
        (Type == Graphics::OpenGL::ShaderType::fragment)
    > {};

    /**
     * @brief A factory class for creating OpenGL shaders
     *
     * @warning This class and all its methods are not supposed to be used by the user
     * @note This class does not allow instances
     */
    class ShaderFactory final
    {
        [[noreturn]] static void fail_initialization_error();
        [[noreturn]] static void fail_invalid_context();
        [[noreturn]] static void fail_invalid_source();
        [[noreturn]] static void fail_compile_error();

    public:
        /**
         * @brief Destroys the shader
         *
         * @param context The context that was used to create a shader
         * @param handle The shader handle
         */
        static void destroy(Graphics::OpenGL::Context& context, GLuint handle) noexcept;

        /**
         * @brief Creates an OpenGL shader
         *
         * @param context The valid context to check it loaded
         * @param type The shader type
         * @param source The shader source code
         *
         * @throw std::invalid_argument If the context is invalid
         * @throw std::invalid_argument If the source is empty or too large
         *
         * @throw std::logic_error If calls not from the context creation thread
         * @throw std::runtime_error If the initialization fails
         * @throw std::runtime_error If the compilation fails
         *
         * @return A valid raw handle to the newly created shader
         */
        [[nodiscard]] static GLuint create(
            std::shared_ptr<Graphics::OpenGL::Context>& context,
            Graphics::OpenGL::ShaderType type,
            std::string_view source);

        using deleter_type = decltype(&destroy);
        using resource_type = ResourceBase<GLuint, deleter_type>;

        ShaderFactory() = delete;

        ShaderFactory(const ShaderFactory&) = delete;
        ShaderFactory(ShaderFactory&&) = delete;

        ShaderFactory& operator=(const ShaderFactory&) = delete;
        ShaderFactory& operator=(ShaderFactory&&) = delete;
    };
}

namespace Coli::Graphics::inline OpenGL
{
    /// @brief OpenGL shader class
    template <OpenGL::ShaderType Type>
        requires (Detail::OpenGL::ValidShaderType<Type>::value)
    class COLI_EXPORT BasicShader final :
        public Detail::OpenGL::ShaderFactory::resource_type
    {
        using factory_type = Detail::OpenGL::ShaderFactory;
        using resource_base = typename factory_type::resource_type;

        friend class Coli::Graphics::Detail::OpenGL::ProgramFactory;

        [[nodiscard]] GLuint get_handle() const noexcept;

    public:
        /**
         * @brief Creates a valid shader and compile it with your
         * source code
         *
         * @param context The valid context to check it is loaded
         * @param sourceCode The source code of the new shader
         *
         * @throw std::invalid_argument If the context is invalid
         * @throw std::invalid_argument If the sourceCode is empty
         *
         * @throw std::logic_error If calls not from the context creation thread
         * @throw std::runtime_error If initialization fails
         */
        BasicShader(
            std::shared_ptr<OpenGL::Context> context,
            std::string_view sourceCode
        );

        /**
         * @brief Accepts a moved shader
         *
         * @throw std::logic_error If moved from other thread
         */
        BasicShader(BasicShader&&) noexcept(false);

        /**
         * @copydoc BasicShader(BasicShader&&)
         * @return A reference to itself
         */
        BasicShader& operator=(BasicShader&&) noexcept(false);

        BasicShader(const BasicShader&) = delete;
        BasicShader& operator=(const BasicShader&) = delete;

        /// @brief Deletes the shader
        ~BasicShader() noexcept;
    };

#if COLI_BUILD
#else
    extern template class COLI_EXPORT BasicShader<ShaderType::vertex>;
    extern template class COLI_EXPORT BasicShader<ShaderType::fragment>;
#endif

    using VertexShader = BasicShader<ShaderType::vertex>;
    using FragmentShader = BasicShader<ShaderType::fragment>;
}

#endif
