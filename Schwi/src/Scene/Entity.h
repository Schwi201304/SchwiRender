#pragma once

#include <entt/entt.hpp>

#include "SceneLayer.h"

namespace schwi {
	class SCHWI_API Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Ref<SceneLayer> scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			SW_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_SceneLayer->GetRegistry().emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_SceneLayer->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			SW_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_SceneLayer->GetRegistry().get<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_SceneLayer->GetRegistry().all_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			SW_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_SceneLayer->GetRegistry().remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator entt::entity() const { return m_EntityHandle; }

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_SceneLayer == other.m_SceneLayer;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
	private:
		entt::entity m_EntityHandle{ entt::null };
		Ref<SceneLayer> m_SceneLayer = nullptr;
	};

}