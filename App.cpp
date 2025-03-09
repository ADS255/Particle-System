#include "App.h"

void APIENTRY OpenGlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::string src;
	switch (source) {
	case GL_DEBUG_SOURCE_API: src = "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: src = "Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: src = "Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: src = "Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION: src = "Application"; break;
	case GL_DEBUG_SOURCE_OTHER: src = "Other"; break;
	}

	std::string typ;
	switch (type) {
	case GL_DEBUG_TYPE_ERROR: typ = "Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typ = "Deprecated Behavior"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typ = "Undefined Behavior"; break;
	case GL_DEBUG_TYPE_PORTABILITY: typ = "Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE: typ = "Performance"; break;
	case GL_DEBUG_TYPE_OTHER: typ = "Other"; break;
	case GL_DEBUG_TYPE_MARKER: typ = "Marker"; break;
	}

	std::string sev;
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: sev = "High"; break;
	case GL_DEBUG_SEVERITY_MEDIUM: sev = "Medium"; break;
	case GL_DEBUG_SEVERITY_LOW: sev = "Low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: sev = "Notification"; break;
	}

	std::cout << "OpenGL Error [" << id << "]: " << message << std::endl;
	std::cout << " Source: " << src << std::endl;
	std::cout << " Type: " << typ << std::endl;
	std::cout << " Severity: " << sev << "\n" << std::endl;
}

/*void Editor() {
	static int particleCount;

	ImGui::Begin("Particle System Editor");

	// Display particle properties for editing
	ImGui::InputInt("Count", &particleCount);

	// Position input (x, y)
	ImGui::InputFloat3("Position", &EmitterProperties.position[0]);

	// Color input (RGBA normalized)
	ImGui::ColorEdit4("Color", EmitterProperties.colour); // Make sure this is a float[4] (RGBA)

	// Velocity input (x, y)
	ImGui::InputFloat3("Velocity", &EmitterProperties.velocity[0]);

	// Size input
	ImGui::InputFloat("Size", &EmitterProperties.size);

	// Display current particle values in the editor
	ImGui::Text("Particle Properties:");
	ImGui::Text("Position: (%.2f, %.2f)", EmitterProperties.position[0], EmitterProperties.position[1]);
	ImGui::Text("Color: (%.2f, %.2f, %.2f, %.2f)", EmitterProperties.colour[0], EmitterProperties.colour[1], EmitterProperties.colour[2], EmitterProperties.colour[3]);
	ImGui::Text("Velocity: (%.2f, %.2f)", EmitterProperties.velocity[0], EmitterProperties.velocity[1]);
	ImGui::Text("Size: %.2f", EmitterProperties.size);

	// Apply button to update the emitter with the new particle values
	if (ImGui::Button("Apply")) {

		float colour[] = { 1.0f,1.0f,0.0f,1.0f };
		glm::vec3 pos = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 velocity = glm::vec3(1.0f, 1.0f, 1.0f);
		float size = 1.0f;
		float lifetime = 10.0f;

		Particle p = Particle(colour, pos, velocity, size, lifetime);
		BaseParticleEmitter* newEmitter = new BaseParticleEmitter(p, 10);

		if (bpe) {
			// Clear old emitter and create a new one with updated particle values
			delete bpe; // Dereference the pointer-to-pointer to delete the original emitter
			bpe = nullptr;
		}

		// Now assign the new emitter
		bpe = newEmitter;

		// Create a new particle emitter with the updated particle settings
	}

	ImGui::End();
}*/


int main()
{
	int width = 800, height = 600;
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
	glDebugMessageCallback(OpenGlDebugCallback, 0);

	glViewport(0, 0, width, height);

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 cameraPosition(0.0f, 30.0f, 30.0f);
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

	glm::vec4 colour = glm::vec4 (1.0f,1.0f,0.0f,1.0f);
	glm::vec3 pos = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 velocity = glm::vec3(1.0f, 1.0f, 1.0f);
	float size = 1.0f;
	float lifetime = 10.0f;

	Particle p = Particle(colour, pos, velocity, size, lifetime);
	BaseParticleEmitter emitter = BaseParticleEmitter();


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

		/*
		float radius = 30.0f; // Distance from origin
		float angle = glfwGetTime() *0.25f; // Use time to animate

		cameraPosition.x = radius * cos(angle);
		cameraPosition.z = radius * sin(angle);
		cameraPosition.y = 30.0f; // Keep Y constant for a horizontal orbit

		// Recalculate view matrix
		view = glm::lookAt(cameraPosition, targetPosition, up);

		// Recalculate MVP
		mvp = projection * view * model;*/

		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		ImGui::Begin("Metrics");
		ImGui::Text("Frame Time: %.3f ms", deltaTime * 1000.0);
		ImGui::Text("FPS: %.1f", 1.0 / deltaTime);
		ImGui::End();

		emitter.Editor();
		emitter.Metrics();

		ImGui::Render();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		emitter.Update(deltaTime);
		emitter.Render(view,projection);

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
