#pragma once
#include <JsonBox/JsonBox.h>
#include "GLMesh.h"

#ifdef _DEBUG
#pragma comment(lib, "JsonBox_d.lib")
#else
#pragma comment(lib, "JsonBox.lib")
#endif

namespace SerraPlo {

	class Renderer3D {
		std::vector<GLMesh*> meshList;

	public:
		explicit Renderer3D();
		~Renderer3D();

		void LoadMeshes(std::string &filePath);

		void Begin();
		void Render();
		void End();
	};

}



