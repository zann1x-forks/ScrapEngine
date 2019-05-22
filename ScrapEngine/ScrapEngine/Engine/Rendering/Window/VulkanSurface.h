#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Engine/Rendering/Window/GameWindow.h>
#include <Engine/Rendering/Instance/VukanInstance.h>

namespace ScrapEngine
{
	namespace Render
	{
		class VulkanSurface
		{
		private:
			/**
			 * \brief represents an abstract type of surface to present rendered images to.
			 */
			vk::SurfaceKHR surface_;

			//Singleton static instance
			static VulkanSurface* instance_;

			//The constructor is private because this class is a Singleton
			VulkanSurface() = default;
		public:
			//Method used to init the class with parameters because the constructor is private
			void init(ScrapEngine::Render::GameWindow* window_ref);

			~VulkanSurface();

			//Singleton static function to get or create a class instance
			static VulkanSurface* get_instance();

			void create_surface(ScrapEngine::Render::GameWindow* window_ref);

			vk::SurfaceKHR* get_surface();
		};
	}
}
