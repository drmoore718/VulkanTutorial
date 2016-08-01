#pragma once

#define AUTO_ARG(x) decltype(x), x

struct VTag
{

};

VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
	static auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
	static auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

	if (func != nullptr)
	{
		func(instance, callback, pAllocator);
	}
}

template <typename T, typename Allocator, Allocator allocator, typename Deleter, Deleter d>
class VWrapper
{
public:

	VWrapper(const VTag& tag)
	{
		deleter = std::bind(d, std::placeholders::_1, nullptr);
	}

	VWrapper(const VkInstance& instance)
	{
		deleter = std::bind(d, instance, std::placeholders::_1, nullptr);
	}

	VWrapper(const VkDevice& device)
	{
		deleter = std::bind(d, device, std::placeholders::_1, nullptr);
	}

	~VWrapper()
	{
		cleanup();
	}

	template <typename... Args>
	void reset(const VTag& tag, Args... args)
	{
		cleanup();
		if (allocator(args..., &object) != VK_SUCCESS)
		{
			throw std::runtime_error(std::string("Failed to allocate ") + typeid(this).name());
		}
	}

	template <typename... Args>
	void reset(const VkInstance& instance, Args... args)
	{
		cleanup();
		deleter = std::bind(d, instance, std::placeholders::_1, nullptr);
		if (allocator(instance, args..., &object) != VK_SUCCESS)
		{
			throw std::runtime_error(std::string("Failed to allocate ") + typeid(this).name());
		}
	}

	template <typename... Args>
	void reset(const VkDevice& device, Args... args)
	{
		cleanup();
		deleter = std::bind(d, device, std::placeholders::_1, nullptr);
		if (allocator(device, args..., &object) != VK_SUCCESS)
		{
			throw std::runtime_error(std::string("Failed to allocate ") + typeid(this).name());
		}
	}

	operator const T&() const
	{
		return object;
	}

	T take()
	{
		T obj = object;
		object = VK_NULL_HANDLE;
		return obj;
	}

	void give(VWrapper other)
	{
		other.cleanup();
		other.deleter = deleter;
		other.object = object;
		object = VK_NULL_HANDLE;
	}
	//	template <typename T, typename Allocator, Allocator allocator, typename Deleter, Deleter d>
	//class VWrapper

private:
	T object { VK_NULL_HANDLE };
	std::function<void(T)> deleter;

	void cleanup()
	{
		if (object != VK_NULL_HANDLE)
		{
			deleter(object);
			object = VK_NULL_HANDLE;
		}
	}
};

using VdInstance = VWrapper<VkInstance, AUTO_ARG(vkCreateInstance), AUTO_ARG(vkDestroyInstance)>;
using VdDebugReportCallbackEXT = VWrapper<VkDebugReportCallbackEXT, AUTO_ARG(CreateDebugReportCallbackEXT), AUTO_ARG(DestroyDebugReportCallbackEXT)>;
using VdDevice = VWrapper<VkDevice, AUTO_ARG(vkCreateDevice), AUTO_ARG(vkDestroyDevice)>;
using VdSurface = VWrapper<VkSurfaceKHR, AUTO_ARG(glfwCreateWindowSurface), AUTO_ARG(vkDestroySurfaceKHR)>;
using VdSwapchain = VWrapper<VkSwapchainKHR, AUTO_ARG(vkCreateSwapchainKHR), AUTO_ARG(vkDestroySwapchainKHR)>;
using VdImageView = VWrapper<VkImageView, AUTO_ARG(vkCreateImageView), AUTO_ARG(vkDestroyImageView)>;
using VdShaderModule = VWrapper<VkShaderModule, AUTO_ARG(vkCreateShaderModule), AUTO_ARG(vkDestroyShaderModule)>;
using VdRenderPass = VWrapper<VkRenderPass, AUTO_ARG(vkCreateRenderPass), AUTO_ARG(vkDestroyRenderPass)>;
using VdPipelineLayout = VWrapper<VkPipelineLayout, AUTO_ARG(vkCreatePipelineLayout), AUTO_ARG(vkDestroyPipelineLayout)>;
using VdPipeline = VWrapper<VkPipeline, AUTO_ARG(vkCreateGraphicsPipelines), AUTO_ARG(vkDestroyPipeline)>;
using VdFramebuffer = VWrapper<VkFramebuffer, AUTO_ARG(vkCreateFramebuffer), AUTO_ARG(vkDestroyFramebuffer)>;
using VdCommandPool = VWrapper<VkCommandPool, AUTO_ARG(vkCreateCommandPool), AUTO_ARG(vkDestroyCommandPool)>;
using VdSemaphore = VWrapper<VkSemaphore, AUTO_ARG(vkCreateSemaphore), AUTO_ARG(vkDestroySemaphore)>;
