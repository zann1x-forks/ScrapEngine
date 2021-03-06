#pragma once

#include <string>
#include <vector>
#include <Engine/Rendering/Model/Model/Mesh/Mesh.h>

namespace ScrapEngine
{
	namespace Render
	{
		class VulkanModel
		{
		private:
			std::vector<Mesh*> model_meshes_;
		public:
			VulkanModel(const std::string& input_model_path);
			~VulkanModel();

			const std::vector<Mesh*>* get_meshes() const;
		};
	}
}
