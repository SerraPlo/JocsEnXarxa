#include "GLMesh.h"
#include <algorithm>
#include <vector>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma comment(lib, "assimp.lib") 

namespace SerraPlo {

	struct MeshData {
		std::vector<float> vertices;
		std::vector<unsigned int> elements;
	};

	struct ASSMesh : MeshData {
		ASSMesh(aiMesh* mesh) {
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
		}
	};

	GLMesh::GLMesh(const char *filePath) {
		const aiScene* pScene = aiImportFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);
		MeshData mesh = ASSMesh(pScene->mMeshes[0]);
		aiReleaseImport(pScene);
		elements = mesh.elements.size();
		///Create VAO
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		///Create VBO
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size()*sizeof(float), &mesh.vertices[0], GL_STATIC_DRAW);
		///Create EBO
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.elements.size()*sizeof(unsigned int), &mesh.elements[0], GL_STATIC_DRAW);
		///Configure vertex input
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	GLMesh::~GLMesh() {
		if (vao) glDeleteVertexArrays(1, &vao);
		if (vbo) glDeleteBuffers(1, &vbo);
		if (ebo) glDeleteBuffers(1, &ebo);
	}

}
