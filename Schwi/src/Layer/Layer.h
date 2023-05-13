#pragma once

#include "Events/Event.h"
#include "Core/Timestep.h"

namespace schwi {
	class SCHWI_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_Name; }

	protected:
		std::string m_Name;

	};

}