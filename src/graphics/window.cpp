#include "coli/graphics/window.h"

namespace Coli::Graphics::Detail::inline OpenGL
{
    void WindowFactory::fail_invalid_param(std::string_view msg) {
        throw std::invalid_argument(msg.data());
    }

    void WindowFactory::fail_initialize_error() {
        throw std::runtime_error("Failed to create a window");
    }

    void WindowFactory::fail_no_monitor() {
        throw std::runtime_error("No monitor found");
    }

    void WindowFactory::fail_another_window() {
        throw std::logic_error("Another window is bound");
    }

    GLFWwindow* WindowFactory::create(
        std::shared_ptr<Graphics::OpenGL::Context>& context,
        std::string_view title,
        unsigned width,
        unsigned height
    ) {
        if (!context)
            fail_invalid_param("Invalid context");

        context->verify_thread();

        if (context->has_window())
            fail_another_window();

        if (title.empty())
            fail_invalid_param("Title is empty");

        auto const window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

        if (!window)
            fail_initialize_error();

        context->register_window(window);
        return window;
    }

    void WindowFactory::destroy(Graphics::OpenGL::Context& context, GLFWwindow* window) noexcept {
        if (window) {
            glfwDestroyWindow(window);
            context.unregister_window();
        }
    }
}

namespace Coli::Graphics::inline OpenGL
{
    Window::Settings::Settings(std::string_view t, unsigned w, unsigned h) :
        title(t),
        width(w),
        height(h)
    {}

    Window::Settings::Settings(Settings const&) = default;
    Window::Settings::Settings(Settings&&) noexcept = default;

    Window::Settings& Window::Settings::operator=(Settings const&) = default;
    Window::Settings& Window::Settings::operator=(Settings&&) noexcept = default;

    Window::Settings::~Settings() noexcept = default;

    Window::Window(std::shared_ptr<OpenGL::Context> context, Settings const& settings) :
        resource_base(
            context,
            Graphics::Detail::OpenGL::WindowFactory::create(context, settings.title, settings.width, settings.height),
            std::addressof(Graphics::Detail::OpenGL::WindowFactory::destroy)
        )
    {}

    Window::Window(Window&&) noexcept(false) = default;
    Window& Window::operator=(Window&&) noexcept(false) = default;

    Window::~Window() noexcept = default;

    std::pair<unsigned, unsigned> Window::size() const
    {
        if (is_valid()) {
            int width, height;
            glfwGetWindowSize(myHandle, &width, &height);

            return std::make_pair<unsigned, unsigned>(width, height);
        }

        resource_base::fail_call_on_invalid();
    }

    bool Window::should_close() const
    {
        if (is_valid())
           return glfwWindowShouldClose(myHandle);

        resource_base::fail_call_on_invalid();
    }
}