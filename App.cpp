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

	float vertices[] = {
		0.0f,  0.5f, 0.0f,  // Top vertex
	   -0.5f, -0.5f, 0.0f,  // Bottom-left vertex
		0.5f, -0.5f, 0.0f   // Bottom-right vertex
	};

	unsigned int vertexArray;
	unsigned int vertexBuffer;
	unsigned int shaderProgram;

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	unsigned int vertexShader = GLUtils::LoadShader("shaders/basic.vert", GL_VERTEX_SHADER);
	unsigned int fragmentShader = GLUtils::LoadShader("shaders/basic.frag", GL_FRAGMENT_SHADER);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "Shader program linking error:\n" << infoLog << std::endl;
	}


	double previousFrameTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - previousFrameTime;
		previousFrameTime = currentTime;

		//std::cout << deltaTime * 1000.0 << " ms" << std::endl;

		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use shader program
		glUseProgram(shaderProgram);

		// Draw triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}
