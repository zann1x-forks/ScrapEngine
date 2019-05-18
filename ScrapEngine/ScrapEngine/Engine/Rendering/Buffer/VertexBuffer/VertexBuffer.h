#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include "../../Base/Vertex.h"

namespace ScrapEngine {
	namespace Render {
		class VertexBuffer
		{
		private:
			vk::Buffer vertex_buffer_;
			vk::DeviceMemory vertex_buffer_memory_;
		public:
			VertexBuffer(const std::vector<ScrapEngine::Vertex>* vertices);
			~VertexBuffer();

			vk::Buffer* get_vertex_buffer();
		};
	}
}

