#pragma once

// For use by SpriteSpark Applications

// -----------------SparkCore---------------------
#include "SparkCore/HeaderFiles/Core.h"
#include "SparkCore/HeaderFiles/Application.h"
#include "SparkCore/HeaderFiles/Log.h"
#include "SparkCore/HeaderFiles/GlobalThreadPool.h"
#include "SparkCore/HeaderFiles/Application.h"
#include "SparkCore/HeaderFiles/Layer.h"
#include "SparkCore/HeaderFiles/Input.h"
#include "SparkCore/HeaderFiles/KeyCodes.h"
#include "SparkCore/HeaderFiles/MouseCodes.h"
#include "SparkCore/HeaderFiles/Camera.h"
#include "SparkCore/HeaderFiles/Renderer.h"
#include "SparkCore/HeaderFiles/FrameInfo.h"
// -----------------------------------------------

// ------------------Platform---------------------
#include "Platform/Vulkan/HeaderFiles/VulkanDevice.h"
#include "Platform/Vulkan/HeaderFiles/VulkanModel.h"
#include "Platform/Vulkan/HeaderFiles/VulkanTexture.h"
#include "Platform/Vulkan/HeaderFiles/VulkanSwapChain.h"
#include "Platform/Vulkan/HeaderFiles/VulkanPipeline.h"
#include "Platform/Vulkan/HeaderFiles/VulkanBuffer.h"
#include "Platform/Vulkan/HeaderFiles/VulkanDescriptors.h"
// -----------------------------------------------

// -----------------SparkSystems------------------
#include "SparkSystems/HeaderFiles/RenderSystem.h"
// -----------------------------------------------

// -----------------SparkEvents-------------------
#include "SparkEvents/Event.h"
#include "SparkEvents/ApplicationEvent.h"
#include "SparkEvents/KeyEvent.h"
#include "SparkEvents/MouseEvent.h"
#include "SparkEvents/GlobalEventDispatcher.h"
// -----------------------------------------------

// -----------------SparkObjects------------------
#include "SparkObjects/HeaderFiles/GameObject.h"
// -----------------------------------------------

// ----------------Entry Point--------------------
#include "SparkCore/HeaderFiles/EntryPoint.h"
// -----------------------------------------------