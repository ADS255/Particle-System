#include "App.h"

int main()
{
	int width = 1920, height = 1080;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(width, height, "Particle System", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD." << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GLUtils::OpenGlDebugCallback, 0);

	glViewport(0, 0, width, height);

	GLUtils::Info();

	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 cameraPosition(0.0f, 0.0f, 30.0f);
	glm::vec3 targetPosition(0.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);

	glm::mat4 view = glm::lookAt(cameraPosition, targetPosition, up);

	float fov = 90.0f;
	float aspectRatio = (float)width / (float)height;
	float nearPlane = 0.1f;
	float farPlane = 1000.0f;

	glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

	glm::vec3 cameraDirection = glm::normalize(cameraPosition - targetPosition);

	glm::vec3 right = glm::normalize(glm::cross(up, cameraDirection));

	//glm::mat4 mvp = projection * view * model;


	//BaseParticleEmitter_2 emitter = BaseParticleEmitter_2();
	//BaseParticleEmitter_1 emitter = BaseParticleEmitter_1();
	BaseParticleEmitter_3 emitter = BaseParticleEmitter_3();
	//BaseParticleEmitter emitter = BaseParticleEmitter();

	PerformanceProfiler profiler = PerformanceProfiler(&emitter);

	// Variables to control camera
	float radius = 30.0f;
	float angleSpeed = 0.25f;
	float cameraHeight = 30.0f;
	bool orbitEnabled = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460 core");


	double previousFrameTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - previousFrameTime;
		previousFrameTime = currentTime;


		// Animate or stop orbiting camera
		static float angle = 0.0f;
		if (orbitEnabled) {
			angle = glfwGetTime() * angleSpeed;
		}

		// Update camera position based on controls
		cameraPosition.x = radius * cos(angle);
		cameraPosition.z = radius * sin(angle);
		cameraPosition.y = cameraHeight;

		// Recalculate view matrix
		view = glm::lookAt(cameraPosition, targetPosition, up);


		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Camera Controls");
		ImGui::Checkbox("Enable Orbit", &orbitEnabled);
		ImGui::SliderFloat("Radius", &radius, 1.0f, 100.0f);
		ImGui::SliderFloat("Angle Speed", &angleSpeed, 0.0f, 2.0f);
		ImGui::SliderFloat("Camera Height", &cameraHeight, -50.0f, 50.0f);
		ImGui::End();

		emitter.Editor();
		profiler.Display();

		ImGui::Render();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		emitter.Update(deltaTime, cameraPosition);
		emitter.Render(view, projection);
		profiler.BenchMark(deltaTime);


		glfwSwapBuffers(window);
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();

	glfwDestroyWindow(window);
	glfwTerminate();
}
