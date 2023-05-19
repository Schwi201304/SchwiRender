#pragma once

#include "Core/Window.h"

struct GLFWwindow;

namespace schwi {
	class Context;

	class SCHWI_API WindowsWindow :public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height; }
		inline std::pair<uint32_t, uint32_t> GetResolution() const override { return std::make_pair(m_Data.Width, m_Data.Height); }
		inline float GetAspect() const override { return static_cast<float>(m_Data.Width) / static_cast<float>(m_Data.Height); }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		inline virtual void* GetNativeWindow() const { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		Context* m_Context;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}