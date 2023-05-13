#pragma once

#include "Core/Core.h"

namespace schwi {
	enum class SCHWI_API TextureType
	{
		NONE,
		DIFFUSE,
		SPECULAR,
		NORMAL,
		DISPLACEMENT

	};

	class SCHWI_API Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual TextureType GetType() const = 0;
		virtual std::string GetPath() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class SCHWI_API Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path, TextureType type = TextureType::DIFFUSE);
	};
}