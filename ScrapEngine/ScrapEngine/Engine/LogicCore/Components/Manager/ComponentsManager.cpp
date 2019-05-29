#include <Engine/LogicCore/Components/Manager/ComponentsManager.h>

ScrapEngine::Core::ComponentsManager::
ComponentsManager(Render::RenderManager* input_render_manager_ref)
	: render_manager_ref_(input_render_manager_ref)
{
}

ScrapEngine::Core::MeshComponent* ScrapEngine::Core::ComponentsManager::create_new_mesh_component(
	const std::string& vertex_shader_path, const std::string& fragment_shader_path, const std::string& model_path,
	const std::vector<std::string>& textures_path)
{
	Render::VulkanMeshInstance* mesh = render_manager_ref_->load_mesh(vertex_shader_path, fragment_shader_path,
	                                                                  model_path,
	                                                                  textures_path);
	MeshComponent* mesh_component = new MeshComponent(mesh);
	loaded_meshes_.insert(
		std::pair<MeshComponent*, Render::VulkanMeshInstance*>(mesh_component, mesh));
	return mesh_component;
}

void ScrapEngine::Core::ComponentsManager::destroy_mesh_component(
	MeshComponent* component_to_destroy)
{
	const std::map<MeshComponent*, Render::VulkanMeshInstance*>::iterator position =
		loaded_meshes_.find(component_to_destroy);
	if (position != loaded_meshes_.end())
	{
		render_manager_ref_->unload_mesh(position->second);
		loaded_meshes_.erase(position);
	}
}
