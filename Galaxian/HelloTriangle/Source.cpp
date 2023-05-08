/* C�digo desenvolvido por Paulo Henrique Lanius */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "Shader.h"
#include "Sprite.h"


// Prot�tipos de fun�oes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLuint generateTexture(string filePath, int &width, int &height);
bool testOut(Sprite spr1);
bool testCollision(Sprite spr1, Sprite spr2);

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 800, HEIGHT = 600;

// Declara��o das sprites
Sprite background, ship, projectile, explosion;
Sprite enemys[3][6];

int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GALAXIAN!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	


	// Compilando e buildando o programa de shader
	Shader shader("Galaxian.vs", "Galaxian.fs");

	int bgwidth, bgheight;
	int shipwidth, shipheight;
	int pjwidth, pjheight;
	int exwidth, exheight;
	int i1width, i1height;
	int i2width, i2height;
	int i3width, i3height;	

	// Gerando texturas
	GLuint texID7 = generateTexture("../../textures/Galaxian/Background.jpg", bgwidth, bgheight);
	GLuint texID = generateTexture("../../textures/Galaxian/Nave.png", shipwidth, shipheight);
	GLuint texID2 = generateTexture("../../textures/Galaxian/Tiro.png", pjwidth, pjheight);
	GLuint texID3 = generateTexture("../../textures/Galaxian/Explos�o.png", exwidth, exheight);
	GLuint texID4 = generateTexture("../../textures/Galaxian/Inimigo1.png", i1width, i1height);
	GLuint texID5 = generateTexture("../../textures/Galaxian/Inimigo2.png", i2width, i2height);
	GLuint texID6 = generateTexture("../../textures/Galaxian/Inimigo3.png", i3width, i3height);
	
	// Inicialza��o das sprites
	background.initialize(texID7, bgwidth, bgheight, 1, 1);
	background.setShader(&shader);
	background.setPosition(glm::vec3(400, 300, 0));
	background.setScale(glm::vec3(bgwidth, bgheight, 1));
	background.activate();

	float scaleIncreaseWidth = 50, scaleIncreaseHeigth = 30; // Vari�veis usadas para scala das sprites
	ship.initialize(texID, shipwidth, shipheight, 1, 2);
	ship.setShader(&shader);
	ship.setPosition(glm::vec3(400, 100, 0));
	ship.setScale(glm::vec3(shipwidth + scaleIncreaseWidth, shipheight + scaleIncreaseHeigth, 1));
	ship.activate();

	projectile.initialize(texID2, pjwidth, pjheight, 1, 1);
	projectile.setShader(&shader);
	projectile.setPosition(glm::vec3(400, 140, 0));
	projectile.setScale(glm::vec3(pjwidth + scaleIncreaseWidth /10.0, pjheight + scaleIncreaseHeigth /3.0, 1));

	explosion.initialize(texID3, exwidth, exheight, 1, 4);
	explosion.setShader(&shader);
	explosion.setPosition(glm::vec3(0, 0, 0));
	explosion.setScale(glm::vec3(exwidth + scaleIncreaseWidth * 3, exheight + scaleIncreaseHeigth, 1));
	
	int spaceWidth = 50, spaceHeigth = 50, lastWidth = 250, lastHeigth = 500;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			switch (i)
			{
				case 0: 
					enemys[i][j].initialize(texID4, i1width, i1height, 1, 4);
					enemys[i][j].setShader(&shader);
					enemys[i][j].setPosition(glm::vec3(lastWidth, lastHeigth, 0));
					enemys[i][j].setScale(glm::vec3(i1width + scaleIncreaseWidth * 3, i1height + scaleIncreaseHeigth, 1));
					break;
				case 1:	
					enemys[i][j].initialize(texID5, i2width, i2height, 1, 4);
					enemys[i][j].setShader(&shader);
					enemys[i][j].setPosition(glm::vec3(lastWidth, lastHeigth, 0));
					enemys[i][j].setScale(glm::vec3(i2width + scaleIncreaseWidth * 3, i2height + scaleIncreaseHeigth, 1));
					break;
				case 2:
					enemys[i][j].initialize(texID6, i3width, i3height, 1, 4);
					enemys[i][j].setShader(&shader);
					enemys[i][j].setPosition(glm::vec3(lastWidth, lastHeigth, 0));
					enemys[i][j].setScale(glm::vec3(i3width + scaleIncreaseWidth * 3, i3height + scaleIncreaseHeigth, 1));
					break;
				default:
					break;
			}

			lastWidth += spaceWidth;			
		}
		lastHeigth -= spaceHeigth;
		lastWidth = 250;
	}	

	glUseProgram(shader.ID);

	//Matriz de proje��o paralela ortogr�fica
	glm::mat4 projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);

	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	//Ativar o buffer de textura
	glActiveTexture(GL_TEXTURE0);

	//Habilitar teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	//Habilitar a transpar�ncia
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int i = 0;
	bool ladoMovimento = true;
	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(1);
		glPointSize(5);		

		Sleep(16); // Delay para atualiza��o n�o ser t�o r�pida

		background.update();
		background.draw();

		ship.update();
		ship.draw();
		
		// L�gica de movimenta��o dos inimigos
		bool stop = false;
		for (int i = 0; i < 3 && !stop; i++)
		{
			for (int j = 0; j < 6 && !stop; j++)
			{
				if (i % 2 == 0) {	
					if (ladoMovimento) {
						stop = !enemys[i][j].moveLeft();
					}
					else {
						stop = !enemys[i][j].moveRight();
					}
				}
				else {
					if (ladoMovimento) {
						stop = !enemys[i][j].moveRight();
					}
					else {
						stop = !enemys[i][j].moveLeft();
					}
				}

				if (!stop) {
					enemys[i][j].update();
					enemys[i][j].draw();
				}
				else {
					ladoMovimento = !ladoMovimento;
				}
			}
		}

		// L�gica de movimenta��o do proj�til
		if (projectile.isActive()) {
			projectile.moveUp(); 
			

			bool out = testOut(projectile);
			bool col = false;
			glm::vec3 positionE = projectile.getPosition();
			// Testa colis�o com todos inimigos
			for (int i = 0; i < 3 && !col; i++)
			{
				for (int j = 0; j < 6 && !col; j++)
				{
					col = testCollision(projectile, enemys[i][j]);
					if (col) {
						positionE = enemys[i][j].getPosition();
						enemys[i][j].deactivate();
					}
				}
			}

			if (col || out) {
				// Esconde proj�til e volta a posi��o de tiro
				projectile.deactivate();
				glm::vec3 positionShip = ship.getPosition();
				positionShip.y += 40;
				projectile.setPosition(positionShip);
				ship.setFrame(0);

				if (col) {
					// Gera explos�o
					explosion.setPosition(positionE);
					explosion.activate();
					i = 0;
				}				
			}
			projectile.update();
			projectile.draw();
		}

		if (explosion.isActive()) {
			explosion.setFrame((i++/4) % 4);
			explosion.update();
			explosion.draw();
			if (i/4 >= 4) {
				explosion.deactivate();
			}
		}

		glBindVertexArray(0); //Desconectando o buffer de geometria

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
	{
		ship.moveRight();
		if(!projectile.isActive()) projectile.moveRight();
	}
	if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
	{
		ship.moveLeft();
		if (!projectile.isActive()) projectile.moveLeft();
	}
	if (key == GLFW_KEY_W || key == GLFW_KEY_UP)
	{		
		ship.setFrame(1);
		projectile.activate();
	}
	if (key == GLFW_KEY_ENTER) // Reseta inimigos
	{
		int spaceWidth = 50, spaceHeigth = 50, lastWidth = 250, lastHeigth = 500;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				enemys[i][j].setPosition(glm::vec3(lastWidth, lastHeigth, 0));

				enemys[i][j].activate();
				enemys[i][j].activateFrame();
				lastWidth += spaceWidth;
			}
			lastHeigth -= spaceHeigth;
			lastWidth = 250;
		}
	}
}

GLuint generateTexture(string filePath, int &width, int &height)
{
	GLuint texID;
	// Gera o identificador da textura na mem�ria 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Definindo o m�todo de wrapping e de filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Carregando a imagen da textura
	int nrChannels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	//Manda para OpenGL armazenar a textura e gerar o mipmap
	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

bool testOut(Sprite spr1) //Teste para objeto fora da tela
{
	glm::vec2 min1, max1;
	spr1.getAABB(min1, max1);

	return (
		min1.y >= 600 
		|| max1.y >= 600 
		|| min1.y <= 0
		|| max1.y <= 0
		|| min1.x >= 750 
		|| max1.x >= 750
		|| min1.x <= 50
		|| max1.x <= 50);
}

bool testCollision(Sprite spr1, Sprite spr2) //Teste para colis�o
{
	if (!spr2.isActive()) {
		return false;
	}

	glm::vec2 min1, min2, max1, max2;
	spr1.getAABB(min1, max1);
	spr2.getAABB(min2, max2);	

	float ymin = min1.y - min2.y;
	float ymax = max1.y - max2.y;
	float xmin = min1.x - min2.x;
	float xmax = max1.x - max2.x;
	//Testa se diferen�a entre posi��es � pr�xima
	if ((ymin > -4 && ymin < 4 && xmin > -25 && xmin < 25) || (ymax > -4 && ymax < 4 && xmax > -25 && xmax < 25)) {
		return true;
	}

	return false;
}

