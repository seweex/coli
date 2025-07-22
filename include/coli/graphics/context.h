#ifndef COLI_GRAPHICS_CONTEXT_H
#define COLI_GRAPHICS_CONTEXT_H

#include "coli/utility.h"

namespace Coli::Graphics::Detail::inline OpenGL {
    class WindowFactory;
}

namespace Coli::Graphics::inline OpenGL
{
    /**
     * @brief The OpenGL context class
     *
     * @warning This class blocks any access from different threads
     * @warning Only one active instance of this class is allowed
     *
     * @note You must create this class via std::make_shared
     */
    class COLI_EXPORT Context final
    {
        /// @throws std::logic_error
        [[noreturn]] static void fail_incorrect_thread();

        /// @throws std::logic_error
        [[noreturn]] static void fail_already_exists();

        /// @throws std::runtime_error
        [[noreturn]] static void fail_initialize_error();

        /// @throws std::logic_error
        [[noreturn]] static void fail_not_ready();

        /// @throws std::invalid_argument
        [[noreturn]] static void fail_not_owner();

        class Tools;

        friend class Graphics::Detail::OpenGL::WindowFactory;

        /// @throws std::runtime_error
        static void load_opengl(GLFWwindow* handle);

        /**
         * @throws std::runtime_error
         * @throws std::logic_error
         */
        void register_window(GLFWwindow* handle);

        void unregister_window() noexcept;

    public:
        /**
         * @brief Initializes the OpenGL context
         *
         * @throw std::logic_error If the context already exists
         * @throw std::runtime_error If initialization fails
         */
        Context();

        /**
         * @brief Moves the context
         *
         * @param other The context to move from
         *
         * @throw std::logic_error If the creation thread do not match with the current one
         */
        Context(Context&& other) noexcept(false);

        /// @copydoc Context(Context&&)
        Context& operator=(Context&& other) noexcept(false);

        Context(Context const&) = delete;
        Context& operator=(Context const&) = delete;

        /// @brief Destroys the context
        ~Context() noexcept;

        /**
         * @brief Verify the creation thread and the current one match
         *
         * @throw std::logic_error If the threads do not match
         */
        void verify_thread() const;

        /**
         * @brief Verify the context is ready. It should be initialized and have a window
         *
         * @throw std::logic_error If the context are not ready
         */
        void verify_context() const;

        /**
         * @brief Checks does the context has a window
         *
         * @throw std::invalid_argument If calls on a not owning object
         *
         * @return True if the context has a window, false otherwise
         */
        [[nodiscard]] bool has_window() const;

        /**
         * @brief Checks the object owns the context
         *
         * @return True if this object is owning the context, false otherwise
         */
        [[nodiscard]] bool is_owning() const noexcept;

    private:
        std::thread::id myCreationThread;
        bool myIsOwner;
        bool myHasWindow;
    };
}

#endif
