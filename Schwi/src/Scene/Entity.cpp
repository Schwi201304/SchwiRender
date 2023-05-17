#include "swpch.h"
#include "Entity.h"

namespace schwi {
	Entity::Entity(entt::entity handle, Ref<SceneLayer> scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

}