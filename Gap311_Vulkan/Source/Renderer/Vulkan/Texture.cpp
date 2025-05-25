#include "Texture.h"

#include <SDL_image.h>

Texture::Texture(GAP311::VulkanWrapper& vulkanWrapper)
    : m_vulkanContext(vulkanWrapper)
    , m_filePath("Null")
{
    LoadDefaultTexture();
}

Texture::Texture(const char* filePath, GAP311::VulkanWrapper& vulkanWrapper)
    : m_vulkanContext(vulkanWrapper)
    , m_filePath(filePath)
{
    LoadTexture(LoadCompatibleImage(filePath));
}

Texture::~Texture()
{
    if (m_image)
    {
        m_vulkanContext.DestroyImageAndView(m_image, m_view);
        m_vulkanContext.DestroyUniform(shaderUniform);

        if (m_textureSampler != VK_NULL_HANDLE)
        {
            vkDestroySampler(m_vulkanContext.GetDevice(), m_textureSampler, nullptr);
            m_textureSampler = VK_NULL_HANDLE;
        }

        m_image = VK_NULL_HANDLE;
        m_view = VK_NULL_HANDLE;

        m_layout = VK_NULL_HANDLE;
        m_descriptorSet = VK_NULL_HANDLE;
    }
}


void Texture::LoadTexture(SDL_Surface* surface)
{
    if (m_image)
    {
        m_vulkanContext.DestroyImageAndView(m_image, m_view);
    }

    vk::Format format = GAP311::VulkanWrapper::VulkanFormatFromSDLFormat(surface->format->format);
    auto imageView = m_vulkanContext.CreateTexture2DImageAndView({ (uint32_t)surface->w, (uint32_t)surface->h }, format,
        surface->pixels, surface->w * surface->h * surface->format->BytesPerPixel);
    m_image = std::get<0>(imageView);
    m_view = std::get<1>(imageView);

    m_textureSampler = m_vulkanContext.GetDevice().createSampler(
        vk::SamplerCreateInfo()
        .setAddressModeU(vk::SamplerAddressMode::eRepeat)
        .setAddressModeV(vk::SamplerAddressMode::eRepeat)
        .setMinFilter(vk::Filter::eLinear)
        .setMagFilter(vk::Filter::eLinear)
    );

    shaderUniform = m_vulkanContext.CreateUniformForImage(0, m_view, m_textureSampler);
    m_layout = shaderUniform.GetLayout();
    m_descriptorSet = shaderUniform.GetSet();

    SDL_FreeSurface(surface);
}

vk::DescriptorSet* Texture::GetDescriptorSet()
{
    if (!m_descriptorSet)
        m_descriptorSet =  m_vulkanContext.CreateUniformForImage(0, m_view, m_textureSampler);

    return &m_descriptorSet;
}

void Texture::LoadDefaultTexture()
{
    SDL_Surface* magentaSurface = SDL_CreateRGBSurfaceWithFormat(0, 1, 1, 32, SDL_PIXELFORMAT_RGBA32);

    // Fill it with magenta (255, 0, 255, 255)
    Uint32 magentaColor = SDL_MapRGBA(magentaSurface->format, 255, 0, 255, 255);
    SDL_FillRect(magentaSurface, nullptr, magentaColor);

    vk::Format format = GAP311::VulkanWrapper::VulkanFormatFromSDLFormat(magentaSurface->format->format);
    auto imageView = m_vulkanContext.CreateTexture2DImageAndView({ (uint32_t)magentaSurface->w, (uint32_t)magentaSurface->h }, format,
        magentaSurface->pixels, magentaSurface->w * magentaSurface->h * magentaSurface->format->BytesPerPixel);
    m_image = std::get<0>(imageView);
    m_view = std::get<1>(imageView);

    m_textureSampler = m_vulkanContext.GetDevice().createSampler(
        vk::SamplerCreateInfo()
        .setAddressModeU(vk::SamplerAddressMode::eClampToEdge)
        .setAddressModeV(vk::SamplerAddressMode::eClampToEdge)
        .setMinFilter(vk::Filter::eLinear)
        .setMagFilter(vk::Filter::eLinear)
    );

    shaderUniform = m_vulkanContext.CreateUniformForImage(0, m_view, m_textureSampler);
    m_layout = shaderUniform.GetLayout();
    m_descriptorSet = shaderUniform.GetSet();

    SDL_FreeSurface(magentaSurface);

}

SDL_Surface* Texture::LoadCompatibleImage(const char* imageFile, Uint32 pixelFormat)
{
    SDL_Surface* surface = IMG_Load(imageFile);
    if (!surface)
        return nullptr;

    if (surface->format->format != pixelFormat)
    {
        SDL_Surface* newFormat = SDL_ConvertSurfaceFormat(surface, pixelFormat, 0);
        if (newFormat)
        {
            SDL_FreeSurface(surface);
            surface = newFormat;
        }
    }

    return surface;
}
