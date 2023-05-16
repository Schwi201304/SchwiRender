#pragma once
#include "Layer/Layer.h"

namespace schwi {
	class SCHWI_API MenuLayer : public Layer
	{
	public:
		MenuLayer();
		~MenuLayer() = default;

		virtual void OnAttach() override;
		virtual void OnUpdate(Timestep timestep) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		static Ref<MenuLayer> GetInstance() { return s_Instance; };

	private:
		static Ref<MenuLayer> s_Instance;
		bool m_ShowEditWindow = true;
		void EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition);
		void ShowEditWindow();
	};

}