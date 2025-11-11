#ifndef SHADER_H
#define SHADER_H

// Librerías necesarias
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

// Clase Shader: se encarga de cargar, compilar y vincular los shaders de OpenGL
class Shader
{
public:
	GLuint Program;		// ID del programa del shader en OpenGL
	GLuint uniformColor;	// Localidad del color uniforme en el shader
	
	// Recibe las rutas de los archivos de shader (vertex y fragment)
	Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
	{
		// 1. Leer el código fuente de los shaders desde los archivos indicados
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		
		// Habilitar excepciones si ocurre un error al abrir o leer archivos
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			// Abrir archivos de shader
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			// Crear flujos para leer los contenidos de los archivos
			std::stringstream vShaderStream, fShaderStream;

			// Leer el contenido completo de los archivos a los flujos
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			// Cerrar archivos
			vShaderFile.close();
			fShaderFile.close();

			// Convertir los flujos a strings
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		// Convertir los strings a punteros tipo GLchar para OpenGL
		const GLchar *vShaderCode = vertexCode.c_str();
		const GLchar *fShaderCode = fragmentCode.c_str();
		
		// 2. Compilar los shaders
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];

		// Compilación del Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		
		// Compilación del Fragment Shader 
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		
		// Vinculación de los shaders en un programa
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);
		
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// Obtener la localización del uniform "color" en el shader
		uniformColor = glGetUniformLocation(this->Program, "color");
		
		// Eliminar los shaders individuales (ya están enlazados al programa)
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	// Usa (activa) el shader actual
	void Use()
	{
		glUseProgram(this->Program);
	}

	// Asigna un valor flotante a una variable uniforme dentro del shader
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(this->Program, name.c_str()), value);
	}

	// Asigna un vector de 2 componentes (vec2) a una variable uniforme
	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(this->Program, name.c_str()), 1, &value[0]);
	}


	// Retorna la localización del color uniforme
	GLuint getColorLocation()
	{
		return uniformColor;
	}
};

#endif