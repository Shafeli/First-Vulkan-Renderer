#pragma once
#include <filesystem>

#include "../../gap-311/VulkanWrapper.hpp"

class Texture
{
    GAP311::VulkanWrapper& m_vulkanContext;
    const char* m_filePath;

    GAP311::VulkanWrapper::ShaderUniform shaderUniform;

    vk::Image m_image;
    vk::ImageView m_view;
    vk::Sampler m_textureSampler;
    vk::DescriptorSet m_descriptorSet;
    vk::DescriptorSetLayout m_layout;
public:

    Texture(GAP311::VulkanWrapper& vulkanWrapper);
    Texture(const char* filePath, GAP311::VulkanWrapper& vulkanWrapper);
    ~Texture();
    vk::DescriptorSet* GetDescriptorSet();
    vk::DescriptorSetLayout* GetDescriptorLayout() { return &m_layout; }
    bool IsNotNull() const { return (std::string("Null") != m_filePath); }

private:
    void LoadDefaultTexture();
	SDL_Surface* LoadCompatibleImage(const char* imageFile, Uint32 pixelFormat = SDL_PIXELFORMAT_ABGR8888);
    void LoadTexture(SDL_Surface* surface);
};

