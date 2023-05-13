#pragma once

#include "Core/Core.h"

namespace schwi {

	class SCHWI_API Context
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}