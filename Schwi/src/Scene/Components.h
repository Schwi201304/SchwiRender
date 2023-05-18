#pragma once

#include <glm/glm.hpp>
#include <Renderer/Camera.h>
#include <Renderer/Mesh.h>

namespace schwi {
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};


	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3 & translation)
			: Translation(translation) {}
		TransformComponent(const glm::vec3& translation,const glm::vec3& rotation,const glm::vec3& scale)
			: Translation(translation),Rotation(rotation),Scale(scale) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::mat4_cast(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct CameraComponent
	{
		Ref<Camera> camera=nullptr;
		bool Primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const Ref<Camera> _camera)
			: camera(_camera){}
	};

	struct LightComponent
	{
		Ref<Light> light=nullptr;
		LightType lightType = LightType::LightType_None;

		LightComponent() = default;
		LightComponent(const LightComponent&) = default;
		LightComponent(const Ref<Light>& _light,LightType _lightType=LightType::LightType_Basic)
			: light(_light),lightType(_lightType) {}
				
	};

	struct ModelComponent
	{
		Ref<Model> model = nullptr;
		std::string path;

		ModelComponent() = default;
		ModelComponent(const ModelComponent&) = default;
		ModelComponent(const std::string& _path) :path(_path)
		{
			model = CreateRef<Model>(path);
		}
	};

	struct MeshComponent
	{
		Ref<Mesh> mesh = nullptr;
		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(const Ref<Mesh>& mesh)
			: mesh(mesh) {}
		MeshComponent(const MeshType& type, const uint32_t& sample = 1)
			:mesh(CreateRef<Mesh>(type, sample)) {}

		void Reset() { mesh.reset(); }
		void Reload(const MeshType& type, const uint32_t& sample = 1)
		{
			mesh.reset();
			mesh = CreateRef<Mesh>(type, sample);
		}
	};

}