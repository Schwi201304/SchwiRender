#include "Application.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Log.h"

namespace schwi {
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720); 
		if (e.IsInCategory(EventCategoryApplication))
		{
			SW_TRACE(e);
		}
		MouseMoveEvent mme(720, 360);
		if (mme.IsInCategory(EventCategoryInput))
		{
			SW_TRACE(mme);
		}
		WindowCloseEvent wce;
		//while (true);
	}
}