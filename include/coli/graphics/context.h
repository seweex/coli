#ifndef COLI_GRAPHICS_CONTEXT_H
#define COLI_GRAPHICS_CONTEXT_H

#include "coli/utility.h"

/**
 * @brief For internal details.
 * @note The user should not use this namespace.
 */
namespace Coli::Graphics::Detail::inline OpenGL
{
    class WindowFactory;
}

/// @brief Namespace for the all OpenGL-based stuff.
namespace Coli::Graphics::inline OpenGL
{
    /**
     * @brief OpenGL context class.
     * @detail Represent the context of the OpenGL.
     *
     * @note Does not allow multithreading. You should use
     * this class from the thread where you created the context.
     *
     * @note You must create this class via std::make_shared
     *
     * @warning Only one active instance of this class is allowed
     */
    class COLI_EXPORT Context final
    {
        [[noreturn]] static void fail_incorrect_thread();
        [[noreturn]] static void fail_already_exists();
        [[noreturn]] static void fail_initialize_error();
        [[noreturn]] static void fail_not_ready();
        [[noreturn]] static void fail_not_owner();

        class Tools;

        friend class Graphics::Detail::OpenGL::WindowFactory;

        static void load_opengl(GLFWwindow* handle);

        void register_window(GLFWwindow* handle);
        void unregister_window() noexcept;

    public:
        /**
         * @brief Creates context.
         * @details Creates a context and initializes it.
         *
         * @throw std::logic_error If the context already exists;
         * @throw std::runtime_error If initialization fails.
         */
        Context();

        /**
         * @brief Moves context.
         * @detail Moves the context.
         *
         * @param other Other context instance.
         *
         * @throw std::logic_error If the call thread is not the creation thread.
         */
        Context(Context&& other) noexcept(false);

        /// @copydoc Context(Context&&)
        Context& operator=(Context&& other) noexcept(false);

        Context(Context const&) = delete;
        Context& operator=(Context const&) = delete;

        /**
         * @brief Destroys context.
         * @detail Destroys the context. Releases it if destroys
         * the active one.
         */
        ~Context() noexcept;

        /**
         * @brief Verifies creation thread.
         * @brief Verifies the current thread is the creation thread.
         *
         * @throw std::logic_error If the threads do not match.
         */
        void verify_thread() const;

        /**
         * @brief Verifies context ready.
         * @brief Verifies the context is initialized successfully and completely.
         *
         * @throw std::logic_error If the context is not ready.
         */
        void verify_context() const;

        /**
         * @brief Checks window presence.
         * @brief Checks the context has a window
         *
         * @throw std::invalid_argument If calls not on the active context.
         *
         * @return Presence of a window.
         *
         * @retval True If the context has a window;
         * @retval False Otherwise.
         */
        [[nodiscard]] bool has_window() const;

        /**
         * @brief Checks context is active
         * @detail Checks the context is active.
         *
         * @return Checking result.
         *
         * @retval True If called on the active context;
         * @retval True False otherwise.
         */
        [[nodiscard]] bool is_owning() const noexcept;

    private:
        std::thread::id myCreationThread;
        bool myIsOwner;
        bool myHasWindow;
    };
}

#endif
