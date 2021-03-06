#include <Engine/Rendering/Buffer/CommandBuffer/BaseCommandBuffer.h>
#include <Engine/Rendering/RenderPass/BaseRenderPass.h>
#include <Engine/Rendering/Device/VulkanDevice.h>
#include <Engine/Debug/DebugLog.h>

ScrapEngine::Render::BaseCommandBuffer::~BaseCommandBuffer()
{
	free_command_buffers();
}

void ScrapEngine::Render::BaseCommandBuffer::begin_command_buffer(const vk::CommandBufferUsageFlagBits flag)
{
	for (auto& command_buffer : command_buffers_)
	{
		vk::CommandBufferBeginInfo begin_info(flag);

		const vk::Result result = command_buffer.begin(&begin_info);

		if (result != vk::Result::eSuccess)
		{
			Debug::DebugLog::fatal_error(result, "[VulkanCommandBuffer] Failed to begin recording command buffer!");
		}
	}
}

void ScrapEngine::Render::BaseCommandBuffer::close_command_buffer()
{
	for (auto& command_buffer : command_buffers_)
	{
		command_buffer.end();
	}
}

void ScrapEngine::Render::BaseCommandBuffer::end_command_buffer_render_pass()
{
	for (auto& command_buffer : command_buffers_)
	{
		command_buffer.endRenderPass();
	}
}

void ScrapEngine::Render::BaseCommandBuffer::reset_command_buffer()
{
	for (auto& command_buffer : command_buffers_)
	{
		command_buffer.reset(vk::CommandBufferResetFlags());
	}
}

void ScrapEngine::Render::BaseCommandBuffer::free_command_buffers()
{
	if (!command_buffers_.empty())
	{
		VulkanDevice::get_instance()->get_logical_device()->freeCommandBuffers(
			*command_pool_ref_,
			static_cast<uint32_t>(command_buffers_.size()),
			command_buffers_.data());
		command_buffers_.clear();
	}
}

const std::vector<vk::CommandBuffer>* ScrapEngine::Render::BaseCommandBuffer::get_command_buffers_vector() const
{
	return &command_buffers_;
}
