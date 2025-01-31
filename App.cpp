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

	glm::vec3 cameraPosition(0.0f, 3.0f, 3.0f);
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

	glm::mat4 mvp = projection * view * model;

	BaseParticleEmitter bpe = BaseParticleEmitter(1,10);

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

		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Metrics");
		ImGui::Text("Frame Time: %.3f ms", deltaTime * 1000.0);
		ImGui::Text("FPS: %.1f", 1.0 / deltaTime);
		ImGui::End();

		ImGui::Render();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		bpe.Update(deltaTime);
		bpe.Render(mvp);


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
