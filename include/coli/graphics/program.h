#ifndef COLI_GRAPHICS_PROGRAM_H
#define COLI_GRAPHICS_PROGRAM_H

#include "coli/graphics/shader.h"

/**
 * @brief For internal details.
 * @note The user should not use this namespace.
 */
namespace Coli::Graphics::Detail::inline OpenGL
{
    class ProgramFactory final
    {
        [[noreturn]] static void fail_initialization_error();
        [[noreturn]] static void fail_invalid_context();
        [[noreturn]] static void fail_invalid_shader();
        [[noreturn]] static void fail_link_error();

    public:
        static void destroy(Graphics::OpenGL::Context& context, GLuint handle) noexcept;

        [[nodiscard]] static GLuint create(
            std::shared_ptr<Graphics::OpenGL::Context>& context,
            VertexShader const& vertex,
            FragmentShader const& fragment);

        using deleter_type = decltype(&destroy);
        using resource_type = ResourceBase<GLuint, deleter_type>;

        ProgramFactory() = delete;

        ProgramFactory(const ProgramFactory&) = delete;
        ProgramFactory(ProgramFactory&&) = delete;

        ProgramFactory& operator=(const ProgramFactory&) = delete;
        ProgramFactory& operator=(ProgramFactory&&) = delete;
    };
}

/// @brief Namespace for the all OpenGL-based stuff.
namespace Coli::Graphics::inline OpenGL
{
    /// @brief OpenGL program class
    class COLI_EXPORT Program final :
        public Detail::OpenGL::ProgramFactory::resource_type
    {
        using factory_type = Detail::OpenGL::ProgramFactory;
        using resource_base = typename factory_type::resource_type;

        class Binding;

    public:
        /**
         * @brief Creates program.
         * @details Creates a valid program from the user's shaders.
         *
         * @param context The valid context to check it is loaded;
         * @param vertex Vertex shader;
         * @param fragment Fragment shader.
         *
         * @throw std::invalid_argument If the context is invalid;
         * @throw std::invalid_argument If the vertex shader is invalid;
         * @throw std::invalid_argument If the fragment shader is invalid;
         *
         * @throw std::logic_error If calls not from the context creation thread;
         * @throw std::runtime_error If initialization fails.
         */
         Program(
            std::shared_ptr<OpenGL::Context> context,
            VertexShader const& vertex,
            FragmentShader const& fragment
        );

        /**
         * @brief Moves program.
         * @detail Moves the program.
         *
         * @throw std::logic_error If moved from other thread;
         */
        Program(Program&&) noexcept(false);

        /// @copydoc Program(Program&&)
        Program& operator=(Program&&) noexcept(false);

        Program(const Program&) = delete;
        Program& operator=(const Program&) = delete;

        /**
         * @brief Destroys program.
         * @detail Destroys the program.
         */
        ~Program() noexcept;

        /**
         * @brief Binds program.
         * @brief Binds the program to the graphic pipeline.
         *
         * @throw std::invalid_argument If calls on the invalid object;
         * @throw std::logic_error If calls not from the context creation thread;
         * @throw std::logic_error If another program is bound.
         */
        void bind();

        /**
         * @brief Unbinds program.
         * @brief Unbinds the program from the graphic pipeline if
         * the bound program is current.
         */
        void unbind() noexcept;
    };
}

#endif
