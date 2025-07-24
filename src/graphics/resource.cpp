#include "coli/graphics/resource.h"

namespace Coli::Graphics::Detail::inline OpenGL
{
#ifdef COLI_GRAPHICS_RESOURCE_TEMPLATE_HEADER
#undef COLI_GRAPHICS_RESOURCE_TEMPLATE_HEADER
#endif

#define COLI_GRAPHICS_RESOURCE_TEMPLATE_HEADER \
    template <std::regular HandleTy, class DeleterTy> \
    requires ( \
        std::is_nothrow_invocable_v<DeleterTy, Context&, HandleTy> && \
        std::is_move_constructible_v<DeleterTy> \
    )

    COLI_GRAPHICS_RESOURCE_TEMPLATE_HEADER
    void ResourceBase<HandleTy, DeleterTy>::fail_call_on_invalid() {
        throw std::invalid_argument("Call on the invalid handle");
    }

    COLI_GRAPHICS_RESOURCE_TEMPLATE_HEADER
    ResourceBase<HandleTy, DeleterTy>::ResourceBase(
        std::shared_ptr<Graphics::OpenGL::Context> context,
        handle_type handle,
        DeleterTy&& deleter
    ) noexcept (std::is_nothrow_move_constructible_v<DeleterTy>) :
        myContext (std::move(context)),
        myHandle  (handle),
        myDeleter (std::move(deleter))
    {}

    COLI_GRAPHICS_RESOURCE_TEMPLATE_HEADER
    ResourceBase<HandleTy, DeleterTy>::ResourceBase(ResourceBase&& other)
        noexcept(false) requires std::is_nothrow_move_constructible_v<DeleterTy>
    :
        myContext (verify(std::move(other.myContext))),
        myHandle  (other.myHandle),
        myDeleter (std::move(other.myDeleter))
    {
        other.myHandle = handle_type{};
    }

    COLI_GRAPHICS_RESOURCE_TEMPLATE_HEADER
    ResourceBase<HandleTy, DeleterTy>& ResourceBase<HandleTy, DeleterTy>::operator=(ResourceBase&& other)
        noexcept(false) requires std::is_nothrow_move_assignable_v<DeleterTy>
    {
        other.myContext->verify_thread();
        other.myContext->verify_context();

        clear();

        myContext = std::move(other.myContext);
        myHandle  = other.myHandle;
        myDeleter = std::move(other.myDeleter);

        other.myHandle = handle_type {};
        return *this;
    }

    COLI_GRAPHICS_RESOURCE_TEMPLATE_HEADER
    ResourceBase<HandleTy, DeleterTy>::~ResourceBase() noexcept {
        clear();
    }

    COLI_GRAPHICS_RESOURCE_TEMPLATE_HEADER
    std::shared_ptr<Graphics::OpenGL::Context> ResourceBase<HandleTy, DeleterTy>::verify(
        std::shared_ptr<Graphics::OpenGL::Context>&& context
    ) {
        context->verify_thread();
        context->verify_context();

        return context;
    }

    COLI_GRAPHICS_RESOURCE_TEMPLATE_HEADER
    bool ResourceBase<HandleTy, DeleterTy>::is_valid() const noexcept {
        return myHandle != handle_type {};
    }

    COLI_GRAPHICS_RESOURCE_TEMPLATE_HEADER
    void ResourceBase<HandleTy, DeleterTy>::clear() noexcept {
        if (is_valid())
            std::invoke(myDeleter, *myContext, myHandle);

        myHandle = handle_type {};
    }

    template class COLI_EXPORT ResourceBase
        <GLFWwindow*, void(*)(Graphics::OpenGL::Context&, GLFWwindow*) noexcept>;
}