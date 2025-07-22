#ifndef COLI_GRAPHICS_RESOURCE_H
#define COLI_GRAPHICS_RESOURCE_H

#include "coli/graphics/context.h"

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
        /// @brief Type of the resource handle
        using handle_type = std::remove_cvref_t<HandleTy>;

        static_assert(
            std::same_as<handle_type, HandleTy>,
            "Qualified or refs types are not allowed");

    protected:
        ResourceBase(
            std::shared_ptr<Graphics::OpenGL::Context>&& context,
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
}

#endif
