#include "utilities.h"
#include "SimpleModel.h"
#include "Texture.h"
#include "Camera.h"
#include "Orbit.h"
#include "ModelType.h"
#include "TextureType.h"
#include <vector>
#include <string>

// global variables
// settings
unsigned int gWindowWidth = 2180;
unsigned int gWindowHeight = 1440;

// frame stats
float gFrameRate = 60.0f;
float gFrameTime = 1 / gFrameRate;
float gRotationSpeed = 30.0f; // degrees per second
float gScale = 0.5f; // scale of object1
float gScale2 = 0.5f; // scale of object2
float gScale3 = 0.5f; // scale of object3
// texture objects
Texture gDiffuse1, gDiffuse2, gDiffuse3;
Texture gSpecular;



ModelType currentModel1 = ModelType::Cube;
ModelType currentModel2 = ModelType::Sniper;
ModelType currentModel3 = ModelType::Cube;

// current texture choices
TextureType currentTex1 = TextureType::Wood;
TextureType currentTex2 = TextureType::Comic;
TextureType currentTex3 = TextureType::Abstract;

// current cammera modea
Camera gCamera(1); 


// scene content
ShaderProgram gShader;	// shader program object
ShaderProgram gOrbitShader;

glm::mat4 gViewMatrix;			// view matrix
glm::mat4 gProjectionMatrix;	// projection matrix
glm::mat4 gModelMatrix;			// object matrix
glm::mat4 gModelMatrix2;		// object2 matrix
glm::mat4 gModelMatrix3;		// object3 matrix

Light gLight;					// light properties
SimpleModel gModel;				// object model1
SimpleModel gModel2;			// object model2
SimpleModel gModel3;			// object model3
std::map<std::string, Texture> gTextures;	// texture object

Orbit gOrbit(5.0f, 60.0f, gScale2);
Orbit gOrbit2(2.0f, 120.0f, gScale3);

// controls
bool gWireframe = false;	// wireframe control
float gShininess = 5.0f;
float gRotationY = 0.0f;



// function initialise scene and render settings
static void init(GLFWwindow* window)
{
	// set the color the color buffer should be cleared to
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);	// enable depth buffer test

	// compile and link a vertex and fragment shader pair
	gShader.compileAndLink("lightingAndTexture.vert", "diffuseSpecularMaps.frag");
	gOrbitShader.compileAndLink("orbit.vert", "orbit.frag");

	// initialise view matrix
	gViewMatrix = glm::lookAt(glm::vec3(0.0f, 2.0f, 4.0f), 
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// initialise projection matrix
	gProjectionMatrix = glm::perspective(glm::radians(45.0f), 
		static_cast<float>(gWindowWidth) / gWindowHeight, 1.3f, 170.0f);

	// initialise model matrices
	gModelMatrix = glm::mat4(1.0f);

	// initialise point light properties
	gLight.pos = glm::vec3(2.0f, 2.0f, 3.0f);
	gLight.La = glm::vec3(0.2f);
	gLight.Ld = glm::vec3(1.0f);
	gLight.Ls = glm::vec3(1.0f);
	gLight.att = glm::vec3(1.0f, 0.0f, 0.0f);

	// Diffuse textures
	gDiffuse1.generate(getTextureFile(TextureType::Wood));
	gDiffuse2.generate(getTextureFile(TextureType::Metal));
	gDiffuse3.generate(getTextureFile(TextureType::Wood));

	// Specular textures (can be shared if same image)
	gSpecular.generate("./images/specular.bmp");



	// load model
	gModel.loadModel(currentModel1, true);
	gModel2.loadModel(currentModel2, true);
	gModel3.loadModel(currentModel3, true);

}

// function used to update the scene
static void update_scene(GLFWwindow* window)
{
	static ModelType lastModel1 = currentModel1;
	static ModelType lastModel2 = currentModel2;
	static ModelType lastModel3 = currentModel3;
	static TextureType lastTex1 = currentTex1;
	static TextureType lastTex2 = currentTex2;
	static TextureType lastTex3 = currentTex3;


	gViewMatrix = gCamera.getViewMatrix();
	gRotationY += gRotationSpeed * gFrameTime; // auto-rotation

	// Reload model1 if changed
	if (currentModel1 != lastModel1) {
		gModel.loadModel(currentModel1, true);
		lastModel1 = currentModel1;
	}

	// Reload model2 if changed
	if (currentModel2 != lastModel2) {
		gModel2.loadModel(currentModel2, true);
		lastModel2 = currentModel2;
	}

	// Reload model3 if changed
	if (currentModel3 != lastModel3) {
		gModel3.loadModel(currentModel3, true);
		lastModel3 = currentModel3;
	}

	




	// object 1
	glm::mat4 rotation1 = glm::rotate(glm::radians(gRotationY), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 scaling1 = glm::scale(glm::vec3(gScale, gScale, gScale));

	gModelMatrix = rotation1 * scaling1; // combine transforms
	gOrbit.update(gFrameTime);


	//object 2
	glm::mat4 rotation2 = glm::rotate(glm::radians(gRotationY), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 scaling2 = glm::scale(glm::vec3(gScale2, gScale2, gScale2));
	glm::mat4 translation2 = glm::translate(glm::vec3(2.0f, 0.0f, -1.5f)); // new position
	gModelMatrix2 = translation2 * rotation2 * scaling2;  // new model matrix for second object
	glm::mat4 orbit1Matrix = gOrbit.getModelMatrix(gModelMatrix);
	gModelMatrix2 = orbit1Matrix * glm::scale(glm::vec3(gScale2));
	gOrbit2.update(gFrameTime);

	//object 3
	glm::mat4 rotation3 = glm::rotate(glm::radians(gRotationY), glm::vec3(0.0f, 3.0f, 0.0f));
	glm::mat4 scaling3 = glm::scale(glm::vec3(gScale3, gScale3, gScale3));
	glm::mat4 translation3 = glm::translate(glm::vec3(3.0f, 0.0f, -3.5f)); // new position
	gModelMatrix3 = translation3 * rotation3 * scaling3;  // new model matrix for third object
	glm::mat4 orbit2Matrix = gOrbit2.getModelMatrix(gModelMatrix2);
	gModelMatrix3 = orbit2Matrix * glm::scale(glm::vec3(gScale3));
	
}

// function to render the scene
static void render_scene()
{
	// clear colour buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	gOrbit.drawOrbitPath(gViewMatrix, gProjectionMatrix, gModelMatrix, gOrbitShader);
	gOrbit2.drawOrbitPath(gViewMatrix, gProjectionMatrix, gModelMatrix2, gOrbitShader);

	// now bind main shader for objects
	gShader.use();


	// set light properties
	gShader.setUniform("uLight.pos", gLight.pos);
	gShader.setUniform("uLight.La", gLight.La);
	gShader.setUniform("uLight.Ld", gLight.Ld);
	gShader.setUniform("uLight.Ls", gLight.Ls);
	gShader.setUniform("uLight.att", gLight.att);

	// set viewing position
	gShader.setUniform("uViewpoint", glm::vec3(0.0f, 2.0f, 4.0f));

	// calculate matrices
	glm::mat4 MVP = gProjectionMatrix * gViewMatrix * gModelMatrix;
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(gModelMatrix)));

	// calculate matricies 2
	glm::mat4 MVP2 = gProjectionMatrix * gViewMatrix * gModelMatrix2;
	glm::mat3 normalMatrix2 = glm::mat3(glm::transpose(glm::inverse(gModelMatrix2)));

	// calculate matricies 3
	glm::mat4 MVP3 = gProjectionMatrix * gViewMatrix * gModelMatrix3;
	glm::mat3 normalMatrix3 = glm::mat3(glm::transpose(glm::inverse(gModelMatrix3)));

	// set uniform variables object 1
	gShader.setUniform("uModelViewProjectionMatrix", MVP);
	gShader.setUniform("uModelMatrix", gModelMatrix);
	gShader.setUniform("uNormalMatrix", normalMatrix);

	// set textures 1
	gShader.setUniform("uDiffuseMap", 0);
	gShader.setUniform("uSpecularMap", 1);
	gShader.setUniform("uShininess", gShininess);

	glActiveTexture(GL_TEXTURE0);
	switch (currentTex1)
	{
	case TextureType::Wood:     gDiffuse1.bind(); break;
	case TextureType::Comic:    gDiffuse2.bind(); break;
	case TextureType::Abstract: gDiffuse3.bind(); break;
	}

	glActiveTexture(GL_TEXTURE1);
	gSpecular.bind();




	// draw model1
	gModel.drawModel();

	// set uniform variables object 2
	gShader.setUniform("uModelViewProjectionMatrix", MVP2);
	gShader.setUniform("uModelMatrix", gModelMatrix2);
	gShader.setUniform("uNormalMatrix", normalMatrix2);

	// set textures 2
	gShader.setUniform("uDiffuseMap", 0);
	gShader.setUniform("uSpecularMap", 1);
	gShader.setUniform("uShininess", gShininess);


	// Object 2
	glActiveTexture(GL_TEXTURE0);
	switch (currentTex2)
	{
	case TextureType::Wood:     gDiffuse1.bind(); break;
	case TextureType::Comic:    gDiffuse2.bind(); break;
	case TextureType::Abstract: gDiffuse3.bind(); break;
	}
	glActiveTexture(GL_TEXTURE1);
	gSpecular.bind();



	


	// draw model2
	gModel2.drawModel();

	// set uniform variables object 2
	gShader.setUniform("uModelViewProjectionMatrix", MVP3);
	gShader.setUniform("uModelMatrix", gModelMatrix3);
	gShader.setUniform("uNormalMatrix", normalMatrix3);

	// set textures 3
	gShader.setUniform("uDiffuseMap", 0);
	gShader.setUniform("uSpecularMap", 1);
	gShader.setUniform("uShininess", gShininess);

	// Object 3
	glActiveTexture(GL_TEXTURE0);
	switch (currentTex3)
	{
	case TextureType::Wood:     gDiffuse1.bind(); break;
	case TextureType::Comic:    gDiffuse2.bind(); break;
	case TextureType::Abstract: gDiffuse3.bind(); break;
	}
	glActiveTexture(GL_TEXTURE1);
	gSpecular.bind();

	// draw model3
	gModel3.drawModel();



	// flush the graphics pipeline
	glFlush();
}

// key press or release callback function
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// close the window when the ESCAPE key is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		// set flag to close the window
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
	
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) gCamera.setMode(1);
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) gCamera.setMode(2);
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) gCamera.setMode(3);
}

// mouse movement callback function
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// pass cursor position to tweak bar
	TwEventMousePosGLFW(static_cast<int>(xpos), static_cast<int>(ypos));
}

// mouse button callback function
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// pass mouse button status to tweak bar
	TwEventMouseButtonGLFW(button, action);
}

// error callback function
static void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;	// output error description
}

// create and populate tweak bar elements
TwBar* create_UI(const std::string name)
{
	// create a tweak bar
	TwBar* twBar = TwNewBar(name.c_str());

	// AntTweakBar enum mapping
	TwEnumVal modelTypeValues[] = 
	{
		{ static_cast<int>(ModelType::Cube), "Cube" },
		{ static_cast<int>(ModelType::Sniper), "Sniper" },
	};
	TwType TW_MODELTYPES = TwDefineEnum("ModelChoice", modelTypeValues, 2);

	TwEnumVal texTypeValues[] =
	{
		{ static_cast<int>(TextureType::Wood), "Wood" },
		{ static_cast<int>(TextureType::Abstract), "Abstract" },
		{ static_cast<int>(TextureType::Comic), "Comic" },
	};
	TwType TW_TEXTURETYPES = TwDefineEnum("TextureChoice", texTypeValues, 3);


	// give tweak bar the size of graphics window
	TwWindowSize(gWindowWidth, gWindowHeight);
	TwDefine(" TW_HELP visible=false ");	// disable help menu
	TwDefine(" GLOBAL fontsize=3 ");		// set large font size

	TwDefine(" Main label='User Interface' refresh=0.02 text=light size='250 720' position='10 10' ");

	// create frame stat entries
	TwAddVarRO(twBar, "Frame Rate", TW_TYPE_FLOAT, &gFrameRate, " group='Frame Stats' precision=2 ");
	TwAddVarRO(twBar, "Frame Time", TW_TYPE_FLOAT, &gFrameTime, " group='Frame Stats' ");

	// scene controls
	TwAddVarRW(twBar, "Wireframe", TW_TYPE_BOOLCPP, &gWireframe, " group='Controls' ");

	// light controls
	TwAddVarRW(twBar, "Position X", TW_TYPE_FLOAT, &gLight.pos.x, " group='Light' min=-3.0 max=3.0 step=0.01 ");
	TwAddVarRW(twBar, "Position Y", TW_TYPE_FLOAT, &gLight.pos.y, " group='Light' min=-3.0 max=3.0 step=0.01 ");
	TwAddVarRW(twBar, "Position Z", TW_TYPE_FLOAT, &gLight.pos.z, " group='Light' min=-3.0 max=3.0 step=0.01 ");

	// material shininess
	TwAddVarRW(twBar, "Shininess", TW_TYPE_FLOAT, &gShininess, " group='Material' min=1.0 max=20.0 step=1 ");

	// rotation
	TwAddVarRW(twBar, "RotationY", TW_TYPE_FLOAT, &gRotationY, " group='Transformation' min=-180 max=180 step=1 ");
	// size
	TwAddVarRW(twBar, "Scale", TW_TYPE_FLOAT, &gScale," group='Transformation' min=0.1 max=2.0 step=0.1 ");
	TwAddVarRW(twBar, "Model 1", TW_MODELTYPES, reinterpret_cast<int*>(&currentModel1), " group='Transformation' ");
	TwAddVarRW(twBar, "Texture 1", TW_TEXTURETYPES, reinterpret_cast<int*>(&currentTex1), " group='Textures' ");
	
	// size
	TwAddVarRW(twBar, "Scale2", TW_TYPE_FLOAT, &gScale2, " group='Transformation' min=0.1 max=2.0 step=0.1 ");
	TwAddVarRW(twBar, "Model 2", TW_MODELTYPES, reinterpret_cast<int*>(&currentModel2), " group='Transformation' ");
	TwAddVarRW(twBar, "Texture 2", TW_TEXTURETYPES, reinterpret_cast<int*>(&currentTex2), " group='Textures' ");
	
	// size
	TwAddVarRW(twBar, "Scale3", TW_TYPE_FLOAT, &gScale3, " group='Transformation' min=0.1 max=2.0 step=0.1 ");
	TwAddVarRW(twBar, "Model 3", TW_MODELTYPES, reinterpret_cast<int*>(&currentModel3), " group='Transformation' ");
	TwAddVarRW(twBar, "Texture 3", TW_TEXTURETYPES, reinterpret_cast<int*>(&currentTex3), " group='Textures' ");

	return twBar;
}

int main(void)
{
	GLFWwindow* window = nullptr;	// GLFW window handle

	glfwSetErrorCallback(error_callback);	// set GLFW error callback function

	// initialise GLFW
	if (!glfwInit())
	{
		// if failed to initialise GLFW
		exit(EXIT_FAILURE);
	}

	// minimum OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);

	// create a window and its OpenGL context
	window = glfwCreateWindow(gWindowWidth, gWindowHeight, "Tutorial", nullptr, nullptr);

	// check if window created successfully
	if (window == nullptr)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);	// set window context as the current context
	glfwSwapInterval(1);			// swap buffer interval

	// initialise GLEW
	if (glewInit() != GLEW_OK)
	{
		// if failed to initialise GLEW
		std::cerr << "GLEW initialisation failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	gOrbit.init();
	gOrbit2.init();

	// set GLFW callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// initialise scene and render settings
	init(window);

	// initialise AntTweakBar
	TwInit(TW_OPENGL_CORE, nullptr);
	TwBar* tweakBar = create_UI("Main");		// create and populate tweak bar elements

	// timing data
	double lastUpdateTime = glfwGetTime();	// last update time
	double elapsedTime = lastUpdateTime;	// time since last update
	int frameCount = 0;						// number of frames since last update

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		update_scene(window);	// update the scene

		// if wireframe set polygon render mode to wireframe
		if (gWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		render_scene();			// render the scene

		// set polygon render mode to fill
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		TwDraw();				// draw tweak bar

		glfwSwapBuffers(window);	// swap buffers
		glfwPollEvents();			// poll for events

		frameCount++;
		elapsedTime = glfwGetTime() - lastUpdateTime;	// time since last update

		// if elapsed time since last update > 1 second
		if (elapsedTime > 1.0)
		{
			gFrameTime = elapsedTime / frameCount;	// average time per frame
			gFrameRate = 1 / gFrameTime;			// frames per second
			lastUpdateTime = glfwGetTime();			// set last update time to current time
			frameCount = 0;							// reset frame counter
		}
	}

	// uninitialise tweak bar
	TwDeleteBar(tweakBar);
	TwTerminate();

	// close the window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}