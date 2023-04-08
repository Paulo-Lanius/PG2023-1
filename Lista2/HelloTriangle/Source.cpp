#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

// Prot�tipos das fun��es
int setupGeometry();

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 800, HEIGHT = 600;

// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo(Lista 2) - Paulo Lanius!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

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

	// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0,width,height);
	
//Exercicio 4
//glViewport(width/2, height/2, width/2, height/2);

	// Compilando e buildando o programa de shader
	Shader shader("HelloTriangle.vs", "HelloTriangle.fs");

	// Gerando um buffer simples, com a geometria de um tri�ngulo
	GLuint VAO = setupGeometry();

	glUseProgram(shader.ID);

	//Matriz de proje��o paralela ortogr�fica
	glm::mat4 projection = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	
//Exercicio 1
//glm::mat4 projection = glm::ortho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);

//Exercicio 2
//glm::mat4 projection = glm::ortho(0.0, 800.0, 600.0, 0.0, -1.0, 1.0);

//Exercicio 3 
//Ao desenhar um objeto podemos ver ele se modificando na tela, conseguimos
//atrav�s disso manipular as perspectivas que temos em rela��o a ele e trabalhar
//de forma mais simples tratando como pixeis a tela por exemplo.

//Exercicio 5
//Decidi por 200 e 200 para ser melhor de dividir os quadrantes em 100 por 100
//glm::mat4 projection = glm::ortho(0.0, 200.0, 200.0, 0.0, -1.0, 1.0);

	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(1);
		glPointSize(5);

		glBindVertexArray(VAO); //Conectando ao buffer de geometria



		// Chamada de desenho - drawcall		
		glDrawArrays(GL_TRIANGLES, 0, 12);



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
	  // x     y    z    r    g    b
	   -0.5, -0.5, 0.0, 1.0, 0.0, 0.0, //v0
		0.5, -0.5, 0.0, 1.0, 0.0, 0.0, //v1
		0.0,  0.5, 0.0, 1.0, 0.0, 0.0, //v2		
	};


//Exercicio 5	
//int sizeQuadrante = 100;
//GLfloat vertices[] = {
//	  //Primeiro triangulo - Esquerda topo
//	  25, 75, 0.0, 1.0, 0.0, 0.0, //v0
//	  75, 75, 0.0, 1.0, 0.0, 0.0, //v1
//	  50, 25, 0.0, 1.0, 0.0, 0.0, //v2		

//	  //Segundo triangulo - Direita topo
//	  25 + sizeQuadrante, 75, 0.0, 1.0, 0.0, 0.0, //v0
//	  75 + sizeQuadrante, 75, 0.0, 1.0, 0.0, 0.0, //v1
//	  50 + sizeQuadrante, 25, 0.0, 1.0, 0.0, 0.0, //v2		

//	  //Terceiro triangulo - Esquerda baixo
//	  25, 75 + sizeQuadrante, 0.0, 1.0, 0.0, 0.0, //v0
//	  75, 75 + sizeQuadrante, 0.0, 1.0, 0.0, 0.0, //v1
//	  50, 25 + sizeQuadrante, 0.0, 1.0, 0.0, 0.0, //v2

//	  //Quarto triangulo - Direita baixo
//	  25 + sizeQuadrante, 75 + sizeQuadrante, 0.0, 1.0, 0.0, 0.0, //v0
//	  75 + sizeQuadrante, 75 + sizeQuadrante, 0.0, 1.0, 0.0, 0.0, //v1
//	  50 + sizeQuadrante, 25 + sizeQuadrante, 0.0, 1.0, 0.0, 0.0, //v2		
//};

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