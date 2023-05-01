#pragma once

#include "Core.h"
//#include "Events/Event.h"

namespace schwi {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

}