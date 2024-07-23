#pragma once

#include "SparkEvents/Event.h"
#include "SparkCore/HeaderFiles/Camera.h"
#include "SparkObjects/HeaderFiles/GameObject.h"

#include "SparkSystems/HeaderFiles/RenderSystem.h"

namespace SpriteSpark {

	class Layer {

	public:

		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(double deltaTime, Camera& camera, std::vector<GameObject>& gameObjects) {}
		virtual void OnRender(RenderSystem& renderSystem, FrameInfo& frameInfo, std::vector<GameObject>& gameObjects) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:

		std::string m_DebugName;

	};

}