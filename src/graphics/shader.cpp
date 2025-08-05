#include "coli/graphics/shader.h"

namespace Coli::Graphics::Detail::inline OpenGL
{
    void ShaderFactory::fail_initialization_error() {
        throw std::runtime_error("Failed to create an OpenGL shader");
    }

    void ShaderFactory::fail_invalid_context() {
        throw std::invalid_argument("Invalid context");
    }

    void ShaderFactory::fail_invalid_source() {
        throw std::invalid_argument("Invalid source code");
    }

    void ShaderFactory::fail_compile_error() {
        throw std::runtime_error("Failed to compile the shader");
    }

    void ShaderFactory::destroy(Graphics::OpenGL::Context& context, GLuint handle) noexcept
    {
        if (handle != 0)
            glDeleteShader(handle);

        (void)context;
    }

    GLuint ShaderFactory::create(
        std::shared_ptr<Graphics::OpenGL::Context>& context,
        Graphics::OpenGL::ShaderType type,
        std::string_view source
    ) {
        if (!context)
            fail_invalid_context();

        if (source.empty() ||
            source.size() > static_cast<size_t>(std::numeric_limits<GLint>::max())
        )
            fail_invalid_source();

        context->verify_thread();
        context->verify_context();

        auto const handle = glCreateShader(static_cast<GLenum>(type));

        if (handle == 0)
            fail_initialization_error();

        auto const data = source.data();
        auto const size = static_cast<GLint>(source.size());

        glShaderSource(handle, 1, &data, &size);
        glCompileShader(handle);

        GLint status;
        glGetShaderiv(handle, GL_COMPILE_STATUS, std::addressof(status));

        if (status != GL_TRUE)
            fail_compile_error();

        return handle;
    }
}

namespace Coli::Graphics::inline OpenGL
{
    template <OpenGL::ShaderType Type> requires (Detail::OpenGL::ValidShaderType<Type>::value)
    BasicShader<Type>::BasicShader(std::shared_ptr<OpenGL::Context> context, std::string_view sourceCode) :
        resource_base(context, factory_type::create(context, Type, sourceCode), &factory_type::destroy)
    {}

    template <OpenGL::ShaderType Type> requires (Detail::OpenGL::ValidShaderType<Type>::value)
    BasicShader<Type>::BasicShader(BasicShader&&) noexcept(false) = default;

    template <OpenGL::ShaderType Type> requires (Detail::OpenGL::ValidShaderType<Type>::value)
    BasicShader<Type>& BasicShader<Type>::operator=(BasicShader&&) noexcept(false) = default;

    template <OpenGL::ShaderType Type> requires (Detail::OpenGL::ValidShaderType<Type>::value)
    BasicShader<Type>::~BasicShader() noexcept = default;

    template class COLI_EXPORT BasicShader<ShaderType::vertex>;
    template class COLI_EXPORT BasicShader<ShaderType::fragment>;
}