#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//Classe shader
#include "Shader.h"


// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int setupGeometry();

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 800, HEIGHT = 600;

//Exerc�cio 3
float x3 = 400.0, y3 = 200.0;
//

bool exercicio1 = true, exercicio2 = false, exercicio3 = false;

// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();


	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 1 (Lista 3)!", nullptr, nullptr);
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
	Shader shader("HelloTriangle.vs", "HelloTriangle.fs");

	// Gerando um buffer simples, com a geometria de um tri�ngulo
	GLuint VAO = setupGeometry();


	glUseProgram(shader.ID);

	glm::mat4 projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);

	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	// Loop da aplica��o - "game loop"
	float desloc = 0, defor = 0;
	bool up = true, shrink = true;
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
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(1);
		glPointSize(5);

		glBindVertexArray(VAO); //Conectando ao buffer de geometria

		//Exerc�cio 1 - �nicio

		if (exercicio1) {
			//Matriz de modelo -- transforma��es na geometria
			glm::mat4 model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(200.0, 300.0, 0.0));
			float angle = glfwGetTime();
			model = glm::rotate(model, angle, glm::vec3(0.0, 0.0, 1.0));
			model = glm::scale(model, glm::vec3(100.0, 100.0, 1.0));
			GLint modelLoc = glGetUniformLocation(shader.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

			// Chamada de desenho - drawcall

			glDrawArrays(GL_TRIANGLES, 0, 3);
			glDrawArrays(GL_LINE_LOOP, 0, 3);
			glDrawArrays(GL_POINTS, 0, 3);

			glm::mat4 model2 = glm::mat4(1);
			model2 = glm::translate(model2, glm::vec3(600.0, 300.0, 0.0));
			int s2 = 100.0 - defor;
			if (defor >= 100 || defor < 0) {
				shrink = !shrink;
			}
			model2 = glm::scale(model2, glm::vec3(s2, s2, 1.0));
			if (shrink) {
				defor += 0.1;
			}
			else {
				defor -= 0.1;
			}
			GLint modelLoc2 = glGetUniformLocation(shader.ID, "model");
			glUniformMatrix4fv(modelLoc2, 1, FALSE, glm::value_ptr(model2));

			glDrawArrays(GL_TRIANGLES, 0, 3);
			glDrawArrays(GL_LINE_LOOP, 0, 3);
			glDrawArrays(GL_POINTS, 0, 3);

			glm::mat4 model3 = glm::mat4(1);
			int y3 = 300.0 + desloc;
			if (desloc > 200 || desloc < -200) {
				up = !up;
			}
			model3 = glm::translate(model3, glm::vec3(400.0, y3, 0.0));
			if (up) {
				desloc += 0.1;
			}
			else {
				desloc -= 0.1;
			}
			model3 = glm::scale(model3, glm::vec3(100.0, 100.0, 1.0));
			GLint modelLoc3 = glGetUniformLocation(shader.ID, "model");
			glUniformMatrix4fv(modelLoc3, 1, FALSE, glm::value_ptr(model3));

			glDrawArrays(GL_TRIANGLES, 0, 3);
			glDrawArrays(GL_LINE_LOOP, 0, 3);
			glDrawArrays(GL_POINTS, 0, 3);
		}

		//Exerc�cio 1 - Fim

		//Exerc�cio 2 - In�cio

		if (exercicio2) {
			int startx = 50.0, starty = 50.0, cont = 0;
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					glm::mat4 model = glm::mat4(1);
					model = glm::translate(model, glm::vec3(startx, starty, 0.0));
					model = glm::scale(model, glm::vec3(100.0, 100.0, 1.0));
					GLint modelLoc = glGetUniformLocation(shader.ID, "model");
					glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

					glDrawArrays(GL_TRIANGLE_FAN, 3 + (4 * (cont++%3)), 4);
					startx += 100.0;
				}
				starty += 100.0;
				startx = 50.0;
			}
		}		

		//Exerc�cio 2 - Fim

		//Exerc�cio 3 - In�cio

		if (exercicio3) {
			glm::mat4 model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(x3, y3, 0.0));
			model = glm::scale(model, glm::vec3(100.0, 100.0, 1.0));
			GLint modelLoc = glGetUniformLocation(shader.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 3);
			glDrawArrays(GL_LINE_LOOP, 0, 3);
			glDrawArrays(GL_POINTS, 0, 3);		
		}		

		//Exerc�cio 3 - Fim

		glBindVertexArray(0); //Desconectando o buffer de geometria

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
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

	if (key == GLFW_KEY_UP) {
		y3 += 5;
	}
	if (key == GLFW_KEY_DOWN) {
		y3 -= 5;
	}
	if (key == GLFW_KEY_LEFT ) {
		x3 -= 5;
	}
	if (key == GLFW_KEY_RIGHT) {
		x3 += 5;
	}

	if ((key == GLFW_KEY_1 || key == GLFW_KEY_KP_1) && action == GLFW_PRESS) {
		exercicio1 = true;
		exercicio2 = false;
		exercicio3 = false;
	}
	if ((key == GLFW_KEY_2 || key == GLFW_KEY_KP_2) && action == GLFW_PRESS) {
		exercicio1 = false;
		exercicio2 = true;
		exercicio3 = false;
	}
	if ((key == GLFW_KEY_3 || key == GLFW_KEY_KP_3) && action == GLFW_PRESS) {
		exercicio1 = false;
		exercicio2 = false;
		exercicio3 = true;
	}
}

// Esta fun��o est� bastante harcoded - objetivo � criar os buffers que armazenam a 
// geometria de um tri�ngulo
// Apenas atributo coordenada nos v�rtices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A fun��o retorna o identificador do VAO
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do tri�ngulo e as armazenamos de forma
	// sequencial, j� visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do v�rtice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO �nico ou em VBOs separados
	GLfloat vertices[] = {
		//x   y     z    r    g    b
		-0.5 , -0.5 , 0.0, 1.0, 0.0, 0.0, 
		 0.5 , -0.5 , 0.0, 0.0, 1.0, 0.0, 
		 0.0 ,  0.5 , 0.0, 0.0, 0.0, 1.0, 

		-0.5 , -0.5 , 0.0, 1.0, 0.0, 0.0, 
		 0.5 , -0.5 , 0.0, 1.0, 0.0, 0.0, 
		 0.5 ,  0.5 , 0.0, 1.0, 0.0, 0.0, 
		-0.5 ,  0.5 , 0.0, 1.0, 0.0, 0.0,

		-0.5 , -0.5 , 0.0, 0.0, 1.0, 0.0,
		 0.5 , -0.5 , 0.0, 0.0, 1.0, 0.0,
		 0.5 ,  0.5 , 0.0, 0.0, 1.0, 0.0,
		-0.5 ,  0.5 , 0.0, 0.0, 1.0, 0.0,

		-0.5 , -0.5 , 0.0, 0.0, 0.0, 1.0,
		 0.5 , -0.5 , 0.0, 0.0, 0.0, 1.0,
		 0.5 ,  0.5 , 0.0, 0.0, 0.0, 1.0,
		-0.5 ,  0.5 , 0.0, 0.0, 0.0, 1.0,
	};

	GLuint VBO, VAO;

	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posi��o
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}