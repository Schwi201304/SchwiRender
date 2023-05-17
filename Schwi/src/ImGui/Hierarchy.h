#pragma once

#include "Scene/SceneLayer.h"
#include "Scene/Entity.h"

namespace schwi {
	class Hierarchy
	{
	public:
		Hierarchy() = default;
		Hierarchy(const Ref<SceneLayer>& scene);

		void SetContext(const Ref<SceneLayer>& scene);

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:
		Ref<SceneLayer> m_Context;
		Entity m_SelectionContext;
	};

}