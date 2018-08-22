#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

namespace ScrapEngine {

	class VukanInstance
	{
	private:
		VkInstance instance; //Vulkan Instance of the engine - connection between the application and the Vulkan library
	public:
		VukanInstance(std::string app_name, int app_version, std::string engine_name = "No Engine", int engine_version = 1);

		//Destroy the Vulkan Instance
		~VukanInstance(); 

		//Create the Vulkan Instance with the given data
		void createVulkanInstance(std::string app_name, int app_version, std::string engine_name = "No Engine", int engine_version = 1);

		//Return the current Vulkan Instance (if created)
		VkInstance getVulkanInstance() const;
	private:
		std::vector<const char*> getRequiredExtensions();
	};

}
