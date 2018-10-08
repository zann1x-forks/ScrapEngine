#include "RenderManager.h"

ScrapEngine::RenderManager::RenderManager(const ScrapEngine::game_base_info* received_base_game_info)
{
	GameWindow = new ScrapEngine::GameWindow(received_base_game_info->window_WIDTH, received_base_game_info->window_HEIGHT, received_base_game_info->app_name);
	DebugLog::printToConsoleLog("GameWindow created");
	initializeVulkan(received_base_game_info);
}

ScrapEngine::RenderManager::~RenderManager()
{
	DebugLog::printToConsoleLog("Deleting ~RenderManager");
	deleteQueues();
	cleanupSwapChain();

	for (ScrapEngine::VulkanMeshInstance* current_model : LoadedModels) {
		delete current_model;
	}
	delete VulkanRenderSemaphores;
	delete VulkanRenderCommandPool;
	delete VulkanRenderDevice;
	delete VulkanWindowSurface;
	delete VulkanInstance;
	delete GameWindow;
	DebugLog::printToConsoleLog("Deleting ~RenderManager completed");
}

void ScrapEngine::RenderManager::cleanupSwapChain()
{
	DebugLog::printToConsoleLog("---cleanupSwapChain()---");
	delete VulkanRenderColor;
	delete VulkanRenderDepth;
	delete VulkanRenderFrameBuffer;
	deleteCommandBuffers();
	for (int i = 0; i < LoadedModels.size(); i++) {
		LoadedModels[i]->deleteGraphicsPipeline();
	}
	delete VulkanRenderingPass;
	delete VulkanRenderImageView;
	delete VulkanRenderSwapChain;
	DebugLog::printToConsoleLog("---cleanupSwapChain() completed---");
}

void ScrapEngine::RenderManager::deleteQueues()
{
	delete VulkanGraphicsQueue;
	delete VulkanPresentationQueue;
}

void ScrapEngine::RenderManager::deleteCommandBuffers()
{
	VulkanRenderCommandBuffer->freeCommandBuffers(VulkanRenderCommandPool->getCommandPool());
	delete VulkanRenderCommandBuffer;
}

const ScrapEngine::GameWindow * ScrapEngine::RenderManager::getGameWindow() const
{
	return GameWindow;
}

void ScrapEngine::RenderManager::initializeVulkan(const ScrapEngine::game_base_info* received_base_game_info)
{
	DebugLog::printToConsoleLog("---initializeVulkan()---");
	VulkanInstance = new VukanInstance(received_base_game_info->app_name, received_base_game_info->app_version, "ScrapEngine");
	DebugLog::printToConsoleLog("VulkanInstance created");
	VulkanWindowSurface = new VulkanSurface(VulkanInstance, GameWindow);
	DebugLog::printToConsoleLog("VulkanWindowSurface created");
	VulkanRenderDevice = new VulkanDevice(VulkanInstance->getVulkanInstance(), VulkanWindowSurface->getSurface());
	deviceRef = *VulkanRenderDevice->getLogicalDevice();
	DebugLog::printToConsoleLog("VulkanRenderDevice created");
	createQueues();
	VulkanRenderSwapChain = new VulkanSwapChain(VulkanRenderDevice->querySwapChainSupport(VulkanRenderDevice->getPhysicalDevice()),
		VulkanRenderDevice->getCachedQueueFamilyIndices(),
		&deviceRef, VulkanWindowSurface->getSurface(), received_base_game_info->window_WIDTH, received_base_game_info->window_HEIGHT);
	DebugLog::printToConsoleLog("VulkanSwapChain created");
	VulkanRenderImageView = new VulkanImageView(&deviceRef, VulkanRenderSwapChain);
	DebugLog::printToConsoleLog("VulkanImageView created");
	VulkanRenderingPass = new VulkanRenderPass(&deviceRef, VulkanRenderSwapChain->getSwapChainImageFormat(), VulkanRenderDevice->getPhysicalDevice(), VulkanRenderDevice->getMsaaSamples());
	DebugLog::printToConsoleLog("VulkanRenderPass created");
	VulkanRenderCommandPool = new VulkanCommandPool(VulkanRenderDevice->getCachedQueueFamilyIndices(), &deviceRef);
	DebugLog::printToConsoleLog("VulkanCommandPool created");
	VulkanRenderColor = new VulkanColorResources(&deviceRef, VulkanRenderDevice->getPhysicalDevice(), VulkanRenderCommandPool->getCommandPool(), VulkanGraphicsQueue->getgraphicsQueue(), VulkanRenderDevice->getMsaaSamples(), VulkanRenderSwapChain);
	DebugLog::printToConsoleLog("VulkanRenderColor created");
	VulkanRenderDepth = new VulkanDepthResources(&deviceRef, VulkanRenderDevice->getPhysicalDevice(), VulkanRenderCommandPool->getCommandPool(), VulkanGraphicsQueue->getgraphicsQueue(), &VulkanRenderSwapChain->getSwapChainExtent(), VulkanRenderDevice->getMsaaSamples());
	DebugLog::printToConsoleLog("VulkanDepthResources created");
	VulkanRenderFrameBuffer = new VulkanFrameBuffer(VulkanRenderImageView, &deviceRef, &VulkanRenderSwapChain->getSwapChainExtent(), VulkanRenderDepth->getDepthImageView(), VulkanRenderingPass->getRenderPass(), VulkanRenderColor->getColorImageView());
	DebugLog::printToConsoleLog("VulkanFrameBuffer created");
	LoadedModels.push_back(new VulkanMeshInstance("../assets/shader/vert.spv", "../assets/shader/frag.spv", "../assets/models/chess/ChessPieces/Queen.fbx", "../assets/textures/SimpleGreenTexture.png",
		VulkanRenderDevice, VulkanRenderCommandPool->getCommandPool(), VulkanGraphicsQueue->getgraphicsQueue(), VulkanRenderSwapChain, VulkanRenderingPass
	));
	LoadedModels[0]->setMeshLocation(0, 0, -10.0f);
	LoadedModels[0]->setMeshRotation(0, 0, 0);
	LoadedModels[0]->setMeshScale(0.5f, 0.5f, 0.5f);
	LoadedModels.push_back(new VulkanMeshInstance("../assets/shader/vert.spv", "../assets/shader/frag.spv", "../assets/models/chess/ChessPieces/King.fbx", "../assets/textures/SimpleRedTexture.png",
		VulkanRenderDevice, VulkanRenderCommandPool->getCommandPool(), VulkanGraphicsQueue->getgraphicsQueue(), VulkanRenderSwapChain, VulkanRenderingPass
	));
	LoadedModels[1]->setMeshLocation(1, 0, -10.0f);
	LoadedModels[1]->setMeshRotation(0, 0, 0);
	LoadedModels[1]->setMeshScale(0.5f, 0.5f, 0.5f);
	//Test model init
	createCommandBuffers();
	//Vulkan Semaphores
	VulkanRenderSemaphores = new VulkanSemaphoresManager(&deviceRef);
	imageAvailableSemaphoresRef = VulkanRenderSemaphores->getImageAvailableSemaphoresVector();
	renderFinishedSemaphoresRef = VulkanRenderSemaphores->getRenderFinishedSemaphoresVector();
	inFlightFencesRef = VulkanRenderSemaphores->getInFlightFencesVector();
	DebugLog::printToConsoleLog("VulkanSemaphoresManager created");
	DebugLog::printToConsoleLog("---initializeVulkan() completed---");
}

void ScrapEngine::RenderManager::createQueues()
{
	DebugLog::printToConsoleLog("---Begin queues creation---");
	VulkanGraphicsQueue = new GraphicsQueue(&deviceRef, VulkanRenderDevice->getCachedQueueFamilyIndices());
	VulkanPresentationQueue = new PresentQueue(&deviceRef, VulkanRenderDevice->getCachedQueueFamilyIndices());
	DebugLog::printToConsoleLog("---Ended queues creation---");
}

void ScrapEngine::RenderManager::createCommandBuffers()
{
	std::vector<ScrapEngine::VulkanGraphicsPipeline*> pipelines;
	std::vector<const std::vector<vk::DescriptorSet>*> descriptorSets;
	std::vector<ScrapEngine::simple_buffer<Vertex>*> vertexbuffers;
	std::vector<ScrapEngine::simple_buffer<uint32_t>*> indexbuffers;
	for (ScrapEngine::VulkanMeshInstance* mesh : LoadedModels) {
		pipelines.push_back(mesh->getVulkanRenderGraphicsPipeline());
		descriptorSets.push_back(mesh->getVulkanRenderDescriptorSet()->getDescriptorSets());
		vertexbuffers.push_back(mesh->getVertexbuffer());
		indexbuffers.push_back(mesh->getIndexbuffer());
	}
	VulkanRenderCommandBuffer = new VulkanCommandBuffer(
		VulkanRenderFrameBuffer,
		&deviceRef,
		VulkanRenderCommandPool->getCommandPool(),
		VulkanRenderingPass->getRenderPass(),
		&VulkanRenderSwapChain->getSwapChainExtent(),
		pipelines,
		descriptorSets,
		vertexbuffers,
		indexbuffers
	);
	DebugLog::printToConsoleLog("VulkanRenderCommandBuffer created");
}

ScrapEngine::VulkanMeshInstance* ScrapEngine::RenderManager::loadMesh(const std::string & vertex_shader_path, const std::string & fragment_shader_path, const std::string & model_path, const std::string & texture_path)
{
	LoadedModels.push_back(new VulkanMeshInstance(vertex_shader_path, fragment_shader_path, model_path, texture_path,
		VulkanRenderDevice, VulkanRenderCommandPool->getCommandPool(), VulkanGraphicsQueue->getgraphicsQueue(), VulkanRenderSwapChain, VulkanRenderingPass
	));
	deleteCommandBuffers();
	createCommandBuffers();
	return LoadedModels.back();
}

void ScrapEngine::RenderManager::unloadMesh(ScrapEngine::VulkanMeshInstance* meshToUnload)
{
	std::vector<ScrapEngine::VulkanMeshInstance*>::iterator element = find(LoadedModels.begin(), LoadedModels.end(), meshToUnload);
	if (element != LoadedModels.end())
	{
		delete *element;
		LoadedModels.erase(element);
		deleteCommandBuffers();
		createCommandBuffers();
	}
}

void ScrapEngine::RenderManager::drawFrame()
{
	deviceRef.waitForFences(1, &(*inFlightFencesRef)[currentFrame], true, std::numeric_limits<uint64_t>::max());

	uint32_t imageIndex;
	vk::Result result = deviceRef.acquireNextImageKHR(VulkanRenderSwapChain->getSwapChain(), std::numeric_limits<uint64_t>::max(), (*imageAvailableSemaphoresRef)[currentFrame], vk::Fence(), &imageIndex);

	if (result == vk::Result::eErrorOutOfDateKHR) {
		//recreateSwapChain();
		throw std::runtime_error("recreateSwapChain() not ready!");
		return;
	}
	else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR) {
		throw std::runtime_error("RenderManager: Failed to acquire swap chain image!");
	}
	for (int i = 0;i < LoadedModels.size(); i++) {
		LoadedModels[i]->updateUniformBuffer(imageIndex, &VulkanRenderSwapChain->getSwapChainExtent());
	}
	vk::SubmitInfo submitInfo;

	vk::Semaphore waitSemaphores[] = { (*imageAvailableSemaphoresRef)[currentFrame] };;

	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

	submitInfo.setWaitSemaphoreCount(1);
	submitInfo.setPWaitSemaphores(waitSemaphores);
	submitInfo.setPWaitDstStageMask(waitStages);

	submitInfo.setCommandBufferCount(1);
	submitInfo.setPCommandBuffers(&(*VulkanRenderCommandBuffer->getCommandBuffersVector())[imageIndex]);

	vk::Semaphore signalSemaphores[] = { (*renderFinishedSemaphoresRef)[currentFrame] };
	submitInfo.setSignalSemaphoreCount(1);
	submitInfo.setPSignalSemaphores(signalSemaphores);
	
	//DebugLog::printToConsoleLog("---HELLO---PRE---SUBMIT---");
	deviceRef.resetFences(1, &(*inFlightFencesRef)[currentFrame]);

	if (VulkanGraphicsQueue->getgraphicsQueue()->submit(1, &submitInfo, (*inFlightFencesRef)[currentFrame]) != vk::Result::eSuccess) {
		throw std::runtime_error("RenderManager: Failed to submit draw command buffer!");
	}
	//DebugLog::printToConsoleLog("---HELLO---AFTER---SUBMIT---");

	vk::PresentInfoKHR presentInfo;

	presentInfo.setWaitSemaphoreCount(1);
	presentInfo.setPWaitSemaphores(signalSemaphores);
	
	vk::SwapchainKHR swapChains[] = { VulkanRenderSwapChain->getSwapChain() };
	presentInfo.setSwapchainCount(1);
	presentInfo.setPSwapchains(swapChains);

	presentInfo.setPImageIndices(&imageIndex);

	result = VulkanPresentationQueue->getPresentQueue()->presentKHR(&presentInfo);

	if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || framebufferResized) {
		//framebufferResized = false;
		//recreateSwapChain();
		throw std::runtime_error("recreateSwapChain() not ready!");
	}
	else if (result != vk::Result::eSuccess) {
		throw std::runtime_error("RenderManager: Failed to present swap chain image!");
	}
		
	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void ScrapEngine::RenderManager::waitDeviceIdle()
{
	VulkanRenderDevice->getLogicalDevice()->waitIdle();
}

void ScrapEngine::RenderManager::recreateSwapChain()
{
	deviceRef.waitIdle();
	//TODO https://vulkan-tutorial.com/Drawing_a_triangle/Swap_chain_recreation
}