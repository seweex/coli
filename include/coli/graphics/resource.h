#ifndef COLI_GRAPHICS_RESOURCE_H
#define COLI_GRAPHICS_RESOURCE_H

#include "coli/graphics/context.h"

/**
 * @brief For internal details.
 * @note The user should not use this namespace.
 */
namespace Coli::Graphics::Detail::inline OpenGL
{
    template <std::regular HandleTy, class DeleterTy>
    requires (
        std::is_nothrow_invocable_v<DeleterTy, Context&, HandleTy> &&
        std::is_move_constructible_v<DeleterTy>
    )
    class COLI_EXPORT ResourceBase
    {
        [[nodiscard]] static std::shared_ptr<Graphics::OpenGL::Context>
        verify(std::shared_ptr<Graphics::OpenGL::Context>&& context);

    public:
        using handle_type = std::remove_cvref_t<HandleTy>;

        static_assert(
            std::same_as<handle_type, HandleTy>,
            "qualified or refs types are not allowed");

    protected:
        [[noreturn]] static void fail_call_on_invalid();

        ResourceBase(
            std::shared_ptr<Graphics::OpenGL::Context> context,
            handle_type handle,
            DeleterTy&& deleter = {}
        ) noexcept(std::is_nothrow_move_constructible_v<DeleterTy>);

    public:
        ResourceBase(ResourceBase&&) noexcept(false) requires std::is_nothrow_move_constructible_v<DeleterTy>;
        ResourceBase& operator=(ResourceBase&&) noexcept(false) requires std::is_nothrow_move_assignable_v<DeleterTy>;

        ResourceBase(ResourceBase const&) = delete;
        ResourceBase& operator=(ResourceBase const&) = delete;

        ~ResourceBase() noexcept;

        [[nodiscard]] bool is_valid() const noexcept;

        void clear() noexcept;

    protected:
        std::shared_ptr<Graphics::OpenGL::Context> myContext;
        handle_type myHandle;

    private:
        DeleterTy myDeleter;
    };

#if COLI_BUILD
#else
    extern template class COLI_EXPORT ResourceBase
        <GLFWwindow*, void(*)(Graphics::OpenGL::Context&, GLFWwindow*) noexcept>;

    extern template class COLI_EXPORT ResourceBase
        <GLuint, void(*)(Graphics::OpenGL::Context&, GLuint) noexcept>;
#endif
}

/// @brief Namespace for the all OpenGL-based stuff.
namespace Coli::Graphics::inline OpenGL
{
    /**
     * @brief RAII binding wrapper.
     * @detail Wraps the RAII binding handle of resources.
     *
     * @tparam Ty Type of bindable object.
     */
    template <class Ty>
        requires (requires (Ty& obj)
    {
        obj.bind();
        obj.unbind();
    })
    class COLI_EXPORT Binding final
    {
    public:
        /**
         * @brief Locks binding.
         * @detail Creates wrapper that binds object to
         * the graphic pipeline and locks the binding.
         *
         * @param object Object to bind.
         */
        explicit Binding(Ty& object) : myObject(object) {
            myObject.bind();
        }

        /**
         * @brief Unlocks binding.
         * @details Destroys the wrapper and unlocks the binding.
         */
        ~Binding() noexcept {
            myObject.unbind();
        }

        Binding(Binding const&) = delete;
        Binding(Binding&&) = delete;

        Binding& operator=(Binding const&) = delete;
        Binding& operator=(Binding&&) = delete;

    private:
        Ty& myObject;
    };
}

#endif
