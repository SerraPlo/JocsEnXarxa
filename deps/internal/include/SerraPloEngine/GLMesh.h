#pragma once
#include <GL/glew.h>
#include <vector>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GLMaterial.h"
#pragma comment(lib, "assimp.lib") 

namespace SerraPlo {

	struct GLMesh {
		GLMaterial material;
		std::vector<float> vertices;
		std::vector<unsigned int> elements;
		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		//unsigned int numElements;
		GLMesh(aiMesh* mesh) {
			const int nv = mesh->mNumVertices;
			const bool foo = mesh->HasTextureCoords(0);
			vertices.reserve(nv);
			for (int i = 0; i < nv; ++i)
				vertices.push_back(mesh->mVertices[i].x),
				vertices.push_back(mesh->mVertices[i].y),
				vertices.push_back(mesh->mVertices[i].z),
				vertices.push_back(mesh->mNormals[i].x),
				vertices.push_back(mesh->mNormals[i].y),
				vertices.push_back(mesh->mNormals[i].z),
				vertices.push_back((foo) ? mesh->mTextureCoords[0][i].x : 0.f),
				vertices.push_back((foo) ? mesh->mTextureCoords[0][i].y : 0.f);
			const int nf = mesh->mNumFaces;
			elements.reserve(nf);
			for (int i = 0; i < nf; ++i)
				elements.push_back(mesh->mFaces[i].mIndices[0]),
				elements.push_back(mesh->mFaces[i].mIndices[1]),
				elements.push_back(mesh->mFaces[i].mIndices[2]);

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
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size()*sizeof(unsigned int), &elements[0], GL_STATIC_DRAW);
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

	struct GLModel {
		std::vector<GLMesh> meshData;
		explicit GLModel() = default;
		explicit GLModel::GLModel(const char* meshPath, const char* diffusePath, const char* normalPath,
			JsonBox::Array specular, JsonBox::Array emissive, float shininess) {
			const aiScene* pScene = aiImportFile(meshPath, aiProcessPreset_TargetRealtime_MaxQuality);
			for (size_t i = 0; i < pScene->mNumMeshes; ++i) {
				meshData.emplace_back(pScene->mMeshes[i]);
				if (diffusePath == nullptr) {
					const aiMaterial* material = pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex];
					int texIndex = 0;
					aiString path;
					if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS) {
						GLMaterial temp(path.data, normalPath, specular, emissive, shininess);
						meshData[i].material = temp;
						continue;
					}
				}
				GLMaterial temp(diffusePath, normalPath, specular, emissive, shininess);
				meshData[i].material = temp;
			}
			aiReleaseImport(pScene);
		}
	};

}
