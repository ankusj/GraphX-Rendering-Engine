#include "pch.h"

#include "Application.h"

#include "VertexArray.h"
#include "Renderer.h"
#include "shaders/Shader.h"
#include "buffers/VertexBuffer.h"
#include "buffers/VertexBufferLayout.h"
#include "buffers/IndexBuffer.h"
#include "model/Vertex.h"
#include "Texture.h"

/* Entities */
#include "entities/Light.h"
#include "entities/Camera.h"

#include "Window.h"
#include "timer/Clock.h"
#include "gui/GraphXGui.h"

namespace engine
{
	using namespace gm;

	Application::Application(std::string& title, int width, int height)
		: m_Window(nullptr), m_Title(title)
	{
		// Initialise the clock and the logging
		Log::Init();
		Clock::Init();

		m_Window = new Window(m_Title, width, height);
		m_Window->SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		// Set the event callback with the window
		m_Window->SetEventCallback([this](Event& e) {
			this->OnEvent(e);
		});

		bool success = InitializeOpenGL();
		if (!success)
		{
			GX_ENGINE_ERROR("Application: Failed to intialize OpenGL");
			ASSERT(success);
		}
	}

	Application::~Application()
	{
		GX_ENGINE_INFO("Application: Closing Application.");
		delete m_Window;
	}

	bool Application::InitializeOpenGL()
	{
		Timer timer("Initializing OpenGL");

		// Initialise GLEW
		if (glewInit() != GLEW_OK)
		{
			GX_ENGINE_ERROR("Application: Error while initializing glew");
			return false;
		}

		// Print the gl version
		GX_ENGINE_INFO(glGetString(GL_VERSION));

		// To enable the depth test
		GLCall(glEnable(GL_DEPTH_TEST));

		// To enable blending
		GLCall(glEnable(GL_BLEND));

		// Blend function
		// src is the alpha of the current pixel
		// dest is the alpha of that is already in the buffer
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// Blend Equation
		// Add the src and dest values to get the result (can be changed to subtract, inverse, etc.)
		GLCall(glBlendEquation(GL_FUNC_ADD));

		return true;
	}

	void Application::OnEvent(Event& e)
	{
		
	}

	void Application::Run()
	{
		// Vertices for the cube including texture coordinates
		// Vertices of the cube to be rendered
		//VertexT vertices[] = {
		//		/*Vertex Positions*/			/* Normal Coordinates */		/* Texture Coordinates */
		//	// Front face
		//	{ Vector3(-1.0f, -1.0f,  1.0f),	  Vector3(-1.0f, -1.0f,  1.0f),		Vector2(0.0f, 0.0f) },	//0
		//	{ Vector3( 1.0f, -1.0f,  1.0f),	  Vector3( 1.0f, -1.0f,  1.0f),		Vector2(0.0f, 1.0f) },	//1
		//	{ Vector3( 1.0f,  1.0f,  1.0f),	  Vector3( 1.0f,  1.0f,  1.0f),		Vector2(1.0f, 1.0f) },	//2
		//	{ Vector3(-1.0f,  1.0f,  1.0f),	  Vector3(-1.0f,  1.0f,  1.0f),		Vector2(0.0f, 1.0f) },	//3

		//	// Back face			
		//	{ Vector3(-1.0f, -1.0f, -1.0f),	  Vector3(-1.0f, -1.0f, -1.0f),		Vector2(1.0f, 0.0f) },	//4
		//	{ Vector3( 1.0f, -1.0f, -1.0f),	  Vector3( 1.0f, -1.0f, -1.0f),		Vector2(0.0f, 0.0f) },	//5
		//	{ Vector3( 1.0f,  1.0f, -1.0f),	  Vector3( 1.0f,  1.0f, -1.0f),		Vector2(0.0f, 1.0f) },	//6
		//	{ Vector3(-1.0f,  1.0f, -1.0f),	  Vector3(-1.0f,  1.0f, -1.0f),		Vector2(1.0f, 1.0f) }	//7
		//};

		// Vertices of the cube to be rendered
		VertexC vertices[] = {
			/*Vertex Positions*/			  /* Normal Coordinates */				/* Colors */
			// Front face
			{ Vector3(-1.0f, -1.0f,  1.0f),	  Vector3(-1.0f, -1.0f,  1.0f),		Vector4(1.0f, 0.0f, 0.0f, 1.0f) },	//0
			{ Vector3( 1.0f, -1.0f,  1.0f),	  Vector3( 1.0f, -1.0f,  1.0f),		Vector4(0.0f, 1.0f, 0.0f, 1.0f) },	//1
			{ Vector3( 1.0f,  1.0f,  1.0f),	  Vector3( 1.0f,  1.0f,  1.0f),		Vector4(0.0f, 0.0f, 1.0f, 1.0f) },	//2
			{ Vector3(-1.0f,  1.0f,  1.0f),	  Vector3(-1.0f,  1.0f,  1.0f),		Vector4(1.0f, 1.0f, 0.0f, 1.0f) },	//3

			// Back face			  
			{ Vector3(-1.0f, -1.0f, -1.0f),	  Vector3(-1.0f, -1.0f, -1.0f),		Vector4(0.0f, 1.0f, 1.0f, 1.0f) },	//4
			{ Vector3( 1.0f, -1.0f, -1.0f),	  Vector3( 1.0f, -1.0f, -1.0f),		Vector4(1.0f, 0.0f, 1.0f, 1.0f) },	//5
			{ Vector3( 1.0f,  1.0f, -1.0f),	  Vector3( 1.0f,  1.0f, -1.0f),		Vector4(1.0f, 0.5f, 1.0f, 1.0f) },	//6
			{ Vector3(-1.0f,  1.0f, -1.0f),	  Vector3(-1.0f,  1.0f, -1.0f),		Vector4(1.0f, 0.0f, 1.0f, 1.0f) }	//7
		};

		// Indices into the vertex buffer
		unsigned int indices[] = {
			// Front face
			3, 0, 2,
			2, 0, 1,

			// Top Face
			7, 3, 6,
			6, 3, 2,

			// Back Face
			6, 5, 7,
			7, 5, 4,

			// Bottom Face
			0, 4, 1,
			1, 4, 5,

			// Right face
			2, 1, 6,
			6, 1, 5,

			// Left Face
			7, 4, 3,
			3, 4, 0
		};

		VertexArray vao;
		VertexBuffer vbo(vertices, 8 * sizeof(VertexC));
		//VertexBuffer vbo(vertices, 8 * sizeof(VertexT));	// For Using Textures
		VertexBufferLayout layout;
		IndexBuffer ibo(indices, 36);

		// Layout for the vertex positions
		layout.Push<float>(Vector3::Components);

		// Layout for the vertex normals
		layout.Push<float>(Vector3::Components);

		// Layout for the vertex colors
		layout.Push<float>(Vector4::Components);

		// Layout for texture coordinates
		//layout.Push<float>(Vector2::Components);

		// Add the layout to the vertex array
		vao.AddBuffer(vbo, layout);
		ibo.UnBind();
		vao.UnBind();

		// Create a Texture object
		//Texture tex("res/textures/Rendering Pipeline.png");
		//tex.Bind();

		// Basic Lighting Shader 
		Shader shader("res/shaders/BasicLightingShader.shader");
		//Shader shader("res/shaders/BasicTexture.shader");
		shader.Bind();
		shader.SetUniform1f("u_AmbientStrength", 0.01f);
		shader.SetUniform1f("u_Shininess", 256.0f);
		shader.SetUniform1f("u_Reflectivity", 1.0f);
		//shader.SetUniform1i("u_Texture", 0 /* Slot number*/);

		// Simple Renderer to render the objects
		Renderer renderer;

		// Camera
		Camera camera(Vector3(0, 0, 3.0f), Vector3(0, 0, 0), Vector3::YAxis);
		Matrix4 view = camera.GetViewMatrix();
		shader.SetUniformMat4f("u_View", view);
		shader.SetUniform3f("u_CameraPos", camera.CameraPosition);

		// Projection Matrix
		//Matrix4 proj = Projection::Ortho(-6.0f, 6.0f, -4.5f, 4.5f, -10.0f, 10.0f);
		Matrix4 proj = Projection::Perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		shader.SetUniformMat4f("u_Projection", proj);

		Light light(Vector3(0, 0, 20.0f), Vector4(1, 1, 1, 1));
		shader.SetUniform3f("u_LightPos", light.Position);
		shader.SetUniform4f("u_LightColor", light.Color);

		int times = 0;
		float then = Clock::GetClock()->GetTime();

		float rotation = 0.0f;
		Vector3 translation(0);
		Vector3 scaleVec(1);
		Vector3 axis(1, 0, 0);

		bool bShowMenu = true;

		// Draw while the window doesn't close
		while (!m_Window->IsClosed())
		{
			// Tick the clock every frame to get the delta time
			Clock::GetClock()->Tick();

			// Update the Gui
			GraphXGui::Update();

			// Calculate the fps
			times++;
			float now = Clock::GetClock()->GetTime();
			if ((now - then) > 1.0f)
			{
				GX_ENGINE_INFO("Frame Rate: {0} FPS", times);
				then = now;
				times = 0;
			}

			// Clear the window 
			m_Window->Clear();

			// Bind the shader and draw the objects
			shader.Bind();

			// Get a new transform window for the cube
			GraphXGui::TransformWindow("Transform", translation, scaleVec, rotation, axis, bShowMenu);

			// Render the GUI
			GraphXGui::Render();

			// Model Matrix
			Translation trans(translation);
			Rotation rotate(rotation * Clock::GetClock()->GetTime(), axis);
			Scaling scale(scaleVec);
			Matrix4 model = trans * rotate * scale;
			shader.SetUniformMat4f("u_Model", model);

			// Normal Transform Matrix (Could be done in the vertex shader, but more efficient here since vertex shader runs for each vertex)
			Matrix3 normal = Matrix3(model);
			shader.SetUniformMat3f("u_Normal", normal);

			// Render the Cube
			renderer.Draw(vao, ibo, shader);

			//Poll events and swap buffers
			m_Window->Update();
		}
	}
}