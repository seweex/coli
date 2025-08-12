#ifndef COLI_GRAPHICS_WINDOW_H
#define COLI_GRAPHICS_WINDOW_H

#include "coli/graphics/resource.h"

/**
 * @brief For internal details.
 * @note The user should not use this namespace.
 */
namespace Coli::Graphics::Detail::inline OpenGL
{
    class COLI_EXPORT WindowFactory final
    {
        [[noreturn]] static void fail_invalid_param(std::string_view msg);
        [[noreturn]] static void fail_initialize_error();
        [[noreturn]] static void fail_no_monitor();
        [[noreturn]] static void fail_another_window();

    public:
        static void destroy(Graphics::OpenGL::Context& context, GLFWwindow* window) noexcept;

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

/// @brief Namespace for the all OpenGL-based stuff.
namespace Coli::Graphics::inline OpenGL
{
    /**
     * @brief OpenGL window class.
     * @detail Represents window.
     *
     * @note Does not allow multithreading. You should use
     * this class from the thread where you created the context.
     */
    class COLI_EXPORT Window final :
        public Graphics::Detail::OpenGL::WindowFactory::resource_type
    {
        using resource_base = Graphics::Detail::OpenGL::WindowFactory::resource_type;

    public:
        /**
         * @brief Window settings.
         * @detail Groups all windows settings used in its initialization.
         */
        struct COLI_EXPORT Settings final
        {
            /**
             * @brief Creates window settings.
             * @details Creates window settings from user-specified
             * parameters.
             *
             * @param t Window title;
             * @param w Window width;
             * @param h Window height.
             *
             * @throw std::bad_alloc Is allocation fails.
             */
            Settings(std::string_view t, unsigned w, unsigned h);

            /**
             * @brief Copies settings.
             * @detail Copies the settings.
             */
            Settings(Settings const&);

            /**
             * @brief Moves settings.
             * @detail Moves the settings.
             */
            Settings(Settings&&) noexcept;

            /// @copydoc Settings(Settings const&)
            Settings& operator=(Settings const&);

            /// @copydoc Settings(Settings&&)
            Settings& operator=(Settings&&) noexcept;

            /**
             * @brief Destroys settings.
             * @detail Destroys the settings.
             */
            ~Settings() noexcept;

            /// @brief Title of the window.
            std::string title;

            /// @brief Width of the window.
            unsigned width;

            /// @brief Height of the window.
            unsigned height;
        };

        /**
         * @brief Creates window.
         * @details Creates a valid window.
         *
         * @param context The valid context the window will bound to;
         * @param settings Window settings.
         *
         * @throw std::invalid_argument If context is invalid;
         * @throw std::invalid_argument If `settings.title` is empty;
         *
         * @throw std::logic_error If calls not from the context creation thread;
         * @throw std::logic_error If another window bound to the context;
         *
         * @throw std::runtime_error If initialization fails.
         */
        Window(std::shared_ptr<OpenGL::Context> context, Settings const& settings);

        /**
         * @brief Moves window.
         * @details Moves the window.
         *
         * @throw std::logic_error If the window moves from other thread.
         */
        Window(Window&&) noexcept(false);

        /// @copydoc Window(Window&&)
        Window& operator=(Window&&) noexcept(false);

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        /**
         * @brief Destroys window.
         * @details Destroys the window.
         */
        ~Window() noexcept;

        /**
         * @brief Returns window size.
         * @brief Returns the sizes of the window.
         *
         * @throw std::invalid_argument If calls on invalid.
         *
         * @return The sizes of the window { width, height }.
         */
        [[nodiscard]] std::pair<unsigned, unsigned> size() const;

        /**
         * @brief Returns window closing.
         * @brief Returns the window closing status.
         *
         * @throw std::invalid_argument If calls on invalid.
         *
         * @return The window closing status.
         * @retval True If window is closing;
         * @retval False Otherwise.
         */
        [[nodiscard]] bool should_close() const;

    private:
        using resource_base::myContext;
        using resource_base::myHandle;
    };
}

#endif
