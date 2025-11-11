// Librerías estándar
#include <iostream>
#include <cmath>

// Librerías de OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Librería para cargar imágenes (texturas)
#include "stb_image.h"

// Librerías matemáticas de GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Librería adicional para cargar texturas
#include "SOIL2/SOIL2.h"

// Archivos propios del proyecto
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Declaración de prototipos de funciones
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Dimensiones de la ventana
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Definición y configuración de la cámara
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Atributos de la luz
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Posiciones de luces puntuales
glm::vec3 pointLightPositions[] = {
	// Luces para Acuario
	glm::vec3(4.17f,  2.60f,  1.3f),
	glm::vec3(9.43f,  2.60f,  1.3f),
	glm::vec3(4.17f,  2.60f, 10.13f),
	glm::vec3(9.43f,  2.60f, 10.13f),

	// Luces para Polar
	glm::vec3(-2.75f,  2.2f,  1.52f),
	glm::vec3(-8.2f,   2.2f,  1.52f),
	glm::vec3(-2.81f,  2.2f,  9.96f),
	glm::vec3(-8.25f,  2.2f,  9.96f),

	// Luces para Sabana
	glm::vec3(3.25f,  2.2f,  -1.4f),
	glm::vec3(9.4f,   2.2f,  -1.4f),
	glm::vec3(3.25f,  2.2f,  -9.05f),
	glm::vec3(9.5f,   2.2f,  -9.05f),

	// Luces para Selva
	glm::vec3(-3.5f,  2.6f,  -2.15f),
	glm::vec3(-7.6f,  2.6f,  -1.6f),
	glm::vec3(-3.45f, 2.6f,  -8.47f),
	glm::vec3(-8.3f,  2.6f,  -8.5f),

	// Luces para Parque
	glm::vec3(9.48f,  2.2f,  -13.0f),
	glm::vec3(-8.97f, 2.2f,  -13.05f),
	glm::vec3(4.0f,   2.2f,  -17.0f),
	glm::vec3(-2.14f, 2.2f,  -13.05f),
	glm::vec3(2.14f,  2.2f,  -13.05f),
	glm::vec3(-4.0f,  2.2f,  -17.f),
};

// Ángulo inicial de rotación
float angle = glm::radians(45.0f);

// Datos de vértices para un cubo
float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 Light1 = glm::vec3(0);
//Anim
float rotBall = 0.0f;
float rotDog = 0.0f;
int dogAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;



//KeyFrames
float dogPosX , dogPosY , dogPosZ  ;

#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame {
	
	float rotDog;
	float rotDogInc;
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;

	float head;
	float headInc;

	float Flegs;
	float FlegsInc;

	float Rlegs;
	float RlegsInc;

	float tail;
	float tailInc;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)  //Guarda la posición donde está el modelo
{

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].dogPosX = dogPosX;
	KeyFrame[FrameIndex].dogPosY = dogPosY;
	KeyFrame[FrameIndex].dogPosZ = dogPosZ;

	KeyFrame[FrameIndex].rotDog = rotDog;

	KeyFrame[FrameIndex].head = head;

	KeyFrame[FrameIndex].Flegs = FLegs;
	KeyFrame[FrameIndex].Rlegs = RLegs;
	KeyFrame[FrameIndex].tail = tail;

	FrameIndex++;
}

void resetElements(void)  // Regresa a la primera foto
{
	dogPosX = KeyFrame[0].dogPosX;
	dogPosY = KeyFrame[0].dogPosY;
	dogPosZ = KeyFrame[0].dogPosZ;

	rotDog = KeyFrame[0].rotDog;

	head = KeyFrame[0].head; 

	FLegs = KeyFrame[0].Flegs;
	RLegs = KeyFrame[0].Rlegs;
	tail = KeyFrame[0].tail;

}
void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;

	KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;

	KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;

	KeyFrame[playIndex].FlegsInc = (KeyFrame[playIndex + 1].Flegs - KeyFrame[playIndex].Flegs) / i_max_steps;
	KeyFrame[playIndex].RlegsInc = (KeyFrame[playIndex + 1].Rlegs - KeyFrame[playIndex].Rlegs) / i_max_steps;
	KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;
}

//Variables para el control del tiempo entre fotogramas
// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Inicializa la librería GLFW (necesaria para crear la ventana y el contexto OpenGL)
	glfwInit();

	// Crea una ventana GLFW de tamaño 800x600 y título "Proyecto Final Zoologico"
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final Zoologico", nullptr, nullptr);

	// Verifica si la ventana se creó correctamente
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	// Asocia la ventana al contexto actual de OpenGL
	glfwMakeContextCurrent(window);

	// Obtiene las dimensiones reales del framebuffer
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Configura las funciones de callback
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// Indica a GLEW que use un enfoque moderno para obtener las funciones de OpenGL
	glewExperimental = GL_TRUE;

	// Inicializa GLEW para poder usar las funciones de OpenGL
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define el tamaño del área de dibujo (viewport) dentro de la ventana
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Carga y compila los programas de shaders
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");


	//models
	Model DogBody((char*)"Models/DogBody.obj");
	Model HeadDog((char*)"Models/HeadDog.obj");
	Model DogTail((char*)"Models/TailDog.obj");
	Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Ball((char*)"Models/ball.obj");

	// Modelos de la fachada (entrada principal y estructura)
	Model AguaF((char*)"ModelosFachada/AguaF.obj");
	Model Arcos((char*)"ModelosFachada/Arcos.obj");
	Model Bancas((char*)"ModelosFachada/Bancas.obj");
	Model BarandalZ((char*)"ModelosFachada/BarandalZ.obj");
	Model Campana((char*)"ModelosFachada/Campana.obj");
	Model Faros((char*)"ModelosFachada/FarosF.obj");
	Model Fuente((char*)"ModelosFachada/Fuente.obj");
	Model Letras((char*)"ModelosFachada/Letras.obj");
	Model ParedF1((char*)"ModelosFachada/ParedF1.obj");
	Model ParedF2((char*)"ModelosFachada/ParedF2.obj");
	Model ParedFA1((char*)"ModelosFachada/ParedFA1.obj");
	Model ParedFA2((char*)"ModelosFachada/ParedFA2.obj");
	Model ParedFA3((char*)"ModelosFachada/ParedFA3.obj");
	Model ParedFA4((char*)"ModelosFachada/ParedFA4.obj");
	Model ParedFA5((char*)"ModelosFachada/ParedFA5.obj");
	Model ParedLD((char*)"ModelosFachada/ParedLD.obj");
	Model ParedLF((char*)"ModelosFachada/ParedLF.obj");
	Model ParedLF1((char*)"ModelosFachada/ParedLF1.obj");
	Model ParedLI((char*)"ModelosFachada/ParedLI.obj");
	Model ParedT((char*)"ModelosFachada/ParedT.obj");
	Model PisoZoo((char*)"ModelosFachada/Piso.obj");
	Model Pasto((char*)"ModelosFachada/PisoPasto.obj");
	Model PlataformaZ((char*)"ModelosFachada/PlataformaZ.obj");
	Model Reloj((char*)"ModelosFachada/Reloj.obj");
	Model Soportes((char*)"ModelosFachada/Soportes.obj");
	Model TechoF((char*)"ModelosFachada/TechoF.obj");
	Model TorreI((char*)"ModelosFachada/TorreI.obj");
	Model TorreS((char*)"ModelosFachada/TorreS.obj");
	Model Trabes((char*)"Modelos/Trabes.obj");
	Model VidrioF((char*)"ModelosFachada/VidrioF.obj");

	/// Modelos del área del Acuario
	Model AguaA((char*)"ModelosAcuario/AguaA.obj");
	Model Algas((char*)"ModelosAcuario/Algas.obj");
	Model ArenaA((char*)"ModelosAcuario/ArenaA.obj");
	Model CartelT((char*)"ModelosAcuario/CartelT.obj");
	Model Coral((char*)"ModelosAcuario/Coral.obj");
	Model Hongo1((char*)"ModelosAcuario/Hongo1.obj");
	Model Hongo2((char*)"ModelosAcuario/Hongo2.obj");
	Model Hongo3((char*)"ModelosAcuario/Hongo3.obj");
	Model Hongo4((char*)"ModelosAcuario/Hongo4.obj");
	Model Hongo5((char*)"ModelosAcuario/Hongo5.obj");
	Model Hongo6((char*)"ModelosAcuario/Hongo6.obj");
	Model ParedA((char*)"ModelosAcuario/ParedA.obj");
	Model RocasA((char*)"ModelosAcuario/RocasA.obj");
	Model VidrioA((char*)"ModelosAcuario/VidrioA.obj");
	Model VidrioAF((char*)"ModelosAcuario/VidrioAF.obj");

	// Modelos del hábitat Polar
	Model AguaP((char*)"ModelosPolar/AguaP.obj");
	Model BarandalP((char*)"ModelosPolar/BarandalP.obj");
	Model CartelP((char*)"ModelosPolar/CartelP.obj");
	Model EstanqueP((char*)"ModelosPolar/EstanqueP.obj");
	Model Nieve((char*)"ModelosPolar/Nieve.obj");
	Model PlataformaP((char*)"ModelosPolar/PlataformaP.obj");

	// Modelos del hábitat Sabana
	Model AguaJ((char*)"ModelosSabana/AguaJ.obj");
	Model BarandalJ((char*)"ModelosSabana/BarandalJ.obj");
	Model BaseJ((char*)"ModelosSabana/BaseJ.obj");
	Model CartelJ((char*)"ModelosSabana/CartelJ.obj");
	Model PastoSeco((char*)"ModelosSabana/PastoSeco.obj");
	Model RocaJ1((char*)"ModelosSabana/RocaJ1.obj");
	Model RocaJ2((char*)"ModelosSabana/RocaJ2.obj");
	Model RocaJ3((char*)"ModelosSabana/RocaJ3.obj");
	Model RocaJ4((char*)"ModelosSabana/RocaJ4.obj");

	// Modelos del hábitat Selva
	Model BarandalTg((char*)"ModelosSelva/BarandalTg.obj");
	Model CartelTg((char*)"ModelosSelva/CartelTg.obj");
	Model CasaTg((char*)"ModelosSelva/CasaTg.obj");
	Model Palma1((char*)"ModelosSelva/Palma1.obj");
	Model Palma2((char*)"ModelosSelva/Palma2.obj");
	Model Palma3((char*)"ModelosSelva/Palma3.obj");
	Model ParedTg((char*)"ModelosSelva/ParedTg.obj");
	Model ParedTg1((char*)"ModelosSelva/ParedTg1.obj");
	Model ParedTg2((char*)"ModelosSelva/ParedTg2.obj");
	Model PisoTg((char*)"ModelosSelva/PisoTg.obj");
	Model TechoTg((char*)"ModelosSelva/TechoTg.obj");
	Model TroncoP1((char*)"ModelosSelva/TroncoP1.obj");
	Model TroncoP2((char*)"ModelosSelva/TroncoP2.obj");
	Model TroncoP3((char*)"ModelosSelva/TroncoP3.obj");
	Model Volcan((char*)"ModelosSelva/Volcan.obj");

	//KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++) //Incremento de cada keyframe
	{
		KeyFrame[i].dogPosX = 0;
		KeyFrame[i].dogPosY = 0;
		KeyFrame[i].dogPosZ = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDog = 0;
		KeyFrame[i].rotDogInc = 0;

		KeyFrame[i].head = 0;
		KeyFrame[i].headInc = 0;

		KeyFrame[i].Flegs = 0;
		KeyFrame[i].FlegsInc = 0;
		KeyFrame[i].Rlegs = 0;
		KeyFrame[i].RlegsInc = 0;
		KeyFrame[i].tail = 0;
		KeyFrame[i].tailInc = 0;
	}


	// Se crean los identificadores para el VAO, VBO y EBO
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Se enlaza el VAO 
	glBindVertexArray(VAO);

	// Se enlaza el VBO y se le asignan los datos del arreglo de vértices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Atributo de los vértices
	// Atributo de posición 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Atributo de normales 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Conffiguración de shaders y texturas
	// Se indica al shader cuál textura corresponde al material difuso y especular
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	// Matriz de proyección en perspectiva
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Carga de texturas
	GLuint texAgua = SOIL_load_OGL_texture("Texturas/Agua.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texAlga = SOIL_load_OGL_texture("Texturas/Alga.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texArena = SOIL_load_OGL_texture("Texturas/Arena.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texBambu = SOIL_load_OGL_texture("Texturas/Bambu.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texCoral = SOIL_load_OGL_texture("Texturas/Coral.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texEstanque = SOIL_load_OGL_texture("Texturas/Estanque.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texHongo = SOIL_load_OGL_texture("Texturas/Hongo.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texLadrillo = SOIL_load_OGL_texture("Texturas/Ladrillo.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texLadrillo1 = SOIL_load_OGL_texture("Texturas/Ladrillo1.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texMadera = SOIL_load_OGL_texture("Texturas/Madera.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texMetalBlanco = SOIL_load_OGL_texture("Texturas/Metal blanco.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texMetalDorado = SOIL_load_OGL_texture("Texturas/Metal dorado.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texMetalNegro = SOIL_load_OGL_texture("Texturas/Metal negro.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texMetalVerde = SOIL_load_OGL_texture("Texturas/Metal verde.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texNieve = SOIL_load_OGL_texture("Texturas/Nieve.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texPalma = SOIL_load_OGL_texture("Texturas/Palma.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texPasto = SOIL_load_OGL_texture("Texturas/Pasto.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texPastoSeco = SOIL_load_OGL_texture("Texturas/PastoSeco.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texPastoSelva = SOIL_load_OGL_texture("Texturas/PastoSelva.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texPiedra = SOIL_load_OGL_texture("Texturas/Piedra.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texPiso = SOIL_load_OGL_texture("Texturas/Piso.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texPlastico = SOIL_load_OGL_texture("Texturas/Plastico.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texTronco = SOIL_load_OGL_texture("Texturas/Tronco.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texVidrio = SOIL_load_OGL_texture("Texturas/Vidrio.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	GLuint texVolcan = SOIL_load_OGL_texture("Texturas/Volcan.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);


	// Se activa la prueba de profundidad para que los objetos
	glEnable(GL_DEPTH_TEST);

	// Bucle principal
	while (!glfwWindowShouldClose(window)) // Mientras la ventana no se cierre
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Procesa los eventos de entrada (teclado, ratón, etc.)
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Matriz temporal usada para transformaciones
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp


		// Configuración de luces
		// Se usa el shader correspondiente para establecer los valores de iluminación.
		lightingShader.Use();

		// Se le indica al shader cuál unidad de textura  utilizar
		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		// Posición de la cámara
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Luz Direccional
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 0.0f, -0.5f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.2f, 0.2f, 0.2f);


		// Luces de area del Acuario (Azul)
		// Luz Puntual 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.2f, 0.2f, 6.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.25f);


		// Luz Puntual 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.25f);

		// Luz Puntual 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.25f);

		// Luz Puntual 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.25f);

		// Luces del area de Polar (Blanca)
		// Luz Puntual 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].quadratic"), 0.25f);

		// Luz Puntual 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].position"), pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].quadratic"), 0.25f);

		// Luz Puntual 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].position"), pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].quadratic"), 0.25f);

		// Luz Puntual 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].position"), pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].quadratic"), 0.25f);

		// Luces del area de la Sabana (Amarilla)
		// Luz Puntual 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].position"), pointLightPositions[8].x, pointLightPositions[8].y, pointLightPositions[8].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].quadratic"), 0.25f);

		// Luz Puntual 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].position"), pointLightPositions[9].x, pointLightPositions[9].y, pointLightPositions[9].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[9].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[9].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[9].quadratic"), 0.25f);

		// Luz Puntual 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[10].position"), pointLightPositions[10].x, pointLightPositions[10].y, pointLightPositions[10].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[10].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[10].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[10].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[10].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[10].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[10].quadratic"), 0.25f);

		// Luz Puntual 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[11].position"), pointLightPositions[11].x, pointLightPositions[11].y, pointLightPositions[11].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[11].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[11].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[11].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[11].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[11].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[11].quadratic"), 0.25f);

		// Luces del area de la Selva (Verde)
		// Luz Puntual 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[12].position"), pointLightPositions[12].x, pointLightPositions[12].y, pointLightPositions[12].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[12].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[12].diffuse"), 0.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[12].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[12].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[12].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[12].quadratic"), 0.25f);

		// Luz Puntual 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[13].position"), pointLightPositions[13].x, pointLightPositions[13].y, pointLightPositions[13].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[13].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[13].diffuse"), 0.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[13].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[13].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[13].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[13].quadratic"), 0.25f);

		// Luz Puntual 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[14].position"), pointLightPositions[14].x, pointLightPositions[14].y, pointLightPositions[14].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[14].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[14].diffuse"), 0.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[14].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[14].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[14].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[14].quadratic"), 0.25f);

		// Luz Puntual 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[15].position"), pointLightPositions[15].x, pointLightPositions[15].y, pointLightPositions[15].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[15].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[15].diffuse"), 0.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[15].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[15].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[15].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[15].quadratic"), 0.25f);

		// Luces del area del parque (Rosa)
		// Luz Puntual 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[16].position"), pointLightPositions[16].x, pointLightPositions[16].y, pointLightPositions[16].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[16].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[16].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[16].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[16].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[16].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[16].quadratic"), 0.25f);

		// Luz Puntual 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[17].position"), pointLightPositions[17].x, pointLightPositions[17].y, pointLightPositions[17].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[17].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[17].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[17].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[17].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[17].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[17].quadratic"), 0.25f);

		// Luz Puntual 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[18].position"), pointLightPositions[18].x, pointLightPositions[18].y, pointLightPositions[18].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[18].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[18].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[18].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[18].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[18].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[18].quadratic"), 0.25f);

		// Luz Puntual 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[19].position"), pointLightPositions[19].x, pointLightPositions[19].y, pointLightPositions[19].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[19].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[19].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[19].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[19].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[19].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[19].quadratic"), 0.25f);

		// Luz Puntual 5
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[20].position"), pointLightPositions[20].x, pointLightPositions[20].y, pointLightPositions[20].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[20].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[20].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[20].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[20].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[20].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[20].quadratic"), 0.25f);

		// Luz Puntual 6
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[21].position"), pointLightPositions[21].x, pointLightPositions[21].y, pointLightPositions[21].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[21].ambient"), 0.05f, 0.05f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[21].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[21].specular"), 0.2f, 0.2f, 6.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[21].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[21].linear"), 0.25f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[21].quadratic"), 0.25f);


		// Luz tipo linterna
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Propiedades del material
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Matriz de vista: se genera con la posición y orientación de la cámara.
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Se obtienen las ubicaciones de las variables uniformes en el shader para las matrices de modelo, vista y proyección.
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Se envían las matrices de vista y proyección al shader.
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Inicialización de la matriz modelo
		glm::mat4 model(1);


		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		//Body
		modelTemp = model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
		modelTemp = model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);
		//Head
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadDog.Draw(lightingShader);
		//Tail 
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
		model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogTail.Draw(lightingShader);
		//Front Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);
		//Front Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);
		//Back Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);
		//Back Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_RightLeg.Draw(lightingShader);

		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ball.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 


		// Dibujo de modelos con sus respectivas texturas

		// Se activa el shader de iluminación para aplicar las propiedades de luz y textura
		lightingShader.Use();
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Función para dibujar un modelo con una textura específica
		auto drawModelWithTexture = [&](Model& model, GLuint texture, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f)) {
			glm::mat4 modelMat = glm::mat4(1.0f);
			modelMat = glm::translate(modelMat, position);
			modelMat = glm::scale(modelMat, scale);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);

			model.Draw(lightingShader);
			};

		// Renderizado de los modelos con sus texturas correspondientes
		// Alga
		drawModelWithTexture(Algas, texAlga);

		// Arena
		glBindTexture(GL_TEXTURE_2D, texArena);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Repetir textura horizontalmente
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Repetir textura verticalmente
		lightingShader.setVec2("textureScale", glm::vec2(3.0, 3.0f)); // Escala del patrón de textura
		drawModelWithTexture(ArenaA, texArena);

		// Bambu
		glBindTexture(GL_TEXTURE_2D, texBambu);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		lightingShader.setVec2("textureScale", glm::vec2(3.0f, 3.0f));
		drawModelWithTexture(CasaTg, texBambu);

		//Coral
		drawModelWithTexture(Coral, texCoral);

		//Estamque
		drawModelWithTexture(ParedA, texEstanque);
		glBindTexture(GL_TEXTURE_2D, texEstanque);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		lightingShader.setVec2("textureScale", glm::vec2(3.0f, 3.0f));
		drawModelWithTexture(CasaTg, texBambu);

		// Hongo
		drawModelWithTexture(Hongo1, texHongo);
		drawModelWithTexture(Hongo2, texHongo);
		drawModelWithTexture(Hongo3, texHongo);
		drawModelWithTexture(Hongo4, texHongo);
		drawModelWithTexture(Hongo5, texHongo);
		drawModelWithTexture(Hongo6, texHongo);

		// Ladrillo (paredes, techo, torre, trabes normales, etc.)
		glBindTexture(GL_TEXTURE_2D, texLadrillo1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		lightingShader.setVec2("textureScale", glm::vec2(1.5f, 1.5f));
		drawModelWithTexture(ParedF1, texLadrillo1);

		lightingShader.setVec2("textureScale", glm::vec2(10.0f, 10.0f));
		drawModelWithTexture(ParedLD, texLadrillo1);
		drawModelWithTexture(ParedLI, texLadrillo1);

		lightingShader.setVec2("textureScale", glm::vec2(7.0f, 7.0f));
		drawModelWithTexture(ParedT, texLadrillo1);

		lightingShader.setVec2("textureScale", glm::vec2(4.0f, 4.0f));
		drawModelWithTexture(ParedF2, texLadrillo1);

		lightingShader.setVec2("textureScale", glm::vec2(3.0f, 3.0f));
		drawModelWithTexture(TechoF, texLadrillo1);

		lightingShader.setVec2("textureScale", glm::vec2(270.0f, 20.0f));
		drawModelWithTexture(Trabes, texLadrillo1);

		lightingShader.setVec2("textureScale", glm::vec2(3.0f, 3.0f));
		drawModelWithTexture(BarandalTg, texLadrillo1);

		lightingShader.setVec2("textureScale", glm::vec2(1.5f, 1.5f));
		drawModelWithTexture(ParedTg2, texLadrillo1);

		lightingShader.setVec2("textureScale", glm::vec2(3.5f, 3.5f));
		drawModelWithTexture(ParedTg, texLadrillo1);

		glBindTexture(GL_TEXTURE_2D, texLadrillo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		lightingShader.setVec2("textureScale", glm::vec2(2.0, 2.0f));
		drawModelWithTexture(ParedFA1, texLadrillo);

		lightingShader.setVec2("textureScale", glm::vec2(1.5f, 1.5f));
		drawModelWithTexture(ParedFA2, texLadrillo);

		lightingShader.setVec2("textureScale", glm::vec2(1.5f, 1.5f));
		drawModelWithTexture(ParedFA3, texLadrillo);

		lightingShader.setVec2("textureScale", glm::vec2(1.5f, 1.5f));
		drawModelWithTexture(ParedFA4, texLadrillo);

		lightingShader.setVec2("textureScale", glm::vec2(1.5f, 1.5f));
		drawModelWithTexture(ParedFA5, texLadrillo);

		lightingShader.setVec2("textureScale", glm::vec2(1.5f, 1.5f));
		drawModelWithTexture(ParedLF1, texLadrillo);

		lightingShader.setVec2("textureScale", glm::vec2(1.5f, 1.5f));
		drawModelWithTexture(ParedLF, texLadrillo);

		lightingShader.setVec2("textureScale", glm::vec2(2.0f, 2.0f));
		drawModelWithTexture(TorreI, texLadrillo);

		lightingShader.setVec2("textureScale", glm::vec2(2.0f, 2.0f));
		drawModelWithTexture(TorreS, texLadrillo);

		lightingShader.setVec2("textureScale", glm::vec2(1.0f, 1.0f));
		drawModelWithTexture(ParedTg1, texLadrillo);

		// Madera
		drawModelWithTexture(Bancas, texMadera);
		drawModelWithTexture(BarandalJ, texMadera);

		// Metal blanco
		drawModelWithTexture(BarandalZ, texMetalBlanco);

		// Metal dorado
		drawModelWithTexture(Campana, texMetalDorado);

		// Metal Negro
		drawModelWithTexture(Letras, texMetalNegro);
		drawModelWithTexture(Soportes, texMetalNegro);

		//Metal Verde
		drawModelWithTexture(BarandalP, texMetalVerde);
		drawModelWithTexture(Faros, texMetalVerde);
		drawModelWithTexture(CartelT, texMetalVerde);
		drawModelWithTexture(CartelP, texMetalVerde);
		drawModelWithTexture(CartelJ, texMetalVerde);
		drawModelWithTexture(CartelTg, texMetalVerde);
		drawModelWithTexture(TechoTg, texMetalVerde);

		// Nieve
		drawModelWithTexture(PlataformaP, texNieve);

		// Palma
		drawModelWithTexture(Palma1, texPalma);
		drawModelWithTexture(Palma2, texPalma);
		drawModelWithTexture(Palma3, texPalma);

		// Pasto
		glBindTexture(GL_TEXTURE_2D, texPasto);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		lightingShader.setVec2("textureScale", glm::vec2(8.0, 8.0f));
		drawModelWithTexture(Pasto, texPasto);

		// Pasto Seco
		glBindTexture(GL_TEXTURE_2D, texPastoSeco);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		lightingShader.setVec2("textureScale", glm::vec2(5.0, 5.0f));
		drawModelWithTexture(PastoSeco, texPastoSeco);

		// Pasto Selva
		glBindTexture(GL_TEXTURE_2D, texPastoSelva);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		lightingShader.setVec2("textureScale", glm::vec2(4.0, 4.0f));
		drawModelWithTexture(PisoTg, texPastoSelva);

		// Piedra
		glBindTexture(GL_TEXTURE_2D, texPiedra);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		lightingShader.setVec2("textureScale", glm::vec2(1.5, 1.5f));
		drawModelWithTexture(Arcos, texPiedra);
		drawModelWithTexture(PlataformaZ, texPiedra);
		drawModelWithTexture(RocasA, texPiedra);
		drawModelWithTexture(EstanqueP, texPiedra);
		drawModelWithTexture(BaseJ, texPiedra);
		drawModelWithTexture(RocaJ1, texPiedra);
		drawModelWithTexture(RocaJ2, texPiedra);
		drawModelWithTexture(RocaJ3, texPiedra);
		drawModelWithTexture(RocaJ4, texPiedra);
		drawModelWithTexture(Fuente, texPiedra);

		// Piso
		drawModelWithTexture(PisoZoo, texPiso);

		// Plastico
		drawModelWithTexture(Reloj, texPlastico);

		// Tronco
		drawModelWithTexture(TroncoP1, texTronco);
		drawModelWithTexture(TroncoP2, texTronco);
		drawModelWithTexture(TroncoP3, texTronco);

		// Volcan
		drawModelWithTexture(Volcan, texVolcan);

		// Agua
		drawModelWithTexture(AguaP, texAgua);
		drawModelWithTexture(AguaJ, texAgua);
		drawModelWithTexture(AguaF, texAgua);

		// Activar el blending para permitir transparencia (mezcla de colores RGBA)
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1); // Se activa la transparencia en el shader
		drawModelWithTexture(AguaA, texAgua);

		// Vidrio
		glUniform1f(glGetUniformLocation(lightingShader.Program, "alpha"), 0.5f);
		drawModelWithTexture(VidrioA, texVidrio);
		drawModelWithTexture(VidrioAF, texVidrio);

		// Desactivar blending una vez dibujado el vidrio
		glDisable(GL_BLEND);

		// Se libera el VAO
		glBindVertexArray(0);
	
		// Este VAO se usa para dibujar un cubo pequeño que representa la fuente de luz
		unsigned int lightVAO;
		glGenVertexArrays(1, &lightVAO);
		glBindVertexArray(lightVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// Posición de los primeros 3 floats de cada vértice
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Desvincular VAO
		glBindVertexArray(0);

		// Activar el shader de la lámpara
		lampShader.Use();

		// Obtener las ubicaciones (locations) de las variables uniformes del shader
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Enviar las matrices de vista y proyección al shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		//Dibujar cada lámpara(una por cada luz puntual definida en pointLightPositions[])
		for (GLuint i = 0; i < 22; i++) {
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.3f,0.5f,0.3f));
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//Desenlazar el VAO después de dibujar
		glBindVertexArray(0);
		
		// Intercambiar los buffers de pantalla
		glfwSwapBuffers(window);
	}

	//Finalizar GLFW
	glfwTerminate();

	//Salida del programa
	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	//Dog Controls
	
	if (keys[GLFW_KEY_Z])
	{

		head += 0.25f;

	}

	if (keys[GLFW_KEY_X])
	{

		head -= 0.25f;

	}

	if (keys[GLFW_KEY_P])
	{

		FLegs += 0.25f;

	}

	if (keys[GLFW_KEY_O])
	{

		FLegs -= 0.25f;

	}

	if (keys[GLFW_KEY_N])
	{

		RLegs += 0.25f;

	}

	if (keys[GLFW_KEY_M])
	{

		RLegs -= 0.25f;

	}

	if (keys[GLFW_KEY_B])
	{

		tail += 0.25f;

	}

	if (keys[GLFW_KEY_V])
	{

		tail -= 0.25f;

	}


	if (keys[GLFW_KEY_2])
	{
		
			rotDog += 1.0f;

	}

	if (keys[GLFW_KEY_3])
	{
		
			rotDog -= 1.0f;

	}
			
	if (keys[GLFW_KEY_H])
	{
		dogPosZ += 0.01;
	}

	if (keys[GLFW_KEY_Y])
	{
		dogPosZ -= 0.01;
	}

	if (keys[GLFW_KEY_G])
	{
		dogPosX -= 0.01;
	}

	if (keys[GLFW_KEY_J])
	{
		dogPosX += 0.01;
	}

	// Controles de la cámara
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		// Mueve la cámara hacia adelante
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		// Mueve la cámara hacia atrás
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		// Mueve la cámara hacia la izquierda
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		// Mueve la cámara hacia la derecha
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{

		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}
	
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{

	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}

	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}

	}



	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
			
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
	
	
}
void Animation() {

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			dogPosX += KeyFrame[playIndex].incX;
			dogPosY += KeyFrame[playIndex].incY;
			dogPosZ += KeyFrame[playIndex].incZ;

			rotDog += KeyFrame[playIndex].rotDogInc;

			head += KeyFrame[playIndex].headInc;

			FLegs += KeyFrame[playIndex].FlegsInc;
			RLegs += KeyFrame[playIndex].RlegsInc;
			tail += KeyFrame[playIndex].tailInc;

			i_curr_steps++;
		}

	}
	
}

//Función para el movimiento del mouse
void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}