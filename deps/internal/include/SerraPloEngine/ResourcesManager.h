#pragma once
#include <map>
#include "GameObject.h"
#include "GLMesh.h"
#include "GLMaterial.h"

namespace SerraPlo {
	struct GameObjectMesh {
		GLMesh *meshData{ nullptr };
		int numMeshes{ 0 };
	};
	struct GameObjectMaterial {
		GLMesh *materialData{ nullptr };
		int numMaterials{ 0 };
	};
	class ResourceManager {
	public:
		std::map<uint16_t, GameObjectMesh> meshList;
		std::map<uint16_t, GameObjectMaterial> materialList;
		explicit ResourceManager() = default;
		~ResourceManager() {
			for (auto& entity : meshList)
				glDeleteVertexArrays(1, &entity.second.meshData->vao),
				glDeleteBuffers(1, &entity.second.meshData->vbo),
				glDeleteBuffers(1, &entity.second.meshData->ebo),
				entity.second.numMeshes > 1 ? delete[] entity.second.meshData : delete entity.second.meshData;
			for (auto& entity : materialList) entity.second.numMaterials > 1 ? delete[] entity.second.materialData : delete entity.second.materialData;
		}
		bool Empty() const { return meshList.empty() && materialList.empty(); }
		void LoadMesh(const std::string &filePath) {
			const aiScene* pScene = aiImportFile(filePath.c_str(), aiProcessPreset_TargetRealtime_MaxQuality); // Load scene
			if (pScene->mNumMeshes == 1) { //if not scene to load, only obj
				numMeshes = numMaterials = 1;
				meshData = new GLMesh(pScene->mMeshes[0]);
				materialData = new GLMaterial(diffusePath, normalPath, specular, emissive, shininess); // Create material for mesh
			} else {
				numMeshes = numMaterials = pScene->mNumMeshes;
				meshData = new GLMesh[numMeshes];
				materialData = new GLMaterial[numMaterials];
				for (int i = 0; i < numMeshes; ++i) {
					meshData[i].Load(pScene->mMeshes[i]); // Add each mesh to model
					const aiMaterial* material = pScene->mMaterials[pScene->mMeshes[i]->mMaterialIndex]; // Load material from mesh
					int texIndex = 0; aiString path; // To fill variables
					if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS) { // Load diffuse texture from material
						std::string aiDiffPath = path.data; // Get texture path
						auto pos = aiDiffPath.rfind("models");
						materialData[i] = GLMaterial(LoadAsset(aiDiffPath.substr(pos, aiDiffPath.size() - pos)).c_str(), normalPath, specular, emissive, shininess); // Create material for mesh
					}
				}
			}
			aiReleaseImport(pScene); // Delete scene imported
			std::cout << "Assets loaded" << std::endl;
		}
		GlobalGameObject& Find(const std::string &str) {
			auto &it = gameObjectList.find(str)->second;
			//if (it == gameObjectList.end()) SP_THROW_ERROR("Could not find game object [" + str + "] inside manager list"); 
			return it;
		}
		GameObject FindCopy(const std::string &str) {
			auto it = gameObjectList.find(str);
			if (it == gameObjectList.end()) SP_THROW_ERROR("Could not find game object [" + str + "] inside manager list");
			GameObject temp(it->second.id);
			temp.transform = it->second.transform;
			auto &itModel = it->second.model;
			temp.numMaterials = itModel.numMeshes;
			temp.materials = new GLMaterial[temp.numMaterials];
			for (int i = 0; i < temp.numMaterials; ++i)
				temp.materials[i] = it->second.model.meshData[i].material; ///TODO: now only for .obj
			return temp;
		}
	};

}


/*JsonBox::Value fileData;
fileData.loadFromFile(filePath);
JsonBox::Object wrapper{ fileData.getObject() };
for (auto entity : wrapper) {
std::string key{ entity.first };
JsonBox::Object properties{ entity.second.getObject() };
new (&gameObjectList[key]) GlobalGameObject(entity.first,
LoadAsset(properties["model"].getString()).c_str(),
!properties["diffuse"].isNull() ? LoadAsset(properties["diffuse"].getString()).c_str() : nullptr,
!properties["normal"].isNull() ? LoadAsset(properties["normal"].getString()).c_str() : nullptr,
properties["specular"].getArray(),
!properties["emissive"].isNull() ? properties["emissive"].getArray() : JsonBox::Array{0, 0, 0},
properties["shininess"].getFloat());
/// Load transform attributes
JsonBox::Array tempArray = properties["position"].getArray();
gameObjectList[key].transform.position = { tempArray[0].getFloat(), tempArray[1].getFloat(), tempArray[2].getFloat() };
tempArray = properties["rotation"].getArray();
gameObjectList[key].transform.rotation = { tempArray[0].getFloat(), tempArray[1].getFloat(), tempArray[2].getFloat() };
tempArray = properties["scale"].getArray();
gameObjectList[key].transform.scale = { tempArray[0].getFloat(), tempArray[1].getFloat(), tempArray[2].getFloat() };
}*/
