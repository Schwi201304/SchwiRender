#pragma once

#include "Scene/Entity.h"

namespace schwi {
	class SceneLayer;

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
		Entity m_SelectionContext;
	};

}