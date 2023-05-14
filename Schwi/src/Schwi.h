#pragma once

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Core.h"
#include "Core/Log.h"
#include "Core/Input.h"
#include "Core/Code.h"
#include "Layer/Layer.h"
#include "ImGui/ImGuiLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Camera.h"
#include "Renderer/Texture.h"
#include "Renderer/Light.h"
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
#include "Renderer/Model.h"

#include "Core/Application.h"
#include "Scene/CameraController.h"

#define SolutionDir std::string(SOLUTION_DIR)