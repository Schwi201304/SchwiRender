#pragma once
#include "Layer/Layer.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include <imgui.h>
namespace schwi {

	class SCHWI_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		Ref<ImGuiContext> GetCurrentContext() { return m_CurrentContext; }

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
		Ref<ImGuiContext> m_CurrentContext;

	};

}
