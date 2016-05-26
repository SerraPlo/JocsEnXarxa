#pragma once
#include <GL/glew.h>
#include <vector>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GLMaterial.h"
#include "ResourceManager.h"
#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>
#pragma comment(lib, "assimp.lib") 

namespace SerraPlo {

	struct GLMesh {
		GLMaterial material;				// Set of mesh textures and material attributes
		std::vector<float> vertices;		// Array of mesh vertices
		std::vector<unsigned int> indices;	// Array of mesh elements
		GLuint vao, vbo, ebo;				/// TODO: optimize 1 VAO & 1 VBO for multiple meshes
		unsigned int numElements;			// How many faces does the mesh have
		explicit GLMesh() = default;
		GLMesh(aiMesh* mesh) { Load(mesh); }
		void Load(aiMesh* mesh) {
			const int nv = mesh->mNumVertices;
			const bool foo = mesh->HasTextureCoords(0);
			vertices.reserve(nv);
			for (int i = 0; i < nv; ++i)
				vertices.push_back(mesh->mVertices[i].x), vertices.push_back(mesh->mVertices[i].y), vertices.push_back(mesh->mVertices[i].z),
				vertices.push_back(mesh->mNormals[i].x), vertices.push_back(mesh->mNormals[i].y), vertices.push_back(mesh->mNormals[i].z),
				vertices.push_back((foo) ? mesh->mTextureCoords[0][i].x : 0.f), vertices.push_back((foo) ? mesh->mTextureCoords[0][i].y : 0.f);
			const int nf = mesh->mNumFaces;
			indices.reserve(nf);
			for (int i = 0; i < nf; ++i)
				indices.push_back(mesh->mFaces[i].mIndices[0]), indices.push_back(mesh->mFaces[i].mIndices[1]), indices.push_back(mesh->mFaces[i].mIndices[2]);
			numElements = indices.size();
			///Create VAO
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			///Create VBO
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);
			///Create EBO
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numElements*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
			///Configure vertex input
			glVertexAttribPointer(GLuint(0), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
			glEnableVertexAttribArray(0); // position
			glVertexAttribPointer(GLuint(1), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
			glEnableVertexAttribArray(1); // normal
			glVertexAttribPointer(GLuint(2), 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
			glEnableVertexAttribArray(2); // uv
			glBindVertexArray(0);
		}
	};

#include <algorithm>

	struct GLModel {
		GLMesh *meshData{ nullptr };
		int numMeshes{ 0 };
		explicit GLModel() = default;
		//GLModel(const GLModel &model) { numMeshes = model.numMeshes; meshData = new GLMesh[numMeshes]; meshData = model.meshData; };
		GLModel& operator=(const GLModel& model) {
			numMeshes = model.numMeshes; meshData = new GLMesh[numMeshes]; meshData = model.meshData;
			return *this;
		}
		explicit GLModel::GLModel(const char* meshPath, const char* diffusePath, const char* normalPath, JsonBox::Array specular, JsonBox::Array emissive, float shininess) {
			const aiScene* pScene = aiImportFile(meshPath, aiProcessPreset_TargetRealtime_MaxQuality); // Load scene
			if (pScene->mNumMeshes == 1) { //if not scene to load, only obj
				numMeshes = 1;
				meshData = new GLMesh(pScene->mMeshes[0]);
				meshData->material = GLMaterial(diffusePath, normalPath, specular, emissive, shininess); // Create material for mesh
			} else {
				numMeshes = pScene->mNumMeshes;
				meshData = new GLMesh[pScene->mNumMeshes];
				for (int i = 0; i < numMeshes; ++i) {
					meshData[i].Load(pScene->mMeshes[i]); // Add each mesh to model
					const aiMaterial* material = pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]; // Load material from mesh
					int texIndex = 0; aiString path; // To fill variables
					if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS) { // Load diffuse texture from material
						std::string aiDiffPath = path.data; // Get texture path
						auto pos = aiDiffPath.rfind("models");
						meshData[i].material = GLMaterial(LoadAsset(aiDiffPath.substr(pos, aiDiffPath.size()-pos)).c_str(), normalPath, specular, emissive, shininess); // Create material for mesh
					}
				}
			}
			aiReleaseImport(pScene); // Delete scene imported
		}
	};

}
