#pragma once // Evita que este archivo se incluya más de una vez en el proyecto 

// Librerías necesarias
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SOIL2/SOIL2.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include  "Shader.h"

using namespace std;

// Prototipo de función para cargar texturas
GLint TextureFromFile(const char *path, string directory);

// CLASE MODEL
class Model
{
public:

	// Recibe la ruta del archivo .obj o formato compatible y llama a loadModel()
	Model(GLchar *path)
	{
		this->loadModel(path);
	}

	// Dibuja el modelo
	void Draw(Shader shader)
	{
		for (GLuint i = 0; i < this->meshes.size(); i++)
		{
			this->meshes[i].Draw(shader);
		}
	}

private:
	// Datos del modelo
	vector<Mesh> meshes;				// Contiene todas las mallas del modelo
	string directory;					// Directorio donde se encuentran las texturas
	vector<Texture> textures_loaded;	// Texturas ya cargadas (evita cargar duplicadas)

	// Carga el modelo usando Assimp y procesa la escena completa
	void loadModel(string path)
	{
		
		Assimp::Importer importer;

		// Carga el modelo aplicando transformaciones automáticas
		const aiScene* scene = importer.ReadFile(path,
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace);



		// Verifica si hubo errores al cargar el modelo
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		
		// Obtiene el directorio del archivo (para ubicar texturas)
		this->directory = path.substr(0, path.find_last_of('/'));

		// Procesa el nodo raíz 
		this->processNode(scene->mRootNode, scene);
	}

	//Procesa cada nodo de la jerarquía del modelo
	void processNode(aiNode* node, const aiScene* scene)
	{
		// Procesa todas las mallas del nodo actual
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			// Cada nodo almacena índices a las mallas reales del modelo
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			this->meshes.push_back(this->processMesh(mesh, scene));

			// Verifica si la malla tiene normales
			if (mesh->HasNormals()) {
				// Acceder a las normales
				for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
					glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
					// usar la normal
				}
			}

		}

		// Procesa los nodos hijos de manera recursiva
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			this->processNode(node->mChildren[i], scene);
		}
	}
	//  Procesa una malla individual y devuelve un objeto Mesh
	Mesh processMesh(aiMesh *mesh, const aiScene *scene)
	{

		vector<Vertex> vertices;
		vector<GLuint> indices;
		vector<Texture> textures;

		// Recorre todos los vértices de la malla
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.

			// Posicion de vértice
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			// Posición de la normal
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			// Coordenadas de textura
			if (mesh->mTextureCoords[0]) // Sí hay coordenadas de textura
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else // No hay coordenadas de textura
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			// Recorre las caras(triángulos) de la malla
			for (GLuint i = 0; i < scene->mNumMeshes; i++) {
				aiMesh* mesh = scene->mMeshes[i];
				if (!mesh->HasTextureCoords(0)) {
					std::cout << "Mesh " << mesh->mName.C_Str() << " NO tiene UVs!" << std::endl;
				}
			}


			vertices.push_back(vertex);
		}

		// Recorre todas las caras de la malla y obtiene los índices
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (GLuint j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// Procesa materiales (texturas)
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			// Carga texturas difusas (color base)
			vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			// Carga texturas especulares (brillo)
			vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		// Devuelve un objeto Mesh con toda la información lista
		return Mesh(vertices, indices, textures);
	}

	// Carga texturas de un material
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;

		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			GLboolean skip = false;

			// Verifica si la textura ya fue cargada
			for (GLuint j = 0; j < textures_loaded.size(); j++)
			{
				if (textures_loaded[j].path == str)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; 

					break;
				}
			}

			// Si no estaba cargada, se carga desde el archivo
			if (!skip)
			{   
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);

				this->textures_loaded.push_back(texture);  // Guarda la textura para no duplicarla
			}
		}

		return textures;
	}
};

// Función para cargar una textura desde archivo
GLint TextureFromFile(const char *path, string directory)
{
	// Combina el directorio y el nombre del archivo
	string filename = string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;

	// Carga la imagen desde disco
	unsigned char *image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

	// Asocia y configura la textura
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Configura parámetros de la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	return textureID;
}