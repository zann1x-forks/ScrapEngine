#include <Engine/LogicCore/Components/MeshComponent/MeshComponent.h>

ScrapEngine::Core::MeshComponent::MeshComponent(Render::VulkanMeshInstance* input_vulkan_mesh)
	: SComponent("MeshComponent"), vulkan_mesh_(input_vulkan_mesh)
{
}

void ScrapEngine::Core::MeshComponent::set_component_location(const SVector3& location)
{
	SComponent::set_component_location(location);
	vulkan_mesh_->set_mesh_location(location);
}

void ScrapEngine::Core::MeshComponent::set_component_rotation(const SVector3& rotation)
{
	SComponent::set_component_rotation(rotation);
	vulkan_mesh_->set_mesh_rotation(rotation);
}

void ScrapEngine::Core::MeshComponent::set_component_scale(const SVector3& scale)
{
	SComponent::set_component_scale(scale);
	vulkan_mesh_->set_mesh_scale(scale);
}

void ScrapEngine::Core::MeshComponent::add_component_rotation(const SVector3& rotation)
{
	SComponent::add_component_rotation(rotation);
	vulkan_mesh_->set_mesh_rotation(get_component_rotation());
}

bool ScrapEngine::Core::MeshComponent::get_is_visible() const
{
	return vulkan_mesh_->get_is_visible();
}

void ScrapEngine::Core::MeshComponent::set_is_visible(const bool visible) const
{
	vulkan_mesh_->set_is_visible(visible);
}

bool ScrapEngine::Core::MeshComponent::get_is_static() const
{
	return vulkan_mesh_->get_is_static();
}

void ScrapEngine::Core::MeshComponent::set_is_static(const bool is_static) const
{
	vulkan_mesh_->set_is_static(is_static);
}

bool ScrapEngine::Core::MeshComponent::get_cast_shadows() const
{
	return vulkan_mesh_->get_cast_shadows();
}

void ScrapEngine::Core::MeshComponent::set_cast_shadows(const bool cast_shadows) const
{
	return vulkan_mesh_->set_cast_shadows(cast_shadows);
}

bool ScrapEngine::Core::MeshComponent::get_frustum_check() const
{
	return vulkan_mesh_->get_frustum_check();
}

void ScrapEngine::Core::MeshComponent::set_frustum_check(const bool should_check) const
{
	vulkan_mesh_->set_frustum_check(should_check);
}

void ScrapEngine::Core::MeshComponent::update_component_location()
{
	SComponent::update_component_location();
	vulkan_mesh_->set_mesh_location(get_component_location());
}

void ScrapEngine::Core::MeshComponent::update_component_rotation()
{
	SComponent::update_component_rotation();
	vulkan_mesh_->set_mesh_location(get_component_location());
	vulkan_mesh_->set_mesh_rotation(get_component_rotation());
}

void ScrapEngine::Core::MeshComponent::update_component_scale()
{
	SComponent::update_component_scale();
	vulkan_mesh_->set_mesh_scale(get_component_scale());
}
