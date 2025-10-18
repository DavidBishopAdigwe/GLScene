#include "Main/Engine.h"
#include <ranges>


GLFWwindow* Engine::m_window{};

Vec3f cubePositions[] = {
	Vec3f( 0.0f,  0.0f,  0.0f),
	Vec3f( 2.0f,  5.0f, -15.0f),
	Vec3f(-1.5f, -2.2f, -2.5f),
	Vec3f(-3.8f, -2.0f, -12.3f),
	Vec3f( 2.4f, -0.4f, -3.5f),
	Vec3f(-1.7f,  3.0f, -7.5f),
	Vec3f( 1.3f, -2.0f, -2.5f),
	Vec3f( 1.5f,  2.0f, -2.5f),
	Vec3f( 1.5f,  0.2f, -1.5f),
	Vec3f(-1.3f,  1.0f, -1.5f)
};
glm::vec3 pointLightPositions[] = {
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -3.0f)
};
void Engine::run()
{
	// initialize the engine, main loop
	// TODO: replace and construct better with systems
	init(m_window);
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();

		deltaTimeUpdate();

		imguiUse();
		sInput();
		sLighting();
		sRendering();

		imguiRender();

		glfwSwapBuffers(m_window);
	}

	// shutdown imgui and terminate glfw
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}



void Engine::init(GLFWwindow*& window)
{
	// initialize glfw and give window hints for the version and the opengl profile we want to use
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create the glfw window
	window = glfwCreateWindow(screen::width, screen::height, screen::title, nullptr, nullptr);

	// error handing glfw window
	if (!window)
	{
		std::cout << "FAILED TO INITIALIZE GLFW WINDOW. \n";
		glfwTerminate();
		return;
	}

	// tell glfw what window we are using, self-ex
	glfwMakeContextCurrent(window);

	// load glad + error handing
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "FAILED TO INITIALIZE GLAD. \n";
	}

	glfwSwapInterval(0);
	// set callbacks, input mode and enable depth
	glEnable(GL_DEPTH_TEST);
	glfwSetFramebufferSizeCallback(m_window, Engine::frameBufferSizeCallback);
	glfwSetScrollCallback         (m_window, Engine::scrollCallback);
	glfwSetCursorPosCallback      (m_window, Engine::mouseCallback);
	glfwSetKeyCallback            (m_window, Engine::keyCallback);
	glfwSetInputMode              (m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	imguiInit();


	// as is obvious, materials must be created after shaders and textures
	createTextures();
	createShaders();
	createMeshes();
	createMaterials();

	for (int i = 1; i < IM_ARRAYSIZE(cubePositions); i++)
	{
		createCube("Cube", "container",
			cubePositions[i], 20.0f * i,
			cubePositions[i].getNormalized(),
			Vec3f(2.0f));
	}

	for (int j = 0; j < 4; j++)
	{
		createPointLight(Vec3f(pointLightPositions[j].x,pointLightPositions[j].y, pointLightPositions[j].z), j);
	}


	//createFloor();
//	createDirectionalLight();

	// TODO: change scene creation



}

void Engine::sRendering()
{
	// Tell opengl what color we want glClear to clear the color buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	 // Clear color and depth buffers


	for (auto e : m_entityManager.entities)
	{
		if (e.hasComponent<MeshRenderer>() && e.hasComponent<Transform>())
		{
			auto& cTransform = e.getComponent<Transform>();
			auto& cRenderer = e.getComponent<MeshRenderer>();

			glm::mat4 worldMat = glm::mat4{1.0f};
			worldMat = glm::translate(worldMat, static_cast<glm::vec3>(cTransform.getPosition()));
			worldMat = glm::rotate(worldMat, glm::radians(cTransform.getRotationAngle()), static_cast<glm::vec3>(cTransform.getRotationAxis()));
			worldMat = glm::scale(worldMat, static_cast<glm::vec3>(cTransform.getScale()));
			cRenderer.m_material->use(camera.getProjectionMatrix(), camera.getViewMatrix(), worldMat, camera.getPosition());
			cRenderer.m_mesh->draw();
		}
	}

}

void Engine::sLighting()
{
	for (auto e: m_entityManager.entities)
	{
		if (e.hasComponent<PointLight>())
		{
			auto& pointLight = e.getComponent<PointLight>();
			auto& transform = e.getComponent<Transform>();

			for (auto ent: m_entityManager.entities)
			{
				if (ent.hasComponent<MeshRenderer>() && e != ent)
				{
					auto& renderer = ent.getComponent<MeshRenderer>();
					pointLight.use(renderer, transform);
				}
			}

		}
		if (e.hasComponent<DirectionalLight>())
		{
			auto& directionalLight = e.getComponent<DirectionalLight>();

			for (auto ent: m_entityManager.entities)
			{
				if (ent.hasComponent<MeshRenderer>() && !e.hasComponent<PointLight>())
				{
					auto& renderer = ent.getComponent<MeshRenderer>();

					directionalLight.use(renderer, e.getComponent<Transform>());
				}
			}
		}
	}
}

void Engine::sInput()
{

	camera.processMouseInput(m_window, static_cast<float>(screen::xOffset), static_cast<float>(screen::yOffset), static_cast<float>(screen::scrollOffset));
	camera.processKeyboardInput(m_window, static_cast<float>(m_deltaTime));

	screen::xOffset = 0.0f;
	screen::yOffset = 0.0f;
	screen::scrollOffset = 0.0f;
}

void Engine::deltaTimeUpdate()
{
	auto currentTime = static_cast<float>(glfwGetTime());
	m_deltaTime = currentTime - m_lastFrameTime;
	m_lastFrameTime = currentTime;
}

void Engine::imguiInit()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();
}

void Engine::imguiUse()
{
	// setup imgui for a new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// initialize and assign entity names for imgui dropdown
	std::vector<const char*> entityNames{};
	entityNames.reserve(m_entityManager.entities.size());
	for (auto& e : m_entityManager.entities)
	{
		entityNames.push_back(e.getTag().c_str());
	}

	std::vector<const char*> materialNames{};
	for (const auto &key: materials | std::views::keys)
	{
		materialNames.push_back(key.c_str());
	}

	ImGui::Begin("Linking ts broke my ass");

	if (ImGui::BeginTabBar("Content"))
	{
		if (ImGui::BeginTabItem("Entities"))
		{
			static int entityIndex{};

			static float entityPosition[3]{};
			static float entityScale[3]{};
			static float entityRotationAxis[3]{};
			static float entityRotationAngle{};
			ImGui::Combo("Entities", &entityIndex, entityNames.data(), static_cast<int>(entityNames.size()));
			if (entityIndex >= 0)
			{

				auto& currentEntity = m_entityManager.entities[entityIndex];
				auto& entityTransform = currentEntity.getComponent<Transform>();
				for (int index = 0; index <= 2; ++index)
				{
					entityPosition[index] = entityTransform.getPosition()[index];
					entityScale[index] = entityTransform.getScale()[index];
					entityRotationAxis[index] = entityTransform.getRotationAxis()[index];
				}
				entityRotationAngle = entityTransform.getRotationAngle();

				ImGui::DragFloat3("Position", entityPosition);
				ImGui::DragFloat("Rotation Angle", &entityRotationAngle);
				ImGui::DragFloat3("Rotation Axis", entityRotationAxis);
				ImGui::DragFloat3("Scale", entityScale);

				entityTransform.rotate(entityRotationAngle, Vec3f(entityRotationAxis[0], entityRotationAxis[1], entityRotationAxis[2]));
				entityTransform.setPosition(Vec3f(entityPosition[0], entityPosition[1], entityPosition[2]));
				entityTransform.setScale(Vec3f(entityScale[0], entityScale[1], entityScale[2]));
			}

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Materials"))
		{
			static int materialIndex{};
			ImGui::Combo("Materials", &materialIndex, materialNames.data(), static_cast<int>(materialNames.size()));
			if (materialIndex >= 0)
			{
				std::shared_ptr<Material> currentMaterial = materials[materialNames[materialIndex]];
				static float materialColor[3]{};
				static float ambientStr{};
				static float diffuseStr{};
				static float specularStr{};
				static float shininess{};

				ambientStr = currentMaterial->getAmbience();
				diffuseStr = currentMaterial->getDiffuse();
				specularStr = currentMaterial->getSpecular();
				shininess = currentMaterial->getShininess();

				for (int index = 0; index <= 2; ++index)
				{
					materialColor[index] = currentMaterial->getColor()[index];
				}

				ImGui::DragFloat("Ambience", &ambientStr);
				ImGui::ColorEdit3("Colour", &materialColor[0]);
				ImGui::DragFloat("Diffuse", &diffuseStr);
				ImGui::DragFloat("Specular", &specularStr);
				ImGui::DragFloat("Shininess", &shininess);

				currentMaterial->setColor(Vec3f(materialColor[0], materialColor[1], materialColor[2]));
				currentMaterial->setDiffuse(diffuseStr);
				currentMaterial->setAmbience(ambientStr);
				currentMaterial->setSpecular(specularStr);
				currentMaterial->setShininess(shininess);
			}
			ImGui::EndTabItem();
		}


		ImGui::EndTabBar();

	}
	std::stringstream frameRateText{};
	frameRateText << "Delta Time: " << 1000/(1/m_deltaTime) << " ms/s";
	ImGui::Text(frameRateText.str().c_str());

	// Entity Tab

	ImGui::End();
}


void Engine::imguiRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void Engine::createTextures()
{
	textures["default"] = std::make_shared<Texture>("Textures/empty.jpg", GL_REPEAT, GL_RGB);
	textures["aphex"] = std::make_shared<Texture>("Textures/aphex.gif", GL_REPEAT, GL_RGB);
	textures["floor"] = std::make_shared<Texture>("Textures/woodenFloor.jpg", GL_REPEAT,	GL_RGB);

	textures["container"] = std::make_shared<Texture>("Textures/container_diffuse.png", "Textures/container_specular.png", GL_REPEAT, GL_RGBA, GL_RGBA);
	textures["container"]->setEmissiveMap("Textures/matrix.jpg", GL_RGB);
}


void Engine::createShaders()
{
	shaders["default"] = std::make_shared<Shader>("Shaders/default.vert", "Shaders/default.frag");
	shaders["textured"] = std::make_shared<Shader>("Shaders/default.vert", "Shaders/textured.frag");
	shaders["plain"] = std::make_shared<Shader>("Shaders/default.vert", "Shaders/plain.frag");
	shaders["lightAffected"] = std::make_shared<Shader>("Shaders/default.vert", "Shaders/lightAffected.frag");
}

void Engine::createMeshes()
{
	meshes["cube"] = std::make_shared<Mesh>(Cube::vertices, Cube::indices, 36, sizeof(Cube::vertices), sizeof(Cube::indices));
	meshes["plane"] = std::make_shared<Mesh>(Floor::vertices, Floor::indices, 6, sizeof(Floor::vertices), sizeof(Floor::indices));
}

void Engine::createMaterials()
{
	materials["lit"] = std::make_shared<Material>(shaders["default"]);
	materials["unlit"] = std::make_shared<Material>(shaders["plain"]);
	materials["aphex"] = std::make_shared<Material>(shaders["textured"], textures["aphex"]);
	materials["floor"] = std::make_shared<Material>(shaders["textured"], textures["floor"]);
	materials["container"] = std::make_shared<Material>(shaders["textured"], textures["container"]);
}

void Engine::createFloor()
{
	Entity floor = m_entityManager.createEntity("Floor");
	floor.addComponent<MeshRenderer>(materials["floor"], meshes["plane"]);
	floor.addComponent<Transform>(Vec3f(0.0f), 90.0f, Vec3f(1.0f,0.0f, 0.0f), Vec3f(50.0f));
}

void Engine::createSpotLight()
{
	Entity light = m_entityManager.createEntity("Spot Light");
	light.addComponent<MeshRenderer>(materials["unlit"], meshes["cube"]);
	light.addComponent<Transform>(Vec3f(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z));
	light.addComponent<SpotLight>(Vec3f(camera.m_direction.x, camera.m_direction.y, camera.m_direction.z), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)));
}

void Engine::createPointLight(Vec3f position, int id)
{
	Entity light = m_entityManager.createEntity("Point Light");
	light.addComponent<MeshRenderer>(materials["unlit"], meshes["cube"]);
	light.addComponent<Transform>(position);
	light.addComponent<PointLight>(1, 0.09, 0.032, id);
}

void Engine::createDirectionalLight()
{
	Entity light = m_entityManager.createEntity("Directional Light");
	light.addComponent<Transform>(Vec3f(0.0f));
	light.addComponent<DirectionalLight>(Vec3f(0.0f, -10.0f, 5.0f).getNormalized(), 0);
}

void Engine::createCube(const std::string& name, const char* materialName, Vec3f position, float rotationAngle, Vec3f rotationAxis,
                        Vec3f scale)
{
	Entity cube = m_entityManager.createEntity(name);
	cube.addComponent<MeshRenderer>(materials[materialName], meshes["cube"]);
	cube.addComponent<Transform>(position, rotationAngle, rotationAxis, scale);
}






void Engine::keyCallback(GLFWwindow* window, int key, int action, int scancode, int mods)
{

	if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		screen::shiftLock = !screen::shiftLock;
		if (screen::shiftLock)
		{
			screen::firstMouse = true;
			glfwSetCursorPos(m_window, static_cast<float>(screen::width) / 2.0f, static_cast<float>(screen::height) / 2.0f);
		}
		glfwSetInputMode(m_window, GLFW_CURSOR, screen::shiftLock ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}
}

void Engine::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	screen::width = width;
	screen::height = height;
}

void Engine::mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (!screen::shiftLock) return;
	if (screen::firstMouse)
	{
		screen::lastX = xPos;
		screen::lastY = yPos;
		screen::firstMouse = false;
	}

	screen::xOffset = xPos - screen::lastX;
	screen::yOffset = screen::lastY - yPos;

	screen::lastX = xPos;
	screen::lastY = yPos;
}

void Engine::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	screen::scrollOffset = yOffset;
}



