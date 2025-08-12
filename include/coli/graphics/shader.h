#ifndef COLI_GRAPHICS_SHADER_H
#define COLI_GRAPHICS_SHADER_H

#include "coli/graphics/resource.h"

/// @brief Namespace for the all OpenGL-based stuff.
namespace Coli::Graphics::inline OpenGL
{
    /**
     * @brief Shader type.
     * @details Enumerator for qualifying the type of shader.
     */
    enum class ShaderType
    {
        /// @brief Make the shader do vertex work.
        vertex = GL_VERTEX_SHADER,

        /// @brief Make the shader do fragment work.
        fragment = GL_FRAGMENT_SHADER
    };
}

/**
 * @brief For internal details.
 * @note The user should not use this namespace.
 */
namespace Coli::Graphics::Detail::inline OpenGL
{
    class ProgramFactory;

    template <Graphics::OpenGL::ShaderType Type>
    struct ValidShaderType : public std::bool_constant<
        (Type == Graphics::OpenGL::ShaderType::vertex) ||
        (Type == Graphics::OpenGL::ShaderType::fragment)
    > {};

    class ShaderFactory final
    {
        [[noreturn]] static void fail_initialization_error();
        [[noreturn]] static void fail_invalid_context();
        [[noreturn]] static void fail_invalid_source();
        [[noreturn]] static void fail_compile_error();

    public:
        static void destroy(Graphics::OpenGL::Context& context, GLuint handle) noexcept;

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
    /**
     * @brief OpenGL shader class.
     * @detail Represent the shader of qualified type.
     *
     * @tparam Type Qualification of the shader.
     *
     * @note Does not allow multithreading. You should use
     * this class from the thread where you created the context.
     */
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
         * @brief Creates shader.
         * @details Creates a valid shader and compile it with user's
         * source code.
         *
         * @param context The valid context to check it is loaded;
         * @param sourceCode Source code of the new shader.
         *
         * @throw std::invalid_argument If the context is invalid;
         * @throw std::invalid_argument If the source code is empty;
         *
         * @throw std::logic_error If calls not from the context creation thread;
         * @throw std::runtime_error If initialization fails;
         */
        BasicShader(
            std::shared_ptr<OpenGL::Context> context,
            std::string_view sourceCode
        );

        /**
         * @brief Moves shader.
         * @detail Moves the shader.
         *
         * @throw std::logic_error If moved from other thread;
         */
        BasicShader(BasicShader&&) noexcept(false);

        /// @copydoc BasicShader(BasicShader&&)
        BasicShader& operator=(BasicShader&&) noexcept(false);

        BasicShader(const BasicShader&) = delete;
        BasicShader& operator=(const BasicShader&) = delete;

        /**
         * @brief Destroys shader.
         * @details Destroys the shader.
         */
        ~BasicShader() noexcept;
    };

#if COLI_BUILD
#else
    extern template class COLI_EXPORT BasicShader<ShaderType::vertex>;
    extern template class COLI_EXPORT BasicShader<ShaderType::fragment>;
#endif

    /// @brief Vertex shader.
    using VertexShader = BasicShader<ShaderType::vertex>;

    /// @brief Fragment shader.
    using FragmentShader = BasicShader<ShaderType::fragment>;
}

#endif
