#include <Engine/LogicCore/Components/SComponent.h>
#include <glm/mat4x4.hpp>
#include <Engine/Debug/DebugLog.h>

ScrapEngine::Core::SComponent::SComponent(const std::string& component_name) : SObject(component_name)
{
}

ScrapEngine::Core::SComponent::~SComponent()
{
	//Empty here
}

void ScrapEngine::Core::SComponent::set_component_location(const glm::vec3& location)
{
	object_world_transform_.location = location;
	object_relative_transform_.location = object_world_transform_.location - father_transform_.location;
}

void ScrapEngine::Core::SComponent::set_component_rotation(const glm::vec3& rotation)
{
	//TODO
	object_world_transform_.rotation = rotation;
}

void ScrapEngine::Core::SComponent::set_component_scale(const glm::vec3& scale)
{
	object_world_transform_.scale = scale;
	object_relative_transform_.scale = object_world_transform_.scale - father_transform_.scale;
}

void ScrapEngine::Core::SComponent::update_component_location()
{
	object_world_transform_.location = father_transform_.location + object_relative_transform_.location;
}

void ScrapEngine::Core::SComponent::update_component_rotation()
{
	//TODO
	object_world_transform_.rotation = father_transform_.rotation + object_relative_transform_.rotation;

	const glm::mat4 localM = generate_transform_matrix(object_relative_transform_);
	const glm::mat4 fatherM = generate_transform_matrix(father_transform_);
	glm::mat4 fullM = fatherM * localM;

	glm::vec3 pos = glm::vec3(fullM[3][0], fullM[3][1], fullM[3][2]);

	ScrapEngine::Debug::DebugLog::print_to_console_log(pos);

	object_world_transform_.location = pos;
}

void ScrapEngine::Core::SComponent::update_component_scale()
{
	object_world_transform_.scale = father_transform_.scale + object_relative_transform_.scale;
}

void ScrapEngine::Core::SComponent::set_father_transform(const Transform& input_father_transform)
{
	father_transform_ = input_father_transform;
}

glm::vec3 ScrapEngine::Core::SComponent::get_component_location() const
{
	return object_world_transform_.location;
}

glm::vec3 ScrapEngine::Core::SComponent::get_component_rotation() const
{
	return object_world_transform_.rotation;
}

glm::vec3 ScrapEngine::Core::SComponent::get_component_scale() const
{
	return object_world_transform_.scale;
}

glm::vec3 ScrapEngine::Core::SComponent::get_component_relative_location() const
{
	return object_relative_transform_.location;
}

glm::vec3 ScrapEngine::Core::SComponent::get_component_relative_rotation() const
{
	return object_relative_transform_.rotation;
}

glm::vec3 ScrapEngine::Core::SComponent::get_component_relative_scale() const
{
	return object_relative_transform_.scale;
}
