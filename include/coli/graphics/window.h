#ifndef COLI_GRAPHICS_WINDOW_H
#define COLI_GRAPHICS_WINDOW_H

#include "coli/graphics/resource.h"

namespace Coli::Graphics::Detail::inline OpenGL
{
    /**
     * @brief A factory class for creating OpenGL windows
     *
     * @warning This class and all its methods are not supposed to be used by the user
     * @note This class does not allow instances
     */
    class COLI_EXPORT WindowFactory final
    {
        /// @throw std::invalid_argument
        [[noreturn]] static void fail_invalid_param(std::string_view msg);

        /// @throw std::runtime_error
        [[noreturn]] static void fail_initialize_error();

        /// @throw std::runtime_error
        [[noreturn]] static void fail_no_monitor();

        /// @throw std::logic_error
        [[noreturn]] static void fail_another_window();

    public:
        /**
         * @brief Destroys the window
         *
         * @param context The context that was used to create the window
         * @param window The window handle
         */
        static void destroy(Graphics::OpenGL::Context& context, GLFWwindow* window) noexcept;

        /**
         * @brief Creates a GLFW window
         *
         * @param context The valid context the window will bound to
         * @param title The window title
         * @param width The window width
         * @param height The window height
         *
         * @throw std::invalid_argument If context is invalid
         * @throw std::invalid_argument If title is empty
         *
         * @throw std::logic_error If calls not from the context creation thread
         * @throw std::logic_error If another window bound to the context
         *
         * @throw std::runtime_error If initialization fails
         *
         * @return A valid raw handle to the newly created window
         */
        [[nodiscard]] static GLFWwindow* create(
            std::shared_ptr<Graphics::OpenGL::Context>& context,
            std::string_view title,
            unsigned width,
            unsigned height
        );

        using deleter_type = decltype(&destroy);
        using resource_type = OpenGL::ResourceBase<GLFWwindow*, deleter_type>;

        WindowFactory() = delete;

        WindowFactory(const WindowFactory&) = delete;
        WindowFactory(WindowFactory&&) = delete;

        WindowFactory& operator=(const WindowFactory&) = delete;
        WindowFactory& operator=(WindowFactory&&) = delete;
    };
}

namespace Coli::Graphics::inline OpenGL
{
    /**
     * @brief The OpenGL window class
     *
     * @warning This class allows access only from the creation thread
     */
    class COLI_EXPORT Window final :
        public Graphics::Detail::OpenGL::WindowFactory::resource_type
    {
        using resource_base = Graphics::Detail::OpenGL::WindowFactory::resource_type;

    public:
        /// @brief Struct for group the window settings
        struct COLI_EXPORT Settings final
        {
            /**
             * @brief Creates window settings
             *
             * @param t Title of the window
             * @param w Width of the window
             * @param h Height of the window
             *
             * @throw std::bad_alloc Is allocation fails
             */
            Settings(std::string_view t, unsigned w, unsigned h);

            Settings(Settings const&);
            Settings(Settings&&) noexcept;

            Settings& operator=(Settings const&);
            Settings& operator=(Settings&&) noexcept;

            ~Settings() noexcept;

            /// @brief Title of the window
            std::string title;

            /// @brief Width of the window
            unsigned width;

            /// @brief Height of the window
            unsigned height;
        };

        /**
         * @brief Creates a window
         *
         * @param context The valid context the window will bound to
         * @param settings The window settings
         *
         * @throw std::invalid_argument If context is invalid
         * @throw std::invalid_argument If `settings.title` is empty
         *
         * @throw std::logic_error If calls not from the context creation thread
         * @throw std::logic_error If another window bound to the context
         *
         * @throw std::runtime_error If initialization fails
         */
        Window(std::shared_ptr<OpenGL::Context> context, Settings const& settings);

        /**
         * @brief Moves the window
         *
         * @throw std::logic_error If the window moves into other thread
         */
        Window(Window&&) noexcept(false);

        /// @copydoc Window(Window&&)
        Window& operator=(Window&&) noexcept(false);

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        ~Window() noexcept;

        /**
         * @brief Gets the sizes of the window
         *
         * @throw std::invalid_argument If calls on the null handle
         *
         * @return The sizes of the window { width, height }
         */
        [[nodiscard]] std::pair<unsigned, unsigned> size() const;

        /**
        * @brief Gets the window's close flag
        *
        * @throw std::invalid_argument If calls on the null handle
        *
        * @return True if window is closing, false otherwise
        */
        [[nodiscard]] bool should_close() const;

    private:
        using resource_base::myContext;
        using resource_base::myHandle;
    };
}

#endif
