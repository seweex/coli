#include "coli/graphics/program.h"

namespace Coli::Graphics::Detail::inline OpenGL
{
    void ProgramFactory::fail_initialization_error() {
        throw std::invalid_argument("Failed to create an OpenGL program");
    }

    void ProgramFactory::fail_invalid_context() {
        throw std::invalid_argument("Invalid context");
    }

    void ProgramFactory::fail_invalid_shader() {
        throw std::invalid_argument("Invalid shader");
    }

    void ProgramFactory::fail_link_error() {
        throw std::runtime_error("Failed to link the program");
    }

    void ProgramFactory::destroy(Graphics::OpenGL::Context& context, GLuint handle) noexcept
    {
        if (handle != 0)
            glDeleteProgram(handle);

        (void)context;
    }

    GLuint ProgramFactory::create(
        std::shared_ptr<Graphics::OpenGL::Context>& context,
        VertexShader const& vertex,
        FragmentShader const& fragment
    ) {
        if (!context)
            fail_invalid_context();

        if (!(vertex.is_valid() && fragment.is_valid()))
            fail_invalid_shader();

        context->verify_thread();
        context->verify_context();

        auto const handle = glCreateProgram();

        if (handle == 0)
            fail_initialization_error();

        glAttachShader(handle, vertex.get_handle());
        glAttachShader(handle, fragment.get_handle());

        glLinkProgram(handle);

        glDetachShader(handle, fragment.get_handle());
        glDetachShader(handle, vertex.get_handle());

        GLint status;
        glGetProgramiv(handle, GL_LINK_STATUS, std::addressof(status));

        if (status != GL_TRUE)
            fail_link_error();

        return handle;
    }
}

namespace Coli::Graphics::inline OpenGL
{
    class Program::Binding final
    {
    public:
        [[noreturn]] static void fail_another_bound() {
            throw std::logic_error("Another program bound");
        }

        static inline GLuint current;
    };

    Program::Program(
        std::shared_ptr<OpenGL::Context> context,
        VertexShader const& vertex,
        FragmentShader const& fragment
    ) :
        resource_base(context, factory_type::create(context, vertex, fragment), &factory_type::destroy)
    {}

    Program::Program(Program&&) noexcept(false) = default;

    Program& Program::operator=(Program&&) noexcept(false) = default;

    Program::~Program() noexcept = default;

    void Program::bind()
    {
        if (!is_valid())
            fail_call_on_invalid();

        if (Binding::current == 0) {
            glUseProgram(myHandle);
            Binding::current = myHandle;
        }
        else
            Binding::fail_another_bound();
    }

    void Program::unbind() noexcept
    {
        if (myHandle != 0 && Binding::current == myHandle) {
            glUseProgram(0);
            Binding::current = 0;
        }
    }
}
