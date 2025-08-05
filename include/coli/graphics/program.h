#ifndef COLI_GRAPHICS_PROGRAM_H
#define COLI_GRAPHICS_PROGRAM_H

#include "coli/graphics/shader.h"

namespace Coli::Graphics::Detail::inline OpenGL
{
    /**
     * @brief A factory class for creating OpenGL programs
     *
     * @warning This class and all its methods are not supposed to be used by the user
     * @note This class does not allow instances
     */
    class ProgramFactory final
    {
        [[noreturn]] static void fail_initialization_error();
        [[noreturn]] static void fail_invalid_context();
        [[noreturn]] static void fail_invalid_shader();
        [[noreturn]] static void fail_link_error();

    public:
        /**
         * @brief Destroys the shader
         *
         * @param context The context that was used to create a program
         * @param handle The program handle
         */
        static void destroy(Graphics::OpenGL::Context& context, GLuint handle) noexcept;

        /**
         * @brief Creates an OpenGL program
         *
         * @param context The valid context to check it loaded
         * @param vertex A vertex shader
         * @param fragment A fragment shader
         *
         * @throw std::invalid_argument If the context is invalid
         * @throw std::invalid_argument If the vertex shader is invalid
         * @throw std::invalid_argument If the fragment shader is invalid
         *
         * @throw std::logic_error If calls not from the context creation thread
         * @throw std::runtime_error If initialization fails
         *
         * @return A valid raw handle to the newly created program
         */
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
        * @brief Creates a valid program and links it with your
        * shaders
        *
        * @param context The valid context to check it is loaded
        * @param vertex The vertex shader
        * @param fragment The fragment shader
        *
        * @throw std::invalid_argument If the context is invalid
        * @throw std::invalid_argument If the vertex shader is invalid
        * @throw std::invalid_argument If the fragment shader is invalid
        *
        * @throw std::logic_error If calls not from the context creation thread
        * @throw std::runtime_error If initialization fails
        */
        Program(
           std::shared_ptr<OpenGL::Context> context,
           VertexShader const& vertex,
           FragmentShader const& fragment
       );

        /**
         * @brief Accepts a moved program
         *
         * @throw std::logic_error If moved from other thread
         */
        Program(Program&&) noexcept(false);

        /**
         * @copydoc Program(Program&&)
         * @return A reference to itself
         */
        Program& operator=(Program&&) noexcept(false);

        Program(const Program&) = delete;
        Program& operator=(const Program&) = delete;

        /// @brief Deletes the shader
        ~Program() noexcept;

        /**
         * @brief Binds the program to the graphic pipeline
         *
         * @throw std::invalid_argument If calls on the invalid object
         * @throw std::logic_error If calls not from the context creation thread
         * @throw std::logic_error If another program is bound
         */
        void bind();

        /// @brief Unbinds the bound program
        void unbind() noexcept;
    };
}

#endif
