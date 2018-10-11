#pragma once
#include "../Texture/TextureImage.h"
#include "../Texture/TextureImageView.h"
#include "../Texture/TextureSampler.h"
#include "../Buffer/Vertex/VertexBuffer.h"
#include "../Buffer/Index/IndexBuffer.h"
#include "../Buffer/Uniform/UniformBuffer.h"
#include "../Model/VulkanModel.h"
#include "../Buffer/VulkanCommandBuffer.h"
#include "../Descriptor/VulkanDescriptorPool.h"
#include "../Descriptor/VulkanDescriptorSet.h"
#include "../Device/VulkanDevice.h"
#include "../SwapChain/VulkanSwapChain.h"
#include "../RenderPass/VulkanRenderPass.h"
#include "../Buffer/VulkanFrameBuffer.h"
#include "../../Utility/UsefulTypes.h"

namespace ScrapEngine {

	class VulkanMeshInstance
	{
	private:
		ScrapEngine::VulkanGraphicsPipeline* VulkanRenderGraphicsPipeline = nullptr;
		ScrapEngine::TextureImage* VulkanTextureImage = nullptr;
		ScrapEngine::TextureImageView* VulkanTextureImageView = nullptr;
		ScrapEngine::TextureSampler* VulkanTextureSampler = nullptr;
		ScrapEngine::VulkanModel* VulkanRenderModel = nullptr;
		ScrapEngine::VertexBuffer* VulkanRenderVertexBuffer = nullptr;
		ScrapEngine::IndexBuffer* VulkanRenderIndexBuffer = nullptr;
		ScrapEngine::UniformBuffer* VulkanRenderUniformBuffer = nullptr;
		ScrapEngine::VulkanDescriptorPool* VulkanRenderDescriptorPool = nullptr;
		ScrapEngine::VulkanDescriptorSet* VulkanRenderDescriptorSet = nullptr;

		simple_buffer<Vertex>* vertexbuffer = nullptr;
		simple_buffer<uint32_t>* indexbuffer = nullptr;

		ScrapEngine::Transform object_location;
	public:
		VulkanMeshInstance(const std::string& vertex_shader_path, const std::string& fragment_shader_path, const std::string& model_path, const std::string& texture_path,
			ScrapEngine::VulkanDevice* RenderDevice, vk::CommandPool* CommandPool, vk::Queue* graphicsQueue, ScrapEngine::VulkanSwapChain* SwapChain, 
			ScrapEngine::VulkanRenderPass* RenderingPass);
		~VulkanMeshInstance();

		void setMeshLocation(glm::vec3 location);
		void setMeshRotation(glm::vec3 rotation);
		void setMeshScale(glm::vec3 scale);

		glm::vec3 getMeshLocation();
		glm::vec3 getMeshRotation();
		glm::vec3 getMeshScale();

		void updateUniformBuffer(uint32_t currentImage, ScrapEngine::Camera* RenderCamera);
		void deleteGraphicsPipeline();

		ScrapEngine::UniformBuffer* getVulkanRenderUniformBuffer();
		ScrapEngine::VulkanGraphicsPipeline* getVulkanRenderGraphicsPipeline();
		ScrapEngine::VulkanDescriptorSet* getVulkanRenderDescriptorSet();
		ScrapEngine::simple_buffer<ScrapEngine::Vertex>* getVertexbuffer();
		ScrapEngine::simple_buffer<uint32_t>* getIndexbuffer();
	};

}

