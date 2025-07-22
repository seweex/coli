#include "coli/graphics/context.h"

namespace Coli::Graphics::inline OpenGL
{
    class COLI_EXPORT Context::Tools final
    {
    public:
        class GLFW final
        {
        public:
            static inline bool initialized = false;
            static inline std::mutex mutex;
        };

        class GLAD final
        {
        public:
            static inline bool initialized = false;
            static inline std::mutex mutex;
        };
    };

    void Context::fail_incorrect_thread() {
        throw std::runtime_error("Call is not from the creation thread");
    }

    void Context::fail_already_exists() {
        throw std::logic_error("An another instance of the context already exists");
    }

    void Context::fail_initialize_error() {
        throw std::runtime_error("Failed to initialize context");
    }

    void Context::fail_not_ready() {
        throw std::logic_error("The context is not ready");
    }

    Context::Context() :
        myCreationThread(std::this_thread::get_id()),
        myHasWindow(false)
    {
        std::lock_guard lock { Tools::GLFW::mutex };

        if (!Tools::GLFW::initialized) {
            if (glfwInit() == GLFW_TRUE)
            {
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, COLI_OPENGL_VERSION_MAJOR);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, COLI_OPENGL_VERSION_MINOR);

                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

                Tools::GLFW::initialized = true;
                myIsOwner = true;
            }
            else fail_initialize_error();
        }
        else fail_already_exists();
    }

    Context::Context(Context&& other) noexcept(false) {
        *this = std::move(other);
    }

    Context& Context::operator=(Context&& other) noexcept(false)
    {
        other.verify_thread();

        myCreationThread = std::exchange(other.myCreationThread, {});
        myIsOwner = std::exchange(other.myIsOwner, false);
        myHasWindow = std::exchange(other.myHasWindow, false);

        return *this;
    }

    Context::~Context() noexcept
    {
        if (myIsOwner) {
            std::lock_guard lock { Tools::GLFW::mutex };

            glfwTerminate();
            Tools::GLFW::initialized = false;
        }
    }

    void Context::verify_thread() const {
        if (myCreationThread != std::this_thread::get_id())
            fail_incorrect_thread();
    }

    void Context::verify_context() const {
        if (!Tools::GLFW::initialized ||
            !Tools::GLAD::initialized ||
            !myHasWindow ||
            !myIsOwner
        )
            fail_not_ready();
    }
}
