#pragma once

namespace schwi {
	class SceneLayer;

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<SceneLayer>& scene);

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		Ref<SceneLayer> m_SceneLayer;
	};

}