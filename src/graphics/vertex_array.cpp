#include "coli/graphics/vertex_array.h"

namespace Coli::Graphics::Detail::inline OpenGL
{
    void VertexArrayFactory::fail_initialization_error() {
        throw std::runtime_error("Failed to create an OpenGL vertex array");
    }

    void VertexArrayFactory::fail_invalid_context() {
        throw std::invalid_argument("Invalid context");
    }

    void VertexArrayFactory::fail_invalid_storage() {
        throw std::invalid_argument("Invalid storage buffer");
    }

    void VertexArrayFactory::destroy(Graphics::OpenGL::Context& context, GLuint handle) noexcept
    {
        if (handle != 0)
            glDeleteVertexArrays(1, &handle);

        (void)context;
    }

    GLuint VertexArrayFactory::create(std::shared_ptr<Graphics::OpenGL::Context>& context)
    {
        if (!context)
            fail_invalid_context();

        context->verify_thread();
        context->verify_context();

        GLuint handle;
        glCreateVertexArrays(1, &handle);

        if (handle == 0)
            fail_initialization_error();

        return handle;
    }

    void VertexArrayFactory::configure(
        Graphics::OpenGL::VertexArray& array,
        std::shared_ptr<VertexStorage>& vertices,
        std::shared_ptr<IndexStorage>& indices,
        std::vector<VertexAttributes> const& vertexAttributes,
        size_t vertexSize
    ) {
        if (!(vertices && indices))
            fail_invalid_storage();

        Binding vaoBinding { array };
        Binding vboBinding { *vertices };
        Binding eboBinding { *indices };

        for (size_t i = 0; i < vertexAttributes.size(); ++i)
        {
            glVertexAttribPointer(
                static_cast<GLuint>(i),
                static_cast<GLint>(vertexAttributes[i].length),
                vertexAttributes[i].type,
                GL_FALSE,
                static_cast<GLsizei>(vertexSize),
                reinterpret_cast<void*>(vertexAttributes[i].offset)
            );

            glEnableVertexAttribArray(static_cast<GLuint>(i));
        }
    }
}

namespace Coli::Graphics::inline OpenGL
{
    class VertexArray::Binding final
    {
    public:
        [[noreturn]] static void fail_another_bound() {
            throw std::logic_error("Another vertex array is bound");
        }

        static inline GLuint current = 0;
    };

    VertexArray::VertexArray(VertexArray&&) noexcept(false) = default;
    VertexArray& VertexArray::operator=(VertexArray&&) noexcept(false) = default;

    VertexArray::~VertexArray() noexcept = default;

    std::weak_ptr<VertexStorage> VertexArray::get_vertices()
    {
        if (!is_valid())
            fail_call_on_invalid();

        return myVertices;
    }

    std::weak_ptr<IndexStorage> VertexArray::get_indices()
    {
        if (!is_valid())
            fail_call_on_invalid();

        return myIndices;
    }

    void VertexArray::bind()
    {
        if (!is_valid())
            fail_call_on_invalid();

        if (Binding::current == 0) {
            glBindVertexArray(myHandle);
            Binding::current = myHandle;
        }
        else
            Binding::fail_another_bound();
    }

    void VertexArray::unbind() noexcept
    {
        if (myHandle != 0 && Binding::current == myHandle) {
            glBindVertexArray(0);
            Binding::current = 0;
        }
    }

    void VertexArray::clear() noexcept
    {
        myIndices.reset();
        myVertices.reset();

        resource_base::clear();
    }
}
