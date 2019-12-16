#include "Physics.h"
#include "BoxShape.h"
#include "SphereShape.h"
#include "CapsuleShape.h"
#include "CylinderShape.h"
#include "ConeShape.h"
#include "RigidBody.h"
#include "pch.h"
#include "Application.h"
#include "Common.h"
#include "Log.h"
#include "MeshRenderer.h"
#include "Quad.h"
#include "CameraComp.h"
#include "Input.h"
#include "Resources.h"


Application* Application::m_application = nullptr;
Application::Application()
{
}
void Application::Init()
{
	//performing initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG_DEBUG(SDL_GetError(), Log::Error);
		exit(-1);
	}
	//setting openGL version to 4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	//setting openGL context to core, not compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//creating window
	m_window = SDL_CreateWindow("GP3-GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight, SDL_WINDOW_OPENGL);

	SDL_CaptureMouse(SDL_TRUE);

	OpenGlInit();
	GameInit();

}
void Application::OpenGlInit()
{
	//creating context (our opengl statemachine in which all our GL calls
	//will refer to)
	m_glContext = SDL_GL_CreateContext(m_window);
	CHECK_GL_ERROR();
	SDL_GL_SetSwapInterval(1);
	//initialsing glew (do this after establishing context!)
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}

	//Smooth shading
	GL_ATTEMPT(glShadeModel(GL_SMOOTH));

	//enable depth testing
	GL_ATTEMPT(glEnable(GL_DEPTH_TEST));

	//set less or equal func for depth testing
	GL_ATTEMPT(glDepthFunc(GL_LEQUAL));

	//enabling blending
	glEnable(GL_BLEND);
	GL_ATTEMPT(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	//turn on back face culling
	GL_ATTEMPT(glEnable(GL_CULL_FACE));
	GL_ATTEMPT(glCullFace(GL_BACK));

	glViewport(0, 0, (GLsizei)m_windowWidth, (GLsizei)m_windowHeight);
}

void Application::GameInit()
{

	//load all models
	Resources::GetInstance()->AddModel("cube.obj");
	Resources::GetInstance()->AddModel("monkey.obj");
	Resources::GetInstance()->AddModel("umbreon.obj");
	Resources::GetInstance()->AddModel("sphere.obj");
	Resources::GetInstance()->AddModel("cone.obj");
	
	//load all textures
	Resources::GetInstance()->AddTexture("Wood.jpg");
	Resources::GetInstance()->AddTexture("umbreonTex.jpg");
	Resources::GetInstance()->AddTexture("monkeyTex.jpg");
	Resources::GetInstance()->AddTexture("coneTexture.jpg");
	Resources::GetInstance()->AddTexture("sphereTexture.jpg");
	Resources::GetInstance()->AddTexture("floorTex.jpg");
	
	//load all shaders
	Resources::GetInstance()->AddShader(std::make_shared <ShaderProgram>(ASSET_PATH + "Blinn_VERT.glsl", ASSET_PATH + "Blinn_FRAG.glsl"), "blinn");
	Resources::GetInstance()->AddShader(std::make_shared <ShaderProgram>(ASSET_PATH + "simple_VERT.glsl", ASSET_PATH + "simple_FRAG.glsl"), "simple");
	Resources::GetInstance()->AddShader(std::make_shared <ShaderProgram>(ASSET_PATH + "BlinnPhong_VERT.glsl", ASSET_PATH + "BlinnPhong_FRAG.glsl"), "blinn-Phong");
	Resources::GetInstance()->AddShader(std::make_shared <ShaderProgram>(ASSET_PATH + "NormalMap_VERT.glsl", ASSET_PATH + "NormalMap_FRAG.glsl"), "normal");
	
	//Plain floor
	Entity* a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(new MeshRenderer(
		Resources::GetInstance()->GetModel("cube.obj"),
		Resources::GetInstance()->GetShader("blinn-Phong"),
		Resources::GetInstance()->GetTexture("floorTex.jpg"))
	);
	MeshRenderer* m = a->GetComponent<MeshRenderer>();
	a->GetTransform()->SetPosition(glm::vec3(0, -10.f, -20.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(100.f, 1.f, 100.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(100.f, 1.f, 100.f));


	//Camera component
	a = new Entity();
	m_entities.push_back(a);
	CameraComp* cc = new CameraComp();
	a->AddComponent(cc);
	//cc->Start();
	//Camera position and rotation
	m_entities.back()->GetTransform()->SetPosition(glm::vec3(0, 0, 0));


	//Monkey Model
	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(new MeshRenderer(
		Resources::GetInstance()->GetModel("monkey.obj"),
		Resources::GetInstance()->GetShader("normal"),
		Resources::GetInstance()->GetTexture("monkeyTex.jpg"))
	);
	a->GetTransform()->SetPosition(glm::vec3(10.f, 0.f, -20.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new SphereShape(5.0f));
	//a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(5.f, 5.f, 5.f));

	//Pokemon model
	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(new MeshRenderer(
		Resources::GetInstance()->GetModel("umbreon.obj"),
		Resources::GetInstance()->GetShader("blinn-Phong"),
		Resources::GetInstance()->GetTexture("umbreonTex.jpg"))
	);
	a->GetTransform()->SetPosition(glm::vec3(25.f, 0.f, -20.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(5.f, 7.f, 5.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(100, btVector3());
	a->GetTransform()->SetScale(glm::vec3(5.f, 5.f, 5.f));

	for (int i = 0; i < 100; i++)
	{
		if (i % 2 == 0)
		{
			Entity* a = new Entity();
			m_entities.push_back(a);
			a->AddComponent(
				new MeshRenderer(
					Resources::GetInstance()->GetModel("cone.obj"),
					Resources::GetInstance()->GetShader("blinn-Phong"),
					Resources::GetInstance()->GetTexture("coneTexture.jpg"))
			);
			a->GetTransform()->SetPosition(glm::vec3(0, 5.f * i, -20.f));
			a->AddComponent<RigidBody>();
			//a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.f,1.f, 1.f)));
			a->GetComponent<RigidBody>()->Init(new ConeShape(1.4f, 3.4f));
			a->GetTransform()->SetScale(glm::vec3(0.25f));
		}
		else if (i % 3 == 0)
		{
			Entity* a = new Entity();
			m_entities.push_back(a);
			a->AddComponent(
				new MeshRenderer(
					Resources::GetInstance()->GetModel("cube.obj"),
					Resources::GetInstance()->GetShader("blinn-Phong"),
					Resources::GetInstance()->GetTexture("Wood.jpg"))
			);
			a->GetTransform()->SetPosition(glm::vec3(0, 5.f * i, -20.f));
			a->AddComponent<RigidBody>();
			a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.f, 1.f, 1.f)));
			//a->GetComponent<RigidBody>()->Init(new SphereShape(1.0f));
			a->GetTransform()->SetScale(glm::vec3(1.f, 1.f, 1.f));
		}
		else
		{
			Entity* a = new Entity();
			m_entities.push_back(a);
			a->AddComponent(
				new MeshRenderer(
					Resources::GetInstance()->GetModel("sphere.obj"),
					Resources::GetInstance()->GetShader("blinn-Phong"),
					Resources::GetInstance()->GetTexture("sphereTexture.jpg"))
			);
			a->GetTransform()->SetPosition(glm::vec3(0, 5.f * i, -20.f));
			a->AddComponent<RigidBody>();
			a->GetComponent<RigidBody>()->Init(new SphereShape(1.0f));
			a->GetTransform()->SetScale(glm::vec3(1.f, 1.f, 1.f));
		}

	
		
	}
	
}

void Application::Loop()
{
	m_appState = AppState::RUNNING;
	auto prevTicks = std::chrono::high_resolution_clock::now();
	SDL_Event event;

	bool bPlayPhysicsDemo = false;
	
	while (m_appState != AppState::QUITTING)
	{
		//poll SDL events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				Resources::GetInstance()->ReleaseResources();
				m_appState = AppState::QUITTING;
				break;
			case SDL_WINDOWEVENT_CLOSE:
				Resources::GetInstance()->ReleaseResources();
				m_appState = AppState::QUITTING;
				break;
			case SDL_KEYDOWN:

				//Set the key pressed to true
				INPUT->SetKey(event.key.keysym.sym, true);

				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:

						m_appState = AppState::QUITTING;

					break;

					//Apply force to the models
					case SDLK_q:

						for (int iCount = 2; iCount < m_entities.size(); iCount++)
							m_entities.at(iCount)->GetComponent<RigidBody>()->ApplyForce();

					break;

					//Apply torque to the models
					case SDLK_e:

					for (int iCount = 2; iCount < m_entities.size(); iCount++)
						m_entities.at(iCount)->GetComponent<RigidBody>()->ApplyTorque();

					break;

					//Play the physics demo
					case SDLK_SPACE:

						if (INPUT->GetKeyDown(event.key.keysym.sym))
						{
							LOG_DEBUG("Space pressed this frame", Log::Trace);
						}
						
						bPlayPhysicsDemo = true;

					break;

				}
				
			
			break;
				//record when the user releases a key
			case SDL_KEYUP:

				//Set the key up to false
				INPUT->SetKey(event.key.keysym.sym, false);

				switch (event.key.keysym.sym)
				{
				
				case SDLK_a:

					//GetKeyUp(SDLK_a);
					LOG_DEBUG("Key UP", Log::Trace);

				break;

				case SDLK_SPACE:

				if (INPUT->GetKeyUp(event.key.keysym.sym))
				{
					LOG_DEBUG("Space released this frame", Log::Trace);
				}

				break;

				}

				break;
			case SDL_MOUSEMOTION:
				INPUT->MoveMouse(glm::ivec2(event.motion.xrel, event.motion.yrel));
				glm::ivec2 mouseMovement = INPUT->GetMouseDelta();

				//Set camera movement
				m_entities.at(1)->GetTransform()->RotateEulerAxis(mouseMovement.x*(0.8f), glm::vec3(0, 1, 0));
				m_entities.at(1)->GetTransform()->RotateEulerAxis(mouseMovement.y*(0.8f), m_entities.at(1)->GetTransform()->GetRight());


			break;
			case SDLK_ESCAPE:
				Quit();
				break;

			}
		}

		AllMovements();

		auto currentTicks = std::chrono::high_resolution_clock::now();
		float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks - prevTicks).count() / 100000;
		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;

		if(bPlayPhysicsDemo)
			Physics::GetInstance()->Update(deltaTime);

		Resources::GetInstance()->ReleaseUnusedResourses();

		//Set the uniforms for the Blinn-Phong shader
		glm::vec3 viewPos = Application::GetInstance()->GetCamera()->GetParentTransform()->GetPosition();
		Resources::GetInstance()->GetShader("blinn-Phong")->Use();
		Resources::GetInstance()->GetShader("blinn-Phong")->setMat4("view", Application::GetInstance()->GetCamera()->GetView());
		Resources::GetInstance()->GetShader("blinn-Phong")->setMat4("projection", Application::GetInstance()->GetCamera()->GetProj());
		Resources::GetInstance()->GetShader("blinn-Phong")->setVec3("objectColor", glm::vec3(1.0f, 1.f, 1.f));
		Resources::GetInstance()->GetShader("blinn-Phong")->setVec3("PosView", glm::vec3(viewPos.x, viewPos.y, viewPos.z));
		Resources::GetInstance()->GetShader("blinn-Phong")->setVec3("lightPos", glm::vec3(1.2f, 1.f, 2.0f));

		//Set the uniforms for the normal mapping shader
		Resources::GetInstance()->GetShader("normal")->Use();
		Resources::GetInstance()->GetShader("normal")->setMat4("view", Application::GetInstance()->GetCamera()->GetView());
		Resources::GetInstance()->GetShader("normal")->setMat4("projection", Application::GetInstance()->GetCamera()->GetProj());
		Resources::GetInstance()->GetShader("normal")->setVec3("PosView", glm::vec3(viewPos.x, viewPos.y, viewPos.z));
		Resources::GetInstance()->GetShader("normal")->setVec3("lightPos", glm::vec3(1.2f, 1.f, 2.0f));
		

		// update and render all entities
		Update(deltaTime);
		Render();
		SDL_GL_SwapWindow(m_window);

	}
}

void Application::Update(float deltaTime)
{
	for (auto& a : m_entities)
	{
		a->OnUpdate(deltaTime);
	}
}

//Done to make the multi movement
void Application::AllMovements()
{
	//First person camera movement
	if (INPUT->GetKey(SDLK_a))
	{
		m_entities.at(1)->GetTransform()->AddPosition(-m_entities.at(1)->GetTransform()->GetRight());
	}
	else if (INPUT->GetKey(SDLK_d))
	{
		m_entities.at(1)->GetTransform()->AddPosition(m_entities.at(1)->GetTransform()->GetRight());
	}

	if (INPUT->GetKey(SDLK_w))
	{
		m_entities.at(1)->GetTransform()->AddPosition(m_entities.at(1)->GetTransform()->GetForward());
	}
	else if (INPUT->GetKey(SDLK_s))
	{
		m_entities.at(1)->GetTransform()->AddPosition(-m_entities.at(1)->GetTransform()->GetForward());
	}


	//Umbreon pokemon model
	if (INPUT->GetKey(SDLK_LEFT))
	{
		m_entities.at(3)->GetTransform()->AddPosition(-m_entities.at(3)->GetTransform()->GetRight());
	}
	else if (INPUT->GetKey(SDLK_RIGHT))
	{
		m_entities.at(3)->GetTransform()->AddPosition(m_entities.at(3)->GetTransform()->GetRight());
	}
	if (INPUT->GetKey(SDLK_UP))
	{
		m_entities.at(3)->GetTransform()->AddPosition(m_entities.at(3)->GetTransform()->GetForward());
	}
	else if (INPUT->GetKey(SDLK_DOWN))
	{
		m_entities.at(3)->GetTransform()->AddPosition(-m_entities.at(3)->GetTransform()->GetForward());
	}
}

void Application::Render()
{
	/* Clear context */
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_mainCamera->Recalculate();
	for (auto& a : m_entities)
	{
		a->OnRender();
	}
}

void Application::SetCamera(Camera* camera)
{
	if (camera != nullptr)
	{
		m_mainCamera = camera;
	}
}

void Application::Quit()
{
	//Close SDL
	Physics::GetInstance()->Quit();
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit();
}
Application::~Application()
{
}
Application* Application::GetInstance()
{
	if (m_application == nullptr)
	{
		m_application = new Application();
	}
	return m_application;
}
void Application::Run()
{
	Init();
	Loop();
	Quit();
}
