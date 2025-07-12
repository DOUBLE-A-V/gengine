#include "Render.h"

uint Render::VBO, Render::VAO, Render::EBO;
GLFWwindow* Render::window;

Shader* Render::ourShader;
uint Render::texture;

float Render::vertices[] = {
	// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};
uint Render::indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
};

vector<Render::Sprite*> Render::sprites;

int Render::windowWidth, Render::windowHeight;

float lastTime = 0.0f;

int Render::init(GLFWwindow* renderWindow) {
	Render::window = renderWindow;

	glGenVertexArrays(1, &Render::VAO);
	glGenBuffers(1, &Render::VBO);
	glGenBuffers(1, &Render::EBO);

	glBindVertexArray(Render::VAO);

	glBindBuffer(GL_ARRAY_BUFFER, Render::VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Render::EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Render::indices), Render::indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD); // this is default
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Render::ourShader = new Shader("vertexShader.vs", "fragmentShader.fs");

	glfwSwapInterval(1);

	return 0;
}

void Render::terminate() {
	for (Render::Sprite* sprite : Render::sprites) {
		delete& (sprite->texture->vertices);
		delete sprite->texture;
		delete sprite;
	}
	delete &Render::sprites;
	glDeleteVertexArrays(1, &Render::VAO);
	glDeleteBuffers(1, &Render::VBO);
	glDeleteBuffers(1, &Render::EBO);
	glfwTerminate();
}
void Render::renderFrame() {
	glClearColor(0, 0.5f, 1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	(*Render::ourShader).use();
	for (Sprite* sprite : Render::sprites) {
		sprite->checkChanges();

		(*Render::ourShader).setFloat("alpha", sprite->texture->alpha);
		(*Render::ourShader).setFloat("red", sprite->texture->red);
		(*Render::ourShader).setFloat("green", sprite->texture->green);
		(*Render::ourShader).setFloat("blue", sprite->texture->blue);
		glBindBuffer(GL_ARRAY_BUFFER, Render::VBO);
		glBufferData(GL_ARRAY_BUFFER, sprite->texture->verticesSize, sprite->texture->vertices.data(), GL_STATIC_DRAW);

		glBindTexture(GL_TEXTURE_2D, sprite->texture->texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	float currentTime = glfwGetTime();
	float deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	//std::cout << "FPS: " << (1 / deltaTime) << "/" << deltaTime << std::endl;

	glfwSwapBuffers(Render::window);
	glfwPollEvents();
}
Render::Texture* Render::loadTexture(const char* path) {
	uint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	float calcPosx = (float)0 / windowWidth / 2; // posx instead of zero
	float calcPosy = (float)0 / windowHeight / 2;
	float calcWidth = ((float)width / 2) / (windowWidth / 2);
	float calcHeight = ((float)height / 2) / (windowHeight / 2);

	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	vertices[0] = calcWidth + calcPosx;
	vertices[1] = calcHeight + calcPosy;
	vertices[8] = calcWidth + calcPosx;
	vertices[9] = -calcHeight + calcPosy;
	vertices[16] = -calcWidth + calcPosx;
	vertices[17] = -calcHeight + calcPosy;
	vertices[24] = -calcWidth + calcPosx;
	vertices[25] = calcHeight + calcPosy;
	Texture* textureObject = new Texture(texture, vector<float>(vertices, vertices + sizeof(vertices) / sizeof(vertices[0])), width, height, sizeof(vertices));
	return textureObject;
}
Render::Texture::Texture(uint id, vector<float> vertices, int width, int height, size_t verticesSize) {
	this->texture = id;
	this->vertices = vertices;
	this->width = width;
	this->height = height;
	this->posx = posx;
	this->posy = posy;
	this->verticesSize = verticesSize;

	this->calcPosx = (float)posx / (windowWidth / 2);
	this->calcPosy = (float)posy / (windowHeight / 2);
	this->calcWidth = ((float)width / 2) / (windowWidth / 2);
	this->calcHeight = ((float)height / 2) / (windowHeight / 2);
	this->alpha = 1;
	this->red = 1;
	this->green = 1;
	this->blue = 1;
}
void Render::Texture::setFilter(int filter) {
	glBindTexture(GL_TEXTURE_2D, this->texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
}
void Render::Texture::resize(int newWidth, int newHeight) {

	this->width = newWidth;
	this->height = newHeight;

	this->calcWidth = ((float)newWidth / 2) / (windowWidth / 2);
	this->calcHeight = ((float)newHeight / 2) / (windowHeight / 2);


	this->vertices[0] = this->calcWidth + this->calcPosx;
	this->vertices[1] = this->calcHeight + this->calcPosy;
	this->vertices[8] = this->calcWidth + this->calcPosx;
	this->vertices[9] = -this->calcHeight + this->calcPosy;
	this->vertices[16] = -this->calcWidth + this->calcPosx;
	this->vertices[17] = -this->calcHeight + this->calcPosy;
	this->vertices[24] = -this->calcWidth + this->calcPosx;
	this->vertices[25] = this->calcHeight + this->calcPosy;
}

void Render::Texture::repos(int newx, int newy) {
	this->posx = newx;
	this->posy = newy;

	this->calcPosx = (float)newx / (windowWidth / 2);
	this->calcPosy = (float)newy / (windowHeight / 2);


	this->vertices[0] = this->calcWidth + this->calcPosx;
	this->vertices[1] = this->calcHeight + this->calcPosy;
	this->vertices[8] = this->calcWidth + this->calcPosx;
	this->vertices[9] = -this->calcHeight + this->calcPosy;
	this->vertices[16] = -this->calcWidth + this->calcPosx;
	this->vertices[17] = -this->calcHeight + this->calcPosy;
	this->vertices[24] = -this->calcWidth + this->calcPosx;
	this->vertices[25] = this->calcHeight + this->calcPosy;
}

Render::Vector2::Vector2(int x, int y) {
	this->x = (float)x;
	this->y = (float)y;
}
Render::Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}

void Render::Sprite::resize(int newWidth, int newHeight) {
	if (newWidth != this->size.x || newHeight != this->size.y) {
		this->texture->resize(newWidth, newHeight);
	}
	this->size.x = newWidth;
	this->size.y = newHeight;
}
Render::Vector2 Render::Sprite::getSize() {
	return this->size;
}
void Render::Sprite::checkChanges() {
	if (this->position.x != this->oldPosition.x || this->position.y != this->oldPosition.y) {
		this->texture->repos(this->position.x, this->position.y);
		this->oldPosition.x = this->position.x;
		this->oldPosition.y = this->position.y;
	}
}
Render::Vector2::Vector2() {
	this->x = 0;
	this->y = 0;
}