#ifndef COLI_GRAPHICS_CONTEXT_H
#define COLI_GRAPHICS_CONTEXT_H

#include "coli/utility.h"

namespace Coli::Graphics::inline OpenGL
{
    /**
     * @brief The OpenGL context class
     *
     * @warning This class blocks access from different threads
     * @warning Only one existing instance of this class is allowed
     *
     * @note You must create this class via std::make_shared
     */
    class COLI_EXPORT Context final
    {
        [[noreturn]] static void fail_incorrect_thread();
        [[noreturn]] static void fail_already_exists();
        [[noreturn]] static void fail_initialize_error();
        [[noreturn]] static void fail_not_ready();

        class Tools;

        friend class Window;

        void register_window(GLFWwindow* handle) noexcept;

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
         * @throw std::runtime_error If the creation thread do not match with the current one
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
         * @throw std::runtime_error If the threads do not match
         */
        void verify_thread() const;

        /**
         * @brief Verify the context is ready. It should be initialized and have a window
         *
         * @throw std::runtime_error If the context are not ready
         */
        void verify_context() const;

    private:
        std::thread::id myCreationThread;
        bool myIsOwner;
        bool myHasWindow;
    };
}

#endif
