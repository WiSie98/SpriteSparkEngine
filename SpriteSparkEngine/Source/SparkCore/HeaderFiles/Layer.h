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
		virtual void OnInit(Camera& camera) {}
		virtual void OnUpdate(float deltaTime, Camera& camera) {}
		virtual void OnRender(FrameInfo& frameInfo) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:

		std::string m_DebugName;

	};

}