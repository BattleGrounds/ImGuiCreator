#pragma once

#include <vulkan/vulkan.h>
#include <filesystem>
#include <imgui_impl_vulkan.h>
// A struct to manage data related to one image in vulkan
struct MyTextureData
{
	VkDescriptorSet DS;         // Descriptor set: this is what you'll pass to Image()
	int             Width;
	int             Height;
	int             Channels;

	// Need to keep track of these to properly cleanup
	VkImageView     ImageView;
	VkImage         Image;
	VkDeviceMemory  ImageMemory;
	VkSampler       Sampler;
	VkBuffer        UploadBuffer;
	VkDeviceMemory  UploadBufferMemory;

	MyTextureData() { memset(this, 0, sizeof(*this)); }
};

void InitImageLoader(ImGui_ImplVulkan_InitInfo& info, ImGui_ImplVulkanH_Window& w);

ImTextureID LoadImage(std::filesystem::path filename);

void UnloadImage(ImTextureID t_id);

