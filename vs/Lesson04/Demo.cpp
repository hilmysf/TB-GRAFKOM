#include "Demo.h"


GLuint texture[10];


Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	
	BuildKasur1();
	BuildKasur2();
	BuildLemariKecil();
	BuildPintu();
	BuildLemariBesar();
	BuildPintu2();

	BuildPlaneLantai();
	BuildPlaneTembok1();
	BuildPlaneTembok2();
	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAOkasur1);
	glDeleteBuffers(1, &VBOkasur1);
	glDeleteBuffers(1, &EBOkasur1);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth / 2;
	double midY = screenHeight / 2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);




}

void Demo::Update(double deltaTime) {

}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// set lighting attributes
	GLint lightPosLoc = glGetUniformLocation(this->activeProgram, "lightPos");
	glUniform3f(lightPosLoc, 0, 1, 0);
	GLint viewPosLoc = glGetUniformLocation(this->activeProgram, "viewPos");
	glUniform3f(viewPosLoc, 0, 2, 3);
	GLint lightColorLoc = glGetUniformLocation(this->activeProgram, "lightColor");
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	
	
	DrawKasur1();
	DrawKasur2();
	DrawLemariKecil();
	DrawLemariBesar();
	DrawPintu2();
	DrawPintu();
	
	DrawPlaneLantai();
	DrawPlaneTembok1();
	DrawPlaneTembok2();
	DrawPlaneTembok3();
	DrawPlaneTembok4();
	glDisable(GL_DEPTH_TEST);
}



void Demo::BuildKasur1() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureKasur1);
	glBindTexture(GL_TEXTURE_2D, textureKasur1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("dark_wood.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-0.5, -0.5, 0.5, 0, 0,  // 0
		0.5, -0.5, 0.5, 1, 0,   // 1
		0.5,  0.5, 0.5, 1, 1,   // 2
		-0.5,  0.5, 0.5, 0, 1,  // 3

		// right
		0.5,  0.5,  0.5, 0, 0,  // 4
		0.5,  0.5, -0.5, 1, 0,  // 5
		0.5, -0.5, -0.5, 1, 1,  // 6
		0.5, -0.5,  0.5, 0, 1,  // 7

		// back
		-0.5, -0.5, -0.5, 0, 0, // 8 
		0.5,  -0.5, -0.5, 1, 0, // 9
		0.5,   0.5, -0.5, 1, 1, // 10
		-0.5,  0.5, -0.5, 0, 1, // 11

		// left
		-0.5, -0.5, -0.5, 0, 0, // 12
		-0.5, -0.5,  0.5, 1, 0, // 13
		-0.5,  0.5,  0.5, 1, 1, // 14
		-0.5,  0.5, -0.5, 0, 1, // 15

		// upper
		0.5, 0.5,  0.5, 0, 0,   // 16
		-0.5, 0.5,  0.5, 1, 0,  // 17
		-0.5, 0.5, -0.5, 1, 1,  // 18
		0.5, 0.5, -0.5, 0, 1,   // 19

		// bottom
		-0.5, -0.5, -0.5, 0, 0, // 20
		0.5, -0.5, -0.5, 1, 0,  // 21
		0.5, -0.5,  0.5, 1, 1,  // 22
		-0.5, -0.5,  0.5, 0, 1, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAOkasur1);
	glGenBuffers(1, &VBOkasur1);
	glGenBuffers(1, &EBOkasur1);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOkasur1);

	glBindBuffer(GL_ARRAY_BUFFER, VBOkasur1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOkasur1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawKasur1()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureKasur1);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAOkasur1); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	// set lighting materials attributes (Emerald)
	GLint ambientMatLoc = glGetUniformLocation(this->activeProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 0.0215f, 0.1745f, 0.0215f);
	GLint diffuseMatLoc = glGetUniformLocation(this->activeProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 0.07568f, 0.61424f, 0.07568f);
	GLint specularMatLoc = glGetUniformLocation(this->activeProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.633f, 0.727811f, 0.633f);
	GLint shininessMatLoc = glGetUniformLocation(this->activeProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 0.6f);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(3, 0.5, 0));
	model = glm::scale(model, glm::vec3(1, 2, 4));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildKasur2() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureKasur2);
	glBindTexture(GL_TEXTURE_2D, textureKasur2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("kasur2.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-0.5, -0.5, 0.5, 0, 0,  // 0
		0.5, -0.5, 0.5, 1, 0,   // 1
		0.5,  0.5, 0.5, 1, 1,   // 2
		-0.5,  0.5, 0.5, 0, 1,  // 3

		// right
		0.5,  0.5,  0.5, 0, 0,  // 4
		0.5,  0.5, -0.5, 1, 0,  // 5
		0.5, -0.5, -0.5, 1, 1,  // 6
		0.5, -0.5,  0.5, 0, 1,  // 7

		// back
		-0.5, -0.5, -0.5, 0, 0, // 8 
		0.5,  -0.5, -0.5, 1, 0, // 9
		0.5,   0.5, -0.5, 1, 1, // 10
		-0.5,  0.5, -0.5, 0, 1, // 11

		// left
		-0.5, -0.5, -0.5, 0, 0, // 12
		-0.5, -0.5,  0.5, 1, 0, // 13
		-0.5,  0.5,  0.5, 1, 1, // 14
		-0.5,  0.5, -0.5, 0, 1, // 15

		// upper
		0.5, 0.5,  0.5, 0, 0,   // 16
		-0.5, 0.5,  0.5, 1, 0,  // 17
		-0.5, 0.5, -0.5, 1, 1,  // 18
		0.5, 0.5, -0.5, 0, 1,   // 19

		// bottom
		-0.5, -0.5, -0.5, 0, 0, // 20
		0.5, -0.5, -0.5, 1, 0,  // 21
		0.5, -0.5,  0.5, 1, 1,  // 22
		-0.5, -0.5,  0.5, 0, 1, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAOkasur2);
	glGenBuffers(1, &VBOkasur2);
	glGenBuffers(1, &EBOkasur2);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOkasur2);

	glBindBuffer(GL_ARRAY_BUFFER, VBOkasur2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOkasur2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawKasur2()
{
	glUseProgram(shaderProgram);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureKasur2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAOkasur2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	// set lighting materials attributes (Emerald)
	GLint ambientMatLoc = glGetUniformLocation(this->activeProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 0.0215f, 0.1745f, 0.0215f);
	GLint diffuseMatLoc = glGetUniformLocation(this->activeProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 0.07568f, 0.61424f, 0.07568f);
	GLint specularMatLoc = glGetUniformLocation(this->activeProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.633f, 0.727811f, 0.633f);
	GLint shininessMatLoc = glGetUniformLocation(this->activeProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 0.6f);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.5, 0, 0));
	model = glm::scale(model, glm::vec3(4, 1, 4));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildLemariKecil() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureLemariKecil);
	glBindTexture(GL_TEXTURE_2D, textureLemariKecil);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("wood02.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-0.5, -1.0, 0.5, 0, 0,  // 0
		0.5, -1.0, 0.5, 1, 0,   // 1
		0.5,  1.0, 0.5, 1, 1,   // 2
		-0.5,  1.0, 0.5, 0, 1,  // 3

		// right
		0.5,  1.0,  0.5, 0, 0,  // 4
		0.5,  1.0, -0.5, 1, 0,  // 5
		0.5, -1.0, -0.5, 1, 1,  // 6
		0.5, -1.0,  0.5, 0, 1,  // 7

		// back
		-0.5, -1.0, -0.5, 0, 0, // 8 
		0.5,  -1.0, -0.5, 1, 0, // 9
		0.5,   1.0, -0.5, 1, 1, // 10
		-0.5,  1.0, -0.5, 0, 1, // 11

		// left
		-0.5, -1.0, -0.5, 0, 0, // 12
		-0.5, -1.0,  0.5, 1, 0, // 13
		-0.5,  1.0,  0.5, 1, 1, // 14
		-0.5,  1.0, -0.5, 0, 1, // 15

		// upper
		0.5, 1.0,  0.5, 0, 0,   // 16
		-0.5, 1.0,  0.5, 1, 0,  // 17
		-0.5, 1.0, -0.5, 1, 1,  // 18
		0.5, 1.0, -0.5, 0, 1,   // 19

		// bottom
		-0.5, -1.0, -0.5, 0, 0, // 20
		0.5, -1.0, -0.5, 1, 0,  // 21
		0.5, -1.0,  0.5, 1, 1,  // 22
		-0.5, -1.0,  0.5, 0, 1, // 23

		//front laci 1
		-0.4, 0.25, 0.6, 0, 0,  // 24
		0.4, 0.25, 0.6, 1, 0,   // 25
		0.4,  0.75, 0.6, 1, 1,   // 26
		-0.4,  0.75, 0.6, 0, 1,  // 27

		// right laci 1
		0.4,  0.75,  0.6, 0, 0,  // 28
		0.4,  0.75, 0.5, 1, 0,  // 29
		0.4, 0.25, 0.5, 1, 1,  // 30
		0.4, 0.25,  0.6, 0, 1,  // 31

		// back laci 1
		-0.4, 0.25, 0.5, 0, 0, // 32
		0.4,  0.25, 0.5, 1, 0, // 33
		0.4,   0.75, 0.5, 1, 1, // 34
		-0.4,  0.75, 0.5, 0, 1, // 35

		// left laci 1
		-0.4, 0.25, 0.5, 0, 0, // 36
		-0.4, 0.25,  0.6, 1, 0, // 37
		-0.4,  0.75,  0.6, 1, 1, // 38
		-0.4,  0.75, 0.5, 0, 1, // 39

		// upper laci 1
		0.4, 0.75,  0.6, 0, 0,   // 40
		-0.4, 0.75,  0.6, 1, 0,  // 41
		-0.4, 0.75, 0.5, 1, 1,  // 42
		0.4, 0.75, 0.5, 0, 1,   // 43

		// bottom laci 1
		-0.4, 0.25, 0.5, 0, 0, // 44
		0.4, 0.25, 0.5, 1, 0,  // 45
		0.4, 0.25,  0.6, 1, 1,  // 46
		-0.4, 0.25,  0.6, 0, 1, // 47

		//front laci 2
		-0.4, -0.35, 0.6, 0, 0,  // 48
		0.4, -0.35, 0.6, 1, 0,   // 49
		0.4,  0.15, 0.6, 1, 1,   // 50
		-0.4,  0.15, 0.6, 0, 1,  // 51

		// right laci 2
		0.4,  0.15,  0.6, 0, 0,  // 52
		0.4,  0.15, 0.5, 1, 0,  // 53
		0.4, -0.35, 0.5, 1, 1,  // 54
		0.4, -0.35,  0.6, 0, 1,  // 55

		// back laci 2
		-0.4, -0.35, 0.5, 0, 0, // 56
		0.4,  -0.35, 0.5, 1, 0, // 57
		0.4,   0.15, 0.5, 1, 1, // 58
		-0.4,  0.15, 0.5, 0, 1, // 59

		// left laci 2
		-0.4, -0.35, 0.5, 0, 0, // 60
		-0.4, -0.35,  0.6, 1, 0, // 61
		-0.4,  0.15,  0.6, 1, 1, // 62
		-0.4,  0.15, 0.5, 0, 1, // 63

		// upper laci 2
		0.4, 0.15,  0.6, 0, 0,   // 64
		-0.4, 0.15,  0.6, 1, 0,  // 65
		-0.4, 0.15, 0.5, 1, 1,  // 66
		0.4, 0.15, 0.5, 0, 1,   // 67

		// bottom laci 2
		-0.4, -0.35, 0.5, 0, 0, // 68
		0.4, -0.35, 0.5, 1, 0,  // 69
		0.4, -0.35,  0.6, 1, 1,  // 70
		-0.4, -0.35,  0.6, 0, 1, // 71
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,   // bottom

		24,  25,  26,  24,  26,  27,   // front
		28,  29, 30,  28,  30,  31,   // right
		32,  33,  34, 32,  34, 35,  // back
		36, 37, 38, 36, 39, 38,  // left
		40, 42, 41, 40, 43, 42,  // upper
		44, 46, 45, 44, 47, 46,   // bottom

		48, 49, 50, 48, 50, 51,   // front
		52, 53, 54, 52, 54, 55,   // right
		56, 57, 58, 56, 58, 59,  // back
		60, 62, 61, 60, 63, 62,  // left
		64, 66, 65, 64, 67, 66,  // upper
		68, 70, 69, 68, 71, 70   // bottom
	};

	glGenVertexArrays(1, &VAOlemariKecil);
	glGenBuffers(1, &VBOlemariKecil);
	glGenBuffers(1, &EBOlemariKecil);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOlemariKecil);

	glBindBuffer(GL_ARRAY_BUFFER, VBOlemariKecil);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOlemariKecil);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawLemariKecil()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureLemariKecil);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAOlemariKecil); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	// set lighting materials attributes (Emerald)
	GLint ambientMatLoc = glGetUniformLocation(this->activeProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 0.0215f, 0.1745f, 0.0215f);
	GLint diffuseMatLoc = glGetUniformLocation(this->activeProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 0.07568f, 0.61424f, 0.07568f);
	GLint specularMatLoc = glGetUniformLocation(this->activeProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.633f, 0.727811f, 0.633f);
	GLint shininessMatLoc = glGetUniformLocation(this->activeProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 0.6f);

	glm::mat4 model;
	angle = 30;

	model = glm::translate(model, glm::vec3(3, 0, -4));
	model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(2, 1, 1.5));
	

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 108, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildLemariBesar() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureLemariBesar);
	glBindTexture(GL_TEXTURE_2D, textureLemariBesar);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("misba_wood.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
  // front
  -1.0, -6.0, 2, 0, 0,   	// 0             
   2.0, -6.0, 2, 1, 0,   	// 1
   2.0,  4.0, 2, 1, 1,   		// 2
  -1.0,  4.0, 2, 0, 1,   		// 3


   2.0,  4.0,  2, 0, 0,  	// 4
   2.0,  4.0, 0, 1, 0,	 	// 5
   2.0, -6.0, 0, 1, 1,   		// 6
   2.0, -6.0,  2, 0, 1,  	// 7


  -1.0, -6.0, 0, 0, 0,	 	// 8 
   2.0, -6.0, 0, 1, 0,   	// 9
   2.0,  4.0, 0, 1, 1,   	// 10
  -1.0,  4.0, 0, 0, 1,   	// 11


  -1.0, -6.0, 0, 0, 0,   	// 12
  -1.0, -6.0,  2, 1, 0,  	// 13
  -1.0,  4.0,  2, 1, 1,  	// 14
  -1.0,  4.0, 0, 0, 1,   	// 15


   2.0, 4.0,  2, 0, 0,   	 // 16
  -1.0, 4.0,  2, 1, 0,   	// 17
  -1.0, 4.0, 0, 1, 1,    	// 18
  2.0, 4.0, 0, 0, 1,     	// 19


  -1.0, -6.0, 0, 0, 0,   	// 20
  2.0, -6.0, 0, 1, 0,    	// 21
  2.0, -6.0,  2, 1, 1,   	// 22
  -1.0, -6.0,  2, 0, 1,  	// 23
	};



	unsigned int indices[] = {
	  0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAOLemariBesar);
	glGenBuffers(1, &VBOLemariBesar);
	glGenBuffers(1, &EBOLemariBesar);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOLemariBesar);

	glBindBuffer(GL_ARRAY_BUFFER, VBOLemariBesar);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOLemariBesar);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawLemariBesar()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureLemariBesar);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAOLemariBesar); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	// set lighting materials attributes (Emerald)
	GLint ambientMatLoc = glGetUniformLocation(this->activeProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 0.0215f, 0.1745f, 0.0215f);
	GLint diffuseMatLoc = glGetUniformLocation(this->activeProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 0.07568f, 0.61424f, 0.07568f);
	GLint specularMatLoc = glGetUniformLocation(this->activeProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.633f, 0.727811f, 0.633f);
	GLint shininessMatLoc = glGetUniformLocation(this->activeProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 0.6f);

	glm::mat4 model;
	angle = 30;

	model = glm::translate(model, glm::vec3(0, 2, 5));
	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(1, 0.4, 1));


	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 108, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildPintu2() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &textureKaca);
	glBindTexture(GL_TEXTURE_2D, textureKaca);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("kaca.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
  // front
  -1.0,	-6.0,	 0,		0,		0,		// 0
   0.5,	-6.0,	 0,		1,		0,		// 1
   0.5,	 4.0,	 0,		1,		1,		// 2
  -1.0,	 4.0,	 0,		0,		1,		// 3
   0.5,	 4.0,	 0,		0,		0,		// 4
   0.5,	 4.0,	 -0.1,	1,		0,		// 5
   0.5,	 -6.0,	 -0.1,	1,		1,		// 6
   0.5,	 -6.0,	 0,		0,		1,		// 7
  -1.0,	 -6.0,	 -0.1,	0,		0,		// 8 
   0.5,	 -6.0,	 -0.1,	1,		0,		// 9
   0.5,	  4.0,	 -0.1,	1,		1,		// 10
  -1.0,	  4.0,	 -0.1,	0,		1,		// 11
  -1.0,	 -6.0,	 -0.1,	0,		0,		// 12
  -1.0,	 -6.0,	 0,		1,		0,		// 13
  -1.0,	  4.0,	 0,		1,		1,		// 14
  -1.0,	  4.0,	 -0.1,	0,		1,		// 15
   0.5,	  4.0,	 0,		0,		0,		// 16
  -1.0,	  4.0,	 0,		1,		0,		// 17
  -1.0,	  4.0,	 -0.1,	1,		1,		// 18
  0.5,	  4.0,	 -0.1,	0,		1,		// 19
  -1.0,	 -6.0,	 -0.1,	0,		0,		// 20
  0.5,	 -6.0,	 -0.1,		1,		0,		// 21
  0.5,	 -6.0,	 0,		1,		1,		// 22
  -1.0,	 -6.0,	 0,		0,		1,		// 23
	};


	unsigned int indices[] = {
	  0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAOKaca);
	glGenBuffers(1, &VBOKaca);
	glGenBuffers(1, &EBOKaca);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOKaca);

	glBindBuffer(GL_ARRAY_BUFFER, VBOKaca);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOKaca);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawPintu2()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureKaca);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureKaca);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);
	glUseProgram(shaderProgram);



	glBindVertexArray(VAOKaca); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	angle = 30;

	model = glm::translate(model, glm::vec3(0, 2, 5));
	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(1, 0.4, 1));


	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 108, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildPintu() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texturepintu);
	glBindTexture(GL_TEXTURE_2D, texturepintu);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("kaca.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
  // front
  0.5,	-6.0,	 0,		0,		0,		// 0
   2,	-6.0,	 0,		1,		0,		// 1
   2,	 4.0,	 0,		1,		1,		// 2
  0.5,	 4.0,	 0,		0,		1,		// 3
   2,	 4.0,	 0,		0,		0,		// 4
   2,	 4.0,	 -0.1,	1,		0,		// 5
   2,	 -6.0,	 -0.1,	1,		1,		// 6
   2,	 -6.0,	 0,		0,		1,		// 7
  0.5,	 -6.0,	 -0.1,	0,		0,		// 8 
   2,	 -6.0,	 -0.1,	1,		0,		// 9
   2,	  4.0,	 -0.1,	1,		1,		// 10
  0.5,	  4.0,	 -0.1,	0,		1,		// 11
  0.5,	 -6.0,	 -0.1,	0,		0,		// 12
  0.5,	 -6.0,	 0,		1,		0,		// 13
  0.5,	  4.0,	 0,		1,		1,		// 14
  0.5,	  4.0,	 -0.1,	0,		1,		// 15
   2,	  4.0,	 0,		0,		0,		// 16
  0.5,	  4.0,	 0,		1,		0,		// 17
  0.5,	  4.0,	 -0.1,	1,		1,		// 18
  2,	  4.0,	 -0.1,	0,		1,		// 19
  0.5,	 -6.0,	 -0.1,	0,		0,		// 20
  2,	 -6.0,	 -0.1,		1,		0,		// 21
  2,	 -6.0,	 0,		1,		1,		// 22
  0.5,	 -6.0,	 0,		0,		1,		// 23
	};



	unsigned int indices[] = {
	  0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAOPintu);
	glGenBuffers(1, &VBOPintu);
	glGenBuffers(1, &EBOPintu);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAOPintu);

	glBindBuffer(GL_ARRAY_BUFFER, VBOPintu);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOPintu);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawPintu()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturepintu);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texturepintu);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);

	GLint shininessMatLoc = glGetUniformLocation(shaderProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);
	glUseProgram(shaderProgram);



	glBindVertexArray(VAOPintu); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glm::mat4 model;
	angle = 30;

	model = glm::translate(model, glm::vec3(0, 2, 5));
	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(1, 0.4, 1));


	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 108, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

//lantai
void Demo::BuildPlaneLantai()
{
	// Load and create a texture 
	glGenTextures(1, &textureLantai);
	glBindTexture(GL_TEXTURE_2D, textureLantai);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("floor_wood.JPG", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-8.0, -0.5, -8.0,  0,  0,
		 8.0, -0.5, -8.0, 8,  0,
		 8.0, -0.5,  8.0, 8, 8,
		-8.0, -0.5,  8.0,  0, 8,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAOlantai);
	glGenBuffers(1, &VBOlantai);
	glGenBuffers(1, &EBOlantai);

	glBindVertexArray(VAOlantai);

	glBindBuffer(GL_ARRAY_BUFFER, VBOlantai);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOlantai);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawPlaneLantai()
{
	glUseProgram(shaderProgram);
	// set lighting materials attributes (yellow plastic)
	GLint ambientMatLoc = glGetUniformLocation(this->activeProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 0.0f, 0.0f, 0.0f);
	GLint diffuseMatLoc = glGetUniformLocation(this->activeProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 0.5f, 0.5f, 0.0f);
	GLint specularMatLoc = glGetUniformLocation(this->activeProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.60f, 0.60f, 0.50f);
	GLint shininessMatLoc = glGetUniformLocation(this->activeProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureLantai);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAOlantai); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

//tembok
void Demo::BuildPlaneTembok1()
{
	// Load and create a texture 
	glGenTextures(1, &textureTembok);
	glBindTexture(GL_TEXTURE_2D, textureTembok);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("white_wall.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		0, 4, -8,  0,  0,
		-0, -0.5, -8, 8,  0,
		-0, -0.5,  8, 8, 8,
		0, 4,  8,  0, 8, 

	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAOtembok1);
	glGenBuffers(1, &VBOtembok1);
	glGenBuffers(1, &EBOtembok1);

	glBindVertexArray(VAOtembok1);

	glBindBuffer(GL_ARRAY_BUFFER, VBOtembok1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOtembok1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}
void Demo::DrawPlaneTembok1()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTembok);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAOtembok2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	// set lighting materials attributes (yellow plastic)
	GLint ambientMatLoc = glGetUniformLocation(this->activeProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 0.0f, 0.0f, 0.0f);
	GLint diffuseMatLoc = glGetUniformLocation(this->activeProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 0.5f, 0.5f, 0.0f);
	GLint specularMatLoc = glGetUniformLocation(this->activeProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.60f, 0.60f, 0.50f);
	GLint shininessMatLoc = glGetUniformLocation(this->activeProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, -8));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildPlaneTembok2()
{
	// Load and create a texture 
	glGenTextures(1, &textureTembok);
	glBindTexture(GL_TEXTURE_2D, textureTembok);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("white_wall.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		8, 4, 0,  0,  0,
		-8, 4, 0, 8,  0,
		-8, -0.5,  0, 8, 8,
		8, -0.5,  0,  0, 8,

	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAOtembok2);
	glGenBuffers(1, &VBOtembok2);
	glGenBuffers(1, &EBOtembok2);

	glBindVertexArray(VAOtembok2);

	glBindBuffer(GL_ARRAY_BUFFER, VBOtembok2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOtembok2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawPlaneTembok2()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTembok);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAOtembok1); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	// set lighting materials attributes (yellow plastic)
	GLint ambientMatLoc = glGetUniformLocation(this->activeProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 0.0f, 0.0f, 0.0f);
	GLint diffuseMatLoc = glGetUniformLocation(this->activeProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 0.5f, 0.5f, 0.0f);
	GLint specularMatLoc = glGetUniformLocation(this->activeProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.60f, 0.60f, 0.50f);
	GLint shininessMatLoc = glGetUniformLocation(this->activeProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(8, 0, 0));
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawPlaneTembok3()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTembok);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAOtembok2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	// set lighting materials attributes (yellow plastic)
	GLint ambientMatLoc = glGetUniformLocation(this->activeProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 0.0f, 0.0f, 0.0f);
	GLint diffuseMatLoc = glGetUniformLocation(this->activeProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 0.5f, 0.5f, 0.0f);
	GLint specularMatLoc = glGetUniformLocation(this->activeProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.60f, 0.60f, 0.50f);
	GLint shininessMatLoc = glGetUniformLocation(this->activeProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 8));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::DrawPlaneTembok4()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTembok);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAOtembok1); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	// set lighting materials attributes (yellow plastic)
	GLint ambientMatLoc = glGetUniformLocation(this->activeProgram, "material.ambient");
	glUniform3f(ambientMatLoc, 0.0f, 0.0f, 0.0f);
	GLint diffuseMatLoc = glGetUniformLocation(this->activeProgram, "material.diffuse");
	glUniform3f(diffuseMatLoc, 0.5f, 0.5f, 0.0f);
	GLint specularMatLoc = glGetUniformLocation(this->activeProgram, "material.specular");
	glUniform3f(specularMatLoc, 0.60f, 0.60f, 0.50f);
	GLint shininessMatLoc = glGetUniformLocation(this->activeProgram, "material.shininess");
	glUniform1f(shininessMatLoc, 10.0f);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-8, 0, 0));

	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

//camera
void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 1.0f;
	posCamZ = 8.0f;
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.001f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}


int main(int argc, char** argv) {
	RenderEngine& app = Demo();
	app.Start("Ruang Tidur 3D", 800, 600, false, false);
}
